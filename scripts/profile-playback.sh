#!/bin/bash
# Fixed playback benchmark for VLC-ng. Writes report under patches/_analysis/profile/
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
PREFIX="${PREFIX:-/tmp/vlc-ng-install}"
export LD_LIBRARY_PATH="$PREFIX/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
export VLC_PLUGIN_PATH="$PREFIX/lib/vlc/plugins"
SAMPLE420="${SAMPLE420:-$ROOT/samples/test-420.mp4}"
SAMPLE444="${SAMPLE444:-$ROOT/samples/duke3d-444.mp4}"
DUR="${PROFILE_DUR:-15}"
TAG="${1:-run}"
OUTDIR="$ROOT/patches/_analysis/profile"
mkdir -p "$OUTDIR"
UTC=$(date -u +%Y%m%dT%H%M%SZ)
REPORT="$OUTDIR/${TAG}-${UTC}.txt"
VLC="$PREFIX/bin/cvlc"
if [[ ! -x "$VLC" ]]; then
  echo "error: $VLC missing" >&2
  exit 1
fi

{
  echo "=== VLC-ng profile $TAG $UTC ==="
  echo "PREFIX=$PREFIX"
  echo "DUR=${DUR}s"
  echo "VLC=$($VLC --version 2>&1 | head -1)"
  echo
  for label in gl-vdpau-420 vdpau-444; do
    echo "--- $label ---"
    case $label in
      gl-vdpau-420)
        CMD=(timeout "$DUR" "$VLC" --no-media-library --no-audio --play-and-exit
             -V gl --avcodec-hw vdpau "$SAMPLE420")
        ;;
      vdpau-444)
        CMD=(timeout "$DUR" "$VLC" --no-media-library --no-audio --play-and-exit
             -V vdpau "$SAMPLE444")
        ;;
    esac
    echo "cmd: ${CMD[*]}"
    # /usr/bin/time -v
    if command -v /usr/bin/time >/dev/null; then
      DISPLAY="${DISPLAY:-:0}" /usr/bin/time -v "${CMD[@]}" 2>&1 | tail -25 || true
    else
      DISPLAY="${DISPLAY:-:0}" "${CMD[@]}" 2>&1 | tail -10 || true
    fi
    echo
  done
  if command -v perf >/dev/null 2>&1; then
    echo "--- perf stat gl-vdpau-420 (optional) ---"
    DISPLAY="${DISPLAY:-:0}" perf stat -o /dev/stdout -- \
      timeout "$DUR" "$VLC" --no-media-library --no-audio --play-and-exit \
      -V gl --avcodec-hw vdpau "$SAMPLE420" 2>&1 | tail -30 || true
  fi
} | tee "$REPORT"
echo "Wrote $REPORT"
