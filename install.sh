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
        # Fall back to the GitHub API. Returns empty body when there
        # are no published releases yet — fail loudly with guidance
        # instead of silently exiting.
        API_BODY=$(curl -sSL "https://api.github.com/repos/$REPO/releases/latest")
        VERSION=$(printf '%s' "$API_BODY" | grep '"tag_name"' | head -1 \
                  | sed 's/.*"tag_name": "\(.*\)".*/\1/')
    fi
    if [ -z "$VERSION" ]; then
        cat >&2 <<EOF
error: couldn't resolve the latest mosaic tag.

  This usually means no GitHub Release has been published for the
  repo yet (tags alone are not enough). To install a specific
  version directly:

      MOSAIC_VERSION=v0.2.1 \\
        curl -sSL https://raw.githubusercontent.com/$REPO/main/install.sh | bash

  Or browse the available tags:
      https://github.com/$REPO/tags
EOF
        exit 1
    fi
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
# The `mosaic` dispatcher routes subcommands to the *-sh scripts
# (which it locates via $SCRIPT_DIR relative to itself). Install
# all four into the same bin dir so they stay siblings.
mkdir -p "$BIN_DIR"
for s in mosaic mosaic-routes.sh mosaic-build.sh mosaic-dev.sh mosaic-new.sh; do
    install -m 755 "$SRC/tools/$s" "$BIN_DIR/$s"
    ok "$s → $BIN_DIR/"
done

# ── Install livereload daemon source ─────────────────────
# mosaic dev builds this AM file the first time livereload kicks in
# (or after a v0.X bump). Lives in $PREFIX/share/mosaic/ so it's
# resolvable via SCRIPT_DIR/../share/mosaic from the bin scripts.
SHARE_DIR="$PREFIX/share/mosaic"
mkdir -p "$SHARE_DIR"
for s in livereload-daemon.am livereload-daemon.toml; do
    if [ -f "$SRC/tools/$s" ]; then
        install -m 644 "$SRC/tools/$s" "$SHARE_DIR/$s"
        ok "$s → $SHARE_DIR/"
    fi
done

# ── PATH hint ────────────────────────────────────────────
case ":$PATH:" in
    *:"$BIN_DIR":*) ;;
    *)  warn "$BIN_DIR is not on \$PATH"
        warn "add it to your shell rc:  export PATH=\"$BIN_DIR:\$PATH\"" ;;
esac

echo ""
ok "Installed. Quickstart:"
echo "    amc package add web@v0.2.1"
echo "    amc package add net-http@v0.2.1"
echo "    mkdir app && curl -s https://raw.githubusercontent.com/$REPO/main/examples/mosaic-fs-demo/app/index.am > app/index.am"
echo "    mosaic dev               # watches + reloads on every save"
echo "    # or for a one-shot build:  mosaic build && ./server"
