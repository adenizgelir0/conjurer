#include <stdio.h>
#include "expr.h"
#include "parse.h"

int main()
{
	char test[50];
	fgets(test, sizeof(test), stdin);
	parser p = new_parser(test);
	Expr *e = parse_expr(&p);
	skip_spaces(&p);
	if(peek(&p) != 0) e = NULL;
	if(e == NULL)
	{
		printf("could not parse\n");
		return 1;
	}
	print_expr(e);
	printf("\n");
}
