#include <stdio.h>

int
main(int argc, char *argv[])
{
	int n = 166;
	int *p;
	p = &n;

	printf("%d, %d\n", n, *p);

	p = NULL;

	printf("%d, %d\n", n, *p);

	return 0;
}