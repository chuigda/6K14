#include "type.h"

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
