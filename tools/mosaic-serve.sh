#!/usr/bin/env bash
# ─────────────────────────────────────────────────────
#  mosaic serve [mosaic.toml] — run the config-driven
#  Mosaic server (no Amalgame code; like nginx/Caddy).
#
#  Runs the prebuilt `mosaic-serve` binary bundled in the
#  release. The binary parses the TOML and serves the whole
#  stack (multi-site static + TLS/ACME + middleware) — see
#  src/mosaic-serve.am and docs.amalgame.me → Mosaic →
#  Configuration.
# ─────────────────────────────────────────────────────
set -euo pipefail
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
CONFIG="${1:-mosaic.toml}"

# Prefer the prebuilt binary (shipped per-platform in the release bundle).
for cand in \
    "$SCRIPT_DIR/mosaic-serve" \
    "$SCRIPT_DIR/../libexec/mosaic-serve" \
    "$SCRIPT_DIR/../share/mosaic/mosaic-serve"; do
    if [ -x "$cand" ]; then
        exec "$cand" "$CONFIG"
    fi
done

cat >&2 <<EOF
mosaic serve: the prebuilt 'mosaic-serve' binary isn't in this install.

The config-driven server (src/mosaic-serve.am) ships as a per-platform
binary in the Mosaic release. To build it from source you need amc plus
the stack packages:

    amc package add web@v0.38.0 net-http tls net-proxy service
    amc build src/mosaic-serve.am -o mosaic-serve
    ./mosaic-serve "$CONFIG"

Config reference: https://docs.amalgame.me/en/mosaic/03-configuration.html
EOF
exit 1
