#!/bin/bash
# Xmux VLC playback test:
#  - human spectator viewer attached on :0
#  - full VLC terminal log to file (tee)
#  - multi-frame screenshots under /tmp/vlc-ng-shots (outside gitignore for agent read)
# Usage: xmux-play-test.sh <test-name> <seconds> [vlc-args...] media
set -euo pipefail

NAME="${1:?test name}"; shift
SECS="${1:?seconds}"; shift
SESSION="${XMUX_SESSION:-vlc-ng}"
LOGDIR="${LOGDIR:-/tmp/vlc-ng/patches/_analysis/xmux-logs}"
SHOTDIR="${SHOTDIR:-/tmp/vlc-ng-shots}"
mkdir -p "$LOGDIR" "$SHOTDIR"

LOG="$LOGDIR/${NAME}.log"
XMUX_RUN_LOG="$LOGDIR/${NAME}.xmux-run.log"
VIEWER_LOG="$LOGDIR/viewer-attach.log"
VIEWER_PIDFILE="$LOGDIR/viewer.pid"

ensure_session() {
  if ! xmux list 2>/dev/null | grep -E "^${SESSION} " | grep -q running; then
    xmux kill "$SESSION" 2>/dev/null || true
    sleep 0.3
    xmux start "$SESSION" --geometry 1280x800 --gl nvidia
    xmux run --gl nvidia "$SESSION" -- xmux-wm >/dev/null 2>&1 || true
    sleep 0.5
  fi
}

ensure_viewer() {
  local need=1
  if [ -f "$VIEWER_PIDFILE" ]; then
    local old; old=$(cat "$VIEWER_PIDFILE" 2>/dev/null || true)
    if [ -n "${old:-}" ] && kill -0 "$old" 2>/dev/null; then need=0; fi
  fi
  if [ "$need" -eq 1 ]; then
    echo "SPECTATOR: starting viewer on host :0"
    ( DISPLAY="${XMUX_ATTACH_DISPLAY:-:0}" xmux attach "$SESSION" --no-reconnect >>"$VIEWER_LOG" 2>&1 ) &
    echo $! >"$VIEWER_PIDFILE"
    sleep 0.8
  fi
  echo "SPECTATOR: xmux attach $SESSION --no-reconnect"
  echo "SPECTATOR: viewer_pid=$(cat "$VIEWER_PIDFILE" 2>/dev/null || echo none)"
}

kill_players() {
  for bin in cvlc vlc; do
    pids=$(pgrep -x "$bin" 2>/dev/null || true)
    if [ -n "$pids" ]; then kill $pids 2>/dev/null || true; fi
  done
  sleep 0.2
}

ensure_session
ensure_viewer
kill_players

echo "TEST $NAME terminal_log=$LOG shots=$SHOTDIR/${NAME}-tN.png"
echo "  args: $*"

xmux run --gl nvidia "$SESSION" -- \
  /tmp/vlc-ng/scripts/play-once.sh --log "$LOG" "$@" >"$XMUX_RUN_LOG" 2>&1 &
bg=$!

# Multi-frame screenshots during playback (for black/flash detection)
nshots=$(( SECS > 2 ? 3 : 2 ))
interval=$(python3 -c "print(max(0.8, $SECS/($nshots+1)))")
for i in $(seq 1 "$nshots"); do
  sleep "$interval"
  shot="$SHOTDIR/${NAME}-t${i}.png"
  xmux screenshot "$SESSION" -o "$shot" 2>&1 | tee -a "$LOG" || true
  # brightness summary into log
  python3 - "$shot" <<'PY' 2>/dev/null | tee -a "$LOG" || true
import sys
from pathlib import Path
from PIL import Image
p=Path(sys.argv[1])
if not p.exists():
    print(f"shot missing: {p}"); raise SystemExit
im=Image.open(p).convert("L"); px=list(im.getdata())
mean=sum(px)/len(px); black=sum(1 for v in px if v<8)/len(px)
print(f"SHOT {p.name}: mean={mean:.1f} black_frac={black:.3f} bytes={p.stat().st_size}")
if black>0.95:
    print("WARNING: likely BLACK SCREEN")
elif mean<15:
    print("WARNING: very dark frame")
else:
    print("SHOT OK: visible content")
PY
done

kill "$bg" 2>/dev/null || true
wait "$bg" 2>/dev/null || true
kill_players

{
  echo
  echo "===== summary $NAME $(date -Iseconds) ====="
  echo "terminal log: $LOG ($(wc -c <"$LOG") bytes)"
  echo "xmux run log: $XMUX_RUN_LOG"
  echo "viewer: $(cat "$VIEWER_PIDFILE" 2>/dev/null || echo none)"
  echo "--- log highlights ---"
  rg -i '4:4:4|trying format|using vout|vdpau_display|using opengl|using back-end|software decode|failed to create|error:|video output: using|successfully opened|Using NVIDIA' "$LOG" | head -40 || true
} | tee -a "$LOG"

echo "OK: $LOG"
