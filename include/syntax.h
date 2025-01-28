#ifndef P6K14_SYNTAX_H
#define P6K14_SYNTAX_H

#include "lex.h"
#include "type.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PL9_SYNTAX_NODE_COMMON \
    struct stPL9SyntaxNode *next; \
    int k;

typedef struct stPL9SyntaxNode {
    PL9_SYNTAX_NODE_COMMON
} PL9SyntaxNode, PL9Program;

typedef enum {
    PL9_SK_LetDeclOnly = 0,
    PL9_SK_LetDef = 1,

    PL9_SK_LetIn = 2,
    PL9_SK_Abs = 3,
    PL9_SK_If = 4,
    PL9_SK_Loop = 5,
    PL9_SK_Break = 6,
    PL9_SK_Continue = 7,
    PL9_SK_Return = 8,
    PL9_SK_Assign = 9,
    PL9_SK_Binary = 10,
    PL9_SK_Unary = 11,
    PL9_SK_Call = 12,
    PL9_SK_Ident = 13,
    PL9_SK_Literal = 14,

    PL9_SK_TypeVar = 15,
    PL9_SK_TypeOp = 16,
    PL9_SK_UnitType = 17,
    PL9_SK_FuncType = 18
} PL9SyntaxNodeKind;

typedef struct stPL9LetDeclNode PL9LetDeclNode;
    typedef struct stPL9LetDeclOnlyNode PL9LetDeclOnlyNode;
    typedef struct stPL9LetDefNode PL9LetDefNode;
typedef struct stPL9ExprNode PL9ExprNode;
    typedef struct stPL9LetInNode PL9LetInNode;
    typedef struct stPL9AbsNode PL9AbsNode;
    typedef struct stPL9IfNode PL9IfNode;
    typedef struct stPL9LoopNode PL9LoopNode;
    typedef struct stPL9BreakNode PL9BreakNode;
    typedef struct stPL9ContinueNode PL9ContinueNode;
    typedef struct stPL9ReturnNode PL9ReturnNode;
    typedef struct stPL9AssignNode PL9AssignNode;
    typedef struct stPL9BinaryNode PL9BinaryNode;
    typedef struct stPL9UnaryNode PL9UnaryNode;
    typedef struct stPL9CallNode PL9CallNode;
    typedef struct stPL9IdentNode PL9IdentNode;
    typedef struct stPL9LiteralNode PL9LiteralNode;
typedef struct stPL9TypeNode PL9TypeNode;
    typedef struct stPL9TypeVarNode PL9TypeVarNode;
    typedef struct stPL9TypeOpNode PL9TypeOpNode;
    typedef struct stPL9UnitTypeNode PL9UnitTypeNode;
    typedef struct stPL9FuncTypeNode PL9FuncTypeNode;

#define PL9_LET_DECL_NODE_COMMON \
    PL9_SYNTAX_NODE_COMMON \
    PL9Token let; \
    PL9Token ident;

struct stPL9LetDeclNode {
    PL9_LET_DECL_NODE_COMMON
};

struct stPL9LetDeclOnlyNode {
    PL9_LET_DECL_NODE_COMMON

    PL9Token native;
    PL9Token dcolon;
    PL9TypeNode *tnode;
};

struct stPL9LetDefNode {
    PL9_LET_DECL_NODE_COMMON

    PL9Token eq;
    PL9ExprNode *expr;
};

struct stPL9TypeVarNode {
    PL9_SYNTAX_NODE_COMMON

    PL9Token ident;
};

struct stPL9TypeOpNode {
    PL9_SYNTAX_NODE_COMMON

    PL9Token op;
    PL9Token lt;
    PL9TypeNode *tlist;
    PL9Token gt;
};

struct stPL9UnitTypeNode {
    PL9_SYNTAX_NODE_COMMON

    PL9Token lparen;
    PL9Token rparen;
};

struct stPL9FuncTypeNode {
    PL9_SYNTAX_NODE_COMMON

    PL9Token lparen;
    PL9TypeNode *tlist;
    PL9Token rparen;
    PL9Token arrow;
    PL9TypeNode *tret;
};

#define PL9_EXPR_NODE_COMMON \
    PL9_SYNTAX_NODE_COMMON \
    PL9Type *infer;

struct stPL9ExprNode {
    PL9_EXPR_NODE_COMMON
};

typedef struct stPL9BindingNode {
    struct stPL9BindingNode *next;

    PL9Token ident;
    PL9Token dcolon;
    PL9TypeNode *tnode;
    PL9Token eq;
    PL9ExprNode *expr;
} PL9BindingNode;

struct stPL9LetInNode {
    PL9_EXPR_NODE_COMMON

    PL9Token let;
    PL9Token rec;
    PL9BindingNode *bdlist;
    PL9Token in;
    PL9ExprNode *expr;
};

typedef struct stPL9FnArgNode {
    struct stPL9FnArgNode *next;

    PL9Token ident;
    PL9Token dcolon;
    PL9TypeNode *tnode;
} PL9FnArgNode;

struct stPL9AbsNode {
    PL9_EXPR_NODE_COMMON

    PL9Token lambda;
    PL9FnArgNode *arglist;
    PL9Token arrow;
    PL9ExprNode *expr;
};

struct stPL9IfNode {
    PL9_EXPR_NODE_COMMON

    PL9Token iftok;
    PL9ExprNode *e1;
    PL9Token thentok;
    PL9ExprNode *e2;
    PL9Token elsetok;
    PL9ExprNode *e3;
};

struct stPL9LoopNode {
    PL9_EXPR_NODE_COMMON

    PL9Token looptok;
    PL9ExprNode *expr;
};

struct stPL9BreakNode {
    PL9_EXPR_NODE_COMMON

    PL9Token breaktok;
    PL9ExprNode *expr;
};

struct stPL9ContinueNode {
    PL9_EXPR_NODE_COMMON

    PL9Token continuetok;
};

struct stPL9ReturnNode {
    PL9_EXPR_NODE_COMMON

    PL9Token returntok;
    PL9ExprNode *expr;
};

struct stPL9AssignNode {
    PL9_EXPR_NODE_COMMON

    PL9Token ident;
    PL9Token coloneq;
    PL9ExprNode *expr;
};

struct stPL9BinaryNode {
    PL9_EXPR_NODE_COMMON

    PL9Token op;
    PL9ExprNode *e1;
    PL9ExprNode *e2;
};

struct stPL9UnaryNode {
    PL9_EXPR_NODE_COMMON

    PL9Token op;
    PL9ExprNode *expr;
};

struct stPL9CallNode {
    PL9_EXPR_NODE_COMMON

    PL9ExprNode *fn;
    PL9ExprNode *args;
};

struct stPL9IdentNode {
    PL9_EXPR_NODE_COMMON

    PL9Token ident;
};

struct stPL9LiteralNode {
    PL9_EXPR_NODE_COMMON

    PL9Token lit;
};

PL9Program *pl9_ParseProgram(PL9Token *toks);
void pl9_FreeProgram(PL9Program *prog);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* P6K14_SYNTAX_H */
