#include "kt/terminal_geometry_adapter.h"

int kt_term_geometry_from_telnet_naws(kt_term_geometry_event_state*state,
                                     const kt_term_geometry_event_ops*ops,void*ctx,
                                     const uint8_t*data,size_t len){
 uint16_t cols,rows;
 if(!state||!ops||!data||len!=4u)return -1;
 cols=(uint16_t)(((uint16_t)data[0]<<8)|(uint16_t)data[1]);
 rows=(uint16_t)(((uint16_t)data[2]<<8)|(uint16_t)data[3]);
 if(!cols||!rows)return -2;
 return kt_term_geometry_event_emit(state,ops,ctx,
        KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS,cols,rows);
}
