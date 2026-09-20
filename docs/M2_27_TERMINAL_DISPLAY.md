# M2.27 — Terminal display / presentation contract

Status: **IMPLEMENTED / HOST-QUALIFIED**

M2.27 defines the platform-neutral boundary between a completed framebuffer and a physical/window-system display backend.

Frames explicitly carry pixels, width, height, stride, pixel format and palette identity. Supported reference formats are INDEXED8 and packed RGB24. The backend contract is begin -> present_rows -> end, with present_rows mandatory and begin/end optional.

Both complete frames and bounded row ranges can be presented. This permits retro display hardware to update only affected scanlines while preserving the same contract for desktop frontends. Full-frame presentation uses the same row-range path.

Host qualification covers full frames, partial rows, range bounds, INDEXED8/RGB24 stride validation and propagation of begin/present/end backend failures. The contract performs no allocation and does not prescribe a display controller, blitter, window system or video timing.

M2.27 does not yet derive dirty rows from terminal-cell changes; that integration is deferred to M2.28.
