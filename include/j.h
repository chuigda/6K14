#ifndef P6K14_J_H
#define P6K14_J_H

#include "syntax.h"
#include "type.h"

#ifdef __cplusplus
extern "C" {
#endif

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

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* P6K14_J_H */
