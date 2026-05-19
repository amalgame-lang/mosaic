# Mosaic

Build tool & CLI for the Amalgame web stack
([amalgame-web](https://github.com/amalgame-lang/amalgame-web) +
[amalgame-net-http](https://github.com/amalgame-lang/amalgame-net-http) +
[amalgame-tls](https://github.com/amalgame-lang/amalgame-tls)).

Mosaic is **not a package**. It's a standalone tool you install once
and run from any project to:

- Scan an `app/` directory and generate a `_routes.am` glue module
  (Next.js App Router style)
- Drive `amc` + `gcc` end-to-end with the right package archives
- **DEV mode** (v0.2+) — `mosaic dev` watches your source tree
  and rebuilds + restarts the server on every save
- *(planned v0.3+)* hot-reload via `dlopen` of a freshly-built
  `app.so` so in-flight WebSocket connections survive
- *(planned v0.3+)* scaffold new Mosaic apps with `mosaic new`

`amalgame-web` is the **runtime library** (Router / Session /
WebContext). Mosaic is the **build tool**. Different lifecycle,
different install path.

## Install

```bash
curl -sSL https://raw.githubusercontent.com/amalgame-lang/mosaic/main/install.sh | bash
```

Drops two scripts into `~/.local/bin`:

- `mosaic-routes.sh` — scan `app/` → generate `_routes.am`
- `mosaic-build.sh`  — full pipeline (routes → amc → gcc → binary)

Override the install dir with `MOSAIC_PREFIX=/usr/local` (or
similar). Make sure `$MOSAIC_PREFIX/bin` is on your `$PATH`.

## Quickstart

```bash
# 1. set up package deps
amc package add web@v0.2.1
amc package add net-http@v0.2.1

# 2. drop route files in app/
mkdir -p app/users app/api
cat > app/index.am <<'AM'
import Amalgame.Web
public class Page {
    public static HttpResponse GET(WebContext ctx) {
        return HttpResponse.New().Html("<h1>Hello Mosaic</h1>")
    }
}
AM
cat > 'app/users/[id].am' <<'AM'
import Amalgame.Web
public class Page {
    public static HttpResponse GET(WebContext ctx) {
        return HttpResponse.New().Text("user " + ctx.Param("id"))
    }
}
AM

# 3. write server.am — see examples/mosaic-fs-demo/server.am

# 4. DEV mode — watches app/ + server.am, rebuilds + restarts on every save
mosaic dev

# or for a one-shot build:
mosaic build
./server
```

## Commands

- **`mosaic dev`** — DEV mode. Builds, runs `./server`, then polls
  the source tree every 500 ms (no inotify-tools dependency). On
  every save the server is killed, the project rebuilt, and a
  fresh server brought up. Ctrl-C to stop cleanly.
- **`mosaic build`** — one-shot end-to-end build: regen routes,
  amc → C, gcc → binary. Picks up package archives via
  `amalgame.lock`.
- **`mosaic routes [DIR]`** — just regen `_routes.am` from the
  given app dir (default `./app`). Mostly useful for debugging
  the generator.

`mosaic-build.sh` reads `amalgame.lock`, finds the cached package
archives, precompiles any missing facades, and links the final
binary. No manual `gcc` invocation, no juggling of `-I` and `-l`
flags.

## Convention (filesystem routing)

```text
app/index.am          → GET /
app/about.am          → GET /about
app/users/[id].am     → GET /users/:id  (param)
app/api/info.am       → GET /api/info   (+ POST/PUT/... if declared)
app/blog/[...slug].am → GET /blog/*slug (catch-all)
```

Each route file declares **one `public class Page`** with one
static method per HTTP verb. No `namespace` declaration — the
generator puts everything into `App` and renames each `class Page`
to `<PathPrefix>_Page` (so `app/users/[id].am`'s `class Page`
becomes `Users_Id_Page`) to keep things collision-free.

```amalgame
// app/users/[id].am
import Amalgame.Web

public class Page {
    public static HttpResponse GET(WebContext ctx) {
        return HttpResponse.New().Text("user " + ctx.Param("id"))
    }
    public static HttpResponse POST(WebContext ctx) {
        return HttpResponse.New().Status(201).Text("created")
    }
}
```

## What's NOT in v0.1 yet

- **`mosaic dev` (file watcher + hot reload).** Today every edit
  needs a full rebuild + restart. v0.2 plans `gcc -shared -fPIC` →
  `app.so` + inotify + `dlopen` hot-swap so route changes don't
  need a process restart. The dlopen path also unblocks proper
  source mapping (errors point at `app/X.am` directly instead of
  the generated `_routes.am`).
- **`mosaic new` scaffolding.** v0.3.
- **A real `mosaic` binary.** Today the CLI is two bash scripts;
  v0.4+ will rewrite them as proper AM code (`src/*.am`) compiled
  to a `mosaic` binary shipped via release tarball, mirroring how
  `amc` itself is distributed.

## Demo

See [`examples/mosaic-fs-demo/`](examples/mosaic-fs-demo/) for a
working filesystem-routed app served over HTTP/2 (h2c) via
`amalgame-net-http v0.2`'s `Http2.Serve`. Four routes — static,
dynamic `[id]`, nested, multi-verb POST — every response shows
`HTTP/2 2xx` on the wire.

## License

Apache-2.0. See [`LICENSE`](LICENSE).
