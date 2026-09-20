# M2.20 — Glyph decoding and renderer-facing codepoint boundary

Status: **IN PROGRESS**

M2.20 keeps terminal input and screen storage byte-oriented while giving platform renderers a deterministic Unicode-scalar boundary. This preserves classic BBS bytes and avoids coupling the terminal core to fonts, UTF-8 encoders or platform graphics APIs.

The initial implementation exposes `kt_term_decode_codepoint(profile, byte)`. CP437 implements the exact M2.17 box-drawing corpus (vertical/horizontal lines, corners, tees and cross). ASCII is identity-mapped across profiles. Unsupported high bytes currently return U+FFFD rather than silently inventing a glyph mapping.

PETSCII printable decoding now has an explicit first baseline: $41-$5A map to Unicode A-Z, the alternate $61-$7A PETSCII uppercase range also maps to Unicode A-Z, and ASCII-compatible digits/punctuation remain identity-mapped. PETSCII controls remain parser responsibilities rather than renderer glyphs.

Next work expands the renderer contract around PETSCII graphics/shifted character-set semantics without pretending that PETSCII is ASCII or Unicode.

## PETSCII mapping-table policy

PETSCII graphics are represented by explicit byte-indexed tables rather than a growing switch statement. A zero table entry means that the mapping has not yet been qualified and the decoder returns U+FFFD. This makes coverage reviewable and prevents accidental ASCII assumptions.

The first table covers $A0-$BF and currently qualifies $A0-$A6 only. Remaining entries are intentionally unmapped until verified against an authoritative PETSCII character-set reference. The implementation therefore distinguishes known mappings from guesses.
