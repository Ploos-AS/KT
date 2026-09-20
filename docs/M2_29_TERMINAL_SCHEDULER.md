# M2.29 — Terminal presentation scheduler / frame policy

Status: **IMPLEMENTED / HOST-QUALIFIED**

M2.29 adds an allocation-free scheduling layer between M2.28 damage generation and M2.27 display presentation.

The scheduler accumulates dirty scanline ranges, supports immediate and coalesced policies, and presents pending rows through the display contract. Pending damage is cleared only after successful presentation; backend failure preserves it for retry.

Blink timing remains external. A blink tick can discover only terminal rows containing BLINK cells, convert those rows to framebuffer damage, queue them, and present only the affected scanlines. No terminal-state mutation or full-screen invalidation is required.

Policies:
- IMMEDIATE: new damage is marked ready for immediate presentation.
- COALESCE: callers may accumulate multiple updates and present them at an event-loop/frame boundary.

The scheduler owns no timer, sleeps, threads, framebuffer or dynamic memory. Platform event loops remain responsible for cadence.

Host qualification covers no-op frames, range coalescing, take/reset behavior, policy switching, backend failure/retry, successful clearing, blink-only discovery, and blink damage through scheduled partial display presentation.
