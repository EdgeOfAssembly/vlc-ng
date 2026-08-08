# VLC-ng helper targets (include or: make -f Makefile.vlc-ng.mk profile)
# Release/profile: -DNDEBUG so assert() compiles out.
PREFIX ?= /tmp/vlc-ng-install
NPROC := $(shell nproc)

.PHONY: profile profile-build profile-asan-smoke

profile-build:
	@echo "=== profile-build PREFIX=$(PREFIX) -DNDEBUG ==="
	$(MAKE) -C "$(CURDIR)" clean || true
	CFLAGS="-O2 -g -fno-omit-frame-pointer -DNDEBUG" \
	CXXFLAGS="-O2 -g -fno-omit-frame-pointer -DNDEBUG" \
	./scripts/configure-vlc-ng.sh --prefix="$(PREFIX)" --disable-debug --disable-sid
	# Ensure swscale on avcodec
	@grep -q SWSCALE_LIBS modules/Makefile || \
	  sed -i 's/libavcodec_plugin_la_LIBADD = $$(AVCODEC_LIBS) $$(LIBM)/libavcodec_plugin_la_LIBADD = $$(AVCODEC_LIBS) $$(SWSCALE_LIBS) $$(LIBM)/' modules/Makefile
	$(MAKE) -s V=0 -j$(NPROC)
	$(MAKE) -s V=0 install
	@echo "Installed to $(PREFIX)"

profile: profile-build
	PREFIX="$(PREFIX)" ./scripts/profile-playback.sh baseline

# Smoke only (no rebuild)
profile-run:
	PREFIX="$(PREFIX)" ./scripts/profile-playback.sh run
