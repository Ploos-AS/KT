# M2.18 — Portable C terminal-core ABI

Status: **REFERENCE SKELETON IMPLEMENTED**

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

A frontend supplies a `kt_term_ops` callback table and opaque context. The core accepts input through `kt_term_feed()`. Printable bytes are emitted through `put_cell`; control characters use dedicated cursor callbacks. `kt_term_reset()` restores parser and attribute state.

The initial skeleton deliberately keeps the ABI narrow. ANSI/VT parsing, CP437/PETSCII profiles, geometry, scrollback and transfer engines remain layered features and are expanded against the M2.17 golden corpus rather than being coupled to platform code.

## Portability rules

The public header uses C99 fixed-width integer types only. It performs no allocation, file I/O, networking or direct hardware access. Platform frontends own those facilities.

## Acceptance

M2.18 is complete when the ABI/header, reference core skeleton and deterministic host tests compile and exercise reset, printable input and basic control dispatch. The next milestone expands the parser against the M2.17 conformance corpus.
