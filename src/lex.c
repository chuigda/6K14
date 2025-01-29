#include "lex.h"

#include <ctype.h>
#include <stdbool.h>
#include <string.h>

struct stPL9Lexer {
    char const *fname;
    char const *src;

    char const *cursor;
    uint32_t line;
    uint32_t col;

    PL9Token peek;
};

static bool MaybeSkipWhitespace(PL9Lexer *lex);
static bool MaybeSkipComment(PL9Lexer *lex);

static bool IsIdentChar(char c);
static PL9Token TokeniseNumber(PL9Lexer *lex);
static PL9Token TokeniseIdent(PL9Lexer *lex);
static PL9Token TokeniseString(PL9Lexer *lex);
static PL9Token TokeniseSym(PL9Lexer *lex);

PL9Token pl9_NextToken(PL9Lexer *lex) {
    if (lex->peek.k != PL9_TK_Absent) {
        PL9Token ret = lex->peek;
        lex->peek.k = PL9_TK_Absent;
        return ret;
    }

    while (MaybeSkipWhitespace(lex) || MaybeSkipComment(lex)) {;}

    if (*lex->cursor == '\0') {
        return (PL9Token) { PL9_TK_EOI };
    }

    if (isdigit(*lex->cursor)) {
        return TokeniseNumber(lex);
    } else if (isalpha(*lex->cursor)) {
        return TokeniseIdent(lex);
    } else if (*lex->cursor == '"') {
        return TokeniseString(lex);
    } else {
        return TokeniseSym(lex);
    }
}

static bool MaybeSkipWhitespace(PL9Lexer *lex) {
    bool skipped = false;
    while (isspace(*lex->cursor)) {
        if (*lex->cursor == '\n') {
            lex->line++;
            lex->col = 0;
        }
        lex->cursor++;
        lex->col++;
        skipped = true;
    }
    return skipped;
}

static bool MaybeSkipComment(PL9Lexer *lex) {
    if (lex->cursor[0] == '(' && lex->cursor[1] == '*') {
        lex->cursor += 2;
        lex->col += 2;

        while (lex->cursor[0] != '\0') {
            if (lex->cursor[0] == '*' && lex->cursor[1] == ')') {
                lex->cursor += 2;
                lex->col += 2;
                return true;
            }

            if (*lex->cursor == '\n') {
                lex->line++;
                lex->col = 0;
            }

            lex->cursor++;
            lex->col++;
        }
    }

    return false;
}

static PL9Token TokeniseNumber(PL9Lexer *lex) {
    const char *beg = lex->cursor;
    uint32_t line = lex->line;
    uint32_t col = lex->col;

    while (isdigit(*lex->cursor)) {
        lex->cursor++;
        lex->col++;
    }

    if (*lex->cursor == '.') {
        lex->cursor++;
        lex->col++;

        while (isdigit(*lex->cursor)) {
            lex->cursor++;
            lex->col++;
        }

        return (PL9Token) { PL9_TK_Float, line, col, beg, lex->cursor };
    }

    return (PL9Token) { PL9_TK_Int, line, col, beg, lex->cursor };
}

static PL9Token TokeniseIdent(PL9Lexer *lex) {
    const char *beg = lex->cursor;
    uint32_t row = lex->line;
    uint32_t col = lex->col;

    while (IsIdentChar(*lex->cursor)) {
        lex->cursor++;
        lex->col++;
    }

    const char *end = lex->cursor;
    PL9TokenKind k = PL9_TK_Ident;

    if (end - beg == 2) {
        if (strncmp(beg, "is", 2) == 0) {
            k = PL9_TK_Is;
        } else if (strncmp(beg, "if", 2) == 0) {
            k = PL9_TK_If;
        } else if (strncmp(beg, "or", 2) == 0) {
            k = PL9_TK_Or;
        }
    } else if (end - beg == 3) {
        if (strncmp(beg, "and", 3) == 0) {
            k = PL9_TK_And;
        } else if (strncmp(beg, "not", 3) == 0) {
            k = PL9_TK_Not;
        } else if (strncmp(beg, "let", 3) == 0) {
            k = PL9_TK_Let;
        } else if (strncmp(beg, "rec", 3) == 0) {
            k = PL9_TK_Rec;
        } else if (strncmp(beg, "nil", 3) == 0) {
            k = PL9_TK_Nil;
        }
    } else if (end - beg == 4) {
        if (strncmp(beg, "true", 4) == 0) {
            k = PL9_TK_True;
        } else if (strncmp(beg, "else", 4) == 0) {
            k = PL9_TK_Else;
        } else if (strncmp(beg, "then", 4) == 0) {
            k = PL9_TK_Then;
        } else if (strncmp(beg, "loop", 4) == 0) {
            k = PL9_TK_Loop;
        }
    } else if (end - beg == 5) {
        if (strncmp(beg, "false", 5) == 0) {
            k = PL9_TK_False;
        } else if (strncmp(beg, "break", 5) == 0) {
            k = PL9_TK_Break;
        }
    } else if (end - beg == 6) {
        if (strncmp(beg, "return", 6) == 0) {
            k = PL9_TK_Return;
        } else if (strncmp(beg, "native", 6) == 0) {
            k = PL9_TK_Native;
        }
    } else if (end - beg == 7) {
        if (strncmp(beg, "continue", 7) == 0) {
            k = PL9_TK_Continue;
        }
    }

    return (PL9Token) { k, row, col, beg, end };
}

static PL9Token TokeniseString(PL9Lexer *lex) {
    const char *beg = lex->cursor;
    uint32_t row = lex->line;
    uint32_t col = lex->col;

    lex->cursor++;
    lex->col++;

    while (*lex->cursor != '"') {
        if (*lex->cursor == '\0') {
            return (PL9Token) { PL9_TK_Absent };
        }

        if (*lex->cursor == '\n') {
            lex->line++;
            lex->col = 0;
        }

        lex->cursor++;
        lex->col++;
    }

    lex->cursor++;
    lex->col++;

    return (PL9Token) { PL9_TK_String, row, col, beg, lex->cursor };
}

static bool IsIdentChar(char c) {
    return isalnum(c) || c == '_' || c == '-';
}

static PL9Token TokeniseSym(PL9Lexer *lex) {
    const char *beg = lex->cursor;
    uint32_t row = lex->line;
    uint32_t col = lex->col;

    switch (*lex->cursor) {
        case '(':
            lex->cursor++;
            lex->col++;
            return (PL9Token) { PL9_TK_LParen, row, col, beg, lex->cursor };
        case ')':
            lex->cursor++;
            lex->col++;
            return (PL9Token) { PL9_TK_RParen, row, col, beg, lex->cursor };
        case ',':
            lex->cursor++;
            lex->col++;
            return (PL9Token) { PL9_TK_Comma, row, col, beg, lex->cursor };
        case '\\':
            lex->cursor++;
            lex->col++;
            return (PL9Token) { PL9_TK_Backslash, row, col, beg, lex->cursor };
        case ':':
            if (lex->cursor[1] == '=') {
                lex->cursor += 2;
                lex->col += 2;
                return (PL9Token) { PL9_TK_ColonEq, row, col, beg, lex->cursor };
            } else {
                lex->cursor++;
                lex->col++;
                return (PL9Token) { PL9_TK_DColon, row, col, beg, lex->cursor };
            }
        case '=':
            lex->cursor++;
            lex->col++;
            return (PL9Token) { PL9_TK_Eq, row, col, beg, lex->cursor };
        case '<':
            if (lex->cursor[1] == '=') {
                lex->cursor += 2;
                lex->col += 2;
                return (PL9Token) { PL9_TK_LtEq, row, col, beg, lex->cursor };
            } else if (lex->cursor[1] == '>') {
                lex->cursor += 2;
                lex->col += 2;
                return (PL9Token) { PL9_TK_NEq, row, col, beg, lex->cursor };
            } else {
                lex->cursor++;
                lex->col++;
                return (PL9Token) { PL9_TK_Lt, row, col, beg, lex->cursor };
            }
        case '>':
            if (lex->cursor[1] == '=') {
                lex->cursor += 2;
                lex->col += 2;
                return (PL9Token) { PL9_TK_GtEq, row, col, beg, lex->cursor };
            } else {
                lex->cursor++;
                lex->col++;
                return (PL9Token) { PL9_TK_Gt, row, col, beg, lex->cursor };
            }
        case '+':
            lex->cursor++;
            lex->col++;
            return (PL9Token) { PL9_TK_Plus, row, col, beg, lex->cursor };
        case '-':
            lex->cursor++;
            lex->col++;
            return (PL9Token) { PL9_TK_Minus, row, col, beg, lex->cursor };
        case '*':
            lex->cursor++;
            lex->col++;
            return (PL9Token) { PL9_TK_Mul, row, col, beg, lex->cursor };
        case '/':
            lex->cursor++;
            lex->col++;
            return (PL9Token) { PL9_TK_Div, row, col, beg, lex->cursor };
        case '%':
            lex->cursor++;
            lex->col++;
            return (PL9Token) { PL9_TK_Mod, row, col, beg, lex->cursor };
        case '~':
            lex->cursor++;
            lex->col++;
            return (PL9Token) { PL9_TK_Tilde, row, col, beg, lex->cursor };
        case '!':
            if (lex->cursor[1] == '=') {
                lex->cursor += 2;
                lex->col += 2;
                return (PL9Token) { PL9_TK_NEq, row, col, beg, lex->cursor };
            } else {
                lex->cursor++;
                lex->col++;
                return (PL9Token) { PL9_TK_Exclaim, row, col, beg, lex->cursor };
            }
        case ';':
            lex->cursor++;
            lex->col++;
            return (PL9Token) { PL9_TK_Semicolon, row, col, beg, lex->cursor };
        default:
            return (PL9Token) { PL9_TK_Absent };
    }
}

