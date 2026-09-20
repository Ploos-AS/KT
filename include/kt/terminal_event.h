#ifndef KT_TERMINAL_EVENT_H
#define KT_TERMINAL_EVENT_H
#include <stdint.h>
#include <stddef.h>
#include "kt/terminal.h"
#define KT_TERM_MOD_SHIFT 0x01u
#define KT_TERM_MOD_CTRL  0x02u
#define KT_TERM_MOD_ALT   0x04u
#define KT_TERM_MOD_META  0x08u

#define KT_TERM_KEY_SPECIAL 0x110000u
enum {
 KT_TERM_KEY_UP=KT_TERM_KEY_SPECIAL, KT_TERM_KEY_DOWN, KT_TERM_KEY_LEFT, KT_TERM_KEY_RIGHT,
 KT_TERM_KEY_HOME, KT_TERM_KEY_END, KT_TERM_KEY_PAGE_UP, KT_TERM_KEY_PAGE_DOWN,
 KT_TERM_KEY_INSERT, KT_TERM_KEY_DELETE,
 KT_TERM_KEY_F1, KT_TERM_KEY_F2, KT_TERM_KEY_F3, KT_TERM_KEY_F4,
 KT_TERM_KEY_F5, KT_TERM_KEY_F6, KT_TERM_KEY_F7, KT_TERM_KEY_F8,
 KT_TERM_KEY_F9, KT_TERM_KEY_F10, KT_TERM_KEY_F11, KT_TERM_KEY_F12
};

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
typedef struct kt_term_output_ops {
 int (*write)(void *,const uint8_t *,size_t);
} kt_term_output_ops;
typedef struct kt_term_key_output_adapter {
 const kt_term_output_ops *output;
 void *output_ctx;
 uint8_t pending[20];
 size_t pending_len;
 size_t pending_off;
 int last_error;
} kt_term_key_output_adapter;
void kt_term_key_output_adapter_init(kt_term_key_output_adapter *,const kt_term_output_ops *,void *);
const kt_term_input_ops *kt_term_key_output_adapter_ops(void);
int kt_term_key_output_flush(kt_term_key_output_adapter *);

typedef struct kt_term_feed_adapter {
 kt_term *term;
} kt_term_feed_adapter;
void kt_term_feed_adapter_init(kt_term_feed_adapter *,kt_term *);
const kt_term_input_ops *kt_term_feed_adapter_ops(void);
#endif
