# M2.23 — Portable terminal frontend/platform boundary

Status: **IMPLEMENTED / HOST-QUALIFIED**

M2.23 freezes the thin platform boundary above the shared terminal engine.

## Shared frontend

`kt_term_frontend` binds the portable screen, terminal profile, caller-owned render cache, renderer callbacks and optional transport output. Rendering uses the incremental M2.21 path. Key input uses the normalized M2.22 event/VT-output path.

A frontend may be render-only. Output-dependent operations fail explicitly when no output transport is configured.

`kt_term_frontend_invalidate()` provides deterministic full redraw after platform-side state changes. Profile changes and geometry/cache mismatches also invalidate incremental rendering.

## Platform adapter

`kt_term_platform_adapter` is deliberately thin. A native implementation supplies:
- raw local key/modifier normalization into portable KT key identities;
- optional final presentation after portable rendering.

Platform identities currently cover native KT, AmigaOS and Atari ST/TOS. They do not create separate terminal engines.

The shared core retains ownership of ANSI/VT parsing and encoding, CP437/PETSCII semantics, screen state, glyph identity, dirty rendering and transport-neutral output behavior.

## Qualification

Strict-C99 host tests verify:
- full then incremental rendering;
- one-cell dirty updates;
- profile and geometry invalidation;
- explicit frontend invalidation;
- render-only frontend behavior;
- key-to-wire output;
- backpressure and flush recovery;
- three distinct platform adapter identities using different raw key spaces;
- normalization of those platform keys into identical portable key semantics and VT output;
- platform presentation callbacks;
- rejected unknown native key mappings.

CI contains dedicated M2.23 frontend and cross-platform adapter compile/run steps using `-std=c99 -Wall -Wextra -Werror -pedantic`.

This is portable/mock qualification. It does not claim that real KT hardware, AmigaOS or Atari TOS display/keyboard drivers are implemented or physically qualified.
