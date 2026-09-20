#include "kt/terminal_raster.h"
#include <assert.h>
#include <string.h>
static const uint8_t glyph[8]={0x81,0x42,0x24,0x18,0x18,0x24,0x42,0x81};
int main(void){
 uint8_t mem[82],*fb=&mem[1];kt_term_raster r;kt_term_bitmap_render_cell c;unsigned x,y;
 memset(mem,0xcc,sizeof mem);assert(kt_term_raster_init(&r,fb,8,8,10,8,8)==0);kt_term_raster_clear(&r,3);
 assert(mem[0]==0xcc&&mem[81]==0xcc);for(y=0;y<8;y++){assert(fb[y*10+8]==0xcc&&fb[y*10+9]==0xcc);}
 memset(&c,0,sizeof c);c.cell.attr.fg=7;c.cell.attr.bg=2;c.bitmap.width=8;c.bitmap.height=8;c.bitmap.stride=1;c.bitmap.bitmap=glyph;c.bitmap.bitmap_size=8;
 kt_term_raster_draw_bitmap_cell(&r,&c);assert(fb[0]==7&&fb[1]==2&&fb[7]==7);assert(fb[3*10+3]==7&&fb[3*10+4]==7);
 for(y=0;y<8;y++)for(x=8;x<10;x++)assert(fb[y*10+x]==0xcc);assert(mem[0]==0xcc&&mem[81]==0xcc);
 /* clipping: second cell begins exactly outside the 8-pixel framebuffer */
 c.cell.x=1;kt_term_raster_draw_bitmap_cell(&r,&c);assert(mem[0]==0xcc&&mem[81]==0xcc);
 /* inverse swaps foreground/background deterministically */
 c.cell.x=0;c.cell.attr.flags=KT_TERM_ATTR_INVERSE;kt_term_raster_draw_bitmap_cell(&r,&c);assert(fb[0]==2&&fb[1]==7);
 /* partial right clipping: a 6-pixel cell starts at x=6 in a 10-pixel framebuffer */
 {uint8_t clipmem[12];kt_term_raster cr;memset(clipmem,0xaa,sizeof clipmem);assert(kt_term_raster_init(&cr,&clipmem[1],10,1,10,6,1)==0);c.cell.x=1;c.cell.y=0;c.cell.attr.flags=0;c.bitmap.width=8;c.bitmap.height=1;c.bitmap.bitmap=glyph;c.bitmap.bitmap_size=1;kt_term_raster_draw_bitmap_cell(&cr,&c);assert(clipmem[0]==0xaa&&clipmem[11]==0xaa);}
 /* glyph smaller than cell: uncovered pixels are background */
 {uint8_t small[8];kt_term_raster sr;assert(kt_term_raster_init(&sr,small,8,1,8,8,1)==0);memset(small,9,sizeof small);c.cell.x=0;c.bitmap.width=4;c.bitmap.height=1;c.bitmap.bitmap=glyph;c.bitmap.bitmap_size=1;kt_term_raster_draw_bitmap_cell(&sr,&c);assert(small[4]==2&&small[7]==2);}
 assert(kt_term_raster_init(&r,fb,8,8,7,8,8)==-1);return 0;
}
