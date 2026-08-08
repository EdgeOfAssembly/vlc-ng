# Visual QA notes (Xmux + terminal logs)

## Harness
- **Spectator:** `xmux attach vlc-ng --no-reconnect` on host `:0` (required while testing)
- **Terminal log:** `scripts/play-once.sh --log PATH` tees full VLC stdout/stderr
- **Screenshots:** `/tmp/vlc-ng-shots/` (outside repo gitignore so agents can `read_file` images)
- **Runner:** `scripts/xmux-play-test.sh <name> <secs> [vlc-args...] media`

## Results (2026-08-08)

| Test | Terminal | Visual (MCP shots) |
|------|----------|--------------------|
| `-V gl --avcodec-hw none` 420 | opened, glx | **OK** color bars, mean~110 |
| `-V vdpau` 420 | vdpau_display + NVIDIA backend | **BLACK** under Xmux/Xvfb (title bar only) |
| `-V gl --avcodec-hw vdpau` 420 | `Using NVIDIA VDPAU` + glconv_vdpau | **OK** color bars, mean~110 |
| `-V gl` duke3d 444 | `4:4:4 chroma not supported... software` | **OK** game frames visible |

## Notes
- Pure `-V vdpau` presents black on Xmux (Xvfb); prefer **GL display + VDPAU decode**.
- Patch 08 originally segfaulted on `-V gl` (null `psz_type` → `module_list_cap`); fixed with null guards + bank.c harden.
- Intel iHD VAAPI drivers not installed; VAAPI path uses `nvidia_drv_video.so` (NVDEC).
