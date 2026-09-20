#include "kt/terminal_viewport.h"
static int calc(kt_term_viewport*v,uint16_t w,uint16_t h){
 if(!v||!w||!h||!v->cell_width||!v->cell_height)return -1;
 v->pixel_width=w;v->pixel_height=h;v->cols=(uint16_t)(w/v->cell_width);v->rows=(uint16_t)(h/v->cell_height);
 if(!v->cols||!v->rows)return -1;
 v->used_width=(uint16_t)(v->cols*v->cell_width);v->used_height=(uint16_t)(v->rows*v->cell_height);return 0;
}
int kt_term_viewport_init(kt_term_viewport*v,uint16_t w,uint16_t h,uint8_t cw,uint8_t ch){
 if(!v||!cw||!ch)return -1;v->cell_width=cw;v->cell_height=ch;return calc(v,w,h);
}
int kt_term_viewport_resize(kt_term_viewport*v,uint16_t w,uint16_t h){return calc(v,w,h);}
