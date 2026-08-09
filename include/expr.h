#pragma once

#include <stdio.h>
#include "vec.h"

typedef enum { ADD='+', MULT='*', VAR=0, NUM=1 } OP;
typedef enum {
	ONE=1, TWO=2, THREE=3,
	SQUARE='s',
	CUBE='c',
	FIB='f',
	PRIME='p'
} VAL;

typedef struct Expr {
	OP op;
	struct Expr *left;
	struct Expr *right;
	VAL val;
} Expr;

Expr *new_expr(OP op);
Expr *new_var(VAL val);
Expr *new_num(VAL val);
void free_expr(Expr *E);
void print_expr(Expr *E);
void fprint_expr(FILE *out, Expr *E);
Expr *gen_expr(int n);
int isequal_expr(Expr *A, Expr *B);
Expr *clone_expr(Expr *E);
void print_expr_code(Expr *E);
int min_expr(Expr *E, vec *table[]);
int rand_expr(Expr *E, vec *table[]);
int expr_redundant(Expr *E);
