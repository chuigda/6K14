#include "j.h"

char const* pl9_ExplainGreek(PL9Greek greek) {
    static const char* greeks[] = {
        "α", "β", "γ", "δ", "ε",
        "π", "σ", "τ", "η"
    };

    return greeks[greek];
}

