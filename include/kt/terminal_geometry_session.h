#ifndef KT_TERMINAL_GEOMETRY_SESSION_H
#define KT_TERMINAL_GEOMETRY_SESSION_H
#include <stddef.h>
#include <stdint.h>
#include "kt/terminal_geometry_adapter.h"

typedef struct kt_term_geometry_snapshot {
 kt_term_geometry_policy policy;
 uint16_t screen_cols,screen_rows;
 uint16_t viewport_cols,viewport_rows;
 uint16_t remote_cols,remote_rows;
 uint8_t remote_valid;
 uint8_t remote_source_valid;
 kt_term_geometry_source remote_source;
} kt_term_geometry_snapshot;

typedef enum kt_term_geometry_transition {
 KT_TERM_GEOMETRY_TRANSITION_NONE=0,
 KT_TERM_GEOMETRY_TRANSITION_POLICY=1u<<0,
 KT_TERM_GEOMETRY_TRANSITION_SCREEN=1u<<1,
 KT_TERM_GEOMETRY_TRANSITION_VIEWPORT=1u<<2,
 KT_TERM_GEOMETRY_TRANSITION_REMOTE=1u<<3,
 KT_TERM_GEOMETRY_TRANSITION_OWNERSHIP=1u<<4,
 KT_TERM_GEOMETRY_TRANSITION_DISCONNECT=1u<<5
} kt_term_geometry_transition;

typedef void (*kt_term_geometry_notify_fn)(void *,
                                           const kt_term_geometry_snapshot *,
                                           const kt_term_geometry_snapshot *,
                                           uint32_t);

typedef struct kt_term_geometry_notifier {
 kt_term_geometry_notify_fn fn;
 void *ctx;
 uint32_t filter;
} kt_term_geometry_notifier;

typedef struct kt_term_geometry_notifier_set {
 kt_term_geometry_notifier *items;
 size_t count;
 size_t capacity;
 unsigned long generation;
} kt_term_geometry_notifier_set;

typedef struct kt_term_geometry_session {
 kt_term_geometry_event_state events;
 kt_term_geometry_resize_adapter resize;
 kt_term_geometry_notifier *notifier;
 kt_term_geometry_notifier_set *notifiers;
} kt_term_geometry_session;

int kt_term_geometry_session_init(kt_term_geometry_session *,
                                  kt_term_session *,
                                  kt_term_present_scheduler *,
                                  uint8_t,uint16_t,
                                  kt_term_resize_result *);
void kt_term_geometry_session_reset(kt_term_geometry_session *);
void kt_term_geometry_session_bind_notifier(kt_term_geometry_session *,
                                            kt_term_geometry_notifier *);
void kt_term_geometry_notifier_set_init(kt_term_geometry_notifier_set *,
                                        kt_term_geometry_notifier *,size_t);
int kt_term_geometry_notifier_set_add(kt_term_geometry_notifier_set *,
                                      const kt_term_geometry_notifier *,size_t *);
int kt_term_geometry_notifier_set_remove(kt_term_geometry_notifier_set *,size_t);
void kt_term_geometry_session_bind_notifier_set(kt_term_geometry_session *,
                                                kt_term_geometry_notifier_set *);
int kt_term_geometry_session_telnet_naws(kt_term_geometry_session *,
                                         const uint8_t *,size_t);
int kt_term_geometry_session_ssh_pty(kt_term_geometry_session *,
                                     const uint8_t *,size_t);
int kt_term_geometry_session_viewport(kt_term_geometry_session *,
                                      const kt_term_viewport *);
int kt_term_geometry_session_disconnect(kt_term_geometry_session *,
                                        kt_term_geometry_source);
int kt_term_geometry_session_activate_remote(kt_term_geometry_session *,
                                              kt_term_geometry_source);
int kt_term_geometry_session_connect_telnet(kt_term_geometry_session *,
                                            const uint8_t *,size_t);
int kt_term_geometry_session_connect_ssh(kt_term_geometry_session *,
                                         const uint8_t *,size_t);
int kt_term_geometry_session_activate_viewport(kt_term_geometry_session *);
int kt_term_geometry_session_connect_viewport(kt_term_geometry_session *,
                                              const kt_term_viewport *);
int kt_term_geometry_session_snapshot(const kt_term_geometry_session *,
                                      kt_term_geometry_snapshot *);
void kt_term_geometry_notifier_init(kt_term_geometry_notifier *,
                                    kt_term_geometry_notify_fn,void *);
void kt_term_geometry_notifier_set_filter(kt_term_geometry_notifier *,uint32_t);
int kt_term_geometry_session_snapshot_changed(const kt_term_geometry_snapshot *,
                                              const kt_term_geometry_snapshot *);
int kt_term_geometry_session_notify_commit(const kt_term_geometry_session *,
                                           const kt_term_geometry_snapshot *,
                                           const kt_term_geometry_notifier *);
int kt_term_geometry_transition_classify(const kt_term_geometry_snapshot *,
                                         const kt_term_geometry_snapshot *,
                                         uint32_t *);
#endif
