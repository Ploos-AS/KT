# M2.40 — Terminal geometry lifecycle notifications

Status: **IMPLEMENTED / CI PENDING**

M2.40 adds transport/frontend-neutral notifications for committed geometry
lifecycle transitions. A caller-owned notifier receives old and new M2.39
snapshots only after a successful state change.

Telnet, SSH and local viewport updates, explicit REMOTE/VIEWPORT activation and
remote disconnect are instrumented. Duplicate/no-op operations, validation
errors, ownership errors and transactional resize failures do not notify.

Connect helpers intentionally expose two notifications when both stages change
state: first geometry/ownership storage, then policy activation. These are two
real committed transitions rather than an artificial batched event.

Strict-C99 tests cover connect/update/disconnect notifications, duplicate and
invalid suppression, capacity-failure rollback, retry after capacity becomes
available, and wrong-owner rejection.

GitHub Actions qualification is pending before freeze.
