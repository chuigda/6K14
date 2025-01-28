#ifndef P6K14_TYPE_H
#define P6K14_TYPE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PL9_G_Alpha   = 0,
    PL9_G_Beta    = 1,
    PL9_G_Gamma   = 2,
    PL9_G_Delta   = 3,
    PL9_G_Epsilon = 4,

    PL9_G_Pi    = 5,
    PL9_G_Sigma = 6,
    PL9_G_Tau   = 7,
    PL9_G_Eta   = 8
} PL9Greek;

char const* pl9_ExplainGreek(PL9Greek greek);

typedef enum {
    PL9_TypeVar,
    PL9_TypeOp
} PL9TypeKind;

typedef enum {
    PL9_TypeOp_Int      = 0,
    PL9_TypeOp_Float    = 1,
    PL9_TypeOp_Bool     = 2,
    PL9_TypeOp_Char     = 3,
    PL9_TypeOp_String   = 4,
    PL9_TypeOp_Product  = 5,
    PL9_TypeOp_Function = 6
} PL9TypeOperator;

const char *pl9_ExplainTypeOperator(PL9TypeOperator op);

typedef struct stPL9TypeBase {
    uint32_t k;
} PL9Type;

typedef struct stPL9TypeVar {
    uint32_t k;

    PL9Greek greek;
    uint32_t tstamp;
    PL9Type *resolv;
} PL9TypeVar;

typedef struct stPL9TypeOp {
    uint32_t k;

    PL9TypeOperator op;
    uint32_t argc;
    PL9Type *args[];
} PL9TypeOp;

int pl9_TypePrintf(FILE *fp, PL9Type *ty);
int pl9_TypeSprintf(char *buf, PL9Type *ty);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* P6K14_TYPE_H */
