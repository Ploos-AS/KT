# M2.45 — Safe notification dispatch mutation

Status: **IMPLEMENTED / CI PENDING**

M2.45 defines deterministic allocation-free semantics when notification
callbacks mutate the M2.44 subscription set during fan-out.

The notifier set carries a generation counter incremented by add/remove.
Dispatch snapshots the active count and generation before invoking observers
and copies each notifier before its callback. If a callback changes the set,
the current fan-out round stops. This prevents newly added observers from
receiving the transition that created them, prevents removed observers from
being called later in the same round, and makes self-removal safe.

A later committed transition starts a fresh dispatch round and sees the updated
subscription set.

Strict-C99 coverage includes isolated adversarial cases and full geometry
lifecycle E2E cases for self-remove, remove-next and add-during-callback.

GitHub Actions qualification is pending before freeze.
