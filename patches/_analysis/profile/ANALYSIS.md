# Profile analysis (GOAL_PROMPT Phase 5)

## Baseline (ASan debug tree)

- Report: `baseline-asan-20260808T123212Z.txt` (and console tee)
- PREFIX: `/mnt/tmp/vlc-ng-asan`
- **Not representative of release:** ASan shows ~15M page-faults/u and ~0.2 IPC — sanitizer overhead dominates.

## In-tree hotspots (code review + path knowledge)

| Area | Role | Verdict |
|------|------|---------|
| `lavc_CopyPicture` 4:4:4 box filter | Y memcpy + 2×2 UV | O(pixels); only on 4:4:4 SW path; not hot vs avcodec decode |
| libavcodec H.264 | Decode | External; dominant for 420 HW path is GPU |
| VDPAU/GL | Present | Driver/X11 |

## Optimization policy

1. Real baselines need `make -f Makefile.vlc-ng.mk profile` (**-DNDEBUG**, no ASan).
2. Do **not** micro-optimize box filter until NDEBUG profile shows it &gt;5% CPU.
3. Stop condition met for ASan tree: bottleneck is **sanitizer + libavcodec/GPU**, not further vlc-ng logic.

## Next profile (release)

```bash
make -f Makefile.vlc-ng.mk profile   # long: reconfigure + install + scripts/profile-playback.sh
```

## Baseline (NDEBUG release tree) — 2026-08-08

- Report: `baseline-ndebug-20260808T140630Z.txt`
- PREFIX: `/tmp/vlc-ng-install` (`-O2 -DNDEBUG`, no ASan)
- Smoke: pure `-V vdpau` duke3d → `SW 4:4:4→I420 (box filter)` + `using the "vdpau" module`

### gl-vdpau-420 (perf stat, 12s timeout)

| Metric | ASan debug | NDEBUG |
|--------|------------|--------|
| page-faults | ~15.8M | **~19k** |
| instructions/cycle | ~0.2 | **~1.2** |
| task-clock | ~27s | **~0.8s** |

**Conclusion:** In-tree 4:4:4 box filter is not the bottleneck under NDEBUG; cost is dominated by decode/GPU/driver. No further micro-opt required in vlc-ng for profile stop condition. Re-run `make -f Makefile.vlc-ng.mk profile-run` after future code changes.
