# M2.35 — Terminal geometry transport adapters

Status: **IMPLEMENTED / CI PENDING**

M2.35 adds allocation-free protocol/frontend adapters in front of the normalized
M2.34 geometry event boundary.

Supported inputs:

- Telnet NAWS: four-byte network-order columns/rows payload
- SSH PTY geometry: four-byte network-order columns/rows boundary
- local frontend: computed `kt_term_viewport`

The adapters validate framing and reject zero dimensions before publishing a
normalized geometry event. Protocol details do not enter the terminal core.

Duplicate suppression and retry semantics remain owned by M2.34. Consequently a
downstream M2.33 capacity or resize failure does not consume the transport
geometry update.

The E2E qualification covers Telnet NAWS through M2.35 -> M2.34 -> M2.33,
including an insufficient-capacity rollback and successful retry, followed by a
local viewport resize through the same path.

All M2.35 unit and E2E tests are wired for strict C99 with
`-Wall -Wextra -Werror -pedantic`. GitHub Actions qualification is pending
before freeze.
