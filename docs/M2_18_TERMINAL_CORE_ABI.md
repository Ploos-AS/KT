# M2.18 — Portable C terminal-core ABI

Status: **IMPLEMENTED / HOST-QUALIFIED**

M2.18 turns the M2.13–M2.17 terminal contracts and conformance corpus into a small, platform-neutral C ABI.

## Goals

- no dependency on KT hardware registers, an OS, sockets or a particular display;
- caller-owned terminal object with deterministic storage;
- byte-stream input suitable for serial, Telnet and SSH transports;
- explicit screen operations through callbacks;
- reusable by native KT, AmigaOS 2.04+/68000 and Atari ST/TOS frontends;
- keep protocol/transport, terminal state and rendering boundaries separate.

## ABI v1

The public ABI is declared in `include/kt/terminal.h`.

A frontend supplies a `kt_term_ops` callback table and opaque context. The core accepts input through `kt_term_feed()`. Printable bytes are emitted through `put_cell`; control characters use dedicated cursor callbacks. `kt_term_reset()` restores parser and attribute state while preserving the selected terminal profile. ANSI is the default profile at initialization.

The ABI remains deliberately narrow. The reference core implements incremental ANSI CSI parsing for CUP/HVP, relative cursor movement, erase-display and the M2.17 SGR baseline. ANSI, byte-preserving CP437 and PETSCII profiles are explicit. PETSCII clear/home/reverse controls are covered by host vectors. Geometry, glyph rendering, scrollback and transfer engines remain platform/layer responsibilities rather than being coupled to the parser.

## Portability rules

The public header uses C99 fixed-width integer types only. It performs no allocation, file I/O, networking or direct hardware access. Platform frontends own those facilities.

## Acceptance

M2.18 acceptance is satisfied by the ABI/header, portable C reference core and deterministic host test in `tests/test_m2_18_terminal_core.c`. The test covers printable/control dispatch, split CSI parsing, CUP, relative cursor movement, erase-display, SGR attributes/reset, PETSCII baseline controls, byte-preserving CP437 box-drawing input, profile persistence across reset and safe recovery from malformed CSI.

CI compiles the core as strict C99 with `-Wall -Wextra -Werror -pedantic`. The repository-side GitHub API did not expose a status/check result while this milestone was frozen, so the qualification claim here is limited to the implemented deterministic host qualification contract; it does not claim an observed green GitHub Actions run.
