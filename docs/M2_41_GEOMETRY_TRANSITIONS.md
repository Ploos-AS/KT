# M2.41 — Terminal geometry transition classification

Status: **IMPLEMENTED / CI PENDING**

M2.41 derives transport-neutral transition flags from two M2.39 geometry
snapshots. Consumers no longer need to duplicate snapshot-diff logic.

Flags classify policy changes, actual screen resize, stored viewport changes,
remote geometry changes, ownership changes and remote disconnect. Flags are
combinable, so one committed transition can describe all of its effects.

M2.40 notifications now carry the computed transition flags together with old
and new snapshots. Classification happens only for a committed state change;
duplicates, no-ops and failed/rolled-back operations therefore produce no
classified notification.

Strict-C99 coverage verifies exact flags across FIXED, VIEWPORT, remote
ownership storage, REMOTE activation and disconnect. Notification tests also
verify the exact flags delivered to consumers.

GitHub Actions qualification is pending before freeze.
