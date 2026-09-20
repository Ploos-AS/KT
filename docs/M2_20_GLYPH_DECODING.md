# M2.20 — Glyph decoding and renderer-facing codepoint boundary

Status: **IMPLEMENTED / HOST-QUALIFIED**

M2.20 keeps terminal input and screen storage byte-oriented while giving platform renderers a deterministic Unicode-scalar boundary. This preserves classic BBS bytes and avoids coupling the terminal core to fonts, UTF-8 encoders or platform graphics APIs.

The initial implementation exposes `kt_term_decode_codepoint(profile, byte)`. CP437 implements the exact M2.17 box-drawing corpus (vertical/horizontal lines, corners, tees and cross). ASCII is identity-mapped across profiles. Unsupported high bytes currently return U+FFFD rather than silently inventing a glyph mapping.

PETSCII printable decoding now has an explicit first baseline: $41-$5A map to Unicode A-Z, the alternate $61-$7A PETSCII uppercase range also maps to Unicode A-Z, and ASCII-compatible digits/punctuation remain identity-mapped. PETSCII controls remain parser responsibilities rather than renderer glyphs.

Next work expands the renderer contract around PETSCII graphics/shifted character-set semantics without pretending that PETSCII is ASCII or Unicode.

## PETSCII mapping-table policy

PETSCII graphics are represented by explicit byte-indexed tables rather than a growing switch statement. A zero table entry means that the mapping has not yet been qualified and the decoder returns U+FFFD. This makes coverage reviewable and prevents accidental ASCII assumptions.

The upper/graphics $A0-$BF table is populated from the qualified Unicode legacy-computing mapping. Lower/upper mode remains distinct, and bytes without a qualified mapping return U+FFFD rather than being guessed.

## Mapping authority

PETSCII graphics qualification is anchored to Unicode Consortium terminal/legacy-computing material, which explicitly documents PET/VIC-20 and C64/C128 character repertoires and the Symbols for Legacy Computing repertoire. This is preferred over informal byte-to-Unicode tables because many PETSCII glyphs historically lacked exact Unicode equivalents.

The mapping table remains deliberately partial until each byte is matched to a Unicode character with sufficient visual/semantic equivalence. Where Unicode has no faithful scalar, the renderer contract may later use a project-private glyph identifier rather than publish a knowingly incorrect Unicode mapping.


## Qualification

The strict-C99 M2.20 host test covers the M2.17 CP437 box corpus, PETSCII alphabetic semantics in both character sets, representative values across the $A0-$BF graphics table, source-byte/profile/charset preservation, and an end-to-end parser → charset shift → screen cell → glyph-resolution path. It also verifies that parser reset preserves the configured PETSCII profile and active charset.

CI builds this test with `-std=c99 -Wall -Wextra -Werror -pedantic`. GitHub's exposed commit-status/workflow APIs returned no status or workflow run for the qualification commit, so this milestone does not claim an independently observed green Actions result.
