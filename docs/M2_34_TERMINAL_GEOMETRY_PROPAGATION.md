# M2.34 — Terminal geometry propagation

Status: **IMPLEMENTED / CI PENDING**

M2.34 defines one transport-neutral geometry-change boundary between terminal
frontends/transports and the M2.33 transactional resize path.

## Sources

The normalized event carries only:

- source: Telnet NAWS, SSH PTY, or local viewport/frontend
- columns
- rows

Transport-specific parsing remains outside this layer.

## Duplicate suppression

Duplicate state is tracked independently per source. Therefore an 80x25 Telnet
event does not suppress an 80x25 SSH or local event.

An event is cached only after the consumer succeeds. Failed validation, capacity
preflight, resize, or presentation scheduling therefore leaves the event
retryable.

## Resize adapter

`kt_term_geometry_resize_adapter` connects the normalized event callback to
`kt_term_resize_negotiate()`. This gives the path:

transport/frontend -> M2.34 event -> M2.33 transaction -> geometry -> screen ->
presentation scheduler.

No dynamic allocation is introduced.

## Transaction guarantees

Invalid or untrusted geometry must not leave partial remote ownership behind.
M2.31 negotiation now restores its complete geometry snapshot when remote
validation fails, and M2.33 restores its snapshot for any negotiation failure.

A failed 132x43 resize caused by insufficient caller-owned cell capacity does
not consume the M2.34 event. After capacity is increased, the identical event
can be retried and committed.

## Qualification

Strict-C99 tests cover:

- Telnet NAWS, SSH PTY and local sources
- per-source duplicate suppression
- callback failure and retry
- full event-to-M2.33 resize integration
- capacity failure, rollback and retry
- invalid 65535x65535 remote geometry
- ownership preservation
- full-frame presentation scheduling after a committed resize

GitHub Actions qualification is pending before freeze.
