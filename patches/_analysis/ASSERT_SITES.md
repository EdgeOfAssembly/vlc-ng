# ASSERT_SITES — 4:4:4 → I420 path (`modules/codec/avcodec/video.c`)

**Branch:** `feature/hwaccel-patches`  
**Primary file:** `/tmp/vlc-ng/modules/codec/avcodec/video.c`  
**Date:** 2026-08-08  

---

## 1. Path summary: 4:4:4 → I420

### 1.1 Flag and state

| Symbol | Role |
|--------|------|
| `decoder_sys_t::b_sw_444_to_420` | When true, force SW decode + convert to I420 for VDPAU/legacy vouts (NVIDIA has no 4:4:4 surfaces). |
| `decoder_sys_t::p_sws_444` | Cached `SwsContext` for 10/12-bit 444→420; freed on decoder close. |
| `decoder_sys_t::b_direct_rendering` | Forced **false** when 444 path is active so DR cannot map 444 into I420 buffers. |

Init (`InitVideoDecCommon`, ~644–645):

```c
p_sys->b_sw_444_to_420 = false;
p_sys->p_sws_444 = NULL;
```

### 1.2 Detection — `ffmpeg_GetFormat` (~1947–1961)

When the software pix_fmt is planar 4:4:4 (`log2_chroma_w == 0 && log2_chroma_h == 0`, ≥3 components):

1. Log: `"4:4:4 bitstream: SW decode + I420 convert (no HW 444; VDPAU/GL friendly)"`
2. `can_hwaccel = false` — skip VAAPI/VDPAU decode for this stream
3. `p_sys->b_sw_444_to_420 = true`
4. `p_sys->b_direct_rendering = false` — **must** copy via `lavc_CopyPicture`

Else: `b_sw_444_to_420 = false`.

### 1.3 Output chroma override — `lavc_GetVideoFormat` (~163–169)

When `b_sw_444_to_420` and chroma is not palette RGB:

```c
fmt->i_chroma = VLC_CODEC_I420;
```

So the vout is asked for I420 even though lavc produces YUV444P/J/10/12.

### 1.4 DR disable — `lavc_GetFrame` (~1877–1885)

```c
if (!sys->b_direct_rendering || sys->b_sw_444_to_420)
{
    post_mt(sys);
    return avcodec_default_get_buffer2(ctx, frame, flags);
}
```

Belt-and-suspenders: even if DR were re-enabled, `b_sw_444_to_420` alone forces default lavc buffers. Decode path then hits `frame->opaque == NULL` and calls `lavc_CopyPicture` (~1567–1568).

### 1.5 Convert — `lavc_CopyPicture` (~371–538)

Gate (~392–398):

- `sys->b_sw_444_to_420`
- `pic->format.i_chroma == VLC_CODEC_I420`
- `frame->format` ∈ `{ YUV444P, YUVJ444P, YUV444P10LE, YUV444P12LE }`

Geometry (~399–425):

- `src_w/h` from `frame->width/height`
- `out_w/h` from visible size, clamped to plane pitch/lines
- Soft fail (warn + `VLC_EGENERIC`) if bad dims, `i_planes < 3`, NULL `frame->data[0..2]`, or non-positive linesizes

**8-bit fast path** (`YUV444P` / `YUVJ444P`, ~438–498):

- Y: row `memcpy` of `copy_w` for `copy_h` rows; pad remaining Y with `0x10`
- U/V: **2×2 box filter** average with `+2)/4` rounding into half-res chroma
- Edge clamp when odd width/height (`x1`/`y1` reuse last sample)

**10/12-bit path** (~501–538):

- `sws_getCachedContext` → `AV_PIX_FMT_YUV420P`, `SWS_BILINEAR`
- `sws_scale` into `pic->p[0..2]`

### 1.6 Assert sites in this path

| Location | Asserts |
|----------|---------|
| `lavc_CopyPicture` entry ~373–377 | `dec`, `pic`, `frame`, `sys` non-NULL |
| Before box/sws ~428–436 | `pic`, `frame`; `pic->p[0..2].p_pixels`; `frame->data[0..2]`; `out_w/h > 0`; `out_w ≤ p[0].i_pitch`; `out_h ≤ p[0].i_lines` |

Runtime NULL/geometry checks (~413–425) complement asserts (release builds keep soft-fail).

---

## 2. Remaining pointer uses without asserts

Pointers `dec`/`pic`/`frame`/`sys` and Y/U/V `p_pixels` / `frame->data[0..2]` **are** asserted before the convert body. Gaps are **bounds / secondary fields**, not missing top-level NULL checks.

### 2.1 Inside 444 convert (after asserts)

| Use | Risk | Assert? |
|-----|------|---------|
| `pic->p[1].i_pitch` / `pic->p[2].i_pitch` as UV row stride | UV write OOB if pitch &lt; `cw` | **No** — only Y pitch asserted vs `out_w` |
| `pic->p[1].i_lines` / `pic->p[2].i_lines` vs `ch` | UV write OOB if chroma lines short | **No** |
| `frame->linesize[0]` vs `copy_w` | Y read OOB | Soft-checked `> 0` only |
| `frame->linesize[1/2]` vs `copy_w` / `src_w` | UV read OOB in box filter | Soft-checked `> 0` only |
| `su0[x0]`…`sv1[x1]` indexing | Relies on linesize ≥ last index | **No** explicit assert |
| `du[x]` / `dv[x]` for `x < cw` | Relies on chroma pitch ≥ `cw` | **No** |
| `sys->p_sws_444` after `sws_getCachedContext` | NULL handled by soft fail | N/A (checked) |
| `sws_scale` src/dst pointers | Uses asserted `p_pixels` / `frame->data` | OK |

### 2.2 Geometry setup before plane asserts (~402–411)

| Use | Notes |
|-----|-------|
| `pic->p[0].i_visible_pitch/lines`, `i_pitch`, `i_lines` | Struct field reads; `i_planes < 3` checked **after** `p[0]` access. Safe if `picture_t::p[]` is fixed-size (VLC), but order is slightly inverted. |

### 2.3 Callers / setup (same feature path, no local asserts)

| Site | Deref | Assert? |
|------|-------|---------|
| `ffmpeg_GetFormat` ~1916–1917 | `p_context->opaque` → `p_dec` → `p_sys` | **No** (trust lavc opaque) |
| `ffmpeg_GetFormat` ~1951–1953 | `sw_desc->nb_components`, `log2_chroma_*` | `sw_desc != NULL` only |
| `lavc_GetFrame` ~1864–1865 | `ctx->opaque` → `dec` → `sys` | **No** |
| `lavc_GetVideoFormat` ~151–165 | `dec->p_sys`, `fmt` chroma write | **No** (caller-owned) |
| Decode ~1568 | `lavc_CopyPicture(p_dec, p_pic, frame)` | Asserts inside callee |
| Error path ~385 | `sys->p_context->pix_fmt` | `sys` asserted; `p_context` not |

### 2.4 Suggested assert additions (optional hardening)

```c
assert(pic->p[1].i_pitch >= cw && pic->p[2].i_pitch >= cw);
assert(pic->p[1].i_lines >= ch && pic->p[2].i_lines >= ch);
assert(frame->linesize[0] >= copy_w);
assert(frame->linesize[1] >= copy_w && frame->linesize[2] >= copy_w);
```

(Place after `cw`/`ch`/`copy_w` are computed; 8-bit path only for linesize≥copy_w, or use `src_w` before clamp.)

---

## 3. Configure flags — `scripts/configure-vlc-ng.sh`

File: `/tmp/vlc-ng/scripts/configure-vlc-ng.sh`

Relevant HW-accel / scale flags:

| Flag | Line | Purpose |
|------|------|---------|
| `--enable-avcodec` | 41 | lavc decoder (hosts this path) |
| `--enable-avformat` | 42 | demux |
| `--enable-swscale` | 43 | 10/12-bit 444→420 via libswscale |
| `--enable-postproc` | 44 | postproc |
| **`--enable-libva`** | **45** | **VAAPI** |
| **`--enable-vdpau`** | **46** | **VDPAU** |

Comment at top: feature set for GTX 1050 + Intel UHD PRIME. Extra args pass through `"$@"`.

Related helpers (not configure):

- `scripts/xmux-vdpau-host.sh` — pure `-V vdpau` on host `:0`
- `scripts/xmux-play-test.sh` — greps 444 / vout / vdpau logs

---

## 4. Control-flow diagram

```
ffmpeg_GetFormat
  └─ swfmt is 4:4:4?
       ├─ yes → b_sw_444_to_420=true, b_direct_rendering=false, can_hwaccel=false
       └─ no  → b_sw_444_to_420=false

lavc_GetFrame (p_va == NULL)
  └─ b_sw_444_to_420 || !b_direct_rendering
       → avcodec_default_get_buffer2  (no DR)

Decode (frame->opaque == NULL)
  └─ lavc_UpdateVideoFormat → chroma forced I420 if b_sw_444_to_420
  └─ lavc_CopyPicture
       ├─ asserts: dec/pic/frame/sys, p_pixels[0..2], data[0..2], Y pitch/lines
       ├─ 8-bit: Y memcpy + 2×2 box UV
       └─ 10/12-bit: sws_scale → YUV420P
```

---

## 5. Related docs / patches

- `patches/_analysis/VISUAL_QA.md` — green/flash root cause (DR into I420) and box-filter fix
- `patches/_analysis/vaapi-nvidia-limits.md` — NVIDIA VAAPI display limits
- Tree patch `patches/01-video-c-hwaccel-444.patch` is an **older slim** variant (only disables hwaccel on 444); **working tree** `video.c` has the full DR-off + box filter + sws path

---

## 6. Verdict

| Item | Status |
|------|--------|
| 444 detection + HW disable | Present |
| DR disable | Present (`b_direct_rendering=false` + `lavc_GetFrame` guard) |
| I420 output chroma | Present |
| Box filter 8-bit | Present |
| sws 10/12-bit | Present |
| Top-level NULL asserts | Present on convert entry + plane pixels |
| Chroma plane pitch/lines asserts | **Missing** |
| Source linesize ≥ width asserts | **Missing** (only `> 0`) |
| configure libva/vdpau | `--enable-libva` + `--enable-vdpau` |
