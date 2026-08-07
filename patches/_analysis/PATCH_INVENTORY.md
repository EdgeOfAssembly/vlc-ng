# VLC-ng Patch Inventory

**Workspace:** `/tmp/vlc-ng`  
**Sources:**  
- `A__*` ← `$HOME/vlc-patches`  
- `B__*` ← `$HOME/vlc-ng/vlc-patches` (superset; this inventory)  
- Unprefixed copies also present under `_sources/` (same content as A/B for shared basenames)

**Docs consulted:**  
- `$HOME/vlc-ng/PATCHES_AND_CHANGES.md` (2026-07-06; notes 10/12 **REVERTED** for hwaccel breakage)  
- `$HOME/vlc-3.0.23/TODO.md` (4:4:4 + VDPAU status; patches 01/02 working)

**Date of inventory:** 2026-08-08

---

## 1. Unique B__* patches (superset)

| # | Basename | Purpose (1–3 lines) | Files touched | Risk |
|---|----------|---------------------|---------------|------|
| 01 | `01-video-c-hwaccel-444.patch` | Detect software pixfmt 4:4:4 (`log2_chroma_w/h == 0`) and set `can_hwaccel = false` so VDPAU/VAAPI are not offered for H.264 4:4:4. | `modules/codec/avcodec/video.c` | **Runtime / hwaccel** — intentional decode-path change; low risk if correct (fallback to SW). |
| 02 | `02-vdpau-display-444.patch` | In VDPAU display `Open()`, reject `VDP_CHROMA_TYPE_444` with debug log + `goto error` so vout probing falls back (e.g. GL). | `modules/hw/vdpau/display.c` | **Runtime / hwaccel** — display probe only; safe graceful reject. |
| 03 | `03-fix-stringop-overflow-bits.patch` | GCC pragma ignore `-Wstringop-overflow` around `bits_align` / `bits_write` in MPEG mux bit buffer. | `modules/mux/mpeg/bits.h` | **Build-only** (warning silence). |
| 04 | `04-fix-unused-variable-es_out.patch` | After `assert(!i_ret)`, add `(void)(!i_ret)` so release builds don’t warn unused `i_ret`. | `src/input/es_out.h` | **Build-only**. |
| 05 | `05-fix-pl-api-ver-undef.patch` | Guard `PL_API_VER >= 157` with `defined(PL_API_VER)` for older/missing libplacebo. | `modules/video_output/opengl/converter.h` | **Build-only** (compat). |
| 06 | `06-fix-implicit-fallthrough.patch` | Add `/* fall through */` (or normalize spelling) for `-Wimplicit-fallthrough` across many modules. | Many: preparser, linsys_sdi, cdrom, smb, zvbi, speex, ty, es.c, mkv, ncurses, qt controller, upnp, chromecast, archive, ripple, wave | **Build-only** (comments; intentional fallthrough preserved). |
| 07 | `07-fix-stringop-truncation.patch` | Replace unsafe `strncpy` with `memcpy`+NUL (satip, rtp, remoteosd, ogg) and force NUL after `strncpy` (telx). | satip, telx, rtp, remoteosd, ogg | **Build-only** primarily; tiny runtime string safety. **Overlaps** with standalone `satip`/`telx`/`rtp`/`remoteosd`/`ogg` patches. |
| 08 | `08-vout-backend-print-and-help-list.patch` | INFO log selected vout display shortcut; `ListChoiceHelp` for `-V help` / invalid module; validate module names. | `src/modules/modules.c`, `src/config/cmdline.c` | **UX / runtime** (CLI + logging only; no decode path). |
| 09 | `09-update-vlc-man-page.patch` | Modernize ancient `vlc.1` (2005→2026): controls, examples, options pointers. | `doc/vlc.1` | **Docs only**. |
| 10 | `10-suppress-decoder-shutdown-logs.patch` | Around `avcodec_flush_buffers` in `EndVideoDec`, temporarily `av_log_set_level(AV_LOG_FATAL)`. | `modules/codec/avcodec/video.c` | **Runtime / risky** — global FFmpeg log level; **REVERTED** (broke hwaccel). |
| 11 | `11-fix-wavskipheader-i_len.patch` | After shadow rename left `i_len` used but only `i_len2` declared: rename consistently to `i_len` and fix indentation. | `modules/demux/mpeg/es.c` | **Build fix** (compile error after shadow). Depends on shadow patch order. |
| 12a | `12-avcodec-shutdown-log-suppress.patch` | Same idea as **10** (flush-time FATAL log level). Near-duplicate of 10. | `modules/codec/avcodec/video.c` | **Runtime / risky** — **alternative to 10**, not additive. **REVERTED**. |
| 12b | `12-filter-avcodec-shutdown-logs.patch` | Custom `av_log_set_callback` filtering `get_buffer() failed` / `no frame!` strings. | `modules/codec/avcodec/avcommon.h` | **Runtime / risky** — global callback; **REVERTED** with 10/12a. |
| — | `block_FifoCount-deprecation.patch` | Replace deprecated `block_FifoCount` → `vlc_fifo_GetCount`. | stream_output, udp, rtsp, dummy/avi/mp4/mpjpeg/wav/ogg/ts mux, vod, shine | **Build-only** (API deprecation). **Overlaps** ogg with `ogg.patch` / 07. |
| — | `fix-misleading-indentation.patch` | Brace `if` body in AV1 packetizer macro so DTS/PTS assignment is clearly conditional. | `modules/packetizer/av1.c` | **Build-only** (also clarity; could affect control flow if old code was wrong — intended fix). |
| — | `fix-shadow-warnings.patch` | Rename locals/macros that shadow outer names (`i_index_`, `fe_idx_`, `i_len2`, etc.). **Incomplete on es.c:** renames to `i_len2` but leaves `… - i_len - 8` → needs **11**. | Many codecs/demux/mux/filters + `include/vlc_arrays.h` | **Build-only**, but **introduces compile error** in `es.c` until 11. |
| — | `fix-uninitialized-warnings.patch` | Zero-init locals/arrays (`memset`, `= {0}`, hoist `systems = 0`) for `-Wmaybe-uninitialized`. | picture_pool, dtv/linux, pva, dmxmus, avi, mpeg4_iod, libmp4mux, qt, rtpfmt, converter_vdpau, media_list_player, av1_obu, hotkeys | **Build-only** (defensive inits). |
| — | `ogg.patch` | Subset of 07 + block_Fifo: `strncpy→memcpy` on oggds sub_type **and** `block_FifoCount→vlc_fifo_GetCount`. | `modules/mux/ogg.c` | **Build-only**. **Redundant** if both 07 and `block_FifoCount-deprecation` applied. |
| — | `remoteosd.patch` | `strncpy→memcpy` for RFB protocol version string. | `modules/spu/remoteosd.c` | **Build-only**. **Subset of 07**. |
| — | `rtp.patch` | `strncpy→memcpy` for VOD session id seed. | `modules/stream_out/rtp.c` | **Build-only**. **Subset of 07**. |
| — | `satip.patch` | `strncpy→memcpy`+NUL for session/address/port parsing. | `modules/access/satip.c` | **Build-only**. **Subset of 07**. |
| — | `telx.patch` | Force NUL after `strncpy` on teletext line buffers. | `modules/codec/telx.c` | **Build-only**. **Subset of 07**. |

### A__* vs B__* note

- **A** has 01–07 + build helpers (shadow, uninit, FifoCount, misleading-indent, ogg/rtp/satip/telx/remoteosd).  
- **B** adds **08, 09, 10, 11, 12a, 12b** (UX, man, shutdown-log experiments, es.c fix).  
- Prefer **B** as the apply set; treat unprefixed and A copies as duplicates.

### Overlap / redundancy map

```
07-fix-stringop-truncation  ⊃  satip + telx + rtp + remoteosd + (ogg strncpy part)
block_FifoCount-deprecation  ⊃  (ogg FifoCount part) + many mux files
ogg.patch                    =  ogg strncpy + ogg FifoCount  (use 07 + FifoCount OR ogg alone)
10  ≈  12-avcodec-shutdown-log-suppress   (pick at most one; recommend neither)
12-filter-avcodec-shutdown-logs          (orthogonal mechanism; still SKIP)
fix-shadow-warnings  →  requires 11-fix-wavskipheader-i_len  (or fix shadow in-place)
```

---

## 2. Special focus

### 2.1 `01-video-c-hwaccel-444.patch`

- **What:** In avcodec video format negotiation, if SW format is full-chroma 4:4:4, disable hwaccel candidates.  
- **Why:** Nvidia VDPAU (and typical VAAPI H.264 paths) do not decode 4:4:4; without this, users get `get_buffer() failed` / broken decode.  
- **Risk:** Runtime, but **desired** for duke3d.mp4-class content. Does not touch log callbacks.  
- **Pair with:** 02 (display-side reject if someone forces `-V vdpau` on 4:4:4 SW frames).

### 2.2 `02-vdpau-display-444.patch`

- **What:** Early reject of 4:4:4 chroma in VDPAU display open.  
- **Why:** Avoid flicker / “YCbCr format not supported”; let GL take over.  
- **Risk:** Low; only affects VDPAU display probe for 444.

### 2.3 `08-vout-backend-print-and-help-list.patch`

- **What:**  
  1. `msg_Info` when capability is `"vout display"` using last module shortcut (user-facing name).  
  2. `ListChoiceHelp` for module options (`-V help`, bare/invalid `-V`).  
  3. Validate module shortcut against `module_list_cap`.  
- **Risk:** UX only; no hwaccel. Slight behavior change: invalid `-V foo` now **exits 1** with list instead of soft fail — intentional.  
- **Note:** `ListChoiceHelp` calls `exit(0)` after printing — fine for help path.

### 2.4 Shutdown log patches: 10 vs 12a vs 12b

| Patch | Mechanism | Scope | Safer? |
|-------|-----------|-------|--------|
| **10** | `av_log_set_level(FATAL)` around flush only | Global log level briefly | Slightly narrower than 12b |
| **12-avcodec-shutdown-log-suppress** | Same as 10 | Same file/function | **Duplicate of 10** — do not apply both |
| **12-filter-avcodec-shutdown-logs** | Custom `av_log_set_callback` filters error strings forever | All avcodec after init | **Broadest / most invasive** |

**Verdict:**

- They are **alternatives / stacked experiments**, not a required series.  
- **10 ≈ 12a** (same approach). **12b** is a different global filter.  
- Docs (`PATCHES_AND_CHANGES.md`): logging changes **REVERTED** because they caused *“hardware accelerator failed to decode picture”* (likely global log level / callback side-effects on FFmpeg error paths used by VDPAU).  
- **Safer choice: SKIP all of 10, 12a, 12b.** Accept noisy shutdown messages.  
- If revisit later: prefer **per-context** or **only-during-flush** without global callback; never combine 10+12b without isolated testing of 4:2:0 VDPAU.

### 2.5 `11-fix-wavskipheader-i_len.patch` and shadow

- **`fix-shadow-warnings`** renames `i_len` → `i_len2` in `WavSkipHeader` but **does not** update:

  ```c
  const int i_format = GetWLE( p_peek + i_peek - i_len - 8 /* wFormatTag */ );
  ```

  → **`i_len` undeclared** compile error.

- **11** restores consistent name `i_len` (and keeps the improved bounds check from shadow).  
- **Depends on:** apply **after** `fix-shadow-warnings.patch` (or fold 11 into shadow and drop 11).  
- **Does not** depend on 10/12. Independent of hwaccel.

---

## 3. Safe incremental apply order

Apply from VLC source root with `patch -p1 < …` (or `-p0` if paths are absolute — B patches mix `a/` relative and absolute `/tmp/vlc-…` paths; **normalize path strip** when applying). Prefer copies with relative `a/` headers (08, 10, 11) or fix paths first.

### Phase 1 — Pure build / modern GCC (required for clean build)

Order within phase is mostly independent except **shadow → 11**.

1. `05-fix-pl-api-ver-undef.patch`  
2. `03-fix-stringop-overflow-bits.patch`  
3. `04-fix-unused-variable-es_out.patch`  
4. `06-fix-implicit-fallthrough.patch`  
5. `fix-misleading-indentation.patch`  
6. `fix-uninitialized-warnings.patch`  
7. `block_FifoCount-deprecation.patch`  
8. `07-fix-stringop-truncation.patch`  
   - **Do not also apply** standalone `satip` / `telx` / `rtp` / `remoteosd` / `ogg` (redundant; reject/fuzz).  
9. `fix-shadow-warnings.patch`  
10. `11-fix-wavskipheader-i_len.patch`  ← **required after shadow**

**Optional Phase 1 alternatives (if not using 07 + FifoCount):**  
- Use `ogg.patch` + individual satip/telx/rtp/remoteosd **instead of** 07 + FifoCount — not both.

### Phase 2 — Feature / hwaccel (01, 02)

11. `01-video-c-hwaccel-444.patch`  
12. `02-vdpau-display-444.patch`  

**Verify after Phase 2:**  
- 4:2:0 + `-V vdpau` → hw decode  
- 4:4:4 (e.g. duke3d.mp4) → SW + GL, no crash  
- Forced `-V vdpau` on 4:4:4 → graceful fallback

### Phase 3 — UX / docs

13. `08-vout-backend-print-and-help-list.patch`  
14. `09-update-vlc-man-page.patch`  

### Phase 4 — Optional / risky (default: **do not apply**)

15. ~~`10-suppress-decoder-shutdown-logs.patch`~~  
16. ~~`12-avcodec-shutdown-log-suppress.patch`~~  
17. ~~`12-filter-avcodec-shutdown-logs.patch`~~  

Per `PATCHES_AND_CHANGES.md`: these were **REVERTED** after breaking hwaccel. Keep out of the default series.

---

## 4. SKIP list (recommended)

| Patch | Why SKIP |
|-------|----------|
| `10-suppress-decoder-shutdown-logs.patch` | Broke hwaccel; global `av_log` level during flush |
| `12-avcodec-shutdown-log-suppress.patch` | Duplicate of 10; same risk |
| `12-filter-avcodec-shutdown-logs.patch` | Global log callback; same revert class |
| `satip.patch` | Subsumed by **07** |
| `telx.patch` | Subsumed by **07** |
| `rtp.patch` | Subsumed by **07** |
| `remoteosd.patch` | Subsumed by **07** |
| `ogg.patch` | Subsumed by **07** + **block_FifoCount-deprecation** |
| All `A__*` when applying `B__*` | Duplicates |
| Unprefixed twins of the same basename | Duplicates |

**Do not SKIP (if you want the known-good feature set):** 01, 02, 08, 09, 11, and Phase 1 build fixes.

---

## 5. Recommended “known good” series (minimal)

Matches post-revert baseline in `PATCHES_AND_CHANGES.md`:

```text
Phase1: 05, 03, 04, 06, fix-misleading-indentation, fix-uninitialized-warnings,
        block_FifoCount-deprecation, 07, fix-shadow-warnings, 11
Phase2: 01, 02
Phase3: 08, 09
Phase4: (empty — skip 10/12*)
```

---

## 6. Apply caveats

1. **Path style:** Some B patches use absolute paths (`/tmp/vlc-3.0.23.org/…`); others use `a/…`. May need `patch -p0` vs `-p1` or path rewriting.  
2. **Overlap fuzz:** Applying both 07 and satip/telx/… will fail or no-op with rejects.  
3. **Shadow without 11:** hard compile error in `modules/demux/mpeg/es.c`.  
4. **10/12 after 01:** still unsafe — breakage was on hwaccel decode, not only 4:4:4.  
5. **Test matrix after full apply:** build `make -j$(nproc)`; play 4:2:0 VDPAU; play 4:4:4 SW; `vlc -V help`; check INFO line `video output: using the "…" module`.

---

## 7. Source file index (absolute paths)

All under `/tmp/vlc-ng/patches/_sources/`:

- `B__01-video-c-hwaccel-444.patch`
- `B__02-vdpau-display-444.patch`
- `B__03-fix-stringop-overflow-bits.patch`
- `B__04-fix-unused-variable-es_out.patch`
- `B__05-fix-pl-api-ver-undef.patch`
- `B__06-fix-implicit-fallthrough.patch`
- `B__07-fix-stringop-truncation.patch`
- `B__08-vout-backend-print-and-help-list.patch`
- `B__09-update-vlc-man-page.patch`
- `B__10-suppress-decoder-shutdown-logs.patch` ← SKIP
- `B__11-fix-wavskipheader-i_len.patch`
- `B__12-avcodec-shutdown-log-suppress.patch` ← SKIP
- `B__12-filter-avcodec-shutdown-logs.patch` ← SKIP
- `B__block_FifoCount-deprecation.patch`
- `B__fix-misleading-indentation.patch`
- `B__fix-shadow-warnings.patch`
- `B__fix-uninitialized-warnings.patch`
- `B__ogg.patch` / `B__remoteosd.patch` / `B__rtp.patch` / `B__satip.patch` / `B__telx.patch` ← SKIP if using 07+FifoCount

---

*End of inventory.*
