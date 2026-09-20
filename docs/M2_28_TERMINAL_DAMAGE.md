# M2.28 — Incremental terminal damage to display regions

Status: **IMPLEMENTED / HOST-QUALIFIED**

M2.28 connects terminal-state changes to bounded framebuffer presentation.

The incremental renderer can return `kt_term_render_damage`, describing changed terminal rows, cursor movement and whether a complete redraw is required. `kt_term_dirty_rows_from_damage()` converts that semantic damage into pixel scanlines using the configured cell height and framebuffer height.

The pipeline is:

terminal screen -> incremental renderer -> render damage -> dirty pixel rows -> M2.27 display presentation

Properties:
- first render, invalidation, geometry change or profile change can request a full redraw;
- unchanged state produces no dirty region and therefore requires no display presentation;
- changed cells produce only the affected terminal-row scanlines;
- cursor-only movement dirties both old and new cursor rows;
- multiple changed rows are merged deterministically;
- the final region is clipped to framebuffer height;
- all state and storage remain caller-owned and allocation-free.

Host E2E qualification proves a single changed cell on terminal row 1 with an 8-pixel cell height presents only framebuffer scanlines 8..15. It also covers cursor-only movement, multiple changed rows, no-change suppression and a framebuffer ending part-way through the final terminal cell.

M2.28 intentionally tracks row ranges rather than rectangles. Column/rectangle damage can be added later if hardware measurements show that horizontal transfer reduction is worthwhile.
