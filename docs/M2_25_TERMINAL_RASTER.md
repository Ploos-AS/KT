# M2.25 — Reference bitmap rasterizer / framebuffer

Status: **IMPLEMENTED / HOST-QUALIFIED**

M2.25 provides a deterministic platform-neutral reference rasterizer from validated terminal bitmap glyphs to a caller-owned 8-bit framebuffer.

## Contract

The framebuffer has explicit width, height and byte stride. The rasterizer has explicit terminal cell width/height and performs no allocation. Glyph bits are MSB-first. Writes are clipped to the visible framebuffer and do not touch row padding.

Glyphs smaller than a cell leave the remaining cell area as background. Glyphs larger than a cell are clipped to the cell. Foreground/background values come from terminal attributes.

## Attributes

Inverse swaps foreground/background. Underline forces foreground on the final cell scanline. Blink timing is external: the caller sets an explicit blink phase; the rasterizer contains no timer. Cursor visibility is likewise explicit and the reference cursor is drawn on the final cell scanline. Bold remains semantic metadata; M2.25 does not synthesize bold pixels because authentic font/frontends may provide their own bold treatment.

## Qualification

Strict-C99 host tests cover known 8x8 glyph rasterization, foreground/background, stride and row padding, guard bytes, full and partial clipping, glyph/cell size mismatch, inverse, underline, both blink phases and cursor visibility. Invalid framebuffer stride is rejected.

## Deferred

Palette interpretation, colour conversion, scaling, hardware blitters, display timing and platform-specific KT/Amiga/Atari display drivers remain outside the reference rasterizer.
