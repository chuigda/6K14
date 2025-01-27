#ifndef P6K14_J_H
#define P6K14_J_H

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

typedef struct stPL9TypeEnv PL9TypeEnv;

typedef enum {
    PL9_TEnv_Global = 0,
    PL9_TEnv_Lambda = 1,
    PL9_TEnv_Let    = 2,
    PL9_TEnv_Loop   = 3
} PL9TypeEnvKind;

PL9TypeEnv *pl9_MakeTypeEnv(PL9TypeEnvKind k, size_t szHint);
PL9TypeEnv *pl9_DeriveTypeEnv(PL9TypeEnv *env,
                              PL9TypeEnvKind k,
                              size_t szHint);
void pl9_FreeTypeEnv(PL9TypeEnv *env);

PL9TypeVar *pl9_FreshTypeVar(PL9TypeEnv *env, PL9Greek greek);
PL9TypeOp *pl9_MakeTypeOp(PL9TypeEnv *env,PL9TypeOperator op, uint32_t argc);
PL9TypeOp *pl9_MakeTypeOpL(PL9TypeEnv *env,
                           PL9TypeOperator op,
                           uint32_t argc,
                           ...);
PL9TypeOp *pl9_MakeTypeOpV(PL9TypeEnv *env,
                           PL9TypeOperator op,
                           uint32_t argc,
                           PL9Type *args[]);
bool pl9_DefVar(PL9TypeEnv *env, char const *var, PL9Type *ty);

int pl9_TypePrintf(FILE *fp, PL9Type *ty);
int pl9_TypeSprintf(char *buf, PL9Type *ty);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* P6K14_J_H */
