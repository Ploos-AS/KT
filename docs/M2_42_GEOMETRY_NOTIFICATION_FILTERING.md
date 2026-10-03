# M2.42 — Geometry notification filtering

Status: **IMPLEMENTED / CI PENDING**

M2.42 adds consumer-side filtering to the M2.40/M2.41 notification boundary.

Each `kt_term_geometry_notifier` owns a transition filter mask. Initialization
defaults to all transition bits for backward-compatible behavior. Consumers can
select policy, screen, viewport, remote geometry, ownership and disconnect
classes with `kt_term_geometry_notifier_set_filter()`.

A committed transition invokes the callback when any transition bit intersects
the filter. The callback still receives the complete transition mask. A zero
filter suppresses callbacks only; state commits continue normally.

Strict-C99 coverage includes UI-style and transport-style subscriptions,
multi-flag transitions, zero-mask suppression and default initialization.

GitHub Actions qualification is pending before freeze.
