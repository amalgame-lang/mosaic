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

# ── Detect host triple ──────────────────────────────────
# install.sh runs under bash → on Windows that means Git Bash,
# WSL, MSYS2, or Cygwin.  WSL reports Linux (so it picks the
# Linux tarball, which is correct).  Git Bash / MSYS2 / Cygwin
# report MINGW64/MSYS/CYGWIN as $UNAME_S — those want the
# Windows zip.  Native cmd / PowerShell users should download
# the .exe installer from the GitHub Releases page instead of
# running install.sh.
UNAME_S="$(uname -s)"
UNAME_M="$(uname -m)"
TRIPLE=""
case "$UNAME_S-$UNAME_M" in
    Linux-x86_64)        TRIPLE="linux-x86_64" ;;
    Darwin-arm64)        TRIPLE="macos-arm64"  ;;
    Darwin-x86_64)       TRIPLE="macos-arm64"  ;; # Rosetta — close enough
    MINGW64*-x86_64|MSYS*-x86_64|CYGWIN*-x86_64)
                         TRIPLE="windows-x86_64" ;;
    *)                   TRIPLE="" ;;
esac

# ── Download archive ─────────────────────────────────────
TMP="$(mktemp -d)"
trap 'rm -rf "$TMP"' EXIT

# Strategy:
#   1. Try the pre-built per-platform tarball published by
#      .github/workflows/release.yml (fast path, no gcc needed).
#   2. Fall back to the source tarball + locally compile
#      mosaic-supervise — keeps `MOSAIC_VERSION=<old-tag>` working
#      for tags that pre-date the release workflow.

VER_NO_V="${VERSION#v}"
PREBUILT_ASSET=""
PREBUILT_EXT=""
if [ -n "$TRIPLE" ]; then
    if [ "$TRIPLE" = "windows-x86_64" ]; then
        PREBUILT_EXT="zip"
    else
        PREBUILT_EXT="tar.gz"
    fi
    PREBUILT_ASSET="mosaic-${VER_NO_V}-${TRIPLE}.${PREBUILT_EXT}"
fi

USE_PREBUILT=0
if [ -n "$PREBUILT_ASSET" ]; then
    say "Trying pre-built tarball for $TRIPLE..."
    # Pull through get.amalgame.me first — the sovereign cache+stream mirror,
    # where every download is counted in Amalgame's own analytics — with a
    # transparent fall back to GitHub Releases if the mirror is unreachable.
    if curl -fsSL -o "$TMP/mosaic.${PREBUILT_EXT}" "https://get.amalgame.me/mosaic/$VERSION/$PREBUILT_ASSET" 2>/dev/null \
       || curl -fsSL -o "$TMP/mosaic.${PREBUILT_EXT}" "https://github.com/$REPO/releases/download/$VERSION/$PREBUILT_ASSET" 2>/dev/null; then
        USE_PREBUILT=1
        if [ "$PREBUILT_EXT" = "zip" ]; then
            unzip -q "$TMP/mosaic.zip" -d "$TMP"
        else
            tar -xzf "$TMP/mosaic.tar.gz" -C "$TMP"
        fi
        SRC="$(find "$TMP" -maxdepth 1 -type d -name "mosaic-${VER_NO_V}-${TRIPLE}" | head -1)"
        ok "pre-built tarball downloaded"
    else
        warn "no pre-built tarball for $VERSION/$TRIPLE — falling back to source build"
    fi
fi

if [ "$USE_PREBUILT" -eq 0 ]; then
    say "Downloading source tarball..."
    URL="https://github.com/$REPO/archive/refs/tags/$VERSION.tar.gz"
    curl -sSL -o "$TMP/mosaic.tar.gz" "$URL"
    tar -xzf "$TMP/mosaic.tar.gz" -C "$TMP"
    SRC="$(find "$TMP" -maxdepth 1 -type d -name 'mosaic-*' | head -1)"
fi

# ── Install scripts ──────────────────────────────────────
# The `mosaic` dispatcher routes subcommands to the *-sh scripts
# (which it locates via $SCRIPT_DIR relative to itself).  The
# layout differs between the two paths:
#   pre-built tarball: $SRC/bin/* + $SRC/share/mosaic/*
#   source tarball:    $SRC/tools/*  (compiled on the fly)
mkdir -p "$BIN_DIR"

if [ "$USE_PREBUILT" -eq 1 ]; then
    # Pre-built layout — straight copy.  Windows ships
    # mosaic-supervise.exe instead of mosaic-supervise; pick
    # whichever filename exists.
    for s in mosaic mosaic-routes.sh mosaic-build.sh mosaic-dev.sh \
             mosaic-new.sh mosaic-service.sh mosaic-supervise mosaic-supervise.exe; do
        if [ -f "$SRC/bin/$s" ]; then
            install -m 755 "$SRC/bin/$s" "$BIN_DIR/$s"
            ok "$s → $BIN_DIR/"
        fi
    done
else
    # Source layout — copy shell scripts, compile the supervisor.
    for s in mosaic mosaic-routes.sh mosaic-build.sh mosaic-dev.sh mosaic-new.sh mosaic-service.sh; do
        install -m 755 "$SRC/tools/$s" "$BIN_DIR/$s"
        ok "$s → $BIN_DIR/"
    done
    # v0.5+: mosaic-supervise is a tiny C program.  Compile on install
    # — a C compiler is a hard dep of mosaic anyway (every user's
    # project goes through gcc for the final link step).
    if [ -f "$SRC/tools/mosaic-supervise.c" ]; then
        CC="${CC:-cc}"
        if command -v "$CC" >/dev/null 2>&1; then
            "$CC" -O2 -Wall "$SRC/tools/mosaic-supervise.c" -o "$BIN_DIR/mosaic-supervise"
            chmod 755 "$BIN_DIR/mosaic-supervise"
            ok "mosaic-supervise (compiled with $CC) → $BIN_DIR/"
        else
            warn "skipping mosaic-supervise — no C compiler on PATH (\`mosaic supervise\` will be unavailable)"
        fi
    fi
fi

# ── Install livereload daemon source ─────────────────────
# mosaic dev builds this AM file the first time livereload kicks in
# (or after a v0.X bump). Lives in $PREFIX/share/mosaic/ so it's
# resolvable via SCRIPT_DIR/../share/mosaic from the bin scripts.
SHARE_DIR="$PREFIX/share/mosaic"
mkdir -p "$SHARE_DIR"
# Both layouts ship these in different roots; check each.
for s in livereload-daemon.am livereload-daemon.toml; do
    for cand in "$SRC/share/mosaic/$s" "$SRC/tools/$s"; do
        if [ -f "$cand" ]; then
            install -m 644 "$cand" "$SHARE_DIR/$s"
            ok "$s → $SHARE_DIR/"
            break
        fi
    done
done

# ── PATH hint ────────────────────────────────────────────
case ":$PATH:" in
    *:"$BIN_DIR":*) ;;
    *)  warn "$BIN_DIR is not on \$PATH"
        warn "add it to your shell rc:  export PATH=\"$BIN_DIR:\$PATH\"" ;;
esac

echo ""
ok "Installed. Quickstart:"
echo "    mosaic new my-site       # scaffolds project + auto-runs amc package add"
echo "    cd my-site"
echo "    mosaic dev               # http://localhost:3000, livereload on save"
echo "    # or, with WebSocket-surviving hot reload:"
echo "    mosaic dev --supervise   # requires amalgame-net-http v0.8.0+"
echo ""
echo "  Manual flow (no scaffold):"
echo "    amc package add web      # pulls tls + net-http + crypto transitively (amc v0.8.39+)"
echo "    mosaic build && ./server"
