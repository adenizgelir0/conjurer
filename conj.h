#pragma once
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "fib.h"
#include "vec.h"
#include "expr.h"
#include "evec.h"

typedef struct Conj {
	Expr *A,*B;
	//Conjecture statement:
	//Every number of the form A can be written in the form B.
	struct Conj *next;
} Conj;

Conj *new_conj(Expr *A, Expr *B)
{
	Conj *c = malloc(sizeof(Conj));
	c->A = A;
	c->B = B;
	c->next = NULL;
	return c;
}

void extract_vars(Expr *E, evec *ev)
{
    if(E == NULL) return;
    if(E->op == VAR)
    {
        push_evec(ev, E);
        return;
    }
    extract_vars(E->left, ev);
    extract_vars(E->right, ev);
}

int eval_1varexpr(int val, Expr *E)
{
	if(E->op == VAR) return val;
	if(E->op == NUM) return E->val;
	if(E->op == ADD)
	{
		int left = eval_1varexpr(val, E->left);
		int right = eval_1varexpr(val, E->right);
		return left+right;
	}
	int left = eval_1varexpr(val, E->left);
	int right = eval_1varexpr(val, E->right);
	return left*right;
}
int test_1varexpr(int n, Expr *E, vec *table[])
{
	evec *var_nodes = new_evec();
	extract_vars(E, var_nodes);
	Expr *var_node = var_nodes->data[0];
	vec *search_space = table[var_node->val];
	int low=0, high=search_space->size-1;
	while(low<=high)
	{
		int mid = low + (high - low)/2;
		int test_val = search_space->data[mid];
		int result = eval_1varexpr(test_val, E);
		if(result == n){
			/*
			VAL tmp = var_node->val;
			var_node->op = NUM;
			var_node->val = test_val;
			print_expr(E);
			printf("\n");
			var_node->op = VAR;
			var_node->val = tmp;*/
			free_evec(var_nodes);
			return test_val;
		}
		if(result < n) low = mid+1;
		if(result > n) high = mid-1;
	}
	free_evec(var_nodes);
	return 0;
}
int test_expr(int n, Expr *E, vec *table[])
{
	evec *var_nodes = new_evec();
	extract_vars(E, var_nodes);
	if(var_nodes->size == 0) return 0;
	if(var_nodes->size == 1)
	{
		free_evec(var_nodes);
		return test_1varexpr(n, E, table);
	}
	evec *fib_var_nodes = new_evec();
	evec *prime_var_nodes = new_evec();
	evec *square_var_nodes = new_evec();
	evec *cube_var_nodes = new_evec();
	for(int i=0; i<var_nodes->size; i++)
	{
		if((var_nodes->data[i])->val == SQUARE)
			push_evec(square_var_nodes, (var_nodes->data)[i]);
		if((var_nodes->data[i])->val == CUBE)
			push_evec(cube_var_nodes, (var_nodes->data)[i]);
		if((var_nodes->data[i])->val == FIB)
			push_evec(fib_var_nodes, (var_nodes->data)[i]);
		if((var_nodes->data[i])->val == PRIME)
			push_evec(prime_var_nodes, (var_nodes->data)[i]);
	}
	//variables belonging to smaller search spaces are prioritized
	Expr *test_node = NULL;
	if(fib_var_nodes->size > 0)
		test_node = fib_var_nodes->data[0];
	else if(cube_var_nodes->size > 0)
		test_node = cube_var_nodes->data[0];
	else if(square_var_nodes->size > 0)
		test_node = square_var_nodes->data[0];
	else
		test_node = prime_var_nodes->data[0];
	free_evec(var_nodes);
	free_evec(fib_var_nodes);
	free_evec(prime_var_nodes);
	free_evec(square_var_nodes);
	free_evec(cube_var_nodes);
	vec *search_space = table[test_node->val];
	int upper_bound=-1;
	int low=0, high=search_space->size-1;
	test_node->op = NUM;
	VAL tmp_val = test_node->val;
	while(low <= high)
	{
		int mid = low + (high - low)/2;
		int test_val = search_space->data[mid];
		test_node->val = test_val;
		int min = min_expr(E, table);
		if(min == n)
		{
			test_node->op = VAR;
			test_node->val = tmp_val;
			return 1;
		}
		if(min < n) 
		{
			upper_bound = max(upper_bound, mid);
			low = mid+1;
		}
		else
			high=mid-1;
	}
	if(upper_bound == -1)
	{
		test_node->op = VAR;
		test_node->val = tmp_val;
		return 0;
	}
	for(int i=upper_bound; i>=0; i--)
	{
		int test_val = search_space->data[i];
		test_node->val = test_val;
		int test_res = test_expr(n, E, table);
		if(test_res)
		{
			test_node->op = VAR;
			test_node->val = tmp_val;
		   	return 1;
		}
	}
	test_node->op = VAR;
	test_node->val = tmp_val;
	return 0;
}

vec *gen_primes(int cap)
{
	char *sieve = calloc(cap+1, sizeof(char));
	for(int i=2; i*i<=cap; i++)
	{
		if(sieve[i] != 0) continue;
		for(int j=i*i; j<=cap; j+=i)
		{
			sieve[j] = 1;
		}
	}
	vec *primes = new_vec();
	for(int i=2; i<=cap; i++)
		if(sieve[i] == 0)
		   	push_vec(primes, i);
	free(sieve);
	return primes;
}

vec *gen_squares(int cap)
{
	vec *squares = new_vec();
	for(int i=1; i*i<=cap; i++)
	{
		push_vec(squares, i*i);
	}
	return squares;
}

vec *gen_cubes(int cap)
{
	vec *cubes = new_vec();
	for(int i=1; i*i*i<=cap; i++)
	{
		push_vec(cubes, i*i*i);
	}
	return cubes;
}

int rand_expr_bounds(Expr *E, vec *table[], int lower_bound, int upper_bound)
{
	evec *var_nodes = new_evec();
	extract_vars(E, var_nodes);
	if(var_nodes->size == 0)
	{
		free_evec(var_nodes);
		return eval_1varexpr(0, E);
	}
	int rand_ind = rand() % (var_nodes->size);
	Expr *rand_node = var_nodes->data[rand_ind];
	free_evec(var_nodes);
	vec *search_space = table[rand_node->val];
	rand_node->op = NUM;
	VAL tmp_val = rand_node->val;
	int low=0, high=search_space->size-1;
	int space_ubound = -1;
	while(low <= high)
	{
		int mid = low + (high-low)/2;
		int test_val = search_space->data[mid];
		rand_node->val = test_val;
		int min = min_expr(E, table);
		if(min == upper_bound)
		{
			space_ubound = mid;
			break;
		}
		if(min > upper_bound)
			high = mid - 1;
		if(min < upper_bound)
		{
			space_ubound = max(space_ubound, mid);
			low = mid+1;
		}
	}
	if(space_ubound == -1)
	{
		rand_node->op = VAR;
		rand_node->val = tmp_val;
		return -1;
	}
	int res = -1;
	while(res < lower_bound)
	{
		int rand_val = search_space->data[rand()%(space_ubound+1)];
		rand_node->val = rand_val;
		res = rand_expr_bounds(E, table, lower_bound, upper_bound);
	}
	rand_node->op = VAR;
	rand_node->val = tmp_val;
	return res;

}

int expr_variabled(Expr *E)
{
	evec *vars = new_evec();
	extract_vars(E, vars);
	if(vars->size == 0)
	{
		free_evec(vars);
		return 0;
	}
	free_evec(vars);
	return 1;
}
int conj_variabled(Conj *C)
{
	return expr_variabled(C->A)
		&& expr_variabled(C->B);
}
int conj_redundant(Conj *C)
{
	return expr_redundant(C->A)
		|| expr_redundant(C->B);
}

int test_conj(Conj *C, int cap, vec *table[])
{
	//print_expr(C->B);
	//printf("\n");
	int lower_bound = min_expr(C->A, table);
	int upper_bound = cap;
	int n = rand_expr_bounds(C->A, table,
		   	lower_bound, upper_bound);
	return test_expr(n, C->B, table);
}

void print_conj(Conj *C)
{
	print_expr(C->A);
	printf(" => ");
	print_expr(C->B);
	printf("\n");
}

int test_conj_many(Conj *C, int cap, int T, vec *table[])
{
	while(T--)
		if(!test_conj(C, cap, table))
			return 0;
	print_conj(C);
	return 1;

}

Conj* test_conjs(Conj *head, int cap, int T, vec *table[])
{
	Conj *prev = new_conj(head->A, head->B);
	Conj *tmp = prev;
	prev->next = head;
	while(head != NULL)
	{
		if(!test_conj_many(head, cap, T, table))
		{
			prev->next = head->next;
			free(head);
			head = prev->next;
		} 
		else 
		{
			prev = head;
			head = head->next;
		}
	}
	Conj *new_head = tmp->next;
	free(tmp);
	return new_head;
}




void print_conjs(Conj *head)
{
	while(head != NULL)
	{
		print_conj(head);
		head = head->next;
	}
}

int count_conjs(Conj *head)
{
	int c=0;
	while(head != NULL)
		head = head->next, c++;
	return c;
}

void init_table(int N, vec *table[])
{
    table[FIB] = gen_fibs_upto(N);
    table[PRIME] = gen_primes(N);
    table[SQUARE] = gen_squares(N);
    table[CUBE] = gen_cubes(N);
}

