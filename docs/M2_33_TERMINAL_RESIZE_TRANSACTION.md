# M2.33 — Terminal resize transaction/orchestration

Status: **IMPLEMENTED / CI PENDING**

M2.33 defines the transaction boundary between negotiated terminal geometry and the caller-owned screen/presentation state. It builds on M2.31 geometry policy and M2.32 session resize.

## Contract

A resize is committed in this order:

1. select or negotiate geometry;
2. validate the required caller-owned cell capacity;
3. preflight presentation scheduling on a scheduler copy;
4. resize the terminal screen in place;
5. invalidate the bound render cache through the M2.32 session layer;
6. commit scheduler state and report full framebuffer damage.

Operations that fail before commit leave screen and scheduler state unchanged. Geometry-mutating wrappers snapshot M2.31 state and restore it when the resize cannot commit.

## API

`kt_term_resize_apply()`
Applies the geometry already selected in `kt_term_geometry`.

`kt_term_resize_set_policy()`
Changes FIXED/VIEWPORT/REMOTE policy and applies the resulting geometry as one transaction. A capacity failure restores the previous geometry policy and active dimensions.

`kt_term_resize_negotiate()`
Combines LOCAL, Telnet NAWS, or SSH PTY geometry negotiation with resize commit. This prevents an active REMOTE geometry from diverging from the screen when a live remote resize is too large.

`kt_term_resize_release_remote()`
Handles transport disconnect/release. The owning remote source can fall back from REMOTE to FIXED through the same transaction. Release by a non-owner is a no-op.

## Failure and rollback

The caller owns the cell buffer. M2.33 performs no allocation. If the selected `cols * rows` exceeds `cell_capacity`, the operation fails before screen mutation.

Presentation scheduling is performed first on a copy of the scheduler. Only after screen resize succeeds is that copy committed. There is therefore no scheduler failure path after screen mutation.

For policy, negotiation, and release wrappers, the complete `kt_term_geometry` state is snapshotted before mutation. Failed resize restores policy, active dimensions, remote dimensions, validity, and remote-source ownership.

## Damage policy

A committed geometry change reports the entire framebuffer as dirty. This is intentional: shrinking a terminal may require clearing pixels that are no longer occupied by cells, so damage cannot be limited to the new terminal rectangle.

A geometry no-op does not invalidate presentation state or schedule redraw.

## Qualification coverage

`tests/test_m2_33_resize.c` covers:

- FIXED 80x25 -> 80x30 commit;
- no-op resize;
- capacity rejection without screen or scheduler mutation;
- FIXED -> REMOTE 132x43 rollback and retry;
- live Telnet NAWS 80x25 -> 132x43 rollback and retry while REMOTE is active;
- preservation of Telnet remote ownership across rejected NAWS resize;
- non-owner SSH release as a no-op;
- owner Telnet disconnect with REMOTE 132x43 -> FIXED 80x25 fallback;
- preserved terminal content in the retained top-left region;
- full framebuffer damage and scheduler commit.

The CI definition compiles M2.33 with strict C99 flags:

`-std=c99 -Wall -Wextra -Werror -pedantic`

The implementation and CI definition are present on `main`. GitHub Actions success has not yet been observable through the available repository integration, so this milestone remains **CI PENDING** rather than frozen.
