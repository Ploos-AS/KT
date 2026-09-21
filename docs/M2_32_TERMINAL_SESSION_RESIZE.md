# M2.32 — Terminal session and screen resize integration

Status: **IMPLEMENTED / HOST-QUALIFIED**

M2.32 connects the M2.31 logical session geometry to the caller-owned M2.19 screen and carries geometry changes through renderer damage and presentation scheduling.

## Session contract

`kt_term_session` binds:

- an M2.31 `kt_term_geometry`;
- an M2.19 `kt_term_screen`;
- a caller-owned cell buffer and explicit capacity;
- optionally an M2.21 render cache.

No dynamic allocation is performed by the session layer.

`kt_term_session_apply_geometry()` validates the required cell count before changing screen state. Insufficient capacity is rejected before cell data or screen geometry is modified.

## Resize semantics

Resize preserves the overlapping top-left screen region.

- Growing height preserves existing rows and blanks new rows.
- Growing width moves retained rows bottom-up so in-place overlap is safe, then blanks new columns.
- Shrinking width moves retained rows top-down and clips cells outside the new geometry.
- Shrinking height clips rows outside the new geometry.
- New cells use the current screen attributes and default PETSCII charset identity.
- Cursor coordinates are preserved when valid and clamped to the new last column/row when required.
- A no-op geometry application leaves the screen unchanged.

The session owns no alternate screen buffer; preservation is performed in-place in caller-provided storage.

## Renderer integration

A render cache may be attached with `kt_term_session_bind_render_cache()`. Binding invalidates the cache. Any actual screen geometry change invalidates it again.

The next M2.21 incremental render therefore becomes a deterministic full redraw for the new geometry. A subsequent unchanged frame returns to normal zero-damage incremental behavior.

## Geometry-policy integration

Host qualification covers the complete policy sequence:

`FIXED 80x25 -> VIEWPORT 80x30 -> REMOTE 132x43 -> disconnect -> FIXED 80x25`

Screen contents inside the retained region survive these transitions.

## Damage and presentation

A resize-induced full render is passed through M2.28 damage conversion and M2.29 scheduling. With 16-pixel cells:

- 80x25 maps to framebuffer rows 0..399;
- 80x30 maps to framebuffer rows 0..479.

The scheduler receives the complete resized framebuffer region, preventing stale rows from the previous geometry.

## Qualification

Strict-C99 host tests cover:

- 80x25 -> 80x30 height growth;
- 80x30 -> 100x30 width growth;
- 100x30 -> 40x20 shrink;
- content preservation and deterministic blanking;
- cursor clamping;
- insufficient-capacity rejection with byte-for-byte cell-buffer rollback;
- FIXED/VIEWPORT/REMOTE policy transitions;
- render-cache invalidation and full redraw;
- zero redraw on an unchanged frame;
- full resize damage conversion and presentation scheduling.

The CI definitions compile with `-std=c99 -Wall -Wextra -Werror -pedantic`.

Physical display hardware, target memory sizing, real Telnet/SSH resize transports, and platform-specific screen allocation remain later qualification work.
