# M2.19 — Portable screen-state/reference renderer boundary

Status: **REFERENCE SKELETON IMPLEMENTED**

M2.19 adds a deterministic caller-owned terminal screen model above the M2.18 parser. It contains no allocation, OS, video, font or hardware dependency.

The reference screen stores byte cells plus the semantic M2.18 attributes, cursor position and fixed geometry. A frontend binds it to the parser through `kt_term_screen_ops()`. ANSI CUP/relative movement and erase-display mode 2 therefore update the same semantic state on KT and future Amiga/Atari frontends.

Initial acceptance requires fixed caller-owned storage, deterministic clear/init, printable cell placement, cursor movement/positioning and ED2. Scrolling, glyph decoding and platform rendering remain later work.
