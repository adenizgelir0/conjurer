#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "fib.h"
#include "vec.h"
#include "expr.h"
#include "evec.h"
#include "conj.h"
#include "parse.h"

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

void strip_newline(char *s)
{
	for(int i=0; s[i]; i++)
		if(s[i] == '\n')
		{
			s[i] = 0;
			return;
		}
}

Expr *parse_line(char *line)
{
	parser p = new_parser(line);
	Expr *E = parse_expr(&p);
	skip_spaces(&p);
	if(peek(&p) != 0)
	{
		free_expr(E);
		return NULL;
	}
	return E;
}

void save_conjs(Conj *head, char *path)
{
	FILE *out = fopen(path, "w");
	if(out == NULL)
	{
		printf("Could not open %s for writing.\n", path);
		return;
	}
	while(head != NULL)
	{
		fprint_expr(out, head->A);
		fprintf(out, "\n");
		fprint_expr(out, head->B);
		fprintf(out, "\n");
		head = head->next;
	}
	fclose(out);
	printf("Saved survivors to %s.\n", path);
}

Conj *load_conjs(char *path)
{
	FILE *in = fopen(path, "r");
	if(in == NULL)
	{
		printf("Could not open %s for reading.\n", path);
		return NULL;
	}
	char A[1000],B[1000];
	Conj *head = NULL;
	while(fgets(A, sizeof(A), in) != NULL)
	{
		if(fgets(B, sizeof(B), in) == NULL)
			break;
		strip_newline(A);
		strip_newline(B);
		Expr *a = parse_line(A);
		Expr *b = parse_line(B);
		if(a == NULL || b == NULL)
		{
			free_expr(a);
			free_expr(b);
			continue;
		}
		Conj *c = new_conj(a,b);
		c->next = head;
		head = c;
	}
	fclose(in);
	return head;
}

int main()
{
	int N = 100000;
	int n = 100;
	char line[1000];
	vec *table[255];
	printf("Conjurer searches for conjectures of the form A => B.\n");
	printf("This means: every number of the form A can be written in the form B.\n");
	printf("Variables are: s=square, c=cube, f=Fibonacci, p=prime.\n\n");
	printf("Building lookup tables up to %d...\n", N);
	init_table(N, table);
	srand(time(NULL));
	Conj *head = NULL;
	int count=0;
	int T=5;
	printf("Start from a saved survivor file? (y/n): ");
	if(fgets(line, sizeof(line), stdin) != NULL
			&& (line[0] == 'y' || line[0] == 'Y'))
	{
		printf("File path: ");
		if(fgets(line, sizeof(line), stdin) != NULL)
		{
			strip_newline(line);
			head = load_conjs(line);
			count = count_conjs(head);
			printf("Loaded %d survivors.\n", count);
		}
	}
	if(head == NULL)
	{
		printf("Generating expression forms...\n");
		evec *ev = all_trees(2);
		printf("Generated %d forms.\n", ev->size);
		int candidate_count=0;
		for(int i=0; i<ev->size; i++)
		{
			for(int j=0; j<ev->size; j++)
			{
				if(i==j) continue;
				Conj *candidate = new_conj(ev->data[i],ev->data[j]);
				if(conj_variabled(candidate) && !conj_redundant(candidate))
					candidate_count++;
				free(candidate);
			}
		}
		printf("There are %d possible pairs and %d candidates after filtering.\n",
				ev->size * (ev->size - 1), candidate_count);
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
				if(test_conj_many(candidate, n, T, table))
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
	}
	while(1)
	{
		char choice[10];
		printf("\n%d conjectures remain.\n",count);
		printf("[t] test again  [p] print survivors  [s] save survivors  [q] quit\n");
		printf("choice: ");
		if(fgets(choice, sizeof(choice), stdin) == NULL) break;
		if(choice[0] == 'q' || choice[0] == 'Q')
			break;
		if(choice[0] == 'p' || choice[0] == 'P')
		{
			print_conjs(head);
			continue;
		}
		if(choice[0] == 's' || choice[0] == 'S')
		{
			printf("File path: ");
			if(fgets(line, sizeof(line), stdin) != NULL)
			{
				strip_newline(line);
				save_conjs(head, line);
			}
			continue;
		}
		if(choice[0] == 't' || choice[0] == 'T')
		{
			read_test_params(&n, &T);
			printf("Retesting survivors with n=%d and T=%d...\n", n, T);
			head = test_conjs(head, n, T, table);
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
