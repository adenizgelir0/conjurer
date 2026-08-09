#pragma once

#include "evec.h"
#include "expr.h"
#include "vec.h"

typedef struct Conj {
	Expr *A, *B;
	struct Conj *next;
} Conj;

Conj *new_conj(Expr *A, Expr *B);
void extract_vars(Expr *E, evec *ev);
int eval_1varexpr(int val, Expr *E);
int test_1varexpr(int n, Expr *E, vec *table[]);
int test_expr(int n, Expr *E, vec *table[]);
vec *gen_primes(int cap);
vec *gen_squares(int cap);
vec *gen_cubes(int cap);
int rand_expr_bounds(Expr *E, vec *table[], int lower_bound, int upper_bound);
int expr_variabled(Expr *E);
int conj_variabled(Conj *C);
int conj_redundant(Conj *C);
int test_conj(Conj *C, int cap, vec *table[]);
int test_conj_get_counterexample(Conj *C, int cap, vec *table[], int *counterexample);
int test_conj_get_counterexample_expr(Conj *C, int cap, vec *table[], Expr **counterexample_expr);
void print_conj(Conj *C);
int test_conj_many(Conj *C, int cap, int T, vec *table[]);
int test_conj_many_get_counterexample(Conj *C, int cap, int T, vec *table[], int *counterexample);
int test_conj_many_get_counterexample_expr(Conj *C, int cap, int T, vec *table[], Expr **counterexample_expr);
Conj *test_conjs(Conj *head, int cap, int T, vec *table[]);
void print_conjs(Conj *head);
int count_conjs(Conj *head);
void init_table(int N, vec *table[]);
