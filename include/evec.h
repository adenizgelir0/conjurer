#pragma once

#include "expr.h"

typedef struct {
	Expr **data;
	int size;
	int cap;
} evec;

evec *new_evec(void);
void free_evec(evec *v);
void print_evec(evec *v);
void print_evec_codes(evec *v);
void push_evec(evec *v, Expr *elem);
Expr *pop_evec(evec *v);
Expr *peek_evec(evec *v);
evec *gen_many(int n, int T);
evec *all_trees(int n);
void free_many(evec *ev);
