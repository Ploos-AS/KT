# M2.22 — Terminal event and input abstraction

Status: **IMPLEMENTED / HOST-QUALIFIED**

M2.22 defines the portable input/output boundary around the terminal core.

## Input model

Normalized events cover raw bytes, paste, resize, keys and mouse input. Special keys use stable IDs outside the Unicode scalar range. Modifier flags cover Shift, Ctrl, Alt and Meta.

The feed adapter routes BYTE and PASTE events into the existing terminal parser without exposing parser internals to serial, TCP or frontend code.

## Key encoding

The portable encoder translates normalized navigation and F1–F12 keys into deterministic ANSI/VT byte sequences, including modifier parameters. Unsupported keys and insufficient output buffers fail without partial encoding.

## Output model

A transport-neutral write sink separates keyboard/event handling from UART, Telnet and SSH implementations.

The key-output adapter owns a fixed pending buffer and supports:
- partial writes;
- backpressure;
- explicit retry with `kt_term_key_output_flush()`;
- preservation of pending bytes across transport errors;
- detection of invalid over-reporting writers;
- no dynamic allocation.

A successful event dispatch means the event was accepted by the adapter; asynchronous transport state is exposed by pending state and `last_error`.

## Qualification

Strict-C99 host tests cover event dispatch, parser feed integration, normalized keys/modifiers, ANSI/VT encoding, output-sink integration, partial writes, backpressure, retry/recovery and writer contract violations.

CI contains dedicated M2.22 compile/run steps using `-std=c99 -Wall -Wextra -Werror -pedantic`.

GitHub status APIs did not expose a workflow result at freeze time, so this milestone claims host qualification from the deterministic test contract and CI definition, not an observed GitHub Actions PASS.

## Portability

The abstraction is intentionally independent of KT hardware and OS services. Native KT, AmigaOS and Atari ST/TOS frontends can normalize their local keyboard/input events into the same ABI, while serial/Telnet/SSH transports can implement the same output sink.
