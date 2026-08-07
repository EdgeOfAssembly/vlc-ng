# Patch deduplication report

## Sources
| Tag | Path | Count |
|-----|------|-------|
| A | `$HOME/vlc-patches` | 16 |
| B | `$HOME/vlc-ng/vlc-patches` | 22 |

## MD5 cross-dir result
All 16 shared basenames are **byte-identical** (same MD5) between A and B.

B-only (6):
- `08-vout-backend-print-and-help-list.patch`
- `09-update-vlc-man-page.patch`
- `10-suppress-decoder-shutdown-logs.patch`
- `11-fix-wavskipheader-i_len.patch`
- `12-avcodec-shutdown-log-suppress.patch`
- `12-filter-avcodec-shutdown-logs.patch`

## Near-duplicates (case-by-case)
| Patches | Relation | Decision |
|---------|----------|----------|
| 10 vs 12-avcodec | Same idea (raise av_log during flush in `video.c`); different path headers / hunk shape | **SKIP both for now** — prior session REVERTED: broke hwaccel |
| 12-filter | Different approach: global `av_log_set_callback` filter in `avcommon.h` | **SKIP** — same risk class as 10/12 |
| 11 | Fixes fallout from `fix-shadow-warnings` (`i_len` rename) | **APPLY after** shadow patch |

## Canonical set
`/tmp/vlc-ng/patches/*.patch` = unique B set (22 files). `_sources/` keeps A/B provenance copies.
