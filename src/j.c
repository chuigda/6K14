#include "j.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "map.h"

typedef struct stTypeListNode {
    struct stTypeListNode *next;
} TypeListNode;

typedef struct stTypeListNode_TypeOp {
    TypeListNode *next;
    PL9TypeOp ty;
} TypeListNode_TypeOp;

typedef struct stTypeListNode_TypeVar {
    TypeListNode *next;
    PL9TypeVar ty;
} TypeListNode_TypeVar;

typedef struct stTypeEnvGlobal {
    uint64_t tstamps[9];
    TypeListNode *tlist;
    TypeListNode *tlast;
} TypeEnvGlobal;

typedef struct stPL9TypeEnv {
    struct stPL9TypeEnv *parent;
    TypeEnvGlobal *glob;

    PL9TypeEnvKind k;
    bool sized;

    TypeListNode_TypeVar *nglist;
    TypeListNode_TypeVar *nglast;
    void *vars;
} PL9TypeEnv;

static PL9TypeVar *FreshTypeVar(PL9TypeEnv *env, PL9Greek greek);
static PL9TypeOp *MakeTypeOp(PL9TypeEnv *env,
                             PL9TypeOperator op,
                             uint32_t argc);
static PL9TypeOp *MakeTypeOpL(PL9TypeEnv *env,
                              PL9TypeOperator op,
                              uint32_t argc,
                              ...);
static PL9TypeOp *MakeTypeOpV(PL9TypeEnv *env,
                                  PL9TypeOperator op,
                                  uint32_t argc,
                                  PL9Type *args[]);
static bool DefVar(PL9TypeEnv *env, char const *var, PL9Type *ty);
static PL9TypeEnv *MakeTypeEnvWithoutGlobal(PL9TypeEnvKind k, size_t szHint);
static PL9TypeOp *MakeTypeOpUninit(PL9TypeEnv *env,
                                   PL9TypeOperator op,
                                   uint32_t argc);
static void EnqueueGlobalTypeList(PL9TypeEnv *env, TypeListNode *node);

PL9TypeEnv *pl9_MakeTypeEnv(PL9TypeEnvKind k, size_t szHint) {
    PL9TypeEnv *ret = MakeTypeEnvWithoutGlobal(k, szHint);
    ret->glob = malloc(sizeof(TypeEnvGlobal));
    memset(ret->glob, 0, sizeof(TypeEnvGlobal));
    return ret;
}

PL9TypeEnv *pl9_DeriveTypeEnv(PL9TypeEnv *env, PL9TypeEnvKind k, size_t szHint) {
    PL9TypeEnv *ret = MakeTypeEnvWithoutGlobal(k, szHint);
    ret->parent = env;
    ret->glob = env->glob;
    return ret;
}

static PL9TypeEnv *MakeTypeEnvWithoutGlobal(PL9TypeEnvKind k, size_t szHint) {
    PL9TypeEnv *ret = malloc(sizeof(PL9TypeEnv));
    ret->parent = NULL;
    ret->glob = NULL;
    ret->k = k;
    ret->sized = szHint != SIZE_MAX;
    ret->nglist = NULL;
    ret->nglast = NULL;
    if (szHint != SIZE_MAX) {
        ret->vars = MakeVecMap(szHint);
    } else {
        ret->vars = MakeHashMap();
    }
    return ret;
}

void pl9_FreeTypeEnv(PL9TypeEnv *env) {
    if (!env->parent) {
        for (TypeListNode *iter = env->glob->tlist; iter;) {
            TypeListNode *next = iter->next;
            free(iter);
            iter = next;
        }
    }

    if (env->sized) {
        FreeVecMap(env->vars, free);
    } else {
        FreeHashMap(env->vars, free);
    }
}

PL9TypeVar *FreshTypeVar(PL9TypeEnv *env, PL9Greek greek) {
    TypeListNode_TypeVar *node = malloc(sizeof(TypeListNode_TypeVar));
    node->next = NULL;
    if (env->glob->tlist) {
        env->glob->tlast->next = (TypeListNode*) node;
        env->glob->tlast = (TypeListNode*) node;
    } else {
        env->glob->tlist = (TypeListNode*) node;
        env->glob->tlast = (TypeListNode*) node;
    }

    PL9TypeVar *ret = &node->ty;
    ret->greek = greek;
    ret->tstamp = env->glob->tstamps[greek]++;
    ret->resolv = NULL;
    return ret;
}

static PL9TypeOp *MakeTypeOp(PL9TypeEnv *env,
                             PL9TypeOperator op,
                             uint32_t argc) {
    PL9TypeOp *ret = MakeTypeOpUninit(env, op, argc);
    memset(ret->args, 0, argc * sizeof(PL9Type*));
    return ret;
}

static PL9TypeOp *MakeTypeOpL(PL9TypeEnv *env,
                              PL9TypeOperator op,
                              uint32_t argc,
                              ...) {
    PL9TypeOp *ret = MakeTypeOpUninit(env, op, argc);
    va_list ap;
    va_start(ap, argc);
    for (uint32_t i = 0; i < argc; i++) {
        ret->args[i] = va_arg(ap, PL9Type*);
    }
    va_end(ap);
    return ret;
}

static PL9TypeOp *MakeTypeOpV(PL9TypeEnv *env,
                              PL9TypeOperator op,
                              uint32_t argc,
                              PL9Type **args) {
    PL9TypeOp *ret = MakeTypeOpUninit(env, op, argc);
    memcpy(ret->args, args, argc * sizeof(PL9Type*));
    return ret;
}

static PL9TypeOp *MakeTypeOpUninit(PL9TypeEnv *env,
                                   PL9TypeOperator op,
                                   uint32_t argc) {
    TypeListNode_TypeOp *node =
        malloc(sizeof(TypeListNode_TypeOp) + argc * sizeof(PL9Type*));
    node->next = NULL;
    EnqueueGlobalTypeList(env, (TypeListNode*) node);

    PL9TypeOp *ret = &node->ty;
    ret->op = op;
    return ret;
}

static void EnqueueGlobalTypeList(PL9TypeEnv *env, TypeListNode *node) {
    if (env->glob->tlist) {
        env->glob->tlast->next = node;
        env->glob->tlast = node;
    } else {
        env->glob->tlist = node;
        env->glob->tlast = node;
    }
}
