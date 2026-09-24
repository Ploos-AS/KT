# M2.36 — Terminal geometry session binding

Status: **IMPLEMENTED / CI PENDING**

M2.36 binds the M2.35 transport/frontend adapters, M2.34 normalized event state,
and M2.33 transactional resize path into one caller-owned geometry session.

The session provides entry points for Telnet NAWS, SSH PTY, local viewport
updates, and remote disconnect. It introduces no dynamic allocation and keeps
protocol-specific state outside the terminal core.

Remote ownership remains exclusive. A transport cannot steal ownership from the
active Telnet/SSH source. Non-owner disconnect is a no-op. Owner disconnect
uses the transactional resize release path, falls back to FIXED geometry, and
clears that transport's duplicate state only after success. Therefore an
identical geometry update is accepted after reconnect.

Strict-C99 qualification covers Telnet reconnect, FIXED fallback, screen-content
preservation, presentation scheduling, SSH/Telnet ownership exclusion,
non-owner disconnect, and takeover after the owner disconnects.

GitHub Actions qualification is pending before freeze.
