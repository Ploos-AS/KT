#ifndef KT_TERMINAL_EVENT_H
#define KT_TERMINAL_EVENT_H
#include <stdint.h>
typedef enum kt_term_event_type {
 KT_TERM_EVENT_BYTE=0,
 KT_TERM_EVENT_PASTE,
 KT_TERM_EVENT_RESIZE,
 KT_TERM_EVENT_KEY,
 KT_TERM_EVENT_MOUSE
} kt_term_event_type;
typedef struct kt_term_event {
 kt_term_event_type type;
 uint8_t byte;
 uint8_t mods;
 uint16_t width,height;
 uint32_t key;
 const uint8_t *data;
 uint16_t data_len;
} kt_term_event;
typedef struct kt_term_input_ops {
 void (*on_byte)(void *,uint8_t);
 void (*on_paste)(void *,const uint8_t *,uint16_t);
 void (*on_resize)(void *,uint16_t,uint16_t);
 void (*on_key)(void *,uint32_t,uint8_t);
 void (*on_mouse)(void *,uint16_t,uint16_t,uint8_t,uint8_t);
} kt_term_input_ops;
int kt_term_dispatch_event(const kt_term_event *,const kt_term_input_ops *,void *);
#endif
