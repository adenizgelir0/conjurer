#include <stdio.h>
#include "expr.h"

int main()
{
	char test[50];
	scanf("%s",test);
	Expr *e = parse_expr(test);
	print_expr(e);
	printf("\n");
}
