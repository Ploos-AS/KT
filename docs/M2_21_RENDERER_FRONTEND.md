# M2.21 — Portable renderer/frontend contract

Status: **IMPLEMENTED / HOST-QUALIFIED**

M2.21 defines the platform-neutral presentation boundary above the M2.19 screen model and M2.20 glyph identity layer.

## Contract

`kt_term_render()` is the deterministic full-frame reference path. It emits cells in row-major order, followed by the cursor.

`kt_term_render_incremental()` adds caller-owned caching for constrained native frontends. Its first frame is complete; subsequent frames emit only changed cells and a changed cursor. A geometry mismatch or invalid cache forces a complete redraw.

The frontend owns the actual font, bitmap/glyph data, framebuffer or OS drawing calls, clipping and presentation. The portable terminal core has no dependency on KT hardware, AmigaOS, TOS, a graphics library or dynamic allocation.

Callbacks:
- `begin_frame` — optional
- `draw_cell` — required
- `draw_cursor` — optional
- `end_frame` — optional

Each emitted cell carries coordinates, terminal attributes and the M2.20 renderer glyph identity: Unicode fallback, original source byte, profile and PETSCII charset.

## Qualification

The strict-C99 host test verifies:
- deterministic row-major full-frame ordering;
- PETSCII charset identity through the renderer boundary;
- optional frame/cursor callbacks;
- invalid arguments;
- first-frame cache population;
- zero dirty cells on an unchanged frame;
- one-cell incremental redraw;
- cursor-only update;
- explicit cache invalidation;
- geometry mismatch forcing full redraw;
- rejection of undersized cache storage.

CI compiles the M2.21 test with `-std=c99 -Wall -Wextra -Werror -pedantic`.

## Portability target

This ABI is intended to be shared by the native KT frontend and future AmigaOS 2.04+/68000 and Atari ST/TOS terminal frontends. Platform qualification belongs to those frontends; M2.21 qualifies the portable contract and reference renderer.
