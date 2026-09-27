# M2.39 — Terminal geometry lifecycle state reporting

Status: **IMPLEMENTED / CI PENDING**

M2.39 adds a read-only session snapshot so frontends and transports can inspect
geometry lifecycle state without reaching into terminal geometry, screen or
session internals.

The snapshot reports active policy, actual screen columns/rows, stored viewport
geometry, stored remote geometry, remote validity and remote ownership/source.
It performs no allocation and does not mutate session state.

Strict-C99 lifecycle coverage observes snapshots through FIXED, VIEWPORT,
Telnet REMOTE and owner disconnect back to FIXED. The test also verifies that
stored viewport geometry survives remote activation/disconnect while remote
validity and ownership are cleared correctly.

GitHub Actions qualification is pending before freeze.
