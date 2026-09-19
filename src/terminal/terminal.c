#include "kt/terminal.h"

static void kt_term_default_attr(kt_term_attr *attr)
{
    attr->fg = 7u;
    attr->bg = 0u;
    attr->flags = 0u;
}

void kt_term_reset(kt_term *term)
{
    if (term == NULL) {
        return;
    }

    kt_term_default_attr(&term->attr);
    term->parser_state = 0u;
}

void kt_term_init(kt_term *term, const kt_term_ops *ops, void *ctx)
{
    if (term == NULL) {
        return;
    }

    term->ops = ops;
    term->ctx = ctx;
    kt_term_reset(term);
}

void kt_term_feed(kt_term *term, const uint8_t *data, size_t len)
{
    size_t i;

    if (term == NULL || data == NULL || term->ops == NULL) {
        return;
    }

    for (i = 0; i < len; ++i) {
        const uint8_t ch = data[i];

        switch (ch) {
        case '\r':
            if (term->ops->carriage_return != NULL)
                term->ops->carriage_return(term->ctx);
            break;
        case '\n':
            if (term->ops->line_feed != NULL)
                term->ops->line_feed(term->ctx);
            break;
        case '\b':
            if (term->ops->backspace != NULL)
                term->ops->backspace(term->ctx);
            break;
        case '\a':
            if (term->ops->bell != NULL)
                term->ops->bell(term->ctx);
            break;
        default:
            if (ch >= 0x20u && term->ops->put_cell != NULL)
                term->ops->put_cell(term->ctx, ch, &term->attr);
            break;
        }
    }
}
