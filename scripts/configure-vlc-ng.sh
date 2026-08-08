#!/bin/bash
# Autoconf configure for VLC-ng 3.0.23
# Feature set mapped from $HOME/vlc.txt (meson) + HW accel for GTX 1050 + Intel UHD PRIME.
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT"
PREFIX="${PREFIX:-/tmp/vlc-ng-install}"

export LIVE555_CFLAGS="${LIVE555_CFLAGS:--I/usr/include/liveMedia -I/usr/include/BasicUsageEnvironment -I/usr/include/UsageEnvironment -I/usr/include/groupsock}"
export LIVE555_LIBS="${LIVE555_LIBS:--L/usr/lib64 -lliveMedia -lgroupsock -lUsageEnvironment -lBasicUsageEnvironment}"
export CFLAGS="${CFLAGS:--O2 -g -pipe}"
export CXXFLAGS="${CXXFLAGS:--O2 -g -pipe}"

./configure \
  --prefix="$PREFIX" \
  --disable-debug \
  --disable-nls \
  --disable-rpath \
  --disable-pulse \
  --disable-wayland \
  --enable-run-as-root \
  --enable-dbus \
  --enable-sout \
  --enable-lua \
  --enable-live555 \
  --enable-archive \
  --enable-dvdread \
  --enable-dvdnav \
  --enable-bluray \
  --enable-sftp \
  --enable-nfs \
  --enable-v4l2 \
  --disable-libcddb \
  --enable-realrtsp \
  --enable-gme \
  --enable-shout \
  --enable-mod \
  --disable-mpc \
  --enable-mad \
  --enable-mpg123 \
  --enable-avcodec \
  --enable-avformat \
  --enable-swscale \
  --enable-postproc \
  --enable-libva \
  --enable-vdpau \
  --enable-faad \
  --enable-aom \
  --enable-dav1d \
  --enable-vpx \
  --enable-flac \
  --enable-speex \
  --enable-theora \
  --enable-png \
  --enable-jpeg \
  --enable-x264 \
  --enable-x265 \
  --enable-fluidsynth \
  --enable-zvbi \
  --enable-kate \
  --enable-tiger \
  --enable-xcb \
  --enable-alsa \
  --disable-jack \
  --enable-qt \
  --disable-skins2 \
  --enable-ncurses \
  --enable-freetype \
  --enable-fribidi \
  --enable-fontconfig \
  --enable-libass \
  --enable-svg \
  --enable-samplerate \
  --enable-soxr \
  --disable-chromaprint \
  --enable-chromecast \
  --disable-lirc \
  --enable-srt \
  --enable-projectm \
  --enable-avahi \
  --enable-udev \
  --enable-mtp \
  --enable-upnp \
  --enable-libgcrypt \
  --enable-gnutls \
  --enable-secret \
  --enable-notify \
  --enable-taglib \
  --disable-libplacebo \
  --disable-a52 \
  --disable-dca \
  --disable-fdkaac \
  --disable-dc1394 \
  --disable-dv1394 \
  --disable-linsys \
  --disable-smbclient \
  --disable-omxil \
  --disable-gst-decode \
  --disable-sid \
  --with-contrib=no \
  "$@"

echo "Configure done. PREFIX=$PREFIX"
