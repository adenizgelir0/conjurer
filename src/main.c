#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "fib.h"
#include "vec.h"
#include "expr.h"
#include "evec.h"
#include "conj.h"

void read_test_params(int *n, int *T)
{
	char line[100];
	int new_n,new_T;
	printf("Enter n and T, or press enter for defaults (%d %d): ", *n, *T);
	if(fgets(line, sizeof(line), stdin) == NULL) return;
	if(sscanf(line, "%d%d", &new_n, &new_T) == 2)
	{
		*n = new_n;
		*T = new_T;
	}
	else
		printf("Using n=%d and T=%d\n", *n, *T);
}

int test_conj_many_quiet(Conj *C, int cap, int T, vec *table[])
{
	while(T--)
		if(!test_conj(C, cap, table))
			return 0;
	return 1;
}

Conj* test_conjs_quiet(Conj *head, int cap, int T, vec *table[])
{
	if(head == NULL) return NULL;
	Conj *prev = new_conj(head->A, head->B);
	Conj *tmp = prev;
	prev->next = head;
	while(head != NULL)
	{
		if(!test_conj_many_quiet(head, cap, T, table))
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

int main()
{
	int N = 100000;
	int n = 100;
	vec *table[255];
	printf("Conjurer searches for conjectures of the form A => B.\n");
	printf("This means: every number of the form A can be written in the form B.\n");
	printf("Variables are: s=square, c=cube, f=Fibonacci, p=prime.\n\n");
	printf("Building lookup tables up to %d...\n", N);
	init_table(N, table);
	srand(time(NULL));
	printf("Generating expression forms...\n");
	evec *ev = all_trees(2);
	Conj *head = NULL;
	int count=0;
	int T=5;
	printf("Generated %d forms.\n", ev->size);
	read_test_params(&n, &T);
	printf("Testing candidate conjectures with n=%d and T=%d.\n", n, T);
	printf("This may take a moment.\n\n");
	for(int i=0; i<ev->size; i++)
	{
		for(int j=0; j<ev->size; j++)
		{
			if(i==j) continue;
			Expr *A = ev->data[i];
			Expr *B = ev->data[j];
			Conj *candidate = new_conj(A,B);
			if(!conj_variabled(candidate))
			{
				free(candidate);
				continue;
			}
			if(conj_redundant(candidate))
			{
				free(candidate);
				continue;
			}
			if(test_conj_many_quiet(candidate, n, T, table))
			{
				//print_conj(candidate);
				candidate->next = head;
				head = candidate;
				count++;
			}
			else
				free(candidate);
		}
	}
	printf("\nInitial search complete: %d conjectures remain.\n", count);
	while(1)
	{
		char choice[10];
		printf("\n%d conjectures remain.\n",count);
		printf("[t] test again  [p] print survivors  [q] quit\n");
		printf("choice: ");
		if(fgets(choice, sizeof(choice), stdin) == NULL) break;
		if(choice[0] == 'q' || choice[0] == 'Q')
			break;
		if(choice[0] == 'p' || choice[0] == 'P')
		{
			print_conjs(head);
			continue;
		}
		if(choice[0] == 't' || choice[0] == 'T')
		{
			read_test_params(&n, &T);
			printf("Retesting survivors with n=%d and T=%d...\n", n, T);
			head = test_conjs_quiet(head, n, T, table);
			count = count_conjs(head);
			if(count == 0)
			{
				printf("All conjectures were eliminated.\n");
				break;
			}
			continue;
		}
		printf("Unknown choice.\n");
	}
	printf("Goodbye.\n");

}
