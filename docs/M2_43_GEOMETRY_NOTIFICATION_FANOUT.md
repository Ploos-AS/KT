# M2.43 — Geometry notification fan-out

Status: **IMPLEMENTED / CI PENDING**

M2.43 extends the filtered geometry notification boundary with multiple
independent subscribers without dynamic allocation.

A caller-owned `kt_term_geometry_notifier_set` contains an array of M2.42
notifiers. Each subscriber retains its own callback, context and transition
filter. One committed geometry transition is classified once and then offered
to every matching subscriber.

The existing single-notifier binding remains available for compatibility and
can coexist with the notifier set. Empty notifier sets are valid.

Strict-C99 coverage exercises simultaneous UI, transport and diagnostics
subscribers across Telnet connect and disconnect, including multi-flag
transitions and per-subscriber filtering.

GitHub Actions qualification is pending before freeze.
