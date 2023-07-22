#pragma once
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "expr.h"

typedef struct {
	Expr **data;
	int size;
	int cap;
} evec; //expression vector

evec *new_evec()
{
	evec *v = malloc(sizeof(evec));
	v->size=0;
	v->cap=1;
	v->data = malloc(sizeof(Expr*));
	return v;
}

void free_evec(evec *v)
{
	free(v->data);
	free(v);
}

void print_evec(evec *v)
{
	for(int i=0; i<v->size; i++)
	{
		print_expr(v->data[i]);
		printf("\n");
	}
}

void print_evec_codes(evec *v)
{
	for(int i=0; i<v->size; i++)
	{
		print_expr_code(v->data[i]);
		printf("\n");
	}
}

void push_evec(evec *v, Expr *elem)
{
	if(v->size == v->cap)
	{
		v->cap *= 2;
		v->data = realloc(v->data, (v->cap)*sizeof(Expr *));
	}
	v->data[v->size++] = elem;
}

Expr* pop_evec(evec *v)
{
	v->size--;
	return v->data[v->size];
}

Expr* peek_evec(evec *v)
{
	return v->data[v->size-1];
}


evec *gen_many(int n, int T)
{
	evec *ev = new_evec();
	for(int i=0; i<T; i++)
	{
		push_evec(ev,gen_expr(n));
	}
	return ev;
}
evec* all_trees(int n)
{
	char *vals = "scfp\0";
	if(n==0)
	{
		evec *ev = new_evec();
		push_evec(ev,new_num(1));
		push_evec(ev,new_num(2));
		push_evec(ev,new_num(3));
		for(int i=0; vals[i]; i++)
			push_evec(ev, new_var(vals[i]));
		return ev;
	}
	evec* lesser = all_trees(n-1);
	int orig_size = lesser->size;
	for(int i=0; i<orig_size; i++)
	{
		for(int j=i; j<orig_size; j++)
		{
			Expr *caleft = clone_expr(lesser->data[i]);
			Expr *caright = clone_expr(lesser->data[j]);
			Expr *add = new_expr(ADD);
			add->left = caleft;
			add->right = caright;
			push_evec(lesser,add);
			Expr *cmleft = clone_expr(lesser->data[i]);
			Expr *cmright = clone_expr(lesser->data[j]);
			Expr *mult = new_expr(MULT);
			mult->left = cmleft;
			mult->right = cmright;
			push_evec(lesser,mult);
		}
	}
	return lesser;

}


void free_many(evec *ev)
{
	for(int i=0; i<ev->size; i++)
		free_expr(ev->data[i]);
	free_evec(ev);
}

