#ifndef KT_TERMINAL_PLATFORM_H
#define KT_TERMINAL_PLATFORM_H
#include <stdint.h>
#include "kt/terminal_frontend.h"

typedef enum kt_term_platform {
 KT_TERM_PLATFORM_NATIVE=0,
 KT_TERM_PLATFORM_AMIGAOS,
 KT_TERM_PLATFORM_ATARI_TOS
} kt_term_platform;

typedef struct kt_term_platform_key {
 uint32_t code;
 uint8_t mods;
} kt_term_platform_key;

typedef struct kt_term_platform_ops {
 int (*normalize_key)(void *,uint32_t,uint32_t,kt_term_platform_key *);
 int (*present)(void *);
} kt_term_platform_ops;

typedef struct kt_term_platform_adapter {
 kt_term_platform platform;
 kt_term_frontend *frontend;
 const kt_term_platform_ops *ops;
 void *ctx;
} kt_term_platform_adapter;

int kt_term_platform_init(kt_term_platform_adapter *,kt_term_platform,
 kt_term_frontend *,const kt_term_platform_ops *,void *);
int kt_term_platform_key_event(kt_term_platform_adapter *,uint32_t,uint32_t);
int kt_term_platform_present(kt_term_platform_adapter *);
#endif
