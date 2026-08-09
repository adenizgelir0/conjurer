#include "expr.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

Expr *new_expr(OP op)
{
    Expr *E = malloc(sizeof(Expr));
    E->op = op;
    E->left = NULL;
    E->right = NULL;
    E->val = 0;
    return E;
}

Expr *new_var(VAL val)
{
    Expr *E = new_expr(VAR);
    E->val = val;
    return E;
}

Expr *new_num(VAL val)
{
    Expr *E = new_expr(NUM);
    E->val = val;
    return E;
}

void free_expr(Expr *E)
{
    if(E==NULL) return;
    free_expr(E->left);
    free_expr(E->right);
    free(E);
}
void print_expr(Expr *E)
{
    if(E->op == NUM)
    {
        printf("%d",E->val);
        return;
    }
    if(E->op == VAR)
    {
        printf("%c",E->val);
        return;
    }
    printf("(");
    print_expr(E->left);
    printf(" %c ", E->op);
    print_expr(E->right);
    printf(")");
}

Expr *gen_expr(int n)
{
    char *vals = "scfp";
    if(rand()%n == 0)
    {
        if(rand()%4 == 0)
            return new_num((rand()%3)+1);
        return new_var(vals[rand()%4]);
    }
    Expr *left = gen_expr(n-1);
    Expr *right = gen_expr(n-1);
    if(rand()%3)
    {
        Expr *ret = new_expr(ADD);
        ret->left = left;
        ret->right = right;
        return ret;
    }
    Expr *ret = new_expr(MULT);
    ret->left = left;
    ret->right = right;
    return ret;
}
int isequal_expr(Expr *A, Expr *B)
{
	if(A->op != B->op) return 0;
	if(A->op == VAR || A->op == NUM) 
		return A->val == B->val;
	int p1 = isequal_expr(A->left, B->left)
		&& isequal_expr(A->right, B->right);
	int p2 = isequal_expr(A->right, B->left)
		&& isequal_expr(A->left, B->right);
	return p1 || p2;
}

Expr *clone_expr(Expr *E)
{
	if(E==NULL) return NULL;
	Expr *clone = new_expr(E->op);
	clone->val = E->val;
	clone->left = clone_expr(E->left);
	clone->right = clone_expr(E->right);
	return clone;
}

void print_expr_code(Expr *E)
{
	if(E == NULL) return;
	if(E->op == VAR)
	{
		printf("%c", E->val);
		return;
	}
	if(E->op == NUM)
	{
		printf("%c", E->val+'0');
		return;
	}
	printf("%c",E->op);
	print_expr_code(E->left);
	print_expr_code(E->right);
}

int min_expr(Expr *E, vec *table[])
{
    if(E->op == VAR)
        return (table[E->val])->data[0];
    if(E->op == NUM)
        return E->val;
    if(E->op == MULT)
        return min_expr(E->left, table)
            * min_expr(E->right, table);
    return min_expr(E->left, table)
        + min_expr(E->right, table);
}
int rand_expr(Expr *E, vec *table[])
{
    if(E->op == VAR)
	{
		int size = (table[E->val])->size;
        return (table[E->val])->data[rand()%size];
	}
    if(E->op == NUM)
        return E->val;
    if(E->op == MULT)
        return rand_expr(E->left, table)
            * rand_expr(E->right, table);
    return rand_expr(E->left, table)
        + rand_expr(E->right, table);
}

int expr_redundant(Expr *E)
{
	if(E->op == VAR) return 0;
    if(E->op == NUM) return 0;
    if(E->op == MULT)
        return ((E->left)->val == 1)
            || ((E->right)->val == 1)
			|| expr_redundant(E->left)
			|| expr_redundant(E->right);
    if(E->op == ADD)
        return expr_redundant(E->left)
            || expr_redundant(E->right)
			|| (((E->left)->val + ((E->right)->val)) < 4);
    return 0;
}
