# VLC-ng visual / HW-accel QA matrix

Evidence dir (goal): `/tmp/grok-goal-c6ca9929f932/implementer/`  
Shots also: `/tmp/vlc-ng-shots/`

## Pure `-V vdpau` (NVIDIA presentation)

| Session | Picture | Evidence |
|---------|---------|----------|
| Xmux Xvfb (`xmux start … --gl nvidia`) | **BLACK** (backend may open) | `vdpau-pure-xvfb-t*.png` mean≈4.4 |
| `xmux start … --attach-to :0 --gl nvidia` | **OK** | `vdpau-pure-t*.png` mean≈140–150; log: `using the "vdpau" module` + NVIDIA backend |

**Policy:** Do not patch VLC for Xvfb. Use attach-to :0 for pure VDPAU visual QA.  
Helper: `scripts/xmux-vdpau-host.sh`  
Xmux doc: `docs/improved-xvfb.md` (EdgeOfAssembly/Xmux)

## Intel VAAPI (UHD 630 + iHD)

| Check | Result |
|-------|--------|
| Driver | `iHD_drv_video.so` (media-driver 26.2.3) |
| `vainfo --display drm --device $INTEL_RENDER` + `LIBVA_DRIVER_NAME=iHD` | H.264 Main/High VLD listed — `vainfo-intel.txt` |
| VLC `-V gl --avcodec-hw vaapi` + `LIBVA_DRIVER_NAME=iHD` under Xmux | Opens VAAPI (`trying format vaapi`, VAOP); often **SW chroma VAOP→I420** (no zero-copy glconv); picture **non-black** mean≈110 — `vaapi-intel.log`, `vaapi-intel-t*.png` |
| X11 `vainfo` with iHD forced on default GPU | Fails if default DRM is NVIDIA (`unsupported drm device … nvid`) — use **DRM + Intel render node** |

```bash
INTEL=$(readlink -f /dev/dri/by-path/pci-0000:00:02.0-render)
LIBVA_DRIVER_NAME=iHD vainfo --display drm --device "$INTEL"
LIBVA_DRIVER_NAME=iHD cvlc -V gl --avcodec-hw vaapi file.mp4
```

## NVIDIA VAAPI (nvidia-vaapi-driver)

**Hard limit (not fixed in VLC-ng):** no glconv match for zero-copy;  
`Using SW chroma filter for 1280x720 VAOP -> I420` then often falls to VDPAU.  
See `vaapi-nvidia-limits.md` + `vaapi-nvidia.log`.

**Prefer:** `-V gl --avcodec-hw vdpau` for NVIDIA HW decode under Xmux.

## Day-to-day (keep green)

| Path | Result | Evidence |
|------|--------|----------|
| `-V gl --avcodec-hw vdpau` 420 | OK, `Using NVIDIA VDPAU` | `daily-gl-vdpau.*` mean≈110 |
| `-V gl` duke3d **444** | OK native **I444** SW (no forced I420) | `daily-gl-444-fixed.*` — was green with forced I420 |

### 4:4:4 green cast (fixed)

Forced SW 4:4:4→I420 via swscale zeroed/wrong chroma → **pure green** (RGB≈0.5,56,0.4).  
**Fix:** keep native I444 for SW 444; no HW decode of High 4:4:4 (impossible on GTX 1050).

## Recommended commands

```bash
# NVIDIA under Xmux (best)
cvlc -V gl --avcodec-hw vdpau video-420.mp4

# Pure VDPAU picture (host NVIDIA X)
scripts/xmux-vdpau-host.sh video-420.mp4

# Intel VAAPI
LIBVA_DRIVER_NAME=iHD cvlc -V gl --avcodec-hw vaapi video-420.mp4

# 4:4:4 (SW, native I444)
cvlc -V gl duke3d-444.mp4
```

## 4:4:4 pure `-V vdpau` green/flash (fixed v2)

**Root cause:** avcodec direct rendering mapped H.264 4:4:4 into I420
`picture_t` buffers → wrong UV (green) + tearing.

**Fix:** `b_direct_rendering = false` when `b_sw_444_to_420`; decode to
default buffers then `lavc_CopyPicture` 2×2 box filter to I420.

**Proof:** gnome-screenshot during host `-vvv -V vdpau duke3d-444.mp4` —
G−R negative (not green); log: `SW 4:4:4→I420 (box filter) 800x600`.
