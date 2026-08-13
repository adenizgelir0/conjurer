#include <stdio.h>
#include <stdlib.h>
#include "conj.h"
#include "parse.h"

int main()
{
	int N = 100000;
	vec *table[255];
	init_table(N, table);
	char A[100], B[100];
	int n,T;
	while(1)
	{
		printf("Enter the left-hand expression:\n");
		fgets(A, sizeof(A), stdin);
		printf("Enter the right-hand expression:\n");
		fgets(B, sizeof(B), stdin);
		parser pa = new_parser(A);
		parser pb = new_parser(B);
		Expr *a = parse_expr(&pa);
		Expr *b = parse_expr(&pb);
		Conj *c = new_conj(a,b);
		print_conj(c);
		while(1)
		{
			printf("n T: ");
			scanf("%d%d",&n,&T);
			Expr *counterexample;
			if(test_conj_many_get_counterexample_expr(c, n, T, table, &counterexample))
			{
				printf("passed\n");
			}
			else
			{
				print_expr(counterexample);
				printf(" could not be written in the form ");
				print_expr(c->B);
				printf("\n");
				free_expr(counterexample);
				char answer;
				printf("Test a different conjecture? (y/n): ");
				scanf(" %c",&answer);
				if(answer == 'y' || answer == 'Y')
				{
					getchar();
					break;
				}
				return 0;
			}
		}
	}

}
