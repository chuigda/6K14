#include "diag.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

PL9DiagContext *pl9_MakeDiagContext(void) {
    PL9DiagContext *ctx = malloc(sizeof(PL9DiagContext));
    ctx->chain = NULL;
    ctx->haserr = false;
    return ctx;
}

void pl9_FreeDiagContext(PL9DiagContext *ctx) {
    PL9Diag *diag = ctx->chain;
    while (diag) {
        PL9Diag *next = diag->next;
        PL9DiagItem *item = diag->item;
        while (item) {
            PL9DiagItem *next = item->next;
            free(item);
            item = next;
        }
        free(diag);
        diag = next;
    }
    free(ctx);
}

PL9Diag *pl9_Diag(PL9DiagContext *ctx,
                  uint32_t line,
                  uint32_t col,
                  const char *fmt,
                  ...) {
    PL9Diag *diag = malloc(sizeof(PL9Diag));
    diag->next = ctx->chain;
    ctx->chain = diag;
    diag->iserr = false;
    diag->item = NULL;
    diag->tail = NULL;

    va_list ap;
    va_start(ap, fmt);
    pl9_DiagAppend(diag, line, col, fmt, ap);
    va_end(ap);

    return diag;
}

void pl9_DiagAppend(PL9Diag *diag,
                    uint32_t line,
                    uint32_t col,
                    const char *fmt,
                    ...) {
    PL9DiagItem *item = malloc(sizeof(PL9DiagItem));
    item->next = NULL;
    item->line = line;
    item->col = col;
    diag->iserr = true;

    va_list ap;
    va_start(ap, fmt);
    vsnprintf(item->text, 0, fmt, ap);
    va_end(ap);

    if (diag->tail) {
        diag->tail->next = item;
    } else {
        diag->item = item;
    }
    diag->tail = item;
}
