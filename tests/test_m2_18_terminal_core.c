#include "kt/terminal.h"

#include <assert.h>
#include <stddef.h>
#include <stdint.h>

typedef struct fixture {
    uint8_t cells[16];
    size_t count;
    unsigned cr, lf, bs, bell;
} fixture;

static void put_cell(void *ctx, uint8_t ch, const kt_term_attr *attr)
{
    fixture *f = (fixture *)ctx;
    assert(attr != NULL);
    assert(attr->fg == 7u);
    assert(attr->bg == 0u);
    assert(attr->flags == 0u);
    assert(f->count < sizeof f->cells);
    f->cells[f->count++] = ch;
}
static void cr(void *ctx) { ((fixture *)ctx)->cr++; }
static void lf(void *ctx) { ((fixture *)ctx)->lf++; }
static void bs(void *ctx) { ((fixture *)ctx)->bs++; }
static void bell(void *ctx) { ((fixture *)ctx)->bell++; }

int main(void)
{
    static const kt_term_ops ops = { put_cell, cr, lf, bs, bell };
    static const uint8_t input[] = { 'K', 'T', '\r', '\n', '\b', '\a' };
    fixture f = {0};
    kt_term term;

    kt_term_init(&term, &ops, &f);
    assert(term.attr.fg == 7u);
    assert(term.attr.bg == 0u);
    assert(term.attr.flags == 0u);
    assert(term.parser_state == 0u);

    kt_term_feed(&term, input, sizeof input);
    assert(f.count == 2u);
    assert(f.cells[0] == 'K' && f.cells[1] == 'T');
    assert(f.cr == 1u && f.lf == 1u && f.bs == 1u && f.bell == 1u);

    term.attr.fg = 1u;
    term.attr.flags = KT_TERM_ATTR_BOLD;
    term.parser_state = 9u;
    kt_term_reset(&term);
    assert(term.attr.fg == 7u);
    assert(term.attr.bg == 0u);
    assert(term.attr.flags == 0u);
    assert(term.parser_state == 0u);

    kt_term_feed(NULL, input, sizeof input);
    kt_term_feed(&term, NULL, sizeof input);
    return 0;
}
