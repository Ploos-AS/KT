# M2.31 — Terminal geometry negotiation and session-size policy

Status: **IMPLEMENTED / HOST-QUALIFIED**

M2.31 separates physical display geometry from the logical terminal session size.

## Policies

- `FIXED` — keep an explicit session size such as 80x25.
- `VIEWPORT` — follow the complete-cell grid calculated by the M2.30 viewport.
- `REMOTE` — follow geometry negotiated by a remote transport.

The active `cols`/`rows` are selected by policy. Updating an inactive source does not silently resize the active session.

## Sources and ownership

The portable negotiation API identifies `TELNET_NAWS`, `SSH_PTY`, and `LOCAL` sources. Telnet NAWS and SSH PTY feed REMOTE geometry; LOCAL feeds viewport geometry.

Only one remote transport may own remote geometry at a time. A stale update from another transport is rejected. Releasing the owner invalidates remote geometry. If REMOTE was active, the policy deterministically falls back to FIXED.

This milestone defines the portable geometry boundary only. It does not implement the Telnet NAWS wire protocol or SSH PTY transport.

## Viewport integration

`kt_term_geometry_from_viewport()` imports the complete-cell dimensions calculated by M2.30. For example, an 8x16 cell viewport maps 640x400 to 80x25 and 640x480 to 80x30. A FIXED 80x25 session remains 80x25 while the stored viewport geometry changes.

## Resource limits

Geometry is bounded before it can become session state. Defaults are 1x1 through 512x256. `kt_term_geometry_set_limits()` can install tighter platform/session limits.

Limit changes are transactional: if current fixed, viewport, remote, or active geometry would become invalid, the previous complete geometry state is restored. Untrusted remote sizes outside the configured bounds are rejected.

These limits protect the geometry contract; later screen/framebuffer allocation code must still perform its own checked size arithmetic and memory-capacity validation.

## Qualification

Host tests cover:

- FIXED, VIEWPORT, and REMOTE policy switching;
- M2.30 viewport-to-session integration and resize;
- Telnet NAWS, SSH PTY, and LOCAL negotiation sources;
- remote ownership conflicts, release, reconnect, and REMOTE-to-FIXED fallback;
- invalid and zero dimensions;
- default boundary values and extreme remote input such as 65535x65535;
- custom limits and transactional rollback.

The CI job builds the M2.31 tests as strict C99 with warnings treated as errors.

Physical KT display hardware, real Telnet/SSH transports, and target-system qualification remain later milestones.
