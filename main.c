#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "calculator.h"

int main() {
    char input[256];
    printf("Enter an expression: ");
    if (!fgets(input, sizeof(input), stdin)) {
        printf("Input error\n");
        return 1;
    }

    input[strcspn(input, "\n")] = '\0';

    TokenList tokens = tokenize(input);

    printf("--- Tokens ---\n");
    for (size_t i = 0; i < tokens.length; ++i) {
        Token t = tokens.tokens[i];
        if (t.type == TOKEN_NUMBER)
            printf("Number(%d)\n", t.value);
        else if (t.type == TOKEN_END)
            printf("End\n");
        else
            printf("Operator(%c)\n", t.op);
    }
    printf("--------------\n");

    size_t current = 0;
    AstNode *ast = parse_expression(&tokens, &current);

    printf("--- AST ---\n");
    print_ast(ast, 0);
    printf("-----------\n");

    printf("Result: %ld\n", eval(ast));

    free_ast(ast);
    free(tokens.tokens);
    return 0;
}
