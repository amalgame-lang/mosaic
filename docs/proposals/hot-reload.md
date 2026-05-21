# Hot-reload — `mosaic dev` v0.5

**Status:** proposal — not yet implemented. Targets mosaic v0.5.

## Problem

`mosaic dev` v0.4 reloads via *kill + rebuild + restart*. The
running server dies on every save, taking three things with it:

1. **In-flight HTTP requests** (rare in practice — most requests
   finish in < 50 ms, and the rebuild itself is the same order
   of magnitude).
2. **WebSocket connections** (the actual pain — a single edit
   drops every connected browser tab).
3. **In-memory state** (`MemorySessionStore`, rate-limit buckets,
   counters) — usually a feature in DEV mode, but occasionally
   surprising.

Item 2 is the load-bearing one. WebSocket apps in DEV mode are
unusable today: every save triggers a reconnect storm.

## Goal

After a code change, the new binary takes over **without
dropping connected WebSocket clients**. In-flight HTTP requests
finish on the old binary; new requests go to the new binary;
WebSocket connections accepted by the old binary stay alive
until the WebSocket itself closes (or a grace timeout).

In-memory state is intentionally NOT preserved — apps that need
durability should use `signed_cookie` sessions or Redis.

## Non-goals

- Live patching of the running binary (mid-request code swap).
  Too brittle; pretty much every language that tried this gave
  up (Erlang excepted, and only because their runtime is built
  around it).
- Zero-downtime in PROD. That's a separate problem — solved by
  systemd socket activation + a load balancer, not by `mosaic
  dev`.

## Three approaches considered

### Approach A — `dlopen` hot-swap

A supervisor process owns the listening socket. The user's app
ships as a `.so`; the supervisor `dlopen`s it, resolves a known
entry-point symbol (`mosaic_app_handle_conn(int fd)`), and runs
the accept loop in-process.

On rebuild: `dlclose` the old `.so`, `dlopen` the new one, swap
the function pointer. In-flight handler invocations on the OLD
`.so` keep running because `dlclose` is lazy — symbols don't
disappear until the last reference drops.

**Pros:** lowest overhead (one process), simplest mental model
(handler is a function call).

**Cons:**
- amc has no `--shared` mode. Producing a `.so` from AM source
  needs PIC code-gen + a tweak to the link step. ~1 week of amc
  engineering on its own.
- GC state across `dlopen`/`dlclose` boundaries is uncharted —
  Boehm GC keeps a global heap so values from the old `.so`
  remain reachable, but lambdas with captured state might dangle.
- C-runtime statics in the .so (e.g. amalgame-net-http's
  `static int ssl_initialised = 0;`) re-initialise on every
  reload, which is fine. But package-cache `static` state in
  amc (registered types, etc.) would also reset — needs audit.

### Approach B — `SO_REUSEPORT` worker swap (chosen)

A supervisor process and one **worker** process at a time. The
worker listens on the bound port; both supervisor and worker
hold an `SO_REUSEPORT` listening fd, so a new worker can come
up without un-binding.

On rebuild:

1. Supervisor spawns a NEW worker (using the freshly-rebuilt
   binary). Both old and new are listening, kernel
   load-balances new connections.
2. Supervisor sends `SIGTERM` to OLD worker.
3. OLD worker stops accepting (close the listen fd) but keeps
   serving its in-flight handler frames and any open
   WebSocket connections.
4. OLD worker exits when its connection count hits zero OR
   after a 60-second grace timeout (whichever first).

WebSocket connections accepted by the old worker stay alive
until the client itself closes them (or grace timeout). That
satisfies the goal.

**Pros:**
- Zero amc changes. The user's app stays a regular binary.
- The supervisor is ~150 LoC of C / bash. Fits a one-day
  session.
- Survives crashes — if a worker segfaults, the supervisor
  spawns a fresh one without dropping in-flight WS connections
  in the OTHER worker (during an overlap).

**Cons:**
- Two processes during the overlap window. Memory cost is
  doubled briefly.
- Kernel load-balancing across REUSEPORT sockets is not
  ordered — a connection arriving during the overlap may land
  on either worker. Fine for DEV; PROD would want a more
  deterministic policy.

### Approach C — `fork()` + `SCM_RIGHTS` fd-passing

Supervisor accepts connections itself and hands the accepted
`fd` to a worker process via `SCM_RIGHTS` over a Unix socket.

Theoretically cleaner than B — only ONE listener at a time —
but in practice the fd-passing dance is fiddly and the kernel
load-balancing in B is already good enough.

Skipped.

## Decision: Approach B

Cheapest to ship, no amc changes, satisfies the goal.

## Implementation sketch

### Phase 1 — supervisor (`tools/mosaic-supervise.sh` + `mosaic-supervise.c`)

The supervisor is a small C program (~150 LoC). It:

1. Parses `--port PORT --binary PATH`.
2. `socket() + setsockopt(SO_REUSEPORT) + bind() + listen()` to
   reserve the port (so port-typo errors surface in the
   supervisor instead of the worker).
3. `close()` the supervisor's own listen fd — the workers will
   each open their own with the same SO_REUSEPORT. The
   supervisor is now just a process supervisor; it doesn't
   own the socket.
4. `fork()` + `execv()` the worker binary. Save its PID.
5. Sits on `read(stdin)` waiting for commands (`reload\n`).
6. On `reload\n`:
   - `fork()` + `execv()` a NEW worker (the rebuilt binary).
   - Save new PID. Briefly both are running.
   - Sleep 250 ms to let the kernel route a connection to the
     new worker (proving it's alive).
   - `kill(SIGTERM, old_pid)`.
   - `waitpid(old_pid, &status, WNOHANG)` in a 60-second loop;
     after that, `SIGKILL`.

The bridge between `mosaic dev` and the supervisor is a pipe
(supervisor reads `reload\n`-newline-terminated commands from
stdin).

### Phase 2 — net-http graceful shutdown

`Http1.Serve` / `Http1.ServeMt` need to handle `SIGTERM`
gracefully:

- Set a flag and close the listen fd. The accept loop bails out.
- Wait for in-flight handler frames to finish (single-threaded
  variant) or join all worker threads (`ServeMt`).
- WebSocket connections inside the WS frame loop: each `recv()`
  on the WS fd gets `EAGAIN` after a grace timeout; on timeout,
  close. Pre-timeout closes are graceful.
- Exit with status 0.

amalgame-net-http v0.7.x exits via `pthread_exit` in the worker
threads on `Http1.ServeMt`. Need to add `SIGTERM` handler that
flips an `atomic_bool stopping` flag the accept loop checks
every iteration.

### Phase 3 — `mosaic dev` integration

`tools/mosaic-dev.sh` switches from *kill + rebuild + restart*
to *rebuild + write `reload\n` to supervisor stdin*. The
supervisor handles the worker swap.

Three flags:
- `--no-supervise` — keep the v0.4 brutal reload (fallback for
  apps that don't yet handle SIGTERM gracefully).
- `--grace SECONDS` — how long the supervisor waits for an old
  worker to drain (default 60).
- `--overlap MS` — overlap window between spawning the new
  worker and SIGTERM-ing the old (default 250 ms).

## Migration

- amalgame-net-http needs a SIGTERM handler — v0.8.0.
- mosaic v0.5.0 ships the supervisor + the new mosaic-dev
  behaviour.
- The v0.4 brutal reload stays available behind
  `--no-supervise` for the entire v0.5.x window. Drop in v0.6.

## Estimated effort

- net-http v0.8.0 SIGTERM graceful shutdown — 0.5 day
- mosaic-supervise.c (~150 LoC) — 0.5 day
- mosaic-dev.sh integration + tests — 0.5 day
- Manual integration testing (DEV mode WS reconnect storm
  goes away) — 0.5 day

Total: ~2 days of focused engineering.

## Risks

- **Kernel REUSEPORT distribution**: connections arriving during
  the overlap may land on either worker. Documented as expected
  DEV behaviour.
- **Worker startup race**: if the new worker dies on startup
  (e.g. amc errors during the rebuild even though gcc
  succeeded), the supervisor needs to detect that BEFORE
  SIGTERM-ing the old worker. A 250 ms readiness probe (try
  `connect()` to the port) handles the common case; full
  liveness needs a `/healthz`-style endpoint.
- **WebSocket grace timeout**: 60 s default may be too short
  for long-lived browser tabs. Configurable, but the actual
  fix is for browsers to reconnect when their tab regains
  focus — already standard practice on the JS side.
