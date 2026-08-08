# Goal prompt: VLC-ng quality, remaining edges, profile-driven optimize

**Repo:** `/tmp/vlc-ng` · **branch:** `feature/hwaccel-patches` · **remote:** `EdgeOfAssembly/vlc-ng`  
**Base:** VLC 3.0.23 (autoconf). **Sample:** `/tmp/vlc-ng/samples/duke3d-444.mp4` (and `test-420.mp4`).  
**Install (debug/ASan):** prefer `/mnt/tmp/vlc-ng-asan` or rebuild under `/tmp/vlc-ng-install` with sanitizer flags.  
**Xmux:** `/tmp/Xmux` · spectator: `SPECTATOR: xmux attach <session> --no-reconnect`  
**Constraints:** No Pulse, no Wayland. Do **not** patch VLC for Xmux/Xvfb black pure-VDPAU (use host or `attach-to :0`).  
**Agents:** orchestrator + **at most 3** concurrent subagents (`explore` / `plan` / `general-purpose`). Prefer parallel independent work; serialize when one output feeds the next.

**Edge cases already documented:** `TODO.md`, `patches/_analysis/VISUAL_QA.md`.

---

## Ordered plan (execute in this order)

### Phase 0 — Playback test harness (always-on during later phases)

Use **liberally** whenever testing video (especially duke3d):

1. **Xmux MCP** (when session is Xmux): `xmux_screenshot`, `xmux_key`, `xmux_type`, `xmux_windows` as needed.  
2. **Host / attach-to :0:** `gnome-screenshot -d N -f /tmp/vlc-ng-shots/<name>.png` (and `-w` for window).  
3. **Always** tee full VLC terminal output to a log file (`scripts/play-once.sh --log …` or `cvlc -vvv … >log 2>&1`).  
4. Print `SPECTATOR: xmux attach <name> --no-reconnect` when starting Xmux sessions.  
5. Visual QA: multi-frame shots; reject pure-green (G−R ≫ 0) or black (mean≈0) for paths that should show picture.

**Pass criteria:** Every playback claim has log path + ≥1 screenshot path under `/tmp/vlc-ng-shots/` (or scratch dir if in a goal harness).

---

### Phase 1 — Checkpoint + full debug build (sanitizers + asserts)

1. **Git checkpoint** before any FEATURE/FIXUP:
   ```bash
   cd /tmp/vlc-ng
   git status -sb && git diff --stat
   git branch checkpoint/$(date -u +%Y%m%dT%H%M%SZ)-pre-quality
   # or: git tag checkpoint/...
   ```
2. **Full debug build with GCC sanitizers** (ASan + UBSan), frame pointers, no recover:
   ```bash
   export CC=gcc CXX=g++
   export CFLAGS="-O1 -g -fno-omit-frame-pointer -fsanitize=address,undefined -fno-sanitize-recover=all"
   export CXXFLAGS="$CFLAGS"
   export LDFLAGS="-fsanitize=address,undefined -fno-omit-frame-pointer"
   export LIBS="-fsanitize=address,undefined"
   # PREFIX e.g. /mnt/tmp/vlc-ng-asan (disk) if /tmp ramdisk is tight
   ./scripts/configure-vlc-ng.sh --prefix=/mnt/tmp/vlc-ng-asan --enable-debug --disable-sid
   # Ensure plugins link sanitizers (LIBS/LDFLAGS on make if needed)
   make -s V=0 -j$(nproc) LIBS="-fsanitize=address,undefined" LDFLAGS="-fsanitize=address,undefined -fno-omit-frame-pointer"
   make -s V=0 install
   # avcodec must link libswscale for 4:4:4 path
   ```
3. **Asserts (debug only):** In VLC-ng code you touch (especially `modules/codec/avcodec/video.c` 4:4:4 path, vdpau helpers), add `assert(...)` / `vlc_assert_unreachable` **before pointer dereference** and on geometry invariants.  
   - Debug/ASan builds: asserts **on**.  
   - Release/profile: compile with **`-DNDEBUG`** so asserts compile out.  
4. **gdb / valgrind if needed:** On crash or ASan report, capture backtrace (`gdb -batch -ex run -ex bt --args …`). Valgrind only if ASan is clean but logic bugs remain (slow; optional).  
5. **ASAN_OPTIONS** for runs: `abort_on_error=0:halt_on_error=0:detect_leaks=0` for long playback; tighten for crash repro.

**Pass criteria:** Full tree builds with sanitizers; `cvlc -V vdpau duke3d-444.mp4` runs without ASan fatal; asserts present on new/changed pointer paths.

**Agents (≤3):**  
- **[implement]** configure+build+install  
- **[explore]** map assert sites / DR / 444 convert  
- **[review]** scan new asserts + sanitizer link of plugins  

---

### Phase 2 — Fix compilation warnings

1. Rebuild with warnings visible (`make V=1` or drop `-s`); treat project warnings seriously.  
2. Fix **in-tree** warnings introduced or easy wins in modules you own (avcodec 444 path, cmdline/bank fixes, etc.).  
3. Do **not** mass-edit unrelated third-party noise unless it blocks `-Werror` or hides real bugs.  
4. Re-run build until clean for the targets you ship (or document remaining upstream noise with file:line).

**Pass criteria:** No new warnings in touched files; build still green under ASan flags.

**Agents (≤3):**  
- **[explore]** collect warning list from build log  
- **[implement]** fix warnings  
- **[review]** diff review  

---

### Phase 3 — Remaining issues (from `TODO.md`) if possible

Work only what is **fixable in VLC-ng or documented as external**:

| Priority | Item | Success |
|----------|------|---------|
| P0 | Non-ASan day-to-day install rebuild if still on ASan configure | `/tmp/vlc-ng-install` plays duke3d `-V vdpau` |
| P1 | Pure VDPAU under Xmux Xvfb black | **Not** VLC hack — confirm Xmux `attach-to :0` / GL+VDPAU still documented |
| P2 | NVIDIA VAAPI zero-copy | Document only unless easy win |
| P3 | Intel VAAPI DRM/iHD env helpers in scripts | Script + log proof |
| P4 | NVENC smoke (optional) | encode/decode or “not run” |
| P5 | Shutdown log spam without 10/12 patches | only if safe |

**Pass criteria:** `TODO.md` updated; each attempted item has evidence or explicit “won’t fix / external”.

**Agents (≤3):** parallel independent items only (e.g. script helper + docs + one code fix).

---

### Phase 4 — Full regression in **debug/ASan** build

Run **all** primary paths with logs + screenshots (Phase 0):

```bash
PREFIX=/mnt/tmp/vlc-ng-asan   # or current debug prefix
export LD_LIBRARY_PATH=$PREFIX/lib VLC_PLUGIN_PATH=$PREFIX/lib/vlc/plugins
export ASAN_OPTIONS=detect_leaks=0:abort_on_error=0:halt_on_error=0
SAMPLE444=/tmp/vlc-ng/samples/duke3d-444.mp4
SAMPLE420=/tmp/vlc-ng/samples/test-420.mp4

# Host pure VDPAU 444 (must show color, not green)
DISPLAY=:0 $PREFIX/bin/cvlc -vvv --no-media-library --no-audio -V vdpau $SAMPLE444

# Host pure VDPAU 420
DISPLAY=:0 $PREFIX/bin/cvlc -vvv --no-media-library --no-audio -V vdpau $SAMPLE420

# GL + VDPAU decode (Xmux OK)
# xmux start vlc-qa --geometry 1280x800 --gl nvidia
# xmux run … cvlc -V gl --avcodec-hw vdpau $SAMPLE420

# GL 444
# cvlc -V gl $SAMPLE444

# Intel VAAPI
# LIBVA_DRIVER_NAME=iHD cvlc -V gl --avcodec-hw vaapi $SAMPLE420
```

**Pass criteria:** Matrix in `patches/_analysis/VISUAL_QA.md` updated; no green cast; no ASan fatal on happy paths.

**Agents (≤3):**  
- **[test]** host VDPAU matrix  
- **[test]** Xmux GL+VDPAU matrix  
- **[test]** Intel VAAPI + screenshot QA  

---

### Phase 5 — Profile baseline → iterative optimize (no regressions)

VLC-ng is **autoconf**, not a stock `make profile` target. **Add** Makefile convenience targets if missing:

```make
# Example — add to a top-level helper or document exact flags
.PHONY: profile profile-build
profile-build:
	$(MAKE) clean || true
	./scripts/configure-vlc-ng.sh --prefix=$(PREFIX) --disable-debug \
	  CFLAGS="-O2 -g -fno-omit-frame-pointer -DNDEBUG" \
	  CXXFLAGS="-O2 -g -fno-omit-frame-pointer -DNDEBUG"
	$(MAKE) -s V=0 -j$$(nproc) && $(MAKE) -s V=0 install

# "make profile" = rebuild profile binary + run a fixed playback benchmark + write report
profile: profile-build
	./scripts/profile-playback.sh
```

1. **Implement** `scripts/profile-playback.sh` (or equivalent):
   - Fixed media (`duke3d-444` + short `test-420`), fixed duration (e.g. 30s), host `:0` or null vout if measuring decode-only.
   - Capture: wall time, CPU (`/usr/bin/time -v`), optional `perf stat` / `perf record` if available.
   - Write report under `patches/_analysis/profile/baseline-<utc>.txt` and later `opt-N-<utc>.txt`.
2. **Baseline:** `make profile` (or documented command) → save baseline.  
3. **Iterate:**
   - Identify bottleneck **in vlc-ng code** (not NVIDIA driver, not X server): e.g. 4:4:4 box filter, copy paths, unnecessary format churn.
   - One logical optimization per cycle; keep asserts in debug; release uses `-DNDEBUG`.
   - Re-run **playback smoke** (Phase 0 + critical paths). On break: **auto-fix or `git revert` / restore checkpoint** for that change.
   - `make profile` again; keep change only if equal-or-better metrics and tests green.
4. **Stop when:** no further meaningful bottleneck remains **inside vlc-ng** (profile flat / dominated by libavcodec or GPU), or gains &lt; noise.

**Pass criteria:** Baseline + ≥1 optimization report (or written “no in-tree bottleneck”); playback still green; commits FEATURE/FIXUP with evidence.

**Agents (≤3):**  
- **[implement]** profile target + script  
- **[implement]** one optimization at a time (serialized)  
- **[test/review]** smoke + compare profile numbers  

---

## Agent budget (max 3 concurrent)

| Phase | Suggested parallel set |
|-------|------------------------|
| 1 | build ‖ explore asserts ‖ review |
| 2 | explore warnings ‖ implement fixes ‖ review |
| 3 | up to 3 independent remaining items |
| 4 | up to 3 test lanes |
| 5 | profile script ‖ (serial) optimize ‖ test |

Never exceed **3** live subagents. Orchestrator integrates, runs final verification, commits, pushes.

---

## Definition of done

1. Checkpoint exists; debug+ASan build green.  
2. Asserts on critical pointer paths; release/profile uses `-DNDEBUG`.  
3. Warnings fixed or documented for touched code.  
4. Remaining TODO edges addressed or explicitly deferred in `TODO.md`.  
5. Full debug playback matrix green (logs + screenshots, duke3d included).  
6. Profile baseline + optimize loop complete with no playback regressions.  
7. FEATURE/FIXUP commits + `git push` (efficient-git); durable evidence paths recorded.

## Evidence rules

- No “green” without command + exit code.  
- Playback: log + screenshot.  
- Profile: before/after files under `patches/_analysis/profile/`.  
- Broken optimize → revert/fix automatically before next iteration.
