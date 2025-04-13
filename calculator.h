#include <ctype.h>
#include <stdlib.h>

typedef enum {
    TOKEN_NUMBER,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_END
} TokenType;

typedef struct {
    TokenType type;
    int value;  // Used only if type == TOKEN_NUMBER
    char op;    // Used only for operators
} Token;

typedef struct TokenList {
    Token *tokens;
    size_t length;
    size_t capacity;
} TokenList;

typedef struct AstNode {
    Token token;
    struct AstNode *left;
    struct AstNode *right;
} AstNode;

TokenList tokenize(const char *input);
AstNode *parse_expression(TokenList *tokens, size_t *current);
AstNode *parse_term(TokenList *tokens, size_t *current);
AstNode *parse_factor(TokenList *tokens, size_t *current);
long eval(AstNode *node);
void print_ast(AstNode *node, int depth);
void free_ast(AstNode *node);
