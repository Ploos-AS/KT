# M2.26 — Terminal palette / colour contract

Status: **IMPLEMENTED / HOST-QUALIFIED**

M2.26 keeps raster output palette-indexed while defining deterministic reference colour mappings for portable frontends.

ANSI and CP437 use the 16-colour ANSI reference palette. PETSCII uses the 16-colour C64 reference palette. Palettes are exposed as RGB24 reference values but platform backends remain free to quantize them to native hardware colour formats.

The indexed framebuffer therefore remains one byte per pixel and independent of RGB layout. `kt_term_palette_convert_rgb24()` provides an optional portable conversion to packed R,G,B bytes with independent source and destination strides.

Host qualification covers palette sizes, known reference entries, profile mapping, invalid IDs/indices, RGB byte order, independent strides, row padding, guard bytes and invalid geometry. Out-of-range source pixels fail rather than silently wrapping.

Palette values are reference mappings, not a claim that analogue CRT output or every historical machine revision produced identical measured RGB values.
