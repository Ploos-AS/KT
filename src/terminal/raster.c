#include "kt/terminal_raster.h"
int kt_term_raster_init(kt_term_raster*r,uint8_t*p,uint16_t w,uint16_t h,size_t stride,uint8_t cw,uint8_t ch){
 if(!r||!p||!w||!h||stride<w||!cw||!ch)return -1;r->fb.pixels=p;r->fb.width=w;r->fb.height=h;r->fb.stride=stride;r->cell_width=cw;r->cell_height=ch;r->fg=1;r->bg=0;return 0;
}
void kt_term_raster_clear(kt_term_raster*r,uint8_t v){uint16_t y,x;if(!r)return;for(y=0;y<r->fb.height;y++)for(x=0;x<r->fb.width;x++)r->fb.pixels[(size_t)y*r->fb.stride+x]=v;}
void kt_term_raster_draw_bitmap_cell(void*p,const kt_term_bitmap_render_cell*c){
 kt_term_raster*r=p;uint16_t ox,oy,x,y;if(!r||!c)return;ox=(uint16_t)(c->cell.x*r->cell_width);oy=(uint16_t)(c->cell.y*r->cell_height);
 for(y=0;y<r->cell_height;y++)for(x=0;x<r->cell_width;x++){uint16_t px=(uint16_t)(ox+x),py=(uint16_t)(oy+y);uint8_t on=0;if(px>=r->fb.width||py>=r->fb.height)continue;if(x<c->bitmap.width&&y<c->bitmap.height)on=(uint8_t)((c->bitmap.bitmap[(size_t)y*c->bitmap.stride+(x>>3)]>>(7-(x&7)))&1u);{uint8_t fg=c->cell.attr.fg,bg=c->cell.attr.bg;if(c->cell.attr.flags&KT_TERM_ATTR_INVERSE){uint8_t t=fg;fg=bg;bg=t;}r->fb.pixels[(size_t)py*r->fb.stride+px]=on?fg:bg;}}
}
static const kt_term_bitmap_renderer_ops ops={0,kt_term_raster_draw_bitmap_cell,0,0};
const kt_term_bitmap_renderer_ops *kt_term_raster_renderer_ops(void){return &ops;}
