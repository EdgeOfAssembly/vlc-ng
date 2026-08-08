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
