# VLC-ng TODO

Enhanced VLC **3.0.23** for older HW accel (GTX 1050 Pascal + Intel UHD 630 PRIME).  
Branch: `feature/hwaccel-patches` · install: `/tmp/vlc-ng-install` or ASan `/mnt/tmp/vlc-ng-asan`.

## Done (keep green)

- [x] Autoconf build (not meson); configure via `scripts/configure-vlc-ng.sh`
- [x] Phase1 GCC/build patches; HW patches 01/02/08/09
- [x] Pure `-V vdpau` on host (incl. 4:4:4 duke3d) — DR off + box I420 convert
- [x] `-V gl --avcodec-hw vdpau` under Xmux
- [x] 4:4:4 + GL (SW decode)
- [x] Intel iHD VAAPI (`LIBVA_DRIVER_NAME=iHD`)
- [x] VISUAL_QA matrix: `patches/_analysis/VISUAL_QA.md`

## Edge cases / known limits (open)

### Pure `-V vdpau` under default Xmux Xvfb → black

- **Symptom:** VDPAU backend may open; picture stays black in Xmux screenshots.
- **Cause:** NVIDIA presentation needs NVIDIA X; xmux-server is Mesa/llvmpipe Xvfb (GL PRIME copies back; pure VDPAU does not).
- **Workaround:** Host `:0`, or `xmux start … --attach-to :0 --gl nvidia`, or use `-V gl --avcodec-hw vdpau`.
- **Do not:** Patch VLC for Xvfb readback. Fix/document on Xmux side only.
- **Refs:** Xmux `docs/improved-xvfb.md`, `docs/private-stack/README.md`, `scripts/xmux-vdpau-host.sh`

### NVIDIA VAAPI zero-copy weak

- **Symptom:** `--avcodec-hw vaapi` on NVIDIA often hits `SW chroma filter … VAOP → I420` or falls back to VDPAU; no solid glconv zero-copy.
- **Cause:** nvidia-vaapi-driver + PRIME/Xvfb interop limits (not a small VLC-only fix).
- **Workaround:** Prefer **`-V gl --avcodec-hw vdpau`** for NVIDIA HW decode.
- **Refs:** `patches/_analysis/vaapi-nvidia-limits.md`

### H.264 High 4:4:4 — no HW decode on GTX 1050

- **Symptom:** Cannot use VDPAU/NVDEC for High 4:4:4 Predictive bitstreams.
- **Cause:** ASIC does not decode that profile; SW only (by design).
- **Workaround:** SW decode + I420 box filter for pure `-V vdpau`, or native I444 for GL.
- **Refs:** `modules/codec/avcodec/video.c` (`b_sw_444_to_420`)

### Intel VAAPI under PRIME / wrong DRM device

- **Symptom:** `LIBVA_DRIVER_NAME=iHD` with X11 `vainfo` can fail if default DRM is NVIDIA (`unsupported drm device … nvid`).
- **Workaround:** DRM + Intel render node:
  ```bash
  INTEL=$(readlink -f /dev/dri/by-path/pci-0000:00:02.0-render)
  LIBVA_DRIVER_NAME=iHD vainfo --display drm --device "$INTEL"
  LIBVA_DRIVER_NAME=iHD cvlc -V gl --avcodec-hw vaapi file.mp4
  ```

### NVENC encode — not tested

- Playback-focused build; no encode smoke tests yet.
- Optional: short NVENC encode/decode round-trip on GTX 1050.

### Avcodec shutdown log spam

- Patches 10 / 12* (suppress `get_buffer() failed` / `no frame!` on stop) **skipped** — previously broke hwaccel.
- Optional: quieter shutdown without reintroducing that regression.

### ASan vs day-to-day install

- Full ASan/UBSan tree: `/mnt/tmp/vlc-ng-asan` (debug; slower).
- Day-to-day prefix: **`/tmp/vlc-ng-install`** — rebuilt **2026-08-08** with `-O2 -DNDEBUG` (no ASan). Smoke: `-V vdpau` duke3d OK.

## Optional / later

- [ ] Merge `feature/hwaccel-patches` → `vlc-ng` when satisfied
- [ ] Non-ASan release rebuild of `/tmp/vlc-ng-install` from current tree
- [ ] NVENC smoke test
- [ ] Safer shutdown-log quieting (replace 10/12)
- [ ] Drop unused `libm` on xmux-wm if still reported by `ldd -u` (Xmux repo)

## Recommended commands

```bash
# NVIDIA day-to-day (Xmux OK)
cvlc -V gl --avcodec-hw vdpau file-420.mp4

# Pure VDPAU (host picture)
cvlc -V vdpau file.mp4
# or: scripts/xmux-vdpau-host.sh file.mp4

# 4:4:4
cvlc -V vdpau /tmp/vlc-ng/samples/duke3d-444.mp4

# Intel VAAPI
LIBVA_DRIVER_NAME=iHD cvlc -V gl --avcodec-hw vaapi file-420.mp4
```

## Profile helper

```bash
make -f Makefile.vlc-ng.mk profile          # rebuild -DNDEBUG + baseline report
make -f Makefile.vlc-ng.mk profile-run      # run only
# reports: patches/_analysis/profile/
```


## GOAL_PROMPT execution status (2026-08-08)

| Phase | Status |
|-------|--------|
| 0 Harness | done (gnome-screenshot + logs; Xmux MCP when session up) |
| 1 Checkpoint + ASan + asserts | done (`checkpoint/20260808T122355Z-pre-quality`, `/mnt/tmp/vlc-ng-asan`) |
| 2 Warnings | done for touched `video.c`; rest upstream — `patches/_analysis/WARNINGS.md` |
| 3 Remaining edges | helpers: `scripts/vaapi-intel-play.sh`; Xvfb pure VDPAU still external |
| 4 Debug regression | host `-V vdpau` 444 box filter OK; GL+VDPAU 420 OK; Xmux ASan GL shot flaky/black |
| 5 Profile | baseline-asan recorded; NDEBUG profile via `Makefile.vlc-ng.mk` for real numbers |


## GL acceleration check (Xmux)

- Package: **`x11-apps/mesa-progs`** → `glxgears`, `glxinfo`
- Under `xmux start … --gl nvidia`: **GTX 1050**, direct rendering Yes, glxgears ~**686 FPS**
- Evidence: `patches/_analysis/GLX_ACCEL.md`
