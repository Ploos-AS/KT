#ifndef KT_TERMINAL_GEOMETRY_SESSION_H
#define KT_TERMINAL_GEOMETRY_SESSION_H
#include <stddef.h>
#include <stdint.h>
#include "kt/terminal_geometry_adapter.h"

typedef struct kt_term_geometry_session {
 kt_term_geometry_event_state events;
 kt_term_geometry_resize_adapter resize;
} kt_term_geometry_session;

int kt_term_geometry_session_init(kt_term_geometry_session *,
                                  kt_term_session *,
                                  kt_term_present_scheduler *,
                                  uint8_t,uint16_t,
                                  kt_term_resize_result *);
void kt_term_geometry_session_reset(kt_term_geometry_session *);
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
typedef struct kt_term_geometry_snapshot {
 kt_term_geometry_policy policy;
 uint16_t screen_cols,screen_rows;
 uint16_t viewport_cols,viewport_rows;
 uint16_t remote_cols,remote_rows;
 uint8_t remote_valid;
 uint8_t remote_source_valid;
 kt_term_geometry_source remote_source;
} kt_term_geometry_snapshot;

int kt_term_geometry_session_snapshot(const kt_term_geometry_session *,
                                      kt_term_geometry_snapshot *);
#endif
