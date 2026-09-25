# M2.37 — Terminal geometry lifecycle integration

Status: **IMPLEMENTED / CI PENDING**

M2.37 adds lifecycle helpers on top of the M2.36 caller-owned geometry session.
A transport can now ingest its initial geometry and activate REMOTE policy as
one operation instead of open-coding adapter, ownership and policy sequencing.

The lifecycle API provides explicit remote activation plus Telnet and SSH
connect helpers. Live geometry updates continue through the M2.36 entry points,
and disconnect continues through the transactional M2.33 release path.

Ownership remains exclusive. An active SSH owner rejects a Telnet connect (and
vice versa); after owner disconnect and FIXED fallback, another transport may
take ownership. Failed connect/resize operations remain retryable and must not
leave policy, ownership, screen geometry or duplicate-event state partially
committed.

Strict-C99 tests cover Telnet connect/update/disconnect, capacity-failure retry,
SSH activation, cross-transport exclusion and takeover after disconnect.

GitHub Actions qualification is pending before freeze.
