#!/usr/bin/env bash
# ─────────────────────────────────────────────────────────────────
#  mosaic-service.sh — install Mosaic as a system service
#
#  Wraps `mosaic serve <config>` in the host's native service
#  manager so a Mosaic site survives reboots and crashes:
#
#    Linux   → systemd unit   /etc/systemd/system/<name>.service
#    macOS   → launchd daemon  /Library/LaunchDaemons/<name>.plist
#    Windows → SCM service     sc create <name> ...  (Git Bash/MSYS)
#
#  This only does OS-level *registration* — the running binary still
#  owns shutdown handling.  On Linux/macOS the net-http listener
#  already exits cleanly on SIGTERM; on Windows the served binary
#  must call `Service.RunAsService("<name>")` (amalgame-service) so
#  the SCM dispatcher reports status.  See `mosaic service --help`.
#
#  Registration writes to system paths → most actions need root
#  (sudo / Administrator).  `status` and `logs` do not.
#
#  Usage:
#    mosaic service install [CONFIG]   register + enable + start
#    mosaic service uninstall          stop + disable + remove unit
#    mosaic service start|stop|restart
#    mosaic service status
#    mosaic service logs [-f]
# ─────────────────────────────────────────────────────────────────

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$(readlink -f "$0" 2>/dev/null || echo "$0")")" && pwd)"

if [ -t 1 ]; then
    GREEN='\033[0;32m'; YELLOW='\033[0;33m'; CYAN='\033[0;36m'; RED='\033[0;31m'; NC='\033[0m'
else
    GREEN=''; YELLOW=''; CYAN=''; RED=''; NC=''
fi
say()  { echo -e "${CYAN}→${NC} $1"; }
ok()   { echo -e "  ${GREEN}✓${NC} $1"; }
warn() { echo -e "  ${YELLOW}!${NC} $1"; }
die()  { echo -e "${RED}mosaic service: $1${NC}" >&2; exit 1; }

usage() {
    cat <<EOF
mosaic-service — run a Mosaic site as a native system service

Usage:
  mosaic service <action> [OPTIONS] [CONFIG]

Actions:
  install [CONFIG]   Generate the platform service unit for
                     \`mosaic serve CONFIG\` (default: ./mosaic.toml),
                     register it, then enable + start it.
  uninstall          Stop, disable, and remove the service unit.
  start              Start the service.
  stop               Stop the service.
  restart            Restart the service.
  status             Show the service status (no root needed).
  logs [-f]          Tail the service logs (-f to follow).

Options:
  --name NAME        Service name (default: mosaic-<config-dir-basename>).
  --user USER        Run the service as this OS user (systemd/launchd
                     User=; default: the invoking user, or root if none).
  --user-scope       Install a per-user service instead of a system one
                     (systemd --user; no root, no boot without lingering).
  --no-enable        Register but don't enable at boot.
  --no-start         Register but don't start now.
  -h, --help         Show this help.

Platform:
  Linux    systemd unit    /etc/systemd/system/<name>.service
  macOS    launchd daemon  /Library/LaunchDaemons/<name>.plist
  Windows  SCM service      sc create <name> ...   (Git Bash / MSYS2)

Examples:
  cd /srv/my-site && mosaic service install         # uses ./mosaic.toml
  mosaic service install --name belfort prod.toml
  mosaic service install --user www-data /srv/site/mosaic.toml
  mosaic service status
  mosaic service logs -f
  mosaic service uninstall --name belfort

Notes:
  • Registration writes system paths → run with sudo (or as
    Administrator on Windows).  status/logs don't need root.
  • The unit pins WorkingDirectory to the config's directory and
    ExecStart to \`mosaic serve <abs-config>\`, so relative paths in
    mosaic.toml (certs/, public/…) resolve exactly as in the foreground.
EOF
}

# ── Parse ─────────────────────────────────────────────────────────
ACTION="${1:-}"
[ -z "$ACTION" ] && { usage; exit 2; }
case "$ACTION" in -h|--help|help) usage; exit 0 ;; esac
shift || true

NAME=""
RUN_USER=""
USER_SCOPE=0
DO_ENABLE=1
DO_START=1
FOLLOW=0
CONFIG=""

while [ $# -gt 0 ]; do
    case "$1" in
        -h|--help)    usage; exit 0 ;;
        --name)       NAME="$2"; shift 2 ;;
        --user)       RUN_USER="$2"; shift 2 ;;
        --user-scope) USER_SCOPE=1; shift ;;
        --no-enable)  DO_ENABLE=0; shift ;;
        --no-start)   DO_START=0; shift ;;
        -f|--follow)  FOLLOW=1; shift ;;
        -*)           die "unknown flag $1  (try 'mosaic service --help')" ;;
        *)
            [ -n "$CONFIG" ] && die "only one CONFIG argument allowed"
            CONFIG="$1"; shift ;;
    esac
done

# ── Resolve the mosaic dispatcher (for ExecStart) ─────────────────
resolve_mosaic() {
    if [ -x "$SCRIPT_DIR/mosaic" ]; then
        echo "$SCRIPT_DIR/mosaic"
    elif command -v mosaic >/dev/null 2>&1; then
        command -v mosaic
    else
        die "can't locate the 'mosaic' binary on PATH or next to this script"
    fi
}

# ── Resolve config + derive default name ──────────────────────────
resolve_config() {
    local cfg="${CONFIG:-mosaic.toml}"
    [ -f "$cfg" ] || die "config not found: $cfg"
    # Absolute path so the unit is location-independent.
    CONFIG_ABS="$(cd "$(dirname "$cfg")" && pwd)/$(basename "$cfg")"
    CONFIG_DIR="$(dirname "$CONFIG_ABS")"
    if [ -z "$NAME" ]; then
        NAME="mosaic-$(basename "$CONFIG_DIR")"
    fi
}

# Most actions only need a name, not a live config file. install needs both.
derive_name_only() {
    [ -z "$NAME" ] && die "this action needs --name (no CONFIG to derive it from)"
}

OS="$(uname -s)"

# ════════════════════════════════════════════════════════════════
#  systemd (Linux)
# ════════════════════════════════════════════════════════════════
SYSTEMCTL_SCOPE=()    # extra args for systemctl (e.g. --user)
unit_path_systemd() {
    if [ "$USER_SCOPE" -eq 1 ]; then
        echo "$HOME/.config/systemd/user/$NAME.service"
    else
        echo "/etc/systemd/system/$NAME.service"
    fi
}
sysctl() {
    if [ "$USER_SCOPE" -eq 1 ]; then
        systemctl --user "$@"
    elif [ "$(id -u)" -eq 0 ]; then
        systemctl "$@"
    else
        sudo systemctl "$@"
    fi
}
write_file_priv() {
    # write_file_priv <path>  (content on stdin)
    local path="$1"
    if [ "$USER_SCOPE" -eq 1 ] || [ -w "$(dirname "$path")" ]; then
        cat > "$path"
    else
        sudo tee "$path" >/dev/null
    fi
}

install_systemd() {
    resolve_config
    local mosaic_bin unit
    mosaic_bin="$(resolve_mosaic)"
    unit="$(unit_path_systemd)"
    local user_line=""
    if [ "$USER_SCOPE" -eq 0 ]; then
        local svc_user="${RUN_USER:-$(id -un)}"
        user_line="User=$svc_user"
    fi

    say "writing systemd unit → $unit"
    [ "$USER_SCOPE" -eq 1 ] && mkdir -p "$(dirname "$unit")"
    write_file_priv "$unit" <<EOF
# Generated by \`mosaic service install\`. Re-run to regenerate.
[Unit]
Description=Mosaic site ($NAME)
After=network-online.target
Wants=network-online.target

[Service]
Type=simple
$user_line
WorkingDirectory=$CONFIG_DIR
ExecStart=$mosaic_bin serve $CONFIG_ABS
Restart=on-failure
RestartSec=2
# Allow binding :80/:443 without running as full root.
AmbientCapabilities=CAP_NET_BIND_SERVICE
KillSignal=SIGTERM
TimeoutStopSec=20

[Install]
WantedBy=$( [ "$USER_SCOPE" -eq 1 ] && echo default.target || echo multi-user.target )
EOF
    ok "unit written"

    sysctl daemon-reload
    if [ "$DO_ENABLE" -eq 1 ]; then sysctl enable "$NAME" >/dev/null 2>&1 && ok "enabled at boot"; fi
    if [ "$DO_START" -eq 1 ];  then sysctl restart "$NAME" && ok "started"; fi
    echo ""
    ok "installed. manage with:"
    echo "    mosaic service status   --name $NAME$( [ "$USER_SCOPE" -eq 1 ] && echo ' --user-scope')"
    echo "    mosaic service logs -f  --name $NAME$( [ "$USER_SCOPE" -eq 1 ] && echo ' --user-scope')"
}

uninstall_systemd() {
    derive_name_only
    local unit; unit="$(unit_path_systemd)"
    say "removing systemd service $NAME"
    sysctl stop "$NAME"    >/dev/null 2>&1 && ok "stopped"    || warn "not running"
    sysctl disable "$NAME" >/dev/null 2>&1 && ok "disabled"   || true
    if [ "$USER_SCOPE" -eq 1 ] || [ -w "$(dirname "$unit")" ]; then rm -f "$unit"; else sudo rm -f "$unit"; fi
    ok "unit removed"
    sysctl daemon-reload
}

# ════════════════════════════════════════════════════════════════
#  launchd (macOS)
# ════════════════════════════════════════════════════════════════
plist_path() { echo "/Library/LaunchDaemons/$NAME.plist"; }
sudo_mac() { if [ "$(id -u)" -eq 0 ]; then "$@"; else sudo "$@"; fi; }

install_launchd() {
    resolve_config
    local mosaic_bin plist svc_user
    mosaic_bin="$(resolve_mosaic)"
    plist="$(plist_path)"
    svc_user="${RUN_USER:-root}"

    say "writing launchd daemon → $plist"
    sudo_mac tee "$plist" >/dev/null <<EOF
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN"
  "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
  <key>Label</key>            <string>$NAME</string>
  <key>UserName</key>         <string>$svc_user</string>
  <key>WorkingDirectory</key> <string>$CONFIG_DIR</string>
  <key>ProgramArguments</key>
  <array>
    <string>$mosaic_bin</string>
    <string>serve</string>
    <string>$CONFIG_ABS</string>
  </array>
  <key>RunAtLoad</key>        <$( [ "$DO_START" -eq 1 ] && echo true || echo false )/>
  <key>KeepAlive</key>
  <dict><key>SuccessfulExit</key><false/></dict>
  <key>StandardOutPath</key>  <string>/var/log/$NAME.log</string>
  <key>StandardErrorPath</key><string>/var/log/$NAME.log</string>
</dict>
</plist>
EOF
    sudo_mac chown root:wheel "$plist"
    sudo_mac chmod 644 "$plist"
    ok "plist written"
    if [ "$DO_ENABLE" -eq 1 ] || [ "$DO_START" -eq 1 ]; then
        sudo_mac launchctl bootstrap system "$plist" 2>/dev/null \
            || sudo_mac launchctl load "$plist"     # pre-10.11 fallback
        ok "loaded"
    fi
    echo ""
    ok "installed → /var/log/$NAME.log"
}

uninstall_launchd() {
    derive_name_only
    local plist; plist="$(plist_path)"
    say "removing launchd daemon $NAME"
    sudo_mac launchctl bootout system "$plist" 2>/dev/null \
        || sudo_mac launchctl unload "$plist" 2>/dev/null || warn "not loaded"
    sudo_mac rm -f "$plist"
    ok "removed"
}

# ════════════════════════════════════════════════════════════════
#  SCM (Windows, via Git Bash / MSYS2 / Cygwin)
# ════════════════════════════════════════════════════════════════
install_scm() {
    resolve_config
    local mosaic_bin
    mosaic_bin="$(resolve_mosaic)"
    warn "the served binary must call Service.RunAsService(\"$NAME\") for"
    warn "the SCM to track it (amalgame-service ≥0.2). Plain \`mosaic serve\`"
    warn "works but Windows will mark it 'not responding' on stop."
    say "creating service $NAME"
    # binPath must be one quoted string; SCM has no WorkingDirectory.
    sc create "$NAME" \
        binPath= "\"$mosaic_bin\" serve \"$CONFIG_ABS\"" \
        start= $( [ "$DO_ENABLE" -eq 1 ] && echo auto || echo demand ) \
        DisplayName= "Mosaic ($NAME)"
    [ -n "$RUN_USER" ] && sc config "$NAME" obj= "$RUN_USER"
    ok "service created"
    [ "$DO_START" -eq 1 ] && { sc start "$NAME"; ok "started"; }
}

uninstall_scm() {
    derive_name_only
    say "removing service $NAME"
    sc stop "$NAME" 2>/dev/null || warn "not running"
    sc delete "$NAME"
    ok "deleted"
}

# ── Lifecycle actions (start/stop/restart/status/logs) ────────────
do_lifecycle() {
    local verb="$1"
    derive_name_only
    case "$OS" in
        Linux)
            case "$verb" in
                status) sysctl --no-pager status "$NAME" || true ;;
                logs)   if [ "$USER_SCOPE" -eq 1 ]; then
                            journalctl --user -u "$NAME" $( [ "$FOLLOW" -eq 1 ] && echo -f ) --no-pager
                        else
                            ${SUDO:-} journalctl -u "$NAME" $( [ "$FOLLOW" -eq 1 ] && echo -f ) --no-pager
                        fi ;;
                *)      sysctl "$verb" "$NAME" && ok "$verb ok" ;;
            esac ;;
        Darwin)
            case "$verb" in
                status) sudo_mac launchctl print "system/$NAME" 2>/dev/null \
                            || launchctl list | grep -i "$NAME" || warn "not loaded" ;;
                logs)   tail $( [ "$FOLLOW" -eq 1 ] && echo -f ) "/var/log/$NAME.log" ;;
                start)  sudo_mac launchctl kickstart "system/$NAME" && ok "started" ;;
                stop)   sudo_mac launchctl kill SIGTERM "system/$NAME" && ok "stopped" ;;
                restart)sudo_mac launchctl kickstart -k "system/$NAME" && ok "restarted" ;;
            esac ;;
        MINGW*|MSYS*|CYGWIN*)
            case "$verb" in
                status)  sc query "$NAME" ;;
                logs)    warn "no unified log on Windows; check Event Viewer or the app's own logs" ;;
                start)   sc start "$NAME" ;;
                stop)    sc stop  "$NAME" ;;
                restart) sc stop "$NAME" 2>/dev/null || true; sc start "$NAME" ;;
            esac ;;
        *) die "unsupported OS: $OS" ;;
    esac
}

# ── Dispatch ──────────────────────────────────────────────────────
case "$ACTION" in
    install)
        case "$OS" in
            Linux)              install_systemd ;;
            Darwin)             install_launchd ;;
            MINGW*|MSYS*|CYGWIN*) install_scm ;;
            *) die "unsupported OS: $OS" ;;
        esac ;;
    uninstall)
        case "$OS" in
            Linux)              uninstall_systemd ;;
            Darwin)             uninstall_launchd ;;
            MINGW*|MSYS*|CYGWIN*) uninstall_scm ;;
            *) die "unsupported OS: $OS" ;;
        esac ;;
    start|stop|restart|status|logs)
        do_lifecycle "$ACTION" ;;
    *)
        die "unknown action '$ACTION'  (try 'mosaic service --help')" ;;
esac
