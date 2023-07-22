#include <stdio.h>
#include <stdlib.h>
#include "conj.h"

int main()
{
	int N = 100000;
	vec *table[255];
	init_table(N, table);
	char A[100], B[100];
	printf("Enter the conjecture statement:\n");
	scanf("%s %s", A, B);
	Expr *a = parse_expr(A);
	Expr *b = parse_expr(B);
	Conj *c = new_conj(a,b);
	print_conj(c);
	int n,T;
	while(1)
	{
		printf("n T: ");
		scanf("%d%d",&n,&T);
		if(test_conj_many(c, n, T, table))
		{
			printf("passed\n");
		}
		else
			printf("eliminated\n");
	}

}
