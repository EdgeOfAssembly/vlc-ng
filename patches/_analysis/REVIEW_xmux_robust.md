# Code review: Xmux multi-instance robustness

**Tree:** `/tmp/Xmux` (branch `main`, tip ~`3da9712` FEATURE v1 session prune/kill-all/SESSION_POLICY)  
**Scope:** session lifecycle (start/kill/prune/kill_all), signal cleanup, per-session paths, auto-naming, viewer title, CLI contracts, `SESSION_POLICY.md` vs code  
**Mode:** read-only review (no code changes)

---

## 1. Summary

The multi-instance work is a coherent, well-structured step away from the old global-daemon model: each session owns `sessions/<name>/{meta.ini,data.sock,worker.pid,xvfb.pid}`, prune/list/start share one stale-cleanup path, auto-naming from app basename (with wrapper skip and `-2`/`-3` uniqueness) matches agent policy, and the viewer title `xmux (<name>)` makes concurrent attaches distinguishable. Integration tests cover prune, `kill --all`, auto-name, and post-kill directory/socket removal. The main gaps are kill-path correctness when the CLI is not the worker’s parent (broken `waitpid` grace → immediate SIGKILL), incomplete rollback if meta update fails after fork, classic PID-reuse hazards on `kill(pid,0)` liveness, and a few doc/help mismatches (`XMUX_KILL_ALL`, man still mentions a companion daemon). None of these look like double-free or global-socket regressions; they are robustness holes relative to the FEATURE’s own graceful-shutdown claims.

---

## 2. Strengths

- **Per-session layout is first-class and consistent.**  
  `data_sock_path` / `session_worker_pid_path` / `session_xvfb_pid_path` all live under `sessions/<name>/` (`include/xmux/paths.hpp`, `src/common/paths.cpp`). No global data plane.

- **Prune semantics are careful about in-flight start.**  
  `session_prune_stale` skips `worker_pid <= 0` + live X + `state == starting|empty` so a concurrent `start` is not murdered mid-spawn (`session_ctl.cpp` ~551–555).

- **Worker signal handler is async-signal-safe enough.**  
  `on_worker_signal` only stores an `std::atomic<bool>` (`session_ctl.cpp` 38–41). Teardown runs in the normal control path after the poll loop, not in the handler.

- **Dual cleanup (worker self-teardown + parent `teardown_session`)** is redundant in a good way: sock unlink, optional X stop, `SessionStore::remove` appear on both paths (`worker_child_main` 146–156, `teardown_session` 75–112).

- **Auto-naming is thoughtful.**  
  Basename sanitize, wrapper skip (`xmux-wm`, `env`, `nice`, …), and `unique_session_name` with length-safe `-N` suffix (`util.cpp` 70–254). CLI accepts `.`, `--auto`, and flags-first with a later command (`main_cli.cpp` 1526–1643).

- **Viewer title contract implemented end-to-end.**  
  `run_viewer` / `run_viewer_path` take `session_name`; connect and start-attach pass it; title is `xmux (<name>)` (`viewer.cpp` 488–493, `viewer.hpp` 17–18).

- **CLI contracts match cli-design for the new surface.**  
  `prune` / `kill --all` / `start .|--auto` documented in top-level usage and subcommand help; tests assert help strings and behavior (`test_cli_contract.py`, `test_session_control.py`).

- **`SESSION_POLICY.md` is mostly accurate** and correctly steers agents away from default `kill --all` on shared `~/.xmux`.

- **Tests for the FEATURE exist and are meaningful:** stale dir + legacy globals, kill-all, auto-name `true` / `true-2` + `app_name=`, kill removes session dir and sock.

---

## 3. Issues

### Critical

_None that are guaranteed production-breakers on a quiet single-user box._  
(PID-reuse wrong-kill is the closest; filed under Major because probability is low for short-lived agent sessions.)

### Major

1. **`teardown_session` grace wait is broken for non-parent killers**  
   **File:** `src/xmux/session_ctl.cpp` ~81–97  

   ```cpp
   for (int i = 0; i < 20; ++i) {
       if (::waitpid(info.worker_pid, &st, WNOHANG) != 0)
           break;
       ::usleep(50000);
   }
   if (pid_alive(info.worker_pid)) {
       ::kill(info.worker_pid, SIGKILL);
       ...
   }
   ```

   After `xmux start --no-attach`, the worker is reparented to init (or was never a child of the later `xmux kill` process). `waitpid` returns `-1`/`ECHILD` immediately (`!= 0` → break). Result: **SIGTERM then immediate SIGKILL** with no 1s grace.  

   Contrast `stop_xvfb` (`xvfb.cpp` 199–208), which correctly loops on `process_alive` when `waitpid` cannot reap.  

   **Impact:** Worker’s signal path (unlink sock, stop X, remove dir) rarely runs on external `kill`; parent-side teardown usually still cleans up, but this contradicts `SESSION_POLICY.md` “Graceful shutdown” and races with any in-flight worker I/O. Fix: poll `pid_alive` / `kill(pid,0)` with sleep, same pattern as `stop_xvfb`; only use `waitpid` when `getpgid`/parent relationship allows.

2. **Incomplete rollback if `store.update` fails after worker fork**  
   **File:** `session_ctl.cpp` ~378–385  

   On update failure: SIGTERM worker + stop X, but **no** `store.remove`, no wait for worker exit, no sock unlink. Worker may self-clean on SIGTERM, or leave a half-written session dir and a briefly live worker. Start returns failure while disk state is ambiguous.

3. **PID-only liveness → false “live” sessions and wrong-process kill**  
   **Files:** `session_ctl.cpp` `pid_alive` (43–50); `util.cpp` `process_alive` (357–368)  

   Both are `kill(pid, 0)` (+ EPERM ⇒ alive). No start-time, `/proc/<pid>/comm`, or exe check.  

   - **Stale false positive:** reused PID → prune skips; `list` shows `running`; `start` same name fails with “already exists”.  
   - **Wrong kill:** `session_kill` / prune orphan branch can SIGTERM/SIGKILL an unrelated process that inherited the numeric PID.  

   Acceptable risk for a single-user workstation tool if documented; not acceptable if marketed as robust multi-instance lifecycle. Prefer at least `/proc/pid/stat` starttime recorded in meta, or cmdline match (`xmux` worker / `xmux-server`).

4. **Auto-name TOCTOU without retry**  
   **Files:** `util.cpp` `unique_session_name`; `main_cli.cpp` 1639–1667; `session_ctl.cpp` 249–263  

   Name is chosen, then `session_start` runs. Two concurrent `xmux start . -- app` can pick the same free slot; the loser gets hard failure (“session already exists”) instead of advancing to `base-2`. For agent suites that parallelize starts, this is a flake source. Retry loop (or create-with-O_EXCL-style exclusive dir create) would close it.

5. **`XMUX_KILL_ALL` documented as a gate but not enforced**  
   **Files:** `main_cli.cpp` 94; `docs/private-stack/SESSION_POLICY.md` 60; `cmd_kill` 1821–1837  

   Help: `XMUX_KILL_ALL=1 Allow 'xmux kill --all'`. Policy: env “documents suite-wide wipe intent (still pass `--all`)”. Code: `--all` always works with no env check.  

   Either implement a real guard (require env **or** `--all` only when env set — product choice) or drop “Allow” from help so agents are not misled into thinking the env is required/protective.

### Minor

6. **Dead code in prune after `wk_ok` check**  
   **File:** `session_ctl.cpp` 556–568  

   After `if (wk_ok) continue;`, the block `if (worker_pid > 0 && pid_alive(worker_pid)) kill SIGKILL` is unreachable. Harmless confusion.

7. **`session_list` / `cmd_list` double-prune**  
   **Files:** `main_cli.cpp` 1753–1756; `session_ctl.cpp` 462–472  

   `cmd_list` prunes, then `session_list` prunes again. Wasteful only; not wrong.

8. **`kill --all` counts pruned dead names as “killed”**  
   **File:** `session_ctl.cpp` 449–458; `main_cli.cpp` 1835  

   Pruned leftovers are appended to `killed` and printed as `killed N session(s)`. Slightly inflates success metrics / confuses scripts parsing stdout.

9. **`degraded` state is effectively invisible**  
   **File:** `session_ctl.cpp` 222–225  

   `session_load` can set `degraded` (X up, worker dead), but `list` always prunes first, so degraded sessions are removed rather than shown. Either document “degraded is internal only” or stop pruning degraded until an explicit prune (product choice). Policy currently says prune removes worker-dead sessions — consistent with code, but the state name is misleading.

10. **Man page still describes a companion daemon**  
    **File:** `man/xmux.1` lines 18–19  

    “A companion daemon is started automatically when needed.” Conflicts with daemon-free architecture and `SESSION_POLICY.md`. Also missing `.` / `--auto` naming in the `start` section (CLI help is better).

11. **`detach` CLI is a no-op message**  
    **File:** `main_cli.cpp` 1891–1901  

    Prints “close the viewer window”; does not signal worker `detach_client()`. Man still implies detach drops viewers. Pre-existing, but adjacent to multi-view robustness.

12. **Worker stop latency up to ~1s**  
    **File:** `session_ctl.cpp` 133–144  

    Loop uses `sleep(1)` between flag checks. Fine for interactive use; combined with Major #1, external kill almost never observes a graceful worker exit.

13. **App process not tracked on session kill**  
    Apps forked by `start -- cmd` are not in meta; killing the session kills X/worker only. Apps may linger as zombies/orphans until they notice display death. Worth a policy note, not necessarily a code bug.

### Nit

14. Redundant condition in `session_start` existence check (`session_ctl.cpp` 255–256):  
    `(existing.attach_existing && pid_alive(existing.worker_pid))` is subsumed by `pid_alive(existing.worker_pid)`.

15. `parse_display_num` invoked twice with odd `:0` special-case (`session_ctl.cpp` 282–292) — pre-existing clarity issue.

16. Duplicate `#include <cstring>` in `main_cli.cpp` (24, 28).

17. `SessionStore::remove` Doxygen says “or already absent” (`session_store.hpp` 78); behavior depends on `remove_all` error_code — usually OK, wording slightly strong.

18. Policy path is `docs/private-stack/SESSION_POLICY.md` (not top-level `docs/SESSION_POLICY.md`); CLI help points at the correct relative path.

---

## 4. `SESSION_POLICY.md` accuracy vs code

| Policy claim | Code | Match? |
|--------------|------|--------|
| Per-session `data.sock`, `worker.pid`, `xvfb.pid` under `sessions/<name>/` | `paths.cpp` | Yes |
| Viewer title `xmux (<name>)` | `viewer.cpp` 488–493 | Yes |
| Auto-name from basename; wrappers skipped | `session_name_from_argv` | Yes |
| `list` / `start` call prune | `session_list`, `session_start`, `cmd_list` | Yes (list double-calls) |
| `prune` does not kill live workers | `wk_ok → continue` | Yes |
| Prune cleans legacy `xmuxd.*` / `xmux.sock` / `xmux.pid` | `cleanup_legacy_daemon_files` | Yes |
| `kill --all` then prune | `session_kill_all` | Yes |
| Prefer named kill; avoid default `--all` | CLI help + policy | Yes (social rule) |
| Graceful: SIGTERM → worker unlinks sock, stops X, removes dir | Worker path yes; **external kill often SIGKILLs first** (Major #1) | **Partial** |
| No stale sock/pids after kill | Parent teardown + tests | Yes in practice if teardown runs |
| `XMUX_KILL_ALL=1` documents wipe intent | Env unused in code | **Doc-only; help overclaims** |

Overall: policy is a good agent-facing contract; fix Major #1 and the `XMUX_KILL_ALL` wording so docs and kill path agree.

---

## 5. Test gaps

| Gap | Why it matters |
|-----|----------------|
| No test that **external** `kill` leaves ≥50–200ms for worker SIGTERM (or asserts parent cleanup alone is complete under SIGKILL) | Major #1 |
| No **PID reuse** / fake live pid that is not the worker | Stale false positive / wrong kill |
| No **concurrent** double `start . -- app` | Auto-name TOCTOU flakes |
| No unit tests for `session_name_from_argv` wrappers (`env VAR=x app`, `nice -n 5 app`, `xmux-wm app`) | Naming regressions |
| No test for `start` failure after fork (forced update failure) | Rollback hole |
| No test that **live** session survives prune while a second dead dir is removed | Partially covered by `test_prune_removes_stale_session_dir` — good; keep it |
| No test for in-flight `state=starting` not pruned | Race guard untested |
| No assertion on viewer **window title** (needs X11/Xmux) | Title feature unproven in CI |
| Man/help drift not checked for “daemon” wording | Doc quality |
| Formal harness only covers `is_valid_session_name` charset | Fine; naming uniqueness is outside CBMC scope |

Existing coverage that is solid: prune + legacy files, kill-all, auto-name `true`/`true-2`, kill removes dir/sock, EEXIST on second start, CLI help for prune/kill --all.

---

## 6. Race / safety checklist (requested focus)

| Concern | Assessment |
|---------|------------|
| Double-free / double-close of worker objects | **OK** — `SessionWorker::stop` joins once; process `_exit`s |
| Stale `data.sock` after kill | **OK** if `teardown_session` or worker stop runs; tested |
| Stale session false positive | **Risk** — PID reuse (Major #3) |
| Prune vs in-flight start | **OK** — starting guard |
| Concurrent auto-name | **Risk** — TOCTOU (Major #4) |
| Signal safety of handler | **OK** — atomic only |
| Kill from non-parent | **Bug** — no grace (Major #1); cleanup still mostly parent-driven |
| Global sock collision | **OK** — per-session paths only |

---

## 7. Verdict

### **request-changes**

Ship-quality multi-instance design and good tests for the happy path, but the kill grace-wait bug, incomplete post-fork rollback, and PID-only identity checks undercut the FEATURE’s robustness narrative. Address Major #1 and #2 before calling lifecycle “done”; #3/#4 can be follow-ups if explicitly accepted for single-user scope, but should not be silent.

**Suggested fix order**

1. Fix `teardown_session` wait to poll `pid_alive` (mirror `stop_xvfb`).  
2. On `store.update` failure after fork: wait/kill worker, `store.remove`, unlink sock.  
3. Align `XMUX_KILL_ALL` help with policy (or enforce).  
4. Optional: PID identity + auto-name retry under contention.  
5. Man page: drop “companion daemon”; document `.` / `--auto` / `prune` / `kill --all`.

---

## 8. Top 5 issues (executive)

1. **Major — `waitpid` grace broken for external `kill` → immediate SIGKILL** (`session_ctl.cpp` 81–97).  
2. **Major — Incomplete cleanup if meta `update` fails after worker fork** (`session_ctl.cpp` 378–385).  
3. **Major — PID-only liveness: stale “live” sessions + possible wrong-process signal** (`pid_alive` / `process_alive`).  
4. **Major — Auto-name TOCTOU without retry under concurrent start** (`unique_session_name` + `session_start`).  
5. **Minor/Major doc — `XMUX_KILL_ALL` / man daemon wording vs daemon-free code** (`main_cli.cpp` help, `man/xmux.1`, `SESSION_POLICY.md`).
