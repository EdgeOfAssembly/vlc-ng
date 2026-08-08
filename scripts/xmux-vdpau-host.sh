#!/bin/bash
# Pure -V vdpau visual QA on real host display (NVIDIA X).
# Xvfb cannot show NVIDIA VDPAU presentation — use attach-to :0.
set -euo pipefail
SESSION="${XMUX_SESSION:-vlc-vdpau-host}"
PREFIX="${PREFIX:-/tmp/vlc-ng-install}"
MEDIA="${1:-/tmp/vlc-ng/samples/test-420.mp4}"
export LD_LIBRARY_PATH="$PREFIX/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
export VLC_PLUGIN_PATH="$PREFIX/lib/vlc/plugins"

xmux kill "$SESSION" 2>/dev/null || true
sleep 0.2
xmux start "$SESSION" --attach-to :0 --gl nvidia
echo "SPECTATOR: xmux attach $SESSION --no-reconnect"
echo "Pure VDPAU on host :0 — not Xvfb (NVIDIA presentation requires NVIDIA X)"
exec xmux run --gl nvidia "$SESSION" -- \
  "$PREFIX/bin/cvlc" --no-media-library --play-and-exit --verbose 2 \
  -V vdpau "$MEDIA"
