#ifndef P6K14_DIAG_H
#define P6K14_DIAG_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct stPL9DiagContext PL9DiagContext;
typedef struct stPL9Diag PL9Diag;
typedef struct stPL9DiagItem PL9DiagItem;

struct stPL9DiagContext {
    PL9Diag *chain;
    bool haserr;
};

struct stPL9Diag {
    PL9Diag *next;

    bool iserr;
    PL9DiagItem *item;
    PL9DiagItem *tail;
};

struct stPL9DiagItem {
    PL9DiagItem *next;
    uint32_t line;
    uint32_t col;
    char text[];
};

PL9DiagContext *pl9_MakeDiagContext(void);
void pl9_FreeDiagContext(PL9DiagContext *ctx);

PL9Diag *pl9_Diag(PL9DiagContext *ctx,
                  uint32_t line,
                  uint32_t col,
                  const char *fmt,
                  ...);
void pl9_DiagAppend(PL9Diag *diag,
                    uint32_t line,
                    uint32_t col,
                    const char *fmt,
                    ...);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* P6K14_DIAG_H */
