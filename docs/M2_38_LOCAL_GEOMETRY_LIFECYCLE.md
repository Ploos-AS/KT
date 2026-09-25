# M2.38 — Local frontend geometry lifecycle

Status: **IMPLEMENTED / CI PENDING**

M2.38 extends the M2.36/M2.37 session facade with explicit local viewport
lifecycle operations. A frontend can ingest viewport geometry and activate
VIEWPORT policy as one operation, or explicitly reactivate stored viewport
geometry after a remote session.

Local geometry remains independent from remote ownership. While REMOTE is
authoritative, local viewport updates are remembered without resizing the
screen. VIEWPORT can then be explicitly activated, and the existing remote
owner may later reactivate REMOTE. Activating a different remote source is
rejected.

All screen-changing policy transitions use the M2.33 transactional resize path.
Capacity failures leave the previous screen and viewport geometry usable and
the failed local event retryable.

Strict-C99 tests cover local connect/update, capacity failure/retry, remote
takeover, FIXED fallback, viewport reactivation, stored local updates while
REMOTE is active, and deterministic VIEWPORT/REMOTE switching.

GitHub Actions qualification is pending before freeze.
