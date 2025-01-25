#include "j.h"

char const* pl9_ExplainGreek(PL9Greek greek) {
    static const char* greeks[] = {
        "α", "β", "γ", "δ", "ε",
        "π", "σ", "τ", "η"
    };
    return greeks[greek];
}

char const *pl9_ExplainTypeOperator(PL9TypeOperator op) {
    static const char *ops[] = {
        "int", "float", "bool", "char", "string",
        "product", "function"
    };
    return ops[op];
}

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
    PL9TypeEnvHint hint;

    TypeListNode_TypeVar *nglist;
    TypeListNode_TypeVar *nglast;
    void *vars;
} PL9TypeEnv;
