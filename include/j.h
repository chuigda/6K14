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

void pl9_Infer(PL9TypeEnv *env, PL9Program *program);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* P6K14_J_H */
