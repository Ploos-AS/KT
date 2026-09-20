#include "kt/terminal_dirty.h"
#include <assert.h>
int main(void){
 kt_term_dirty_rows d;kt_term_dirty_rows_reset(&d);assert(!d.valid);
 assert(kt_term_dirty_rows_add_cell(&d,4,8,200)==0&&d.valid&&d.first==32&&d.count==8);
 assert(kt_term_dirty_rows_add_cell(&d,6,8,200)==0&&d.first==32&&d.count==24);
 kt_term_dirty_rows_reset(&d);assert(kt_term_dirty_rows_add_cursor(&d,2,5,8,200)==0&&d.first==16&&d.count==32);
 kt_term_dirty_rows_reset(&d);assert(kt_term_dirty_rows_add_cell(&d,3,8,29)==0&&d.first==24&&d.count==5);
 kt_term_dirty_rows_reset(&d);assert(kt_term_dirty_rows_add_cell(&d,4,8,29)==-1&&!d.valid);
 assert(kt_term_dirty_rows_add_cell(&d,0,0,29)==-1);
 return 0;
}
