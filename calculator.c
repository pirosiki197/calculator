#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "calculator.h"

// <expr> ::= <term> { ("+" | "-") <term> } ;
// <term> ::= <factor> { ("*" | "/") <factor> } ;
// <factor> ::= <number> | "(" <expr> ")" | "-" <factor> ;
// <number> ::= <digit> { <digit> } ;
// <digit> ::= "0" .. "9" ;

TokenList tokenize(const char *input) {
    TokenList list = {malloc(16 * sizeof(Token)), 0, 16};
    if (!list.tokens) exit(1);

    size_t i = 0;
    while (input[i]) {
        if (isspace(input[i])) {
            i++;
            continue;
        }

        if (isdigit(input[i])) {
            int num = 0;
            while (isdigit(input[i])) {
                num = num * 10 + (input[i] - '0');
                i++;
            }
            Token token = {TOKEN_NUMBER, num, 0};
            list.tokens[list.length++] = token;
        } else {
            Token token;
            switch (input[i]) {
                case '+': token = (Token){TOKEN_PLUS, 0, '+'}; break;
                case '-': token = (Token){TOKEN_MINUS, 0, '-'}; break;
                case '*': token = (Token){TOKEN_MULTIPLY, 0, '*'}; break;
                case '/': token = (Token){TOKEN_DIVIDE, 0, '/'}; break;
                case '(': token = (Token){TOKEN_LPAREN, 0, '('}; break;
                case ')': token = (Token){TOKEN_RPAREN, 0, ')'}; break;
                default:
                    printf("Unknown character: %c\n", input[i]);
                    exit(1);
            }
            list.tokens[list.length++] = token;
            i++;
        }

        if (list.length >= list.capacity) {
            list.capacity *= 2;
            list.tokens = realloc(list.tokens, list.capacity * sizeof(Token));
            if (!list.tokens) exit(1);
        }
    }

    list.tokens[list.length++] = (Token){TOKEN_END, 0, 0};
    return list;
}

AstNode *new_node(Token token) {
    AstNode *node = malloc(sizeof(AstNode));
    node->token = token;
    node->left = node->right = NULL;
    return node;
}

AstNode *parse_expression(TokenList *tokens, size_t *current) {
    AstNode *node = parse_term(tokens, current);
    while (*current < tokens->length &&
          (tokens->tokens[*current].type == TOKEN_PLUS ||
           tokens->tokens[*current].type == TOKEN_MINUS)) {
        Token token = tokens->tokens[(*current)++];
        AstNode *right = parse_term(tokens, current);
        AstNode *new_node_ = new_node(token);
        new_node_->left = node;
        new_node_->right = right;
        node = new_node_;
    }
    return node;
}

AstNode *parse_term(TokenList *tokens, size_t *current) {
    AstNode *node = parse_factor(tokens, current);
    while (*current < tokens->length &&
          (tokens->tokens[*current].type == TOKEN_MULTIPLY ||
           tokens->tokens[*current].type == TOKEN_DIVIDE)) {
        Token token = tokens->tokens[(*current)++];
        AstNode *right = parse_factor(tokens, current);
        AstNode *new_node_ = new_node(token);
        new_node_->left = node;
        new_node_->right = right;
        node = new_node_;
    }
    return node;
}

AstNode *parse_factor(TokenList *tokens, size_t *current) {
    Token token = tokens->tokens[*current];

    if (token.type == TOKEN_MINUS) {
        (*current)++;
        AstNode *node = new_node(token);
        node->left = new_node((Token){TOKEN_NUMBER, 0, 0});
        node->right = parse_factor(tokens, current);
        return node;
    }

    if (token.type == TOKEN_NUMBER) {
        (*current)++;
        return new_node(token);
    } else if (token.type == TOKEN_LPAREN) {
        (*current)++;
        AstNode *node = parse_expression(tokens, current);
        if (tokens->tokens[*current].type != TOKEN_RPAREN) {
            printf("Expected ')'\n");
            exit(1);
        }
        (*current)++;
        return node;
    } else {
        printf("Unexpected token at position %zu\n", *current);
        exit(1);
    }
}

long eval(AstNode *node) {
    switch (node->token.type) {
        case TOKEN_NUMBER:
            return node->token.value;
        case TOKEN_PLUS:
            return eval(node->left) + eval(node->right);
        case TOKEN_MINUS:
            return eval(node->left) - eval(node->right);
        case TOKEN_MULTIPLY:
            return eval(node->left) * eval(node->right);
        case TOKEN_DIVIDE:
            return eval(node->left) / eval(node->right);
        default:
            return 0;
    }
}

void print_ast(AstNode *node, int depth) {
    for (int i = 0; i < depth; ++i) printf("  ");
    switch (node->token.type) {
        case TOKEN_NUMBER: printf("Number(%d)\n", node->token.value); break;
        default: printf("Operator(%c)\n", node->token.op); break;
    }
    if (node->left) print_ast(node->left, depth + 1);
    if (node->right) print_ast(node->right, depth + 1);
}

void free_ast(AstNode *node) {
    if (!node) return;
    free_ast(node->left);
    free_ast(node->right);
    free(node);
}
