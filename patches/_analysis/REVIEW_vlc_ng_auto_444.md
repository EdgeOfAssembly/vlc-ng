# Code review: VLC-ng auto backend + 4:4:4 box filter

**Scope:** branch `feature/hwaccel-patches` (tip: vlc-ng-auto, 4:4:4 box filter, asserts)  
**Mode:** read-only  
**Date:** 2026-08-08  
**Reviewer:** senior code review (orchestrated)

### Primary artifacts

| Path | Role |
|------|------|
| `/tmp/vlc-ng/scripts/vlc-ng-auto` | Auto `-V` / `--avcodec-hw` policy |
| `/tmp/vlc-ng/scripts/cvlc` | Thin wrapper (`VLC_NG_AUTO=1` default) |
| `/tmp/vlc-ng/.gitignore` | Un-ignore `scripts/cvlc` |
| `/tmp/vlc-ng/modules/codec/avcodec/video.c` | `b_sw_444_to_420`, DR off, box filter, asserts |
| `/tmp/vlc-ng/src/config/cmdline.c` | Patch 08 help/list + `psz_type` null guards (tree) |
| `/tmp/vlc-ng/patches/08-vout-backend-print-and-help-list.patch` | Patch file (stale vs tree) |
| `/tmp/vlc-ng/TODO.md`, `patches/_analysis/VISUAL_QA.md` | Docs / QA claims |

---

## Summary

The auto-backend script is a solid, security-conscious bash tool: array/`exec` argv, `%q` dry-run, explicit `-V` / `--avcodec-hw` preservation, and a **correct default under Xmux/Xvfb** (`-V gl --avcodec-hw vdpau`, never pure `-V vdpau`). The 4:4:4 path correctly disables HW decode and direct rendering, then converts with an 8-bit 2×2 box filter (swscale for 10/12-bit), which matches the pure-VDPAU green/flash fix narrative.

The main product risk is **policy/doc drift on GL 4:4:4**: working-tree code **always** forces I420 when planar 4:4:4 is detected, while `TODO.md` / `VISUAL_QA.md` still claim **native I444 for `-V gl`**. Secondary risks: NDEBUG soft-fail gaps on chroma plane geometry (asserts compile out), stale patch files vs tree (01, 08), and no automated policy matrix for `vlc-ng-auto`.

**Verdict:** **Approve with changes** — ship auto + host pure-VDPAU 444 path after fixing doc/code alignment (or gating I420 to VDPAU-only) and hardening NDEBUG bounds checks.

---

## Strengths

1. **`vlc-ng-auto` Xmux/PRIME policy is right**
   - Virtual X detection: `XMUX_*`, `xmux-server`/`Xvfb` on display number, llvmpipe+NVIDIA/PRIME.
   - NVIDIA + VDPAU → always `CHOICE_VOUT=gl` + `CHOICE_HW=vdpau` whether host or virtual; pure `-V vdpau` is **never** the auto default.
   - Explicit comment and behavior: PRIME offload alone does **not** imply virtual (host `:0` often has `__NV_PRIME_RENDER_OFFLOAD`).
   - User `-V` / `--vout` / `--avcodec-hw` (including `=value` forms) are detected and not overridden.

2. **CLI hygiene (mostly)**
   - No-args → usage, exit 2; `-h`/`--help`; `-v`/`--version` from 0.1.0.
   - `--dry-run` prints shell-quoted command on stdout; human notes on stderr.
   - `--explain` for probe verbosity; `set -euo pipefail`.

3. **Security posture of scripts**
   - No `eval`; `exec "${CMD[@]}"` and `"${HERE}/vlc-ng-auto" "$@"` preserve argv.
   - Dry-run uses `printf %q`.
   - `EXTRA_ENV` values come from `readlink -f` of `/dev/dri` paths + fixed `LIBVA_DRIVER_NAME=iHD`, not free-form user strings.
   - Probes time-bounded (`timeout 3`).

4. **4:4:4 correctness core**
   - `ffmpeg_GetFormat`: planar 4:4:4 → `can_hwaccel=false`, `b_sw_444_to_420=true`, `b_direct_rendering=false`.
   - `lavc_GetFrame`: belt-and-suspenders `!b_direct_rendering \|\| b_sw_444_to_420` → default lavc buffers (no DR into I420 planes).
   - `lavc_GetVideoFormat`: force `VLC_CODEC_I420` for vout negotiation.
   - 8-bit path: Y row `memcpy` + 2×2 UV average with `+2)/4` rounding; edge clamp on odd dims.
   - Soft geometry fail before convert body; `p_sws_444` freed on close.
   - Asserts on entry + Y/UV pitch/lines + plane pointers (debug builds).

5. **cmdline.c tree hardening (beyond patch 08 file)**
   - `cfg->psz_type` / `p_conf->psz_type` / `p_item->psz_type` null-checked before `module_list_cap`.
   - `mods != NULL && n > 0` before iterating; free-safe when empty.

6. **`.gitignore`**
   - Root `cvlc` still ignored (build artifact); `!scripts/cvlc` keeps the wrapper tracked. Correct pattern.

7. **Operational docs**
   - `TODO.md` documents pure-VDPAU-under-Xvfb black as external/Xmux, not a VLC patch target — aligned with `VISUAL_QA.md` and `xmux-vdpau-host.sh`.

---

## Critical

_None found that are clear security/memory-corruption certainties on well-formed streams._

(Closest: NDEBUG OOB on **malformed** 444 frames if plane pitch/linesize lie — filed as Major.)

---

## Major

### M1. Unconditional I420 conversion contradicts GL “native I444” docs

**Code** (`video.c` ~1955–1967, ~165–169): any planar 4:4:4 software format sets `b_sw_444_to_420` and forces `fmt->i_chroma = VLC_CODEC_I420` for **all** vouts.

**Docs claim otherwise:**

- `TODO.md` L38: “SW decode + I420 box filter for pure `-V vdpau`, **or native I444 for GL**”
- `VISUAL_QA.md` L45–50, L64–65: “OK native **I444** SW (no forced I420)”; recommended `cvlc -V gl duke3d-444.mp4`

**Impact:** GL 4:4:4 loses chroma resolution; QA matrix is misleading; “daily-gl-444-fixed” evidence may be stale relative to current tree.

**Fix (pick one and update docs):**

1. **Preferred product behavior:** gate I420 force on vout/capability needing 4:2:0 (e.g. only when VDPAU display / no I444 path), keep native I444 for GL; or  
2. **If intentional simplicity:** rewrite TODO/VISUAL_QA to say “all 444 → SW + I420 box filter (GL included)” and re-shot GL 444 as I420 (not green).

### M2. NDEBUG safety: soft checks weaker than asserts on box-filter path

Under `-DNDEBUG` (day-to-day `/tmp/vlc-ng-install` and `Makefile.vlc-ng.mk profile-build`), `assert()` at ~428–441 disappears.

Soft gate (~413–425) checks:

- dims > 0, `i_planes >= 3`, `frame->data[0..2]`, `linesize[0..2] > 0`

It does **not** re-check (assert-only today):

- `out_w/h` vs Y pitch/lines  
- `(out_w+1)/2` vs U/V pitch/lines  
- `frame->linesize[*] >= copy_w` (or `src_w`)

**Impact:** corrupt/odd lavc frames or mis-sized pictures can OOB-read/write in the box filter on release builds. Debug ASan tree would catch; NDEBUG would not.

**Fix:** promote the critical assert conditions to soft `if (...) { msg_Warn; return VLC_EGENERIC; }` (keep asserts as duplicates for debug).

### M3. Patch files stale vs working tree (reproducibility)

| Artifact | Tree reality | Patch file |
|----------|--------------|------------|
| `patches/01-video-c-hwaccel-444.patch` | Full DR-off + box filter + sws + asserts | Only disables `can_hwaccel` on 444 |
| `patches/08-vout-backend-print-and-help-list.patch` | Null `psz_type` guards + safer `module_list_cap` | Calls `module_list_cap` without null guards |

`ASSERT_SITES.md` already notes patch 01 is slim; same class of issue for 08.

**Impact:** “apply patches from `patches/`” does **not** reconstruct the tip; review/CI from patches alone is wrong.

**Fix:** regenerate normalized patches from `git diff` against clean 3.0.23, or document “tree is source of truth; patches/ historical”.

### M4. No automated tests for `vlc-ng-auto` policy matrix

Policy is the product surface for day-to-day play. There is no `make test` / shell unit covering:

| Scenario | Expected |
|----------|----------|
| Host NVIDIA + VDPAU | `-V gl --avcodec-hw vdpau` |
| Xmux/Xvfb + NVIDIA (virtual_x=1) | same; **not** `-V vdpau` |
| User `-V vdpau` | vout not overridden |
| User `--avcodec-hw none` | hw not overridden |
| Intel + iHD render node | vaapi + `LIBVA_DRIVER_NAME=iHD` |
| `--dry-run` | exit 0, stdout is quotable command |

**Impact:** regressions in the “never pure vdpau on virtual X” rule are easy to reintroduce.

### M5. `VISUAL_QA.md` internal tension (444 sections)

- § “4:4:4 green cast (fixed)” → keep **native I444**, avoid forced I420.  
- § “4:4:4 pure `-V vdpau` green/flash (fixed v2)” → **force I420** + box filter + DR off.

Both can be true **if** I420 is vout-conditional; with unconditional code, the first section is false. Matrix should state measured chroma (`I444` vs `I420`) from current logs, not historical intent.

---

## Minor

### m1. `vlc-ng-auto -v` vs VLC verbose

`-v` / `--version` exit immediately (cli-design compliant). VLC users often type `-v`/`-vv` for verbosity.  
`-vvv` is passed through (not exact `-v`); bare `-v` is not. Document: use `vlc-ng-auto -- -v …` or put `-v` after media / use VLC long options. Consider only treating `--version` as version and leaving `-v` for VLC (trade-off vs house CLI rule).

### m2. `static bool logged` in box-filter path

Process-lifetime, not per-decoder; not thread-safe if multiple 444 decoders log once. Harmless for single-play; prefer `sys->` flag if multi-instance matters.

### m3. Intel VAAPI env always forces `iHD`

On Intel path, `EXTRA_ENV` sets `LIBVA_DRIVER_NAME=iHD` even when only `HAS_VAAPI_ANY` (not `HAS_VAAPI_INTEL`). Fine on this laptop; may break exotic Intel stacks without iHD.

### m4. Hybrid NVIDIA without VDPAU → may pick NVIDIA VAAPI

`HAS_NVIDIA && !HAS_VDPAU` uses `vaapi` if any vainfo works — known weak on NVIDIA (`vaapi-nvidia-limits.md`). Acceptable fallback; `--explain` should make it obvious.

### m5. `ASSERT_SITES.md` slightly stale

Claims UV pitch/lines asserts missing; tree **has** them at ~437–440. Still correct that **soft** NDEBUG checks for those are missing (see M2).

### m6. TODO optional item vs done note

TODO “Non-ASan release rebuild…” still open while “Day-to-day prefix rebuilt 2026-08-08 with `-O2 -DNDEBUG`” is checked in Edge cases. Clarify checkbox state.

### m7. `scripts/cvlc` silent fallback

If `vlc-ng-auto` missing/non-executable, wrapper falls through to plain `PREFIX/bin/cvlc` without warning. Prefer one stderr line when AUTO=1 but auto binary missing.

### m8. Patch 08 validation vs object name

Module validation matches **shortcuts** only, not `module_get_object` internal names. Users typing `vdpau_display` may get “unknown” while shortcut `vdpau` works. UX only.

### m9. `ListChoiceHelp` / error path mutates argv

`strchr` then `*eq = '\0'` temporarily mutates `ppsz_argv` string for `--foo=bar` parsing in error path. Restored after; still slightly hostile if argv is shared. Pre-existing style in this patch family.

---

## Nit

- Redundant `user_has_vout` assignment in both `case` and following `if` for `-V`/`--vout`.
- `CHOICE_HW=any` on AMD without VAAPI omits `--avcodec-hw` (OK) but differs from explicit `none` on software path — document.
- Software path still prefers `-V gl` when DISPLAY set but GLX probe failed; VLC cascade is fine.
- `display_num` assumes `:N` form; `localhost:10.0` works (`#*:`); bare `10` would not — rare.
- Version `0.1.0` — bump when policy table changes (cli-design).
- Comment “VDPAU/GL friendly” on I420 force is ambiguous given M1.

---

## Security (script injection / path handling)

| Check | Result |
|-------|--------|
| Shell injection via media path | **OK** — argv arrays, no word-split into shell |
| Dry-run re-exec safety | **OK** — `%q`; consumer must not `eval` blindly without trust |
| `export "$kv"` | **OK** for current EXTRA_ENV sources; do not later append unsanitized user env |
| `LD_LIBRARY_PATH` prepend PREFIX | **Expected** for private install; PREFIX must be trusted |
| `pgrep`/`glxinfo`/`vainfo` | **OK** — fixed commands, timeouts |
| Path traversal | N/A for auto (does not write media paths) |
| `.gitignore` un-ignore | **OK** — only `scripts/cvlc`, not root build `cvlc` |

No critical script-injection findings.

---

## Correctness: auto policy under PRIME + Xmux

| Environment | Auto choice | Pure `-V vdpau`? | Assessment |
|-------------|-------------|------------------|------------|
| Host `:0` NVIDIA + VDPAU | `-V gl --avcodec-hw vdpau` | No (user can force) | Correct preferred path |
| Xmux Xvfb + NVIDIA PRIME | `-V gl --avcodec-hw vdpau`, `virtual_x=1` | **No** | **Correct** — avoids black presentation |
| Xmux `--attach-to :0` | Still gl+vdpau if XMUX detected | No by default | Safe; pure vdpau via `xmux-vdpau-host.sh` or explicit `-V` |
| User `vlc-ng-auto -V vdpau …` | Respects user vout | Yes if user asked | Correct override semantics |
| Intel-only + iHD | gl + vaapi + iHD/DRM device | N/A | Correct for PRIME DRM footgun |

**Conclusion:** Auto policy **must not and does not** choose pure `-V vdpau` on virtual X. Requirement satisfied.

---

## Test gaps

1. **Unit/smoke for `vlc-ng-auto`** — table-driven `--dry-run --explain` with mocked env (`DISPLAY`, fake `glxinfo`, `XMUX_SESSION`, `HAS_*` via env injection or small probe stubs).  
2. **Regression:** virtual_x=1 never emits `-V vdpau` unless user-supplied.  
3. **444 GL vs VDPAU:** log assert `SW 4:4:4→I420 (box filter)` only when expected; chroma fourcc in vout line.  
4. **NDEBUG fuzz/smoke:** odd width/height 444; ensure no crash under release build.  
5. **cmdline:** `-V help`, `-V notamodule`, null-capability modules (if any) don’t SIGSEGV.  
6. **Wrapper:** `VLC_NG_AUTO=0` does not invoke auto; `VLC_NG_AUTO=1` does.  
7. **Patch apply CI** (if patches remain canonical): apply 01/08 to clean tree must match tip — currently would fail (M3).

Formal verification: **not run** (shell + media pipeline; not CBMC-shaped). Recommend property tests over formal.

---

## Consistency checklist (TODO / VISUAL_QA vs reality)

| Claim | Reality | Status |
|-------|---------|--------|
| Auto never pure vdpau on Xmux | Code chooses gl+vdpau | **Match** |
| Explicit `-V` wins | `user_has_vout` | **Match** |
| Pure vdpau Xvfb black | Documented, not “fixed in VLC” | **Match** |
| Host pure vdpau 444 box filter | Code + profile smoke | **Match** |
| GL 444 native I444 | Code forces I420 always | **Mismatch (M1)** |
| DR off when 444 | `b_direct_rendering=false` + GetFrame guard | **Match** |
| Patch 01 = full 444 fix | Patch slim | **Mismatch (M3)** |
| Patch 08 null guards | Tree yes, patch file no | **Mismatch (M3)** |

---

## Verdict

**Approve with changes.**

Ship blockers before calling the tip “docs-green”:

1. Resolve **M1** (I420-for-all vs native I444 docs) — code or docs, not both stories.  
2. Add **M2** soft bounds checks for NDEBUG box filter.  
3. At least a minimal **M4** dry-run policy test (virtual X ≠ pure vdpau).  
4. Refresh or demote stale **M3** patches / point APPLY docs at git tree.

Auto-backend and Xmux policy are in good shape; 444 pure-VDPAU engineering is sound if NDEBUG hardening lands.

---

## Top 5 issues (executive)

1. **M1** — Unconditional 4:4:4→I420 vs VISUAL_QA/TODO “native I444 on GL”  
2. **M2** — Box-filter bounds mostly assert-only; weak under `-DNDEBUG`  
3. **M3** — `patches/01` and `patches/08` do not match working tree  
4. **M4** — No automated tests for `vlc-ng-auto` decision table  
5. **M5** — VISUAL_QA 444 sections contradict each other without vout-conditional code  

---

## File references (anchors)

```1955:1967:/tmp/vlc-ng/modules/codec/avcodec/video.c
    if (swfmt != AV_PIX_FMT_NONE)
    {
        const AVPixFmtDescriptor *sw_desc = av_pix_fmt_desc_get(swfmt);
        if (sw_desc != NULL && sw_desc->nb_components >= 3 &&
            sw_desc->log2_chroma_w == 0 && sw_desc->log2_chroma_h == 0)
        {
            msg_Dbg(p_dec, "4:4:4 bitstream: SW decode + I420 convert (no HW 444; VDPAU/GL friendly)");
            can_hwaccel = false;
            p_sys->b_sw_444_to_420 = true;
            p_sys->b_direct_rendering = false; /* must copy via lavc_CopyPicture */
        }
```

```1885:1891:/tmp/vlc-ng/modules/codec/avcodec/video.c
        /* DR maps decoder pix_fmt onto picture_t; cannot DR when we force
         * I420 output from a 4:4:4 software pix_fmt (causes green/garbage). */
        if (!sys->b_direct_rendering || sys->b_sw_444_to_420)
        {
            post_mt(sys);
            return avcodec_default_get_buffer2(ctx, frame, flags);
        }
```

```373:441:/tmp/vlc-ng/modules/codec/avcodec/video.c
    assert(dec != NULL);
    ...
        assert((out_w + 1) / 2 <= pic->p[1].i_pitch);
        assert((out_h + 1) / 2 <= pic->p[1].i_lines);
        ...
        assert(frame->linesize[0] >= out_w || frame->linesize[0] >= src_w);
```

```373:385:/tmp/vlc-ng/scripts/vlc-ng-auto
choose_backend() {
  if [[ "$HAS_NVIDIA" -eq 1 && "$HAS_VDPAU" -eq 1 ]]; then
    if [[ "$IS_VIRTUAL_X" -eq 1 || "$IS_XMUX" -eq 1 ]]; then
      ENV_KIND="xmux/xvfb+nvidia"
      CHOICE_VOUT="gl"
      CHOICE_HW="vdpau"
    else
      ENV_KIND="host-nvidia"
      CHOICE_VOUT="gl"
      CHOICE_HW="vdpau"
    fi
```

```482:494:/tmp/vlc-ng/scripts/vlc-ng-auto
if [[ "$user_has_vout" -eq 1 ]]; then
  note "user supplied -V/--vout — not overriding vout"
else
  CMD+=("-V" "$CHOICE_VOUT")
fi
...
```

```61:62:/tmp/vlc-ng/src/config/cmdline.c
        if( cap != NULL )
            n = module_list_cap( &mods, cap );
```
