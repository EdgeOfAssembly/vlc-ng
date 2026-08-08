# Compile warnings (ASan goal build)

Full log: `build-asan-goal.log` (~205 `warning:` lines).

## Fixed in VLC-ng touched code

| File | Issue | Fix |
|------|-------|-----|
| `modules/codec/avcodec/video.c` | unused param `obj` in `InitVideoHwDec` | `VLC_UNUSED(obj)` |
| `modules/codec/avcodec/video.c` | unused `ExtractAV1Profile` | `__attribute__((unused))` |

## Upstream / leave alone

- `#warning` in media_list, clock, event.h
- deprecated block_FifoSize, fluidsynth, libass, srt
- yacc CSSGrammar, longjmp clobbered, harmful_delay
- Makefile suffix-rule noise

No mass-edit of unrelated modules.
