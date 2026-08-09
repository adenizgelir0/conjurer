#include "parse.h"
#include <stdio.h>
#include <stdlib.h>

parser *new_parser(char *str)
{
    parser *p = malloc(sizeof(parser));
    p->str = str;
    p->i = 0;
    return p;
}

char peek(parser *p)
{
    return p->str[p->i];
}

char advance(parser *p)
{
    if(peek(p) == 0) return 0;
    return p->str[p->i++];
}

void skip_spaces(parser *p)
{
    while(peek(p) == ' ' || peek(p) == '\t' || peek(p) == '\n')
        p->i++;
}

int match_char(parser *p, char exp)
{
    if(peek(p) != exp) return 0;
    p->i++;
    return 1;
}

Expr *parse_factor(parser *p)
{
    skip_spaces(p);
    if(match_char(p, '('))
    {
        Expr *inner = parse_expr(p);
        if(inner == NULL) return NULL;
        skip_spaces(p);
        if(match_char(p, ')')) return inner;
        free_expr(inner);
        return NULL;
    }
    char c = advance(p);

    if(c == 's' || c == 'c' || c == 'f' || c == 'p')
        return new_var(c);

    if(c == '1' || c == '2' || c == '3')
        return new_num(c - '0');

    return NULL;
}

Expr *parse_term(parser *p)
{
    Expr *left = parse_factor(p);
    if(left == NULL) return NULL;

    skip_spaces(p);

    while(match_char(p, '*'))
    {
        Expr *node = new_expr(MULT);
        node->left = left;
        node->right = parse_factor(p);
        if(node->right == NULL)
        {
            free_expr(node);
            return NULL;
        }
        left = node;
        skip_spaces(p);
    }

    return left;
}

Expr *parse_expr(parser *p)
{
    Expr *left = parse_term(p);
    if(left == NULL) return NULL;

    skip_spaces(p);

    while(match_char(p, '+'))
    {
        Expr *node = new_expr(ADD);
        node->left = left;
        node->right = parse_term(p);
        if(node->right == NULL)
        {
            free_expr(node);
            return NULL;
        }
        left = node;
        skip_spaces(p);
    }

    return left;
}
