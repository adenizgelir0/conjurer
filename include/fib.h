#pragma once

#include "vec.h"

typedef struct gold_int {
	int a, b;
} gint;

gint new_gint(int a, int b);
gint add_gints(gint A, gint B);
gint mult_gints(gint A, gint B);
gint power(gint g, int p);
void print_gint(gint g);
int min(int a, int b);
int max(int a, int b);
int nthfib(int n);
int max_fib(int upper_bound);
int min_fib(int lower_bound);
vec *gen_fibs_upto(int n);
