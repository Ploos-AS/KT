#ifndef KT_TERMINAL_GEOMETRY_ADAPTER_H
#define KT_TERMINAL_GEOMETRY_ADAPTER_H
#include <stddef.h>
#include <stdint.h>
#include "kt/terminal_geometry_event.h"

int kt_term_geometry_from_telnet_naws(kt_term_geometry_event_state *,
                                     const kt_term_geometry_event_ops *,void *,
                                     const uint8_t *,size_t);
int kt_term_geometry_from_ssh_pty(kt_term_geometry_event_state *,
                                  const kt_term_geometry_event_ops *,void *,
                                  const uint8_t *,size_t);
int kt_term_geometry_from_viewport(kt_term_geometry_event_state *,
                                   const kt_term_geometry_event_ops *,void *,
                                   const kt_term_viewport *);
#endif
