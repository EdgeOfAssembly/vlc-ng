#!/bin/bash
# Play with Intel iHD VAAPI (UHD 630). Logs to stderr path if --log given.
set -euo pipefail
PREFIX="${PREFIX:-/tmp/vlc-ng-install}"
export LD_LIBRARY_PATH="$PREFIX/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
export VLC_PLUGIN_PATH="$PREFIX/lib/vlc/plugins"
export LIBVA_DRIVER_NAME="${LIBVA_DRIVER_NAME:-iHD}"
export LIBVA_DRIVERS_PATH="${LIBVA_DRIVERS_PATH:-/usr/lib64/va/drivers}"
INTEL_RENDER=$(readlink -f /dev/dri/by-path/pci-0000:00:02.0-render 2>/dev/null || true)
if [[ -n "${INTEL_RENDER:-}" ]]; then
  export LIBVA_DRM_DEVICE="$INTEL_RENDER"
fi
MEDIA="${1:?usage: vaapi-intel-play.sh media.mp4}"
shift || true
exec "$PREFIX/bin/cvlc" --no-media-library --no-audio -V gl --avcodec-hw vaapi "$MEDIA" "$@"
