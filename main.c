#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "fib.h"
#include "vec.h"
#include "expr.h"
#include "evec.h"
#include "conj.h"

int main()
{
	int N = 100000;
	int n = 100;
	vec *table[255];
	init_table(N, table);
	srand(time(NULL));
	evec *ev = all_trees(2);
	Conj *head = NULL;
	int count=0;
	int T=5;
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
	while(1)
	{
		printf("%d left\n",count);
		printf("n t: ");
		scanf("%d%d",&n,&T);
		head = test_conjs(head, n, T, table);
		count = count_conjs(head);
	}

}
