#include <stdio.h>
#include "lex.h"

static char const src[] =
    "let native print :: String -> ()\n"
    "let fib :: (Int) -> Int\n"
    "let fib =\\n -> \n"
    "    if n < 2 then\n"
    "        n\n"
    "    else\n"
    "        fib(n - 1) + fib(n - 2)\n"
    "\n"
    "let main = \\ -> print(fib(10))\n";

int main() {
    PL9Lexer *lexer = pl9_MakeLexer("main.ml", src);
    PL9Token tok;

    do {
        tok = pl9_NextToken(lexer);
        printf(
            "Token: %s, content: %.*s\n",
            pl9_ExplainTokenKind(tok.k),
            (int)(tok.valend - tok.valbeg),
            tok.valbeg
        );
    } while (tok.k != PL9_TK_EOI && tok.k != PL9_TK_Absent);

    if (tok.k == PL9_TK_Absent) {
        printf("Error: unexpected token\n");
    }
    pl9_FreeLexer(lexer);
}
