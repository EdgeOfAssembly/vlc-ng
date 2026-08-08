#!/bin/bash
# Usage: play-once.sh --log PATH [vlc-args...] media
# Always tees full VLC stdout+stderr to PATH.
set -euo pipefail
PREFIX="${PREFIX:-/tmp/vlc-ng-install}"
export LD_LIBRARY_PATH="$PREFIX/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
export VLC_PLUGIN_PATH="$PREFIX/lib/vlc/plugins"
# Isolate VLC config only (do not affect parent xmux)
export HOME="${VLC_HOME:-/tmp/vlc-ng-home}"
mkdir -p "$HOME/.config/vlc" "$HOME/.local/share/vlc"

LOG=""
if [ "${1:-}" = "--log" ]; then LOG="$2"; shift 2; fi
LOG="${LOG:-/tmp/vlc-ng/patches/_analysis/xmux-logs/play-once-$$.log}"
mkdir -p "$(dirname "$LOG")"

cat > "$HOME/.config/vlc/vlcrc" <<'CFG'
[qt]
qt-privacy-ask=0
[core]
media-library=0
metadata-network-access=0
CFG

VLC_BIN="$PREFIX/bin/cvlc"
[ -x "$VLC_BIN" ] || VLC_BIN="$PREFIX/bin/vlc"

{
  echo "===== VLC-ng play-once $(date -Iseconds) ====="
  echo "cmd: $VLC_BIN $*"
  echo "DISPLAY=${DISPLAY:-} VLC_HOME=$HOME"
  echo "terminal_log=$LOG"
  echo "================================================"
} | tee "$LOG"

set +e
"$VLC_BIN" \
  --no-media-library --no-metadata-network-access \
  --play-and-exit --no-audio --no-osd \
  --verbose 2 \
  "$@" 2>&1 | tee -a "$LOG"
rc=${PIPESTATUS[0]}
set -e
echo "===== exit $rc $(date -Iseconds) =====" | tee -a "$LOG"
exit "$rc"
