# M2.24 — Terminal font / glyph provider

Status: **IMPLEMENTED / HOST-QUALIFIED**

M2.24 defines the platform-neutral boundary between KT terminal glyph identity and bitmap font data.

## Resolution policy

Glyph lookup is deterministic: exact legacy identity (profile + charset + source byte), Unicode codepoint fallback, then Unicode U+FFFD replacement glyph. This preserves pixel-faithful CP437/PETSCII rendering while allowing Unicode-oriented frontends.

## Bitmap contract

A returned glyph supplies width, height, byte stride, immutable bitmap data and byte size. KT validates non-zero geometry, sufficient stride and sufficient backing storage. Providers own bitmap storage; the core performs no allocation. Both 8x8 and 8x16 are qualified without making either mandatory.

## Renderer integration

The existing codepoint renderer remains available for OS/text frontends. `kt_term_render_bitmap()` adds the optional path: screen cell -> kt_term_glyph -> font provider -> validated bitmap -> bitmap renderer callback. Legacy identity remains available throughout.

## Qualification

Host tests cover legacy-first lookup, Unicode fallback, U+FFFD fallback, 8x8/8x16 glyphs, malformed bitmap rejection, provider errors, and PETSCII lower/upper identity through the complete bitmap path. CI builds these tests as strict C99 with warnings as errors.

This milestone bundles no IBM, Commodore, Amiga, Atari or other third-party font ROM/assets. Font data requires independently documented provenance and compatible licensing.

## Deferred

Framebuffer rasterization, clipping, scaling, palette handling and platform display drivers are outside M2.24.
