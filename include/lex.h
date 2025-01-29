#ifndef P6K14_LEX_H
#define P6K14_LEX_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PL9_TK_Let       = 0,
    PL9_TK_Native    = 1,
    PL9_TK_Rec       = 2,
    PL9_TK_In        = 3,
    PL9_TK_If        = 4,
    PL9_TK_Then      = 5,
    PL9_TK_Else      = 6,
    PL9_TK_Loop      = 7,
    PL9_TK_Break     = 8,
    PL9_TK_Continue  = 9,
    PL9_TK_Return    = 10,

    PL9_TK_And       = 11,
    PL9_TK_Or        = 12,
    PL9_TK_Not       = 13,
    PL9_TK_Is        = 14,
    PL9_TK_True      = 15,
    PL9_TK_False     = 16,
    PL9_TK_Nil       = 17,

    PL9_TK_LParen    = 18,
    PL9_TK_RParen    = 19,
    PL9_TK_Arrow     = 20,
    PL9_TK_Comma     = 21,
    PL9_TK_Lambda    = 22,
    PL9_TK_DColon    = 23,
    PL9_TK_Eq        = 24,
    PL9_TK_ColonEq   = 25,
    PL9_TK_Lt        = 26,
    PL9_TK_Gt        = 27,
    PL9_TK_LtEq      = 28,
    PL9_TK_GtEq      = 29,
    PL9_TK_DEq       = 30,
    PL9_TK_NEq       = 31,
    PL9_TK_Plus      = 32,
    PL9_TK_Minus     = 33,
    PL9_TK_Mul       = 34,
    PL9_TK_Div       = 35,
    PL9_TK_Mod       = 36,
    PL9_TK_Tilde     = 37,
    PL9_TK_Exclaim   = 38,
    PL9_TK_Semicolon = 39,

    PL9_TK_Int       = 40,
    PL9_TK_Float     = 41,
    PL9_TK_Char      = 42,
    PL9_TK_String    = 43,

    PL9_TK_EOF       = 44,
    PL9_TK_Absent    = -1
} PL9TokenKind;

typedef struct {
    PL9TokenKind k;
    unsigned row;
    unsigned col;
    const char *valbeg;
    const char *valend;
} PL9Token;

typedef struct stPL9Lexer PL9Lexer;

PL9Lexer *pl9_MakeLexer(char const *fname, char const *src);
void pl9_FreeLexer(PL9Lexer *lex);

PL9Token pl9_NextToken(PL9Lexer *lex);
PL9Token pl9_PeekToken(PL9Lexer *lex);

#ifdef __cplusplus
}; /* extern "C" */
#endif

#endif /* P6K14_LEX_H */
