#include "kt/terminal_viewport.h"
static void place(kt_term_viewport*v){
 uint16_t rx=(uint16_t)(v->pixel_width-v->used_width),ry=(uint16_t)(v->pixel_height-v->used_height);
 if(v->anchor==KT_TERM_VIEWPORT_CENTER){v->offset_x=(uint16_t)(rx/2u);v->offset_y=(uint16_t)(ry/2u);}
 else if(v->anchor==KT_TERM_VIEWPORT_BOTTOM_RIGHT){v->offset_x=rx;v->offset_y=ry;}
 else {v->offset_x=0;v->offset_y=0;}
}
static int calc(kt_term_viewport*v,uint16_t w,uint16_t h){
 if(!v||!w||!h||!v->cell_width||!v->cell_height)return -1;
 v->pixel_width=w;v->pixel_height=h;v->cols=(uint16_t)(w/v->cell_width);v->rows=(uint16_t)(h/v->cell_height);
 if(!v->cols||!v->rows)return -1;
 v->used_width=(uint16_t)(v->cols*v->cell_width);v->used_height=(uint16_t)(v->rows*v->cell_height);place(v);return 0;
}
int kt_term_viewport_init(kt_term_viewport*v,uint16_t w,uint16_t h,uint8_t cw,uint8_t ch){
 if(!v||!cw||!ch)return -1;v->cell_width=cw;v->cell_height=ch;v->anchor=KT_TERM_VIEWPORT_TOP_LEFT;return calc(v,w,h);
}
int kt_term_viewport_resize(kt_term_viewport*v,uint16_t w,uint16_t h){return calc(v,w,h);}
int kt_term_viewport_set_anchor(kt_term_viewport*v,kt_term_viewport_anchor a){
 if(!v||(a!=KT_TERM_VIEWPORT_TOP_LEFT&&a!=KT_TERM_VIEWPORT_CENTER&&a!=KT_TERM_VIEWPORT_BOTTOM_RIGHT))return -1;
 v->anchor=a;place(v);return 0;
}
