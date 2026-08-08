# GL acceleration under Xmux (glxgears)

## Package

```text
x11-apps/mesa-progs-9.0.0
```

Provides: `/usr/bin/glxgears`, `/usr/bin/glxinfo`

```bash
sudo emerge -1v x11-apps/mesa-progs
```

## Results (2026-08-08)

### `--gl nvidia` (PRIME offload into Xmux Xvfb)

| Check | Result |
|-------|--------|
| `glxinfo -B` | `direct rendering: Yes` |
| OpenGL vendor | NVIDIA Corporation |
| OpenGL renderer | **NVIDIA GeForce GTX 1050/PCIe/SSE2** |
| OpenGL version | 4.6.0 NVIDIA 580.159.04 |
| `glxgears -info` | same renderer; **~686 FPS** over 5s (`3431 frames in 5.0 seconds`) |

Logs: `glxinfo-nvidia.txt`, `glxgears-nvidia2.txt`  
Session: `xmux start glx-test --gl nvidia`  
SPECTATOR: `xmux attach glx-test --no-reconnect`

### `--gl mesa` (private llvmpipe)

| Check | Result |
|-------|--------|
| renderer | llvmpipe (LLVM 21.1.8, 256 bits) |
| Accelerated | **no** (software) |
| Mesa version | 25.3.6 (xmux-mesa) |

Logs: `glxinfo-mesa.txt`

### Screenshots

Xmux root screenshots of glxgears often look black (GL content not always in Xvfb pixmap).  
**Log proof is authoritative** for acceleration. Host `gnome-screenshot` during `DISPLAY=:0 glxgears` shows visible gears (`glxgears-host.png`).

## Conclusion

**NVIDIA GL acceleration works under Xmux** (`--gl nvidia` → GTX 1050, high FPS).  
Mesa path is software llvmpipe as designed for the X server / soft client path.
