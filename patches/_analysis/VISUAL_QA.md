# Visual QA + VDPAU 4:4:4→4:2:0

## Technical fact (NVIDIA GTX 1050)

**H.264 High 4:4:4 Predictive cannot be hardware-decoded by VDPAU/NVDEC.**
The ASIC only decodes 4:2:0 (and limited 4:2:2 for some codecs). You cannot
feed a 4:4:4 bitstream into VDPAU decode. Conversion must be:

```
SW decode (yuv444p) → swscale → I420 → VDPAU import/display or GL
```

## Implemented (vlc-ng)

1. Detect 4:4:4 SW format in `GetFormat` → disable HW decode, set `b_sw_444_to_420`
2. Force decoder output chroma to **I420**
3. `lavc_CopyPicture` uses **libswscale** to convert each frame 444→420
4. VDPAU display queries real 444 surface caps (no blind reject)

## Path matrix (Xmux + NVIDIA PRIME)

| Content | Command | Result |
|---------|---------|--------|
| 420 | `-V gl --avcodec-hw none` | OK picture |
| 420 | `-V gl --avcodec-hw vdpau` | OK picture + NVIDIA VDPAU decode |
| 420 | `-V vdpau` | Opens VDPAU; **black under Xmux/Xvfb** (host :0 better) |
| 444 | `-V gl` | OK; SW decode + I420 convert |
| 444 | `-V vdpau` | Pipeline OK (I420 + vdpau_chroma); **black under Xmux** |

## Still limited

- Pure `-V vdpau` presentation on Xmux/Xvfb stays black (driver presents, pixels not visible in Xvfb capture). Prefer GL display.
- Intel iHD VAAPI still not installed.
- NVENC encode not tested.
