# M2.20 — Glyph decoding and renderer-facing codepoint boundary

Status: **IN PROGRESS**

M2.20 keeps terminal input and screen storage byte-oriented while giving platform renderers a deterministic Unicode-scalar boundary. This preserves classic BBS bytes and avoids coupling the terminal core to fonts, UTF-8 encoders or platform graphics APIs.

The initial implementation exposes `kt_term_decode_codepoint(profile, byte)`. CP437 implements the exact M2.17 box-drawing corpus (vertical/horizontal lines, corners, tees and cross). ASCII is identity-mapped across profiles. Unsupported high bytes currently return U+FFFD rather than silently inventing a glyph mapping.

Next work expands and qualifies PETSCII printable glyph semantics while retaining PETSCII control handling in the M2.18 parser.
