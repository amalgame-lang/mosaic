#!/usr/bin/env bash
# ═══════════════════════════════════════════════════════════
#  Mosaic — installer
#  https://github.com/amalgame-lang/mosaic
#
#  Drops the Mosaic CLI scripts into <prefix>/bin so they're
#  callable as `mosaic-routes.sh` / `mosaic-build.sh` from any
#  Amalgame project directory.
#
#  Usage:
#    curl -sSL https://raw.githubusercontent.com/amalgame-lang/mosaic/main/install.sh | bash
#
#  Env vars:
#    MOSAIC_VERSION — tag to install (default: latest GitHub release)
#    MOSAIC_PREFIX  — install prefix    (default: $HOME/.local)
# ═══════════════════════════════════════════════════════════

set -euo pipefail

REPO="amalgame-lang/mosaic"
VERSION="${MOSAIC_VERSION:-latest}"
PREFIX="${MOSAIC_PREFIX:-$HOME/.local}"
BIN_DIR="$PREFIX/bin"

if [ -t 1 ]; then
    GREEN='\033[0;32m'; YELLOW='\033[0;33m'; CYAN='\033[0;36m'; NC='\033[0m'
else
    GREEN=''; YELLOW=''; CYAN=''; NC=''
fi
say() { echo -e "${CYAN}→${NC} $1"; }
ok()  { echo -e "  ${GREEN}✓${NC} $1"; }
warn(){ echo -e "  ${YELLOW}!${NC} $1"; }

# ── Resolve tag ──────────────────────────────────────────
if [ "$VERSION" = "latest" ]; then
    say "Fetching latest release tag..."
    if command -v gh >/dev/null 2>&1; then
        VERSION=$(gh release view --repo "$REPO" --json tagName -q .tagName 2>/dev/null || echo "")
    fi
    if [ -z "$VERSION" ]; then
        VERSION=$(curl -sSL "https://api.github.com/repos/$REPO/releases/latest" \
                  | grep '"tag_name"' | head -1 | sed 's/.*"tag_name": "\(.*\)".*/\1/')
    fi
    [ -z "$VERSION" ] && { echo "error: couldn't resolve latest tag"; exit 1; }
fi
[ "${VERSION#v}" = "$VERSION" ] && VERSION="v$VERSION"
say "Installing mosaic $VERSION → $PREFIX"

# ── Download archive ─────────────────────────────────────
TMP="$(mktemp -d)"
trap 'rm -rf "$TMP"' EXIT
URL="https://github.com/$REPO/archive/refs/tags/$VERSION.tar.gz"
curl -sSL -o "$TMP/mosaic.tar.gz" "$URL"
tar -xzf "$TMP/mosaic.tar.gz" -C "$TMP"
SRC="$(find "$TMP" -maxdepth 1 -type d -name 'mosaic-*' | head -1)"

# ── Install scripts ──────────────────────────────────────
mkdir -p "$BIN_DIR"
install -m 755 "$SRC/tools/mosaic-routes.sh" "$BIN_DIR/mosaic-routes.sh"
install -m 755 "$SRC/tools/mosaic-build.sh"  "$BIN_DIR/mosaic-build.sh"
ok "mosaic-routes.sh → $BIN_DIR/"
ok "mosaic-build.sh  → $BIN_DIR/"

# ── PATH hint ────────────────────────────────────────────
case ":$PATH:" in
    *:"$BIN_DIR":*) ;;
    *)  warn "$BIN_DIR is not on \$PATH"
        warn "add it to your shell rc:  export PATH=\"$BIN_DIR:\$PATH\"" ;;
esac

echo ""
ok "Installed. Quickstart:"
echo "    amc package add web@v0.2.1"
echo "    amc package add net-http@v0.2.0"
echo "    mkdir app && curl -s https://raw.githubusercontent.com/$REPO/main/examples/mosaic-fs-demo/app/index.am > app/index.am"
echo "    mosaic-build.sh && ./server"
