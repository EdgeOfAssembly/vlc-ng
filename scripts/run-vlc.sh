#!/bin/bash
PREFIX="${PREFIX:-/tmp/vlc-ng-install}"
export LD_LIBRARY_PATH="$PREFIX/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
export VLC_PLUGIN_PATH="$PREFIX/lib/vlc/plugins"
exec "$PREFIX/bin/vlc" "$@"
