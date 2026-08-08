# Review: VLC-ng ↔ Xmux integration

**Date:** 2026-08-08  
**Scope:** Integration concerns only (read-only). No code changes.  
**Trees:** `/tmp/vlc-ng` (`feature/hwaccel-patches`), `/tmp/Xmux` (`main`)  
**Installs:** `/tmp/vlc-ng-install`, `/mnt/tmp/vlc-ng-asan`, `/usr/local/bin/xmux`

---

## Verdict

**CONDITIONAL PASS — policy intent is sound; lifecycle hygiene and cross-docs lag.**

| Area | Score | Notes |
|------|-------|-------|
| Backend policy (GL+VDPAU vs pure VDPAU) | **Strong** | Documented and implemented consistently in both repos |
| `vlc-ng-auto` Xmux/Xvfb detection | **Good** | Correct default choices; some signals are dead/weak |
| SESSION_POLICY vs VLC test scripts | **Partial** | No `kill --all`; SPECTATOR OK; prune/auto-name/reuse not aligned |
| Cross-project docs | **Weak** | One-way refs; no single integration page until this file |
| Durability (`/tmp` vs `/mnt`) | **Risky** | Day-to-day prefix + samples + shots on tmpfs |
| Agent footguns | **Medium** | Global `cvlc` kill; shared `~/.xmux`; fixed session names |

**Ship as-is for day-to-day GL+VDPAU under Xmux.** Tighten scripts/docs before treating the harness as agent-safe default.

---

## 1. Does SESSION_POLICY match how VLC-ng tests start Xmux?

**Canonical policy:** `/tmp/Xmux/docs/private-stack/SESSION_POLICY.md`

| Policy rule | VLC-ng practice | Match? |
|-------------|-----------------|--------|
| Fresh session per test, name from app (`start .` / `--auto`) | Fixed names: `vlc-ng`, `vlc-vdpau-host` | **No** — intentional suite reuse, not auto-name |
| `xmux prune` before start | Neither `xmux-play-test.sh` nor `xmux-vdpau-host.sh` prunes | **No** |
| No `kill --all` by default | Scripts use named `xmux kill "$SESSION"` only | **Yes** |
| Reuse only when explicit | `xmux-play-test.sh` reuses `vlc-ng` if `list` says running | **Partial** — reuse is implicit suite default, not documented as policy exception |
| Print `SPECTATOR: xmux attach <name> --no-reconnect` | Both scripts + `GOAL_PROMPT.md` | **Yes** |
| Prefer isolated `$XMUX_HOME` for tests | VLC scripts use default `~/.xmux` | **No** |

### What VLC scripts actually do

**`scripts/xmux-play-test.sh`** (GL path under Xvfb):

```bash
# ensure_session: reuse if running, else kill+start fixed name
xmux start "$SESSION" --geometry 1280x800 --gl nvidia
xmux run --gl nvidia "$SESSION" -- xmux-wm …
xmux run --gl nvidia "$SESSION" -- play-once.sh …
# SPECTATOR attach on host :0
```

**`scripts/xmux-vdpau-host.sh`** (pure VDPAU on real NVIDIA X):

```bash
xmux kill "$SESSION"
xmux start "$SESSION" --attach-to :0 --gl nvidia
xmux run --gl nvidia "$SESSION" -- cvlc -V vdpau …
```

### Alignment summary

- **Correct:** `--gl nvidia` for PRIME into Xvfb; `--attach-to :0` for pure `-V vdpau`; named kill only; SPECTATOR line; no VLC patches for Xvfb black pure-VDPAU (matches Xmux `docs/improved-xvfb.md`).
- **Drift:** SESSION_POLICY’s “prune + auto-name + fresh session” is the agent default; VLC harness is a **long-lived fixed session** pattern. That is fine for a human-watched matrix **if documented**, but agents reading only SESSION_POLICY will start differently than agents reading only VLC scripts.
- **GOAL_PROMPT** points at `/tmp/Xmux` and SPECTATOR, but **never cites** `SESSION_POLICY.md`.

---

## 2. Does `vlc-ng-auto` correctly detect Xmux/Xvfb vs host?

**File:** `/tmp/vlc-ng/scripts/vlc-ng-auto` (v0.1.0)

### Decision table (implemented)

| Detected env | `-V` | `--avcodec-hw` |
|--------------|-----|----------------|
| Host NVIDIA + VDPAU | `gl` | `vdpau` |
| Xmux/Xvfb + NVIDIA + VDPAU | `gl` | `vdpau` (never pure `vdpau`) |
| Intel + iHD | `gl` | `vaapi` + `LIBVA_DRIVER_NAME=iHD` |
| AMD | `gl` | `vaapi` / any |
| Software | `gl` or `xcb` | `none` |

User-supplied `-V` / `--vout` / `--avcodec-hw` always wins. **Correct for this workstation.**

### Detection signals

| Signal | How | Reliability |
|--------|-----|-------------|
| `XMUX_SESSION` / `XMUX_DISPLAY` env | Forces `IS_XMUX` + `IS_VIRTUAL_X` | **Weak / dead by default** — `xmux run` sets `DISPLAY` + PRIME/`XMUX_GL`, **not** `XMUX_SESSION`/`XMUX_DISPLAY` |
| `pgrep xmux-server` matching `DISPLAY` num | `execlp(…, ":N", …)` → cmdline has `:N` | **Primary reliable signal** for default Xvfb sessions |
| `pgrep Xvfb` matching display | Fallback if stock Xvfb used | OK |
| Host `Xorg`/`X` on display | Notes host; does not clear virtual if xmux already set | OK for attach-to edge cases |
| `llvmpipe`/`softpipe` + NVIDIA/PRIME | Marks virtual | **Misses `--gl nvidia`** — under PRIME, `glxinfo` reports **GTX 1050**, not llvmpipe (`GLX_ACCEL.md`) |
| `__NV_PRIME_RENDER_OFFLOAD` alone | Logged only; does **not** force virtual | **Correct** (host `:0` often has PRIME) |

### Critical path: Xmux + `--gl nvidia` (day-to-day)

1. `xmux start … --gl nvidia` → `xmux-server :N` (Mesa AIGLX server).
2. `xmux run --gl nvidia` → client gets NVIDIA PRIME env; renderer = GTX 1050.
3. `vlc-ng-auto` must see **xmux-server on `:N`**, not llvmpipe.
4. Choice: `-V gl --avcodec-hw vdpau` — **correct** (pure `-V vdpau` would black-screen).

### Critical path: attach-to `:0` (pure VDPAU QA)

1. No `xmux-server` on `:0`; host Xorg owns display.
2. GLX = NVIDIA → `IS_VIRTUAL_X=0` → `env=host-nvidia`.
3. Auto still defaults to **gl+vdpau** (never auto-selects pure `-V vdpau`). Pure picture path remains **explicit** (`-V vdpau` or `xmux-vdpau-host.sh`). **Acceptable.**

### Gaps in detection

1. **`XMUX_SESSION` is not exported by Xmux** — only useful if agents set it; if set on host `:0` runs, labels env as virtual (today both NVIDIA paths still pick gl+vdpau, so behavior is OK; labels/`--explain` can lie).
2. **No check for `XMUX_GL`** (set by `xmux run`) — optional explain aid only; not a virtual-X oracle (attach-to also sets it).
3. **Harness bypass:** `play-once.sh` / `xmux-play-test.sh` call raw `PREFIX/bin/cvlc` with caller args — they do **not** go through `vlc-ng-auto`. Matrix tests are explicit (good); day-to-day agents should use `scripts/vlc-ng-auto` or `scripts/cvlc`.
4. **pgrep race:** if `DISPLAY` unset inside a broken wrapper, probes skip virtual detection and may still pick gl+vdpau from DRI/NVIDIA — usually fine, but `--explain` should be used when debugging.

**Bottom line:** For the supported matrix, auto picks the right flags. Detection is “good enough,” not airtight; do not rely on `XMUX_SESSION` alone.

---

## 3. Gaps and agent footguns

### Missing docs (cross-link)

| From | To | Status |
|------|----|--------|
| VLC `TODO.md` / `VISUAL_QA.md` | Xmux `improved-xvfb.md`, pure-VDPAU black | Present |
| VLC `GOAL_PROMPT.md` | Xmux path + SPECTATOR | Present |
| VLC anything | **`SESSION_POLICY.md`** | **Missing** |
| Xmux `improved-xvfb.md` | Example `cvlc` under `/tmp/vlc-ng-install` | Present (one-way) |
| Either tree | Single “integration contract” | **Was missing** (this review) |

### Footguns

1. **`kill_players` in `xmux-play-test.sh`**  
   `pgrep -x cvlc|vlc` then `kill` — **system-wide**, not session-scoped. Can murder a human’s host VLC or another agent’s player. Worse day-to-day impact than `xmux kill --all` for video work.

2. **`xmux kill --all`**  
   Policy forbids as default on shared `~/.xmux`. VLC scripts avoid it; agents trained on “clean slate” may still wipe spectator demos. Prefer `xmux kill <name>` + `xmux prune`.

3. **Fixed session names on shared home**  
   `vlc-ng` / `vlc-vdpau-host` collide across agents. `start` fails with “session already exists” → agents escalate to `--all`. Mitigations: unique names (`vlc-ng-$$`), `XMUX_HOME` isolation, or explicit reuse docs.

4. **Stale sessions after crash/reboot**  
   `~/.xmux/sessions/*` survives reboot; pids do not. `list`/`start` prune stale dirs, but half-live workers need named `kill`. Always `xmux prune` (or `list`) before assuming a clean slate.

5. **Stale viewer PID file**  
   `xmux-play-test.sh` stores viewer pid under `patches/_analysis/xmux-logs/viewer.pid`. After reboot, file may point at wrong pid or block re-attach logic incorrectly (it checks `kill -0`).

6. **Black-screen false failures**  
   - Pure `-V vdpau` on default Xvfb → black (expected; not a VLC bug).  
   - GL content sometimes missing from Xvfb root pixmap (`GLX_ACCEL.md`) → screenshot mean≈0 while log shows NVIDIA. **Log + multi-frame shots**; don’t fail on a single black root shot for glxgears-class content. VLC windowed playback is usually OK under GL+VDPAU.

7. **Wrong DRM for Intel VAAPI**  
   Default DRM may be NVIDIA; need `LIBVA_DRIVER_NAME=iHD` + Intel render node (`vaapi-intel-play.sh`). Auto sets this for Intel-only; hybrid NVIDIA+Intel with working VDPAU prefers NVIDIA path (correct for this host).

8. **ASan vs day-to-day prefix mixups**  
   Agents may run ASan `cvlc` without `ASAN_OPTIONS=detect_leaks=0` or point `VLC_NG_PREFIX` at the wrong tree.

---

## 4. Durability: `/tmp` vs `/mnt` (what survives reboot)

`/tmp` is **tmpfs** on this workstation — wiped on power-off/reboot.

| Asset | Path | Survives reboot? | Notes |
|-------|------|------------------|-------|
| VLC-ng git worktree | `/tmp/vlc-ng` | **No** | Must `git push` → `EdgeOfAssembly/vlc-ng` |
| Day-to-day install | `/tmp/vlc-ng-install` | **No** | Rebuild after reboot |
| ASan/debug install | `/mnt/tmp/vlc-ng-asan` | **Yes** | Prefer for durable debug binary |
| Samples / shots / harness logs | `/tmp/vlc-ng/samples`, `/tmp/vlc-ng-shots`, `…/xmux-logs` | **No** | Re-copy samples; re-run for evidence |
| Xmux git worktree | `/tmp/Xmux` | **No** | `git push` + `scripts/sync-durable.sh` → `/mnt/Xmux` |
| Xmux CLI + server | `/usr/local/bin/xmux`, `xmux-server`, `xmux-wm` | **Yes** | System install |
| Private Mesa/ALSA | `/usr/local/xmux-mesa`, `/usr/local/xmux-alsa` | **Yes** | |
| Session state | `~/.xmux/sessions/` | **Dir yes / live no** | Stale after reboot → prune |
| Analysis markdown in git | `patches/_analysis/*.md` | Via **git remote** only | Commit + push |

**Done for integration work** = verified playback + commits pushed + (for Xmux) durable sync when policy requires. Do not leave the only copy of patches or review notes only under `/tmp`.

---

## 5. Minimal smoke checklist (agents)

Copy-paste gate before claiming “VLC-ng + Xmux green.”

### Preflight

```bash
# Binaries
test -x /usr/local/bin/xmux && xmux -v
test -x /tmp/vlc-ng-install/bin/cvlc || test -x /mnt/tmp/vlc-ng-asan/bin/cvlc
# Prefer day-to-day unless testing ASan:
export PREFIX="${PREFIX:-/tmp/vlc-ng-install}"
export LD_LIBRARY_PATH="$PREFIX/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
export VLC_PLUGIN_PATH="$PREFIX/lib/vlc/plugins"

# Samples present
test -f /tmp/vlc-ng/samples/test-420.mp4
test -f /tmp/vlc-ng/samples/duke3d-444.mp4

# Session hygiene (SESSION_POLICY)
xmux prune
xmux list   # note any human sessions — do NOT kill --all
```

### A. Auto backend (host or any DISPLAY)

```bash
/tmp/vlc-ng/scripts/vlc-ng-auto --dry-run --explain \
  /tmp/vlc-ng/samples/test-420.mp4
# Expect stderr: env=host-nvidia or xmux/xvfb+nvidia; vout=gl hw=vdpau
```

### B. Xmux GL + VDPAU decode (primary day-to-day)

```bash
SESS="vlc-smoke-$$"
xmux prune
xmux start "$SESS" --geometry 1280x800 --gl nvidia --no-attach
echo "SPECTATOR: xmux attach $SESS --no-reconnect"
xmux run --gl nvidia "$SESS" -- xmux-wm &
sleep 0.5
xmux run --gl nvidia "$SESS" -- \
  env LD_LIBRARY_PATH="$PREFIX/lib" VLC_PLUGIN_PATH="$PREFIX/lib/vlc/plugins" \
  "$PREFIX/bin/cvlc" --no-media-library --no-audio --play-and-exit --verbose 2 \
  -V gl --avcodec-hw vdpau /tmp/vlc-ng/samples/test-420.mp4 \
  > /tmp/vlc-ng-shots/${SESS}.log 2>&1 &
sleep 3
xmux screenshot "$SESS" -o /tmp/vlc-ng-shots/${SESS}.png
# Pass: log has Using NVIDIA / vdpau decode; shot mean not ~0 (or multi-frame OK)
xmux kill "$SESS"
```

### C. Pure VDPAU picture (host NVIDIA X only)

```bash
# Do NOT use default Xvfb for this check
/tmp/vlc-ng/scripts/xmux-vdpau-host.sh /tmp/vlc-ng/samples/test-420.mp4
# Or: DISPLAY=:0 $PREFIX/bin/cvlc -V vdpau … + gnome-screenshot
# Pass: visible picture; log `using the "vdpau" module`
# Fail if only tested under Xvfb (black is expected there)
```

### D. 4:4:4 smoke (host or GL)

```bash
DISPLAY=:0 $PREFIX/bin/cvlc --no-audio --play-and-exit -V vdpau \
  /tmp/vlc-ng/samples/duke3d-444.mp4
# Pass: log `SW 4:4:4→I420 (box filter)`; no pure-green cast in screenshot
```

### E. Cleanup (policy-safe)

```bash
xmux kill "$SESS" 2>/dev/null || true   # only sessions you started
xmux prune
# NEVER default: xmux kill --all
# NEVER: pkill -x cvlc  on a shared desktop without checking
```

### Pass/fail rules

| Claim | Evidence required |
|-------|-------------------|
| GL+VDPAU under Xmux works | log path + ≥1 shot; not pure-VDPAU-on-Xvfb |
| Pure VDPAU works | host `:0` or `attach-to :0` only |
| Auto chose correctly | `--dry-run --explain` line |
| No session wipe | no `kill --all` in the session transcript |

---

## Top recommendations (priority order)

1. **Document the integration contract in VLC-ng**  
   Point `GOAL_PROMPT.md` / `TODO.md` at:
   - `/tmp/Xmux/docs/private-stack/SESSION_POLICY.md`
   - this file (`patches/_analysis/REVIEW_integration_vlc_xmux.md`)
   - rule: default Xmux tests = `--gl nvidia` + `-V gl --avcodec-hw vdpau`; pure `-V vdpau` only via attach-to `:0` / host.

2. **Align harness with SESSION_POLICY (minimal code later)**  
   - `xmux prune` before start.  
   - Unique session names (`vlc-ng-$$` or `start --auto`).  
   - Optional `XMUX_HOME=$(mktemp -d /tmp/xmux-vlc-XXXX)` for agent suites.  
   - Replace `kill_players` with session-scoped kill (track pid from `xmux run` / `play-once`).

3. **Harden `vlc-ng-auto` virtual detection (optional)**  
   - Keep `xmux-server` pgrep as primary.  
   - Treat bare `XMUX_SESSION` as soft signal only if `DISPLAY` is non-`:0` or xmux-server matches.  
   - Document that `xmux run` does not set `XMUX_SESSION`.

4. **Durability defaults for agents**  
   - Prefer `VLC_NG_PREFIX=/mnt/tmp/vlc-ng-asan` only for ASan; rebuild `/tmp/vlc-ng-install` after reboot for day-to-day.  
   - Commit + push analysis/reviews; do not rely on `/tmp/vlc-ng-shots` overnight.

5. **Agent runbook one-liner**  
   ```text
   prune → start unique --gl nvidia → SPECTATOR → run cvlc -V gl --avcodec-hw vdpau → screenshot+log → kill name (not --all)
   ```

---

## File index (absolute paths)

| Role | Path |
|------|------|
| Session policy | `/tmp/Xmux/docs/private-stack/SESSION_POLICY.md` |
| VDPAU/Xvfb limits | `/tmp/Xmux/docs/improved-xvfb.md` |
| Auto backend | `/tmp/vlc-ng/scripts/vlc-ng-auto` |
| Xmux GL play harness | `/tmp/vlc-ng/scripts/xmux-play-test.sh` |
| Pure VDPAU host helper | `/tmp/vlc-ng/scripts/xmux-vdpau-host.sh` |
| Play + log tee | `/tmp/vlc-ng/scripts/play-once.sh` |
| Visual matrix | `/tmp/vlc-ng/patches/_analysis/VISUAL_QA.md` |
| Goal / agent prompt | `/tmp/vlc-ng/GOAL_PROMPT.md` |
| This review | `/tmp/vlc-ng/patches/_analysis/REVIEW_integration_vlc_xmux.md` |

---

## Reviewer sign-off

- **Integration verdict:** CONDITIONAL PASS  
- **Blockers for “agent-default safe”:** global `kill_players`, missing SESSION_POLICY link, fixed shared session names, tmpfs-only day-to-day install  
- **Not blockers for human/day-to-day GL+VDPAU:** backend policy and Xmux start flags are already correct
