#!/bin/bash
# ─────────────────────────────────────────────────────────────────
#  tools/mosaic-dev.sh — DEV mode: watch app/ + rebuild + restart
#
#  Two reload modes, picked by --supervise:
#
#  Default (brutal): kill + rebuild + restart. Simple, but drops
#                    every in-flight HTTP request + WebSocket on
#                    every save.
#
#  --supervise (v0.5+): spawn `mosaic-supervise` once, pipe
#                    `reload\n` commands to it on each rebuild.
#                    The supervisor starts a new worker on the same
#                    port (via SO_REUSEPORT, requires amalgame-net-
#                    http v0.8.0+), waits a short overlap window,
#                    then SIGTERMs the old worker. The old worker
#                    drains in-flight WebSocket connections up to
#                    --supervise-grace seconds before exiting.
#                    WebSocket clients survive the rebuild.
#
#  No filewatcher dependency (inotify-tools / fswatch) — POSIX
#  `find -newer ./server` works everywhere. Costs a tiny bit of
#  CPU but for ~10 .am files the poll is sub-millisecond.
#
#  Usage:
#    mosaic dev [--app <dir>] [--entry <file>] [-o <out>]
#               [--port <int>] [--no-rebuild]
#               [--supervise] [--supervise-grace <s>]
#               [--supervise-overlap-ms <ms>]
#
#  Env: AMC, AMC_RUNTIME (passed through to mosaic-build.sh)
# ─────────────────────────────────────────────────────────────────

set -euo pipefail

APP_DIR="app"
ENTRY="server.am"
OUT=""
PORT=""
SKIP_INITIAL=false
LIVERELOAD=true
LIVERELOAD_PORT=35729
SUPERVISE=false
SUPERVISE_GRACE=60
SUPERVISE_OVERLAP_MS=250

while [ $# -gt 0 ]; do
    case "$1" in
        --app)              APP_DIR="$2"; shift 2 ;;
        --entry)            ENTRY="$2";   shift 2 ;;
        -o)                 OUT="$2";     shift 2 ;;
        --port)             PORT="$2";    shift 2 ;;
        --no-rebuild)       SKIP_INITIAL=true; shift ;;
        --no-livereload)    LIVERELOAD=false; shift ;;
        --livereload-port)  LIVERELOAD_PORT="$2"; shift 2 ;;
        --supervise)        SUPERVISE=true; shift ;;
        --supervise-grace)      SUPERVISE_GRACE="$2"; shift 2 ;;
        --supervise-overlap-ms) SUPERVISE_OVERLAP_MS="$2"; shift 2 ;;
        -h|--help)
            sed -n '2,28p' "$0" | sed 's/^# \{0,1\}//'
            exit 0 ;;
        *) echo "unknown arg: $1" >&2; exit 2 ;;
    esac
done

[ -z "$OUT" ] && OUT="./$(basename "${ENTRY%.am}")"
SCRIPT_DIR="$(cd "$(dirname "$(readlink -f "$0")")" && pwd)"

if [ -t 1 ]; then
    GREEN='\033[0;32m'; YELLOW='\033[0;33m'; CYAN='\033[0;36m'
    RED='\033[0;31m'; DIM='\033[2m'; NC='\033[0m'
else
    GREEN=''; YELLOW=''; CYAN=''; RED=''; DIM=''; NC=''
fi
say()  { echo -e "${CYAN}[dev]${NC} $1"; }
warn() { echo -e "${YELLOW}[dev]${NC} $1" >&2; }
err()  { echo -e "${RED}[dev]${NC} $1" >&2; }

# ── Server lifecycle ────────────────────────────────────────────
#
# Two modes, selected by --supervise:
#
#   default (v0.4 brutal): kill + rebuild + start. Drops every
#                          in-flight HTTP request + WebSocket
#                          connection on every reload.
#
#   --supervise           (v0.5+): spawn mosaic-supervise once, pipe
#                          `reload\n` commands to it on each rebuild.
#                          Supervisor starts a new worker on the same
#                          port (via SO_REUSEPORT, requires amalgame-
#                          net-http v0.8.0+), waits an overlap window
#                          then SIGTERMs the old worker. The old
#                          worker drains in-flight WS connections up
#                          to --supervise-grace seconds before exiting.
#                          WebSocket clients survive the rebuild.

SERVER_PID=""
SUPERVISE_PID=""
SUPERVISE_FIFO=""

stop_server() {
    if [ -n "$SERVER_PID" ] && kill -0 "$SERVER_PID" 2>/dev/null; then
        kill "$SERVER_PID" 2>/dev/null || true
        # Give it a moment to release the port cleanly. SO_REUSEADDR
        # would in theory let us skip this, but TIME_WAIT can still
        # bite on rapid restart of the same port.
        for _ in 1 2 3 4 5; do
            kill -0 "$SERVER_PID" 2>/dev/null || break
            sleep 0.1
        done
        kill -9 "$SERVER_PID" 2>/dev/null || true
        wait "$SERVER_PID" 2>/dev/null || true
    fi
    SERVER_PID=""
}

start_server() {
    "$OUT" &
    SERVER_PID=$!
    say "server PID=${SERVER_PID}"
}

# Supervisor-mode lifecycle.
start_supervisor() {
    local sup_bin=""
    for cand in \
        "$SCRIPT_DIR/mosaic-supervise" \
        "$SCRIPT_DIR/../libexec/mosaic-supervise" \
        "$(command -v mosaic-supervise 2>/dev/null)"; do
        if [ -n "$cand" ] && [ -x "$cand" ]; then sup_bin="$cand"; break; fi
    done
    if [ -z "$sup_bin" ]; then
        err "--supervise: mosaic-supervise binary not found (re-run install.sh)"
        return 1
    fi
    SUPERVISE_FIFO=$(mktemp -u -t mosaic-supervise-XXXXXX).fifo
    mkfifo "$SUPERVISE_FIFO"
    # Keep the FIFO open for writing on fd 9 so the supervisor doesn't
    # see EOF every time we finish writing a single reload line.
    exec 9<>"$SUPERVISE_FIFO"
    "$sup_bin" --binary "$OUT" \
               --grace "$SUPERVISE_GRACE" \
               --overlap "$SUPERVISE_OVERLAP_MS" \
               <"$SUPERVISE_FIFO" &
    SUPERVISE_PID=$!
    say "supervisor PID=${SUPERVISE_PID} (grace ${SUPERVISE_GRACE}s, overlap ${SUPERVISE_OVERLAP_MS}ms)"
}

supervise_reload() {
    if [ -n "$SUPERVISE_PID" ] && kill -0 "$SUPERVISE_PID" 2>/dev/null; then
        echo "reload" >&9
    fi
}

stop_supervisor() {
    if [ -n "$SUPERVISE_PID" ] && kill -0 "$SUPERVISE_PID" 2>/dev/null; then
        echo "quit" >&9 2>/dev/null || true
        # Give the supervisor up to 3s to drain workers + exit.
        for _ in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15; do
            kill -0 "$SUPERVISE_PID" 2>/dev/null || break
            sleep 0.2
        done
        kill -TERM "$SUPERVISE_PID" 2>/dev/null || true
        wait "$SUPERVISE_PID" 2>/dev/null || true
    fi
    # Close our write fd + remove the FIFO.
    exec 9>&- 2>/dev/null || true
    [ -n "$SUPERVISE_FIFO" ] && [ -p "$SUPERVISE_FIFO" ] && rm -f "$SUPERVISE_FIFO"
    SUPERVISE_PID=""
    SUPERVISE_FIFO=""
}

build() {
    if "$SCRIPT_DIR/mosaic-build.sh" --app "$APP_DIR" --entry "$ENTRY" -o "$OUT" \
            > /tmp/mosaic-dev-build.log 2>&1; then
        echo -e "  ${GREEN}✓${NC} build OK"
        return 0
    fi
    echo -e "  ${RED}✗${NC} build failed — last 15 lines:"
    tail -15 /tmp/mosaic-dev-build.log | sed 's/^/    /'
    return 1
}

# ── Livereload daemon ───────────────────────────────────────────
# The daemon is a tiny AM/C program (tools/livereload-daemon.am).
# We compile it on first run, then keep it alive across rebuilds.
# On every successful rebuild we write "reload\n" to a FIFO that
# the daemon reads from stdin; the daemon broadcasts the message
# over WebSocket to every connected browser.

LR_PID=""
LR_FIFO=""
LR_BIN=""

ensure_lr_bin() {
    # Resolve where the daemon source / binary lives. Two possible
    # layouts:
    #   - Dev repo:  $SCRIPT_DIR/livereload-daemon.am next to mosaic-dev.sh
    #   - Installed: $SCRIPT_DIR/../share/mosaic/livereload-daemon.am
    local src=""
    for cand in \
        "$SCRIPT_DIR/livereload-daemon.am" \
        "$SCRIPT_DIR/../share/mosaic/livereload-daemon.am"; do
        if [ -f "$cand" ]; then src="$cand"; break; fi
    done
    if [ -z "$src" ]; then
        warn "livereload-daemon.am not found near $SCRIPT_DIR — disabling livereload"
        LIVERELOAD=false
        return 1
    fi
    # Cache the compiled binary in ~/.cache/mosaic/. Rebuilt only
    # when the source is newer than the binary.
    local cache="$HOME/.cache/mosaic"
    mkdir -p "$cache"
    LR_BIN="$cache/livereload-daemon"
    if [ -x "$LR_BIN" ] && [ "$LR_BIN" -nt "$src" ]; then
        return 0
    fi
    say "building livereload daemon..."
    local srcdir; srcdir="$(dirname "$src")"
    # The daemon's amalgame.toml/lock live alongside the .am file.
    # Build in a temp dir so we don't pollute the source tree.
    local tmp; tmp="$(mktemp -d)"
    cp "$src" "$tmp/livereload-daemon.am"
    cp "$srcdir/livereload-daemon.toml" "$tmp/amalgame.toml"
    (cd "$tmp" && "$AMC" package add net-http@v0.4.0 > /dev/null 2>&1 \
        && "$AMC" build livereload-daemon.am > /tmp/mosaic-lrd-build.log 2>&1 \
        && mv livereload-daemon "$LR_BIN")
    if [ ! -x "$LR_BIN" ]; then
        warn "livereload daemon build failed (see /tmp/mosaic-lrd-build.log) — disabling livereload"
        LIVERELOAD=false
        rm -rf "$tmp"
        return 1
    fi
    rm -rf "$tmp"
    return 0
}

start_lr_daemon() {
    [ "$LIVERELOAD" = "true" ] || return 0
    ensure_lr_bin || return 0
    LR_FIFO="$(mktemp -u /tmp/mosaic-lr.XXXXXX.fifo)"
    mkfifo "$LR_FIFO"
    # Keep the FIFO open from this shell so the daemon doesn't see
    # EOF when no writer is currently connected.
    exec 9<>"$LR_FIFO"
    "$LR_BIN" --port "$LIVERELOAD_PORT" <&9 > /tmp/mosaic-lr.log 2>&1 &
    LR_PID=$!
    sleep 0.3
    say "livereload daemon on :${LIVERELOAD_PORT} (PID=$LR_PID)"
}

stop_lr_daemon() {
    if [ -n "$LR_PID" ] && kill -0 "$LR_PID" 2>/dev/null; then
        kill "$LR_PID" 2>/dev/null || true
        wait "$LR_PID" 2>/dev/null || true
    fi
    exec 9>&- 2>/dev/null || true
    [ -n "$LR_FIFO" ] && [ -p "$LR_FIFO" ] && rm -f "$LR_FIFO"
    LR_PID=""; LR_FIFO=""
}

trigger_reload() {
    [ "$LIVERELOAD" = "true" ] || return 0
    [ -n "$LR_FIFO" ] && [ -p "$LR_FIFO" ] || return 0
    echo "reload" > "$LR_FIFO" 2>/dev/null || true
}

# ── SIGINT/SIGTERM handler: stop daemon + server cleanly ────────

cleanup() {
    if $SUPERVISE && [ -n "$SUPERVISE_PID" ]; then
        say "stopping supervisor (${SUPERVISE_PID})"
        stop_supervisor
    fi
    if [ -n "$SERVER_PID" ]; then
        say "stopping server (${SERVER_PID})"
        stop_server
    fi
    stop_lr_daemon
    say "bye"
    exit 0
}
trap cleanup INT TERM

# ── Resolve amc + livereload daemon ────────────────────────────

AMC="${AMC:-}"
if [ -z "$AMC" ]; then
    if   [ -x "$HOME/.local/bin/amc" ]; then AMC="$HOME/.local/bin/amc"
    elif command -v amc >/dev/null 2>&1; then AMC="$(command -v amc)"
    fi
fi
export AMC

start_lr_daemon

# ── Initial build + run ─────────────────────────────────────────

if ! $SKIP_INITIAL; then
    say "initial build..."
    if build; then
        if $SUPERVISE; then
            start_supervisor
        else
            start_server
        fi
    else
        err "initial build failed — waiting for source changes"
    fi
else
    if [ -x "$OUT" ]; then
        if $SUPERVISE; then
            start_supervisor
        else
            start_server
        fi
    else
        warn "$OUT not found — first change will trigger a build"
    fi
fi

# ── Watch loop ──────────────────────────────────────────────────
#
# `find -newer $OUT` returns any file modified since the last build
# (the binary's mtime is our reference point). Re-runs `find` every
# 500 ms; very cheap for a handful of .am files.

WATCH=("$APP_DIR" "$ENTRY")
[ -f amalgame.toml ] && WATCH+=("amalgame.toml")

say "watching $(printf "%s, " "${WATCH[@]}" | sed 's/, $//') — press Ctrl-C to stop"

while true; do
    sleep 0.5
    if [ ! -x "$OUT" ]; then
        # No binary yet — any source file = trigger
        changes=$(find "${WATCH[@]}" -type f \( -name '*.am' -o -name '*.toml' \) 2>/dev/null | head -1)
    else
        changes=$(find "${WATCH[@]}" -type f \( -name '*.am' -o -name '*.toml' \) \
                  -newer "$OUT" 2>/dev/null | head -1)
    fi
    if [ -n "$changes" ]; then
        say "${YELLOW}change detected${NC} → rebuilding..."
        if $SUPERVISE; then
            # Supervisor stays up across reloads; just rebuild and
            # tell it to swap the worker. If the rebuild fails the
            # current worker keeps serving — better than the brutal
            # path's "drop everything then panic".
            if build; then
                if [ -n "$SUPERVISE_PID" ] && kill -0 "$SUPERVISE_PID" 2>/dev/null; then
                    supervise_reload
                else
                    start_supervisor
                fi
                trigger_reload      # browser refresh signal
            else
                err "rebuild failed — current worker keeps serving"
            fi
        else
            stop_server
            if build; then
                start_server
                trigger_reload      # tell connected browsers to refresh
            else
                err "rebuild failed — keeping old server stopped, waiting for next change"
            fi
        fi
    fi
done
