# Mosaic — Developer Guide

The complete reference for building a production web app on the
Amalgame stack with Mosaic.

> **Scope.** This guide assumes Linux or macOS; Windows users
> should run through WSL2.  Code samples target the stable
> versions as of the most recent release tag.

---

## Table of contents

- [1. Install](#1-install)
  - [1.1 Prerequisites](#11-prerequisites)
  - [1.2 amc (the compiler)](#12-amc-the-compiler)
  - [1.3 Mosaic CLI](#13-mosaic-cli)
  - [1.4 Verify](#14-verify)
- [2. Quickstart](#2-quickstart)
  - [2.1 Scaffold a project](#21-scaffold-a-project)
  - [2.2 Run it](#22-run-it)
  - [2.3 Project layout](#23-project-layout)
- [3. Configuration — `mosaic.toml`](#3-configuration--mosaictoml)
  - [3.1 Schema & override cascade](#31-schema--override-cascade)
  - [3.2 `[server]` — listener](#32-server--listener)
  - [3.3 `[tls]` — HTTPS / ACME](#33-tls--https--acme)
  - [3.4 `[sessions]`](#34-sessions)
  - [3.5 `[security.csrf]`, `[security.cors]`, `[security.headers]`, `[security.rate_limit]`](#35-security)
  - [3.6 `[limits]` & `[logging]`](#36-limits--logging)
- [4. Routes — filesystem routing](#4-routes--filesystem-routing)
  - [4.1 File → URL mapping](#41-file--url-mapping)
  - [4.2 HTTP verbs](#42-http-verbs)
  - [4.3 Dynamic params & splat](#43-dynamic-params--splat)
  - [4.4 `WebContext`: what you get in handlers](#44-webcontext-what-you-get-in-handlers)
  - [4.5 `HttpResponse` builder](#45-httpresponse-builder)
- [5. Middlewares](#5-middlewares)
- [6. Sessions & authentication](#6-sessions--authentication)
- [7. HTTPS with Let's Encrypt](#7-https-with-lets-encrypt)
- [8. WebSockets](#8-websockets)
- [9. Hot reload & DEV mode](#9-hot-reload--dev-mode)
- [10. Production deployment](#10-production-deployment)
- [11. Testing](#11-testing)
- [12. Reference & troubleshooting](#12-reference--troubleshooting)

---

## 1. Install

### 1.1 Prerequisites

| OS | Versions |
|---|---|
| Linux | glibc ≥ 2.35 (Debian 12, Ubuntu 22.04+, Fedora 36+, Mint 21+, …) |
| macOS | 12+ Apple Silicon (Rosetta 2 works for Intel Macs) |
| Windows | WSL2 with one of the supported Linux distros |

System packages — install once:

```sh
# Debian / Ubuntu / Mint
sudo apt install build-essential libgc-dev libssl-dev libcurl4-openssl-dev libnghttp2-dev

# Fedora
sudo dnf install gcc make gc-devel openssl-devel libcurl-devel libnghttp2-devel

# macOS (Homebrew)
brew install bdw-gc openssl@3 nghttp2 curl
```

`gcc` (or `cc`) is required at runtime — every Amalgame project goes
through it for the final link step.  `libgc` is the conservative
GC every binary uses.  `libssl` / `libnghttp2` / `libcurl` are
needed by `amalgame-tls`, `amalgame-net-http`, and HTTP clients.

### 1.2 amc (the compiler)

```sh
# Linux x86_64
VERSION=v0.8.39                         # pick latest from
                                        # https://github.com/amalgame-lang/Amalgame/releases
curl -sSL -o amc.tar.gz \
  "https://github.com/amalgame-lang/Amalgame/releases/download/${VERSION}/amc-${VERSION#v}-linux-x86_64.tar.gz"
tar -xzf amc.tar.gz -C "$HOME/.local" --strip-components 1
```

For macOS arm64 use `amc-${VERSION#v}-macos-arm64.tar.gz`; for
Windows use `amc-${VERSION#v}-windows-x86_64.zip` or the Inno
Setup `.exe` installer (both attached to the release).

Ensure `$HOME/.local/bin` is on your `$PATH`:

```sh
echo 'export PATH="$HOME/.local/bin:$PATH"' >> ~/.bashrc
exec bash
```

### 1.3 Mosaic CLI

```sh
curl -sSL https://raw.githubusercontent.com/amalgame-lang/mosaic/main/install.sh | bash
```

The installer downloads the **pre-built** tarball that matches your
host triple (Linux x86_64 or macOS arm64) from the most recent
GitHub Release, drops everything under `$HOME/.local/{bin,share}/`,
and is done.  No compile step.

If you're behind a proxy or want a specific version:

```sh
MOSAIC_VERSION=v0.5.3 MOSAIC_PREFIX=/usr/local \
  curl -sSL https://raw.githubusercontent.com/amalgame-lang/mosaic/main/install.sh | bash
```

If the pre-built tarball isn't available for your platform / tag,
the installer falls back to the source tarball and compiles
`mosaic-supervise` locally — `cc` from step 1.1 is enough.

### 1.4 Verify

```sh
$ amc --version
amc 0.8.39 (commit ..., built ...-05-21)
$ mosaic version
mosaic v0.5.3
$ mosaic help
mosaic — Amalgame web app build tool
  mosaic new [DIR]      ...
  mosaic build [...]    ...
  mosaic dev [...]      ...
  ...
```

---

## 2. Quickstart

### 2.1 Scaffold a project

```sh
mosaic new my-site
```

This creates `my-site/` with:

- `amalgame.toml` — package manifest with `[dependencies]` declaring
  `amalgame-web`, `-net-http`, `-tls`, `-crypto`.  `amc v0.8.39+`
  auto-pulls them transitively.
- `server.am` — the entry point.  Reads routes via `Routes.Register`
  (filesystem routing) and serves on `:3000`.
- `app/index.am` — sample `GET /` handler.
- `app/api/info.am` — sample `GET /api/info` returning JSON.
- `README.md`, `.gitignore`.

The `mosaic new` command also runs `amc package add web` so your
package cache is primed when you first run `mosaic dev`.  No
extra setup.

### 2.2 Run it

```sh
cd my-site
mosaic dev
```

`mosaic dev` watches `app/` + `server.am` + `amalgame.toml`,
re-runs the build whenever something changes, and restarts your
server.  Default port: `:3000`.  Browser open
[http://localhost:3000](http://localhost:3000).

The dev server also runs a tiny livereload daemon on `:35729`;
add `<script>const w=new WebSocket('ws://localhost:35729/');
w.onmessage=e=>e.data==='reload'?location.reload():null;</script>`
to your HTML and the browser auto-refreshes after every rebuild.

For a one-shot production binary instead:

```sh
mosaic build               # → ./server
./server                   # listens on :3000
```

### 2.3 Project layout

The canonical layout `mosaic new` produces:

```
my-site/
├── amalgame.toml         # package manifest + [dependencies]
├── mosaic.toml           # (optional) framework config — see §3
├── server.am             # Main entrypoint
├── app/                  # filesystem routes
│   ├── index.am          # GET /
│   ├── about.am          # GET /about
│   ├── users/
│   │   ├── index.am      # GET /users
│   │   └── [id].am       # GET /users/:id
│   └── api/
│       └── info.am       # GET /api/info
├── _routes.am            # generated by mosaic-routes.sh (auto-regen)
├── public/               # (optional) static files — served by Static middleware
├── certs/                # cert cache (created by Acme.EnsureCert*)
└── server                # built binary (gitignored)
```

`_routes.am` is regenerated from `app/` on every `mosaic build` /
`mosaic dev`.  Don't edit by hand — the next build will overwrite
your changes.

---

## 3. Configuration — `mosaic.toml`

### 3.1 Schema & override cascade

A Mosaic app reads its config from `mosaic.toml` at the project
root.  The file is **optional** — if absent, every feature uses
its library default.  See
[docs/mosaic-configuration.md](https://github.com/amalgame-lang/Amalgame/blob/main/docs/mosaic-configuration.md)
in the Amalgame repo for the canonical per-key reference.

There are **six** override layers, lowest precedence first:

```
1. Library default        — the class constructor
2. mosaic.toml            — FromMap on each feature's section
3. env vars               — MOSAIC_<SECTION>_<KEY>
4. CLI flags              — mosaic dev --listen :8080
5. Application code       — builder chained in main.am
6. Per-route handler      — resp.Header(...) wins over middleware
```

"Code" is two layers: the library default (constructor) is the
floor, your `main.am` chaining is the middle, and per-route
handler calls are the ceiling.  Handler-wins is intentional —
one route can override a global policy without disabling the
policy elsewhere.

### 3.2 `[server]` — listener

```toml
[server]
listen      = [":3000"]              # bind addresses; multiple = multi-port
workers     = 0                      # HTTPS thread-pool size. 0 = library default (512)
queue_size  = 0                      # kernel listen() backlog on :443. 0 = OS default
```

Mosaic single-binds the first entry of `listen` via
`MosaicServer.ServeHttps`.  Since `amalgame-web` v0.37.0 that path
serves over a **bounded worker pool** (fixed-size thread pool —
512 for HTTPS, 64 for the `:80` redirect — via `HttpsH1Server.ServePool`),
so requests are handled concurrently and the "needs threading" caveat
is gone.  As of `amalgame-web` v0.38.0 the `workers` / `queue_size`
knobs are **wired through** — `mosaic serve` reads them and calls
`MosaicServer.WithWorkers(n)` / `WithBacklog(n)` to size the HTTPS pool
and listen backlog (`0` keeps the library defaults).  Still pending:
multi-port binding of the remaining `listen` entries.

### 3.3 `[tls]` — HTTPS / ACME

Three top-level modes:

- `mode = "off"` — plain HTTP only.
- `mode = "files"` — pre-existing PEM cert+key on disk.
- `mode = "acme"` — auto-provision via Let's Encrypt RFC 8555.

Full schema:

```toml
[tls]
mode         = "off"                 # "off" | "files" | "acme"
min_version  = "1.3"                 # "1.2" | "1.3"  — TLS protocol floor
alpn         = "h2"                  # comma-separated.  "h2,http/1.1" planned
cert_file    = "./certs/site.pem"    # mode = "files"
key_file     = "./certs/site.key"    # mode = "files"

[tls.acme]
enabled       = true                 # mode = "acme"
domain        = "example.com"        # single domain, legacy
domains       = "example.com, www.example.com, api.example.com"
                                      # comma-separated, multi-SAN (v0.11.1+).
                                      # `domains` wins over `domain` when both set.
email         = "ops@example.com"    # required by LE
cert_dir      = "./certs"            # cert cache
acme_server   = ""                   # "" = LE production
                                      # ex: "https://acme-staging-v02.api.letsencrypt.org/directory"
certbot_path  = ""                   # legacy v0.2.x certbot wrapper.  Empty + AcmeNative = native RFC 8555
```

In your code:

```amalgame
import Amalgame.Web         // AcmeConfig, TlsBindingConfig
import Amalgame.Tls         // AcmeNative
import Amalgame.Net.Http    // Https, HttpServerConfig

public class Program {
    public static int Main(string[] args) {
        // 1) Load + validate ACME config
        let acme = AcmeConfig.FromMap(loadedTomlAcmeSection)
        let err  = acme.Validate()
        if (String_Length(err) > 0) {
            Console.WriteError(err); return 2
        }

        // 2) Provision cert (native ACME, no certbot binary needed)
        if (acme.Enabled) {
            let rc = AcmeNative.EnsureCert(acme.DomainsCsv(),
                                           acme.Email,
                                           acme.CertDir,
                                           acme.AcmeServer)
            if (rc != 0) { Console.WriteError("ACME failed"); return 3 }
        }

        // 3) TLS knobs (min version + ALPN)
        let tls = TlsBindingConfig.FromMap(loadedTomlTlsSection)
        let cfg = HttpServerConfig.Default()
                    .WithTlsMinVersion(tls.MinVersionInt())
                    .WithTlsAlpn(tls.Alpn)

        // 4) Serve
        Https.ServeWith(443, acme.CertPath(), acme.KeyPath(), cfg, handler)
        return 0
    }
}
```

### 3.4 `[sessions]`

Two orthogonal axes:

- **`strategy`** — *where* the session lives:
  - `server_side` (default) — id in cookie, data on the server
  - `encrypted_cookie` — data IS the cookie, AEAD-encrypted with
    `amalgame-crypto`
- **`backend`** — only when `strategy = "server_side"`:
  - `memory` (default) — in-process Map, thread-safe under
    `Http1.ServeMt`
  - `redis` — multi-worker / multi-node, via
    `amalgame-database-nosql-redis`
  - `shm` — alias of `memory` (kept for documentation continuity)

```toml
[sessions]
strategy       = "server_side"
backend        = "memory"            # or "redis"
cookie_name    = "MOSAICSID"
ttl_sec        = 86400               # 24h default
secret         = "32-byte-hex-or-shorter-plain"
                                     # base for signed/encrypted cookies
secure         = true                # send Secure attribute (HTTPS only)
httponly       = true                # send HttpOnly attribute (recommended)
samesite       = "Lax"               # "Lax" | "Strict" | "None"
encrypted      = false               # under strategy = encrypted_cookie:
                                     # false = HMAC-signed (v0.8.3)
                                     # true  = AES-256-GCM AEAD (v0.8.5)

# When backend = "redis"
[sessions.redis]
url            = "redis://localhost:6379/0"
prefix         = "sess:"
```

### 3.5 Security

```toml
[security.csrf]
enabled         = true
header_name     = "X-CSRF-Token"
cookie_name     = "csrftoken"
token_ttl_sec   = 7200
safe_methods    = ["GET", "HEAD", "OPTIONS"]

[security.cors]
enabled         = false
allowed_origins = ["https://example.com"]
allowed_methods = ["GET", "POST", "PUT", "DELETE"]
allowed_headers = ["Content-Type", "Authorization"]
allow_credentials = true
max_age_sec     = 600

[security.headers]
preset          = "strict_html"      # "strict_html" | "strict_api" | "off"
hsts            = "max-age=31536000; includeSubDomains; preload"
csp             = "default-src 'self'; img-src 'self' data:;"
x_frame_options = "DENY"

[security.rate_limit]
enabled    = true
backend    = "memory"                # or "redis"
window_sec = 60
limit      = 100                     # requests per IP per window
key        = "ip"                    # "ip" | "session" | "header:X-API-Key"
```

### 3.6 `[limits]` & `[logging]`

```toml
[limits]
max_body_bytes       = 1048576       # 1 MiB
max_header_bytes     = 8192
max_url_bytes        = 2048
header_timeout_sec   = 5             # slowloris guard
body_timeout_sec     = 30
idle_timeout_sec     = 60            # keep-alive close
listen_backlog       = 64

[logging]
level     = "info"                   # "debug" | "info" | "warn" | "error"
format    = "logfmt"                 # "logfmt" | "json"
access    = true                     # access log per request
output    = "stdout"                 # or "/var/log/mosaic.log"
```

---

## 4. Routes — filesystem routing

### 4.1 File → URL mapping

```
app/index.am             →  GET  /
app/about.am             →  GET  /about
app/blog.am              →  GET  /blog
app/blog/index.am        →  GET  /blog/         (note the trailing slash variant)
app/users/[id].am        →  GET  /users/:id     (dynamic param)
app/files/[...path].am   →  GET  /files/*path   (splat, matches everything)
```

The `[name]` brackets become `:name` route params; `[...name]`
becomes `*name` splat.

### 4.2 HTTP verbs

Each `app/*.am` file declares a `public class Page` with one
static method per supported verb:

```amalgame
// app/users/[id].am  → GET /users/:id, POST /users/:id

import Amalgame.Web

public class Page {
    public static HttpResponse GET(ctx: WebContext) {
        let id = ctx.Param("id")
        return HttpResponse.New().Text("user " + id)
    }
    public static HttpResponse POST(ctx: WebContext) {
        let id = ctx.Param("id")
        let body = ctx.Request.Body
        return HttpResponse.New().Status(201).Text("created " + id)
    }
}
```

Methods that don't appear on the class return `405 Method Not
Allowed` automatically (with the right `Allow:` header listing
the verbs that DO exist).

### 4.3 Dynamic params & splat

```amalgame
// app/posts/[year]/[slug].am  →  GET /posts/:year/:slug

public class Page {
    public static HttpResponse GET(ctx: WebContext) {
        let year = ctx.Param("year")
        let slug = ctx.Param("slug")
        return HttpResponse.New().Text(year + " / " + slug)
    }
}
```

Splat — captures the rest of the URL:

```amalgame
// app/files/[...path].am  →  GET /files/*path

public class Page {
    public static HttpResponse GET(ctx: WebContext) {
        let path = ctx.Param("path")        // "css/site.css" for /files/css/site.css
        return HttpResponse.New().Text(path)
    }
}
```

Param values are URL-decoded.

### 4.4 `WebContext`: what you get in handlers

```amalgame
ctx.Request: HttpRequest               // method, path, headers, body
ctx.Param(name: string): string        // route param value, "" if absent
ctx.Query(name: string): string        // ?name=value
ctx.Session: Session                   // current session (created on demand)
ctx.RemoteAddr: string                 // client IP (X-Forwarded-For aware
                                       //   under reverse proxy)
ctx.Cookie(name: string): string       // request cookie, "" if absent
ctx.HeaderIn(name: string): string     // request header, case-insensitive
```

### 4.5 `HttpResponse` builder

Chainable fluent API:

```amalgame
return HttpResponse.New()
    .Status(200)
    .Header("content-type", "application/json")
    .SetCookie("flash", "saved", 60)          // 60-sec cookie
    .Text("{\"ok\":true}")

// shortcuts
return HttpResponse.New().Html("<h1>Hi</h1>")
return HttpResponse.New().Json("{\"ok\":true}")           // sets Content-Type
return HttpResponse.New().Redirect("/login", 302)
return HttpResponse.New().Status(404).Text("Not found")
```

---

## 5. Middlewares

Middlewares wrap the router.  In Mosaic v0.8+ `WebApp` chains them
in a fixed but configurable order — you don't write the chain
yourself; `WebApp.WithX(cfg)` plugs each one in.

```amalgame
import Amalgame.Web

let app = new WebApp()
    .WithRouter(router)                    // your routes
    .WithSession(SessionStore.FromMap(s))
    .WithCsrf(CsrfConfig.FromMap(c))
    .WithCors(CorsConfig.FromMap(c))
    .WithRateLimit(RateLimitConfig.FromMap(r))
    .WithSecurityHeaders(SecurityHeadersConfig.FromMap(h))
    .WithLogging(LogConfig.FromMap(l))

app.ServeMt(3000)                          // multi-thread; or Serve(...) for single
```

The order — outer to inner: logging → rate limit → CORS → CSRF
→ security headers → session → router.  Each middleware is
opt-in (omit the `.WithX(...)` call and that layer is a no-op).

---

## 6. Sessions & authentication

The simplest server-side session, memory-backed:

```amalgame
let store = new MemorySessionStore()
let app = new WebApp().WithSession(store)

// In a handler:
public static HttpResponse POST(ctx: WebContext) {
    ctx.Session.Set("user_id", "42")
    return HttpResponse.New().Redirect("/dashboard")
}

// Read on subsequent requests:
public static HttpResponse GET(ctx: WebContext) {
    let uid = ctx.Session.Get("user_id")
    if (String_Length(uid) == 0) {
        return HttpResponse.New().Redirect("/login")
    }
    return HttpResponse.New().Text("hello " + uid)
}
```

For multi-worker or multi-node, swap the store for Redis:

```amalgame
let store = RedisSessionStore.FromMap(redisCfg)
let app = new WebApp().WithSession(store)
```

For stateless sessions (data IS the cookie), use
`SignedCookieSessionStore`:

```amalgame
let store = SignedCookieSessionStore.FromMap(sessionCfg)
// sessionCfg.encrypted = true enables AES-256-GCM AEAD;
// false uses HMAC-signed plaintext (still tamper-proof, but readable).
```

Use case picker:

| Need | Store |
|---|---|
| Tiny app, one process | `MemorySessionStore` |
| Multi-worker / multi-node | `RedisSessionStore` |
| Stateless, signed | `SignedCookieSessionStore` (encrypted = false) |
| Stateless, encrypted | `SignedCookieSessionStore` (encrypted = true) |

---

## 7. HTTPS with Let's Encrypt

End-to-end pattern using the native ACME (no certbot binary):

```amalgame
import Amalgame.Tls         // AcmeNative
import Amalgame.Web         // AcmeConfig, TlsBindingConfig
import Amalgame.Net.Http    // Https, HttpServerConfig

public class Program {
    public static int Main(string[] args) {
        // Loaded from mosaic.toml's [tls.acme] section
        let acme = AcmeConfig.FromMap(tomlAcmeSection)
        let tls  = TlsBindingConfig.FromMap(tomlTlsSection)

        if (acme.Enabled) {
            // First-run on a real domain: spawns a tiny HTTP listener
            // on :80, does the http-01 dance, writes the cert.  Idem-
            // potent — subsequent runs reuse the cached account.
            let rc = AcmeNative.EnsureCert(acme.DomainsCsv(),
                                           acme.Email,
                                           acme.CertDir,
                                           acme.AcmeServer)
            if (rc != 0) { return 3 }
        }

        let cfg = HttpServerConfig.Default()
                    .WithTlsMinVersion(tls.MinVersionInt())
        let handler = conn => { /* ... */ return 0 }
        return Https.ServeWith(443, acme.CertPath(), acme.KeyPath(),
                               cfg, handler)
    }
}
```

**Iterate on the staging endpoint first** to avoid LE prod
rate limits (5 issuances / week / domain):

```toml
[tls.acme]
acme_server = "https://acme-staging-v02.api.letsencrypt.org/directory"
```

Browsers don't trust staging certs by default — test with
`curl -k https://your-domain/`.  Switch to `""` (LE prod) once
the flow works.

Renewal: re-run the same binary.  The account key + URL are
cached at `<cert_dir>/account.key` / `account.url`.  Cron /
systemd-timer every 60 days (LE certs valid 90 days).

**Prerequisites for any real LE flow:**

- A public domain whose A/AAAA records point at this machine.
- Ports 80 + 443 free.  Either run as root or:
  ```sh
  sudo setcap 'cap_net_bind_service=+ep' ./server
  ```

For a single-file working example, see
[amalgame-tls/examples/letsencrypt-site](https://github.com/amalgame-lang/amalgame-tls/tree/main/examples/letsencrypt-site).

---

## 8. WebSockets

```amalgame
import Amalgame.Net.Http

public class Program {
    public static void Main(string[] args) {
        let handler = conn => {
            while (!WsConn.IsClosed(conn)) {
                let msg = WsConn.ReceiveText(conn)
                if (msg == "") { break }
                WsConn.SendText(conn, "echo: " + msg)
            }
            return 0
        }
        Ws.Serve(8080, handler)
    }
}
```

For TLS-wrapped WebSocket (`wss://`):

```amalgame
Wss.Serve(8443, "./certs/site.pem", "./certs/site.key", handler)
```

Frame parser supports the full RFC 6455 frame set — TEXT, BINARY,
CLOSE, PING, PONG.  PING is auto-answered with PONG transparently.
Fragmentation isn't supported (rare in practice for messages < 64 KB).

---

## 9. Hot reload & DEV mode

Plain mode (default):

```sh
mosaic dev
```

`mosaic dev` watches `app/` + `server.am` + `amalgame.toml` via
POSIX `find -newer` polling every 500 ms.  On every save, it
runs `mosaic build`, kills the old server, starts the new one.

**WebSocket-surviving hot reload (v0.5+):**

```sh
mosaic dev --supervise
```

Under `--supervise`, Mosaic spawns a `mosaic-supervise` process
once.  After every successful rebuild it tells the supervisor to
do this:

1. Spawn the new worker (binds the port via `SO_REUSEPORT`).
2. Wait `--supervise-overlap-ms` (default 250) so the kernel
   routes a few connections to the new worker (proves it's alive).
3. SIGTERM the OLD worker.  amalgame-net-http v0.8.0+ handles
   SIGTERM gracefully: stops accepting, lets in-flight requests
   + open WebSocket connections drain up to `--supervise-grace`
   seconds (default 60), then exits.

Browser WebSocket connections accepted by the OLD worker stay
alive through the rebuild.  When the WS client itself reconnects
or the grace timer fires, the OLD worker exits.

Useful flags:

| Flag | Default | Meaning |
|---|---|---|
| `--supervise` | off | enable the hot-reload supervisor |
| `--supervise-grace SECONDS` | 60 | max drain time for the old worker |
| `--supervise-overlap-ms MS` | 250 | new-worker liveness window |
| `--no-livereload` | (on) | drop the WS browser-refresh daemon |
| `--livereload-port N` | 35729 | override the livereload daemon's port |
| `--no-rebuild` | off | don't build on startup (use existing ./server) |

---

## 10. Production deployment

> Deploying the **config-driven** server (`mosaic serve mosaic.toml`)
> rather than a hand-built `./server`? Skip the hand-written unit:
> `mosaic service install` generates and registers it for you across
> systemd / launchd / Windows SCM (`mosaic service --help`). The manual
> unit below is for a custom `./server` binary you compiled yourself.

A minimal systemd unit:

```ini
# /etc/systemd/system/my-site.service
[Unit]
Description=My Mosaic site
After=network.target

[Service]
Type=simple
User=mosaic
WorkingDirectory=/srv/my-site
ExecStart=/srv/my-site/server
Restart=on-failure
RestartSec=2s

# bind low ports without running as root
AmbientCapabilities=CAP_NET_BIND_SERVICE
CapabilityBoundingSet=CAP_NET_BIND_SERVICE

# read-only system, except for the cert + data dirs
ProtectSystem=strict
ReadWritePaths=/srv/my-site/certs /srv/my-site/data
ProtectHome=true
NoNewPrivileges=true
PrivateTmp=true

[Install]
WantedBy=multi-user.target
```

Build the binary on your dev machine, scp it to the server
alongside `certs/` and any `data/` your app needs.

```sh
mosaic build -o ./dist/server
scp dist/server my-server:/srv/my-site/
ssh my-server 'sudo systemctl restart my-site'
```

For zero-downtime deploys, run two systemd units (`-blue` and
`-green`) behind the supervisor's SO_REUSEPORT — same pattern
as `mosaic dev --supervise`, productionised.  This pattern is
sketched out in
[mosaic/docs/proposals/hot-reload.md](https://github.com/amalgame-lang/mosaic/blob/main/docs/proposals/hot-reload.md).

---

## 11. Testing

Mosaic apps are AM programs — write tests as `tests/*.am` files
with one Main per case, drive them with `amc -o tests/$name
tests/$name.am && ./tests/$name`.  The amalgame-web repo's
`tests/run_tests.sh` is the canonical pattern.

For HTTP-level tests, point a regular HttpClient at your test
server:

```amalgame
import Amalgame.Net.Http

let resp = HttpClient.Get("http://localhost:3000/users/42")
if (resp.Status != 200) {
    Console.WriteError("expected 200, got " + String_FromInt(resp.Status))
}
```

Integration tests can spin a server in a background thread (via
amalgame-threading) and tear it down with
`Http1.RequestShutdown()` after each suite.

---

## 12. Reference & troubleshooting

### Error codes

| `int` from `Http*.Serve*` | Meaning |
|---|---|
| 0  | clean shutdown — process received SIGTERM, drained, exited |
| -1 | handler is NULL (program bug) |
| -2 | `bind()` failed — port in use or unprivileged |
| -3 | OpenSSL not compiled in (Https.* paths only) |
| -4 | cert/key file not readable or didn't match |

| `int` from `AcmeNative.EnsureCert` | Meaning |
|---|---|
| 0  | success — cert written to disk |
| -1 | invalid args (empty domain / email / dir) |
| -2 | ACME HTTPS / directory / account / order setup failure |
| -3 | http-01 challenge failed (port 80 busy or DNS doesn't resolve) |
| -4 | finalize / cert download failed |

### Common errors at install / build time

| Symptom | Cause | Fix |
|---|---|---|
| `error[resolver]: Unknown symbol 'JwsKey'` | amc < 0.8.39, no transitive deps | `amc package add crypto net-http tls` manually, or upgrade amc |
| `cc1: fatal error: Amalgame_Net_Http.h: No such file` | `mosaic build` ran with a stale package cache | `amc package add net-http --force` |
| `Acme.EnsureCert: SpawnChallengeServer failed (port 80 in use?)` | Another process holds :80 | `sudo ss -tlnp \| grep :80` to find it; stop or `setcap` your binary |
| `mosaic-supervise: ... GLIBC_2.38 not found` | Pre-built tarball compiled on a newer Ubuntu than your host | Upgrade the host, or `MOSAIC_VERSION=v0.5.2+` (built on glibc 2.35) |
| `amc package add` waits forever / 404s | raw.githubusercontent.com CDN cache (~5min) | Wait, or re-run after the cache expires |

### Useful environment variables

| Var | Used by | Purpose |
|---|---|---|
| `AMC` | mosaic-build / dev | Absolute path to amc binary |
| `AMC_RUNTIME` | mosaic-build | Override amc's runtime/ dir |
| `AMALGAME_PACKAGES_DIR` | amc | Override `~/.amalgame/packages/` cache root |
| `MOSAIC_PREFIX` | install.sh | Install prefix (default `$HOME/.local`) |
| `MOSAIC_VERSION` | install.sh | Tag to install (default: latest release) |
| `MOSAIC_TLS_ACME_SERVER` | amalgame-tls | Override ACME directory URL |
| `MOSAIC_TLS_CERTBOT_PATH` | amalgame-tls | Path to legacy certbot (rarely needed) |

### Where each piece lives

| Repo | Versions | Purpose |
|---|---|---|
| [amalgame-lang/Amalgame](https://github.com/amalgame-lang/Amalgame) | v0.8.89+ | `amc` compiler |
| [amalgame-lang/amalgame-crypto](https://github.com/amalgame-lang/amalgame-crypto) | v0.8.0+ | SHA-256 / HMAC / AES-256-GCM / JwsKey / scrypt |
| [amalgame-lang/amalgame-tls](https://github.com/amalgame-lang/amalgame-tls) | v0.3.5+ | TLS + ACME (native RFC 8555) |
| [amalgame-lang/amalgame-net-http](https://github.com/amalgame-lang/amalgame-net-http) | v0.29.2+ | HTTP/1, HTTP/2, HTTPS, WS, WSS + graceful shutdown |
| [amalgame-lang/amalgame-web](https://github.com/amalgame-lang/amalgame-web) | v0.37.0+ | Router, sessions, middlewares, WebApp, `MosaicServer` (bounded worker pool) |
| [amalgame-lang/mosaic](https://github.com/amalgame-lang/mosaic) | v0.5.3+ | this build tool |

Optional capability packages (mount on a Mosaic host as needed):

| Repo | Versions | Purpose |
|---|---|---|
| [amalgame-lang/amalgame-net-webdav](https://github.com/amalgame-lang/amalgame-net-webdav) | v0.6.1+ | WebDAV server + multi-user NAS (`WebDavNas`) |
| [amalgame-lang/amalgame-auth](https://github.com/amalgame-lang/amalgame-auth) | v0.2.0+ | HTTP Basic auth, multi-user scrypt credentials, anti-bruteforce `LoginGuard` |
| [amalgame-lang/amalgame-net-proxy](https://github.com/amalgame-lang/amalgame-net-proxy) | v0.2.1+ | Reverse proxy + load balancing (RR / IP-hash / least-conn) |
| [amalgame-lang/amalgame-net-stream](https://github.com/amalgame-lang/amalgame-net-stream) | v0.3.0+ | TCP + UDP raw proxy with LB + IPv6 |

### Getting help

- **Bugs**: each repo has an issue tracker.
- **Questions about the design**: `docs/proposals/` in each repo
  has the long-form rationale for any non-obvious choice.
- **Roadmap**:
  [docs/proposals/beyond-http.md](https://github.com/amalgame-lang/Amalgame/blob/main/docs/proposals/beyond-http.md)
  tracks the nginx/apache-equivalent capabilities. **Shipped:**
  reverse proxy + load balancing (`[[proxy]]`, RR / IP-hash /
  least-conn), static files, SSE, TCP + UDP raw proxy with
  load-balancing + IPv6 (`amalgame-net-stream` v0.3.0 — `TcpProxy` +
  `UdpProxy`, multi-upstream RR / ip-hash / least-conn), WebDAV +
  multi-user NAS (`amalgame-net-webdav` v0.7.0 — `WebDav` Class 1 +
  opt-in Class 2 locks, plus `WebDavNas` for per-user `/home` + shared
  `/shared`, streaming PUT to disk, live `oc:size` directory sizes,
  RFC 3986 href encoding; **CalDAV/CardDAV Phase 1** — MKCALENDAR,
  `calendar-multiget`/`-query` REPORT, ETags/getctag,
  `.well-known` + per-account discovery so clients sync via the account
  URL), HTTP Basic auth (`amalgame-auth` v0.2.0 —
  multi-user scrypt credentials, LAN bypass, anti-bruteforce
  `LoginGuard`). **Partial:** gRPC unary, SMTP *client*, CalDAV
  `calendar-query` time-range filtering + `sync-collection`. **Not built
  yet:** SMTP *server* / IMAP / POP3, SFTP, VPN/WireGuard, DNS/DoH, CDN.
