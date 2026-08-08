# NVIDIA VAAPI hard limits (GTX 1050 + nvidia-vaapi-driver)

## Stack
- Driver: `nvidia_drv_video.so` (nvidia-vaapi-driver)
- GPU: GTX 1050 Pascal
- Display under Xmux: GL via PRIME; X server is Mesa Xvfb

## Observed (quoted from vaapi-nvidia.log)
```
avcodec decoder debug: trying format vaapi
main generic debug: no glconv modules matched
main vout display debug: A filter to adapt decoder VAOP to display I420 is needed
vaapi_filters filter warning: Using SW chroma filter for 1280x720 VAOP -> I420
```
Then VLC often retries and ends on **VDPAU** decode + `glconv_vdpau` for display under Xmux.

## Why not fixable in VLC-ng alone
- No glconv matched for VAAPI→GL zero-copy on this stack.
- SW chroma VAOP→I420 is the only VAAPI display path when forced.
- Prefer `-V gl --avcodec-hw vdpau` for NVIDIA HW decode + visible picture.

## Evidence
- vaapi-nvidia.log, vaapi-nvidia-t1/t2.png (picture non-black via eventual path)
