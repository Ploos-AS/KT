# M2.30 — Terminal viewport and resize policy

Status: **IMPLEMENTED / HOST-QUALIFIED**

M2.30 defines the portable boundary between physical display pixels and logical terminal cells.

## Geometry

A viewport records physical pixel size, cell size, logical columns/rows, complete-cell used area and deterministic pixel offsets. Partial cells are never exposed.

## Sizing modes

- `KT_TERM_VIEWPORT_FIT`: derive the largest complete cell grid that fits.
- `KT_TERM_VIEWPORT_FIXED`: preserve an explicit logical grid such as 80x25. A physical resize that cannot contain the fixed grid is rejected.

Failed resizes are transactional: the previous valid viewport state is retained.

## Placement

`TOP_LEFT`, `CENTER`, and `BOTTOM_RIGHT` place the complete-cell area inside unused physical pixels. Integer division is deterministic; with an odd remainder CENTER leaves the extra pixel on the trailing side.

## Resize events

`kt_term_viewport_resize_adapter` accepts the existing `KT_TERM_EVENT_RESIZE` ABI. Because the M2.22 resize callback is intentionally void, the adapter retains the resize result and exposes it through `kt_term_viewport_resize_adapter_result()`.

This preserves the established event ABI while allowing a frontend/event loop to detect a rejected FIXED resize.

## Qualification

Strict C99 host tests cover 80x30 and 80x25 geometry, non-divisible resolutions, all placement policies, FIT/FIXED transitions, successful resize, rejected resize with full rollback, invalid geometry, and resize-event integration.

This milestone does not prescribe a window system, video controller, display timing, or physical KT display hardware. Physical qualification remains required later.
