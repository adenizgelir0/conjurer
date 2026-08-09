#pragma once

#include "expr.h"

typedef struct {
    char *str;
    int i;
} parser;

parser *new_parser(char *str);
char peek(parser *p);
char advance(parser *p);
void skip_spaces(parser *p);
int match_char(parser *p, char exp);
Expr *parse_factor(parser *p);
Expr *parse_term(parser *p);
Expr *parse_expr(parser *p);
