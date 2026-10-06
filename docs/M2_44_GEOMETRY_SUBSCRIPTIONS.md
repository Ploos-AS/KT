# M2.44 — Geometry notification subscription management

Status: **IMPLEMENTED / CI PENDING**

M2.44 adds bounded subscription management to the M2.43 caller-owned
notification fan-out. No dynamic allocation is introduced.

A notifier set now separates storage capacity from active subscriptions.
`kt_term_geometry_notifier_set_add()` copies a notifier into the first free
slot and can return its stable slot ID. `kt_term_geometry_notifier_set_remove()`
detaches that slot without moving remaining subscribers, preserving their IDs.
Vacated holes are reused by later subscriptions. Capacity exhaustion returns
`-2`; invalid add/remove operations return `-1`.

The M2.43 fan-out test was migrated to the explicit subscription API so its
storage semantics match M2.44. Strict-C99 coverage additionally verifies
capacity exhaustion, hole reuse, stable remaining IDs, invalid/double remove,
and lifecycle continuation after an observer is detached.

GitHub Actions qualification is pending before freeze.
