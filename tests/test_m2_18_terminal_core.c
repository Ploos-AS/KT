#include "kt/terminal.h"

#include <assert.h>
#include <stddef.h>
#include <stdint.h>

typedef struct fixture {
    uint8_t cells[32];
    kt_term_attr attrs[32];
    size_t count;
    unsigned cr, lf, bs, bell, moves, positions, erases;
    int16_t dx, dy;
    uint16_t row, col;
    uint8_t erase_mode;
} fixture;

static void put_cell(void *ctx, uint8_t ch, const kt_term_attr *attr)
{
    fixture *f=(fixture *)ctx;
    assert(attr != NULL);
    assert(f->count < sizeof f->cells);
    f->cells[f->count]=ch;
    f->attrs[f->count]=*attr;
    f->count++;
}
static void cr(void *ctx){((fixture *)ctx)->cr++;}
static void lf(void *ctx){((fixture *)ctx)->lf++;}
static void bs(void *ctx){((fixture *)ctx)->bs++;}
static void bell(void *ctx){((fixture *)ctx)->bell++;}
static void move(void *ctx,int16_t dx,int16_t dy){fixture *f=(fixture *)ctx;f->moves++;f->dx=dx;f->dy=dy;}
static void position(void *ctx,uint16_t row,uint16_t col){fixture *f=(fixture *)ctx;f->positions++;f->row=row;f->col=col;}
static void erase(void *ctx,uint8_t mode){fixture *f=(fixture *)ctx;f->erases++;f->erase_mode=mode;}

int main(void)
{
    static const kt_term_ops ops={put_cell,cr,lf,bs,bell,move,position,erase};
    static const uint8_t basic[]={'K','T','\r','\n','\b','\a'};
    static const uint8_t esc_csi[]={0x1b,'['};
    static const uint8_t cup_tail[]={'1','2',';','3','4','H'};
    static const uint8_t sgr_text[]={0x1b,'[','1',';','3','1',';','4','4',';','7','m','X'};
    static const uint8_t reset_text[]={0x1b,'[','0','m','Y'};
    static const uint8_t up[]={0x1b,'[','5','A'};
    static const uint8_t ed[]={0x1b,'[','2','J'};
    fixture f={0};
    kt_term term;

    kt_term_init(&term,&ops,&f);
    assert(term.attr.fg==7u && term.attr.bg==0u && term.attr.flags==0u);
    assert(term.parser_state==0u);

    kt_term_feed(&term,basic,sizeof basic);
    assert(f.count==2u && f.cells[0]=='K' && f.cells[1]=='T');
    assert(f.cr==1u && f.lf==1u && f.bs==1u && f.bell==1u);

    /* M2.17: CSI parsing must survive arbitrary feed boundaries. */
    kt_term_feed(&term,esc_csi,sizeof esc_csi);
    assert(term.parser_state!=0u);
    kt_term_feed(&term,cup_tail,sizeof cup_tail);
    assert(term.parser_state==0u);
    assert(f.positions==1u && f.row==12u && f.col==34u);

    /* M2.17: relative cursor movement. */
    kt_term_feed(&term,up,sizeof up);
    assert(f.moves==1u && f.dx==0 && f.dy==-5);

    /* M2.17: erase-display baseline. */
    kt_term_feed(&term,ed,sizeof ed);
    assert(f.erases==1u && f.erase_mode==2u);

    /* M2.17: SGR attributes are semantic state attached to cells. */
    kt_term_feed(&term,sgr_text,sizeof sgr_text);
    assert(f.cells[f.count-1u]=='X');
    assert(f.attrs[f.count-1u].fg==1u);
    assert(f.attrs[f.count-1u].bg==4u);
    assert((f.attrs[f.count-1u].flags & KT_TERM_ATTR_BOLD)!=0u);
    assert((f.attrs[f.count-1u].flags & KT_TERM_ATTR_INVERSE)!=0u);

    kt_term_feed(&term,reset_text,sizeof reset_text);
    assert(f.cells[f.count-1u]=='Y');
    assert(f.attrs[f.count-1u].fg==7u && f.attrs[f.count-1u].bg==0u);
    assert(f.attrs[f.count-1u].flags==0u);

    kt_term_reset(&term);
    assert(term.attr.fg==7u && term.attr.bg==0u && term.attr.flags==0u);
    assert(term.parser_state==0u);
    kt_term_feed(NULL,basic,sizeof basic);
    kt_term_feed(&term,NULL,sizeof basic);
    return 0;
}
