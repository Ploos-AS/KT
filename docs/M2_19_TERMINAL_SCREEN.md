# M2.19 — Portable screen-state/reference renderer boundary

Status: **IMPLEMENTED / HOST-QUALIFIED**

M2.19 adds a deterministic caller-owned terminal screen model above the M2.18 parser. It contains no allocation, OS, video, font or hardware dependency.

The reference screen stores byte cells plus the semantic M2.18 attributes, cursor position and fixed geometry. A frontend binds it to the parser through `kt_term_screen_ops()`. ANSI CUP/relative movement and erase-display mode 2 therefore update the same semantic state on KT and future Amiga/Atari frontends.

Acceptance is covered by the deterministic C99 host test in `tests/test_m2_19_terminal_screen.c`: caller-owned storage, clear/init, printable placement, exact M2.17 CR/LF and split-CSI vectors, CUP and relative cursor movement, SGR cell attributes/reset, ED0/ED1/ED2, cursor clamping, wrapping and bottom-of-screen scrolling.

Glyph decoding and platform rendering remain later work. CI contains a strict `-Wall -Wextra -Werror -pedantic` build/run step. GitHub's exposed commit status/workflow APIs returned no status or run for the qualification commit, so this freeze does not claim an independently observed green Actions run.
