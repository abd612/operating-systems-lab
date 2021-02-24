#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
	int n = 166;
	int *p;
	p = &n;

	printf(1, "Referencing valid pointer:\n");
	printf(1,"%d, %d\n", n, *p);

	p = NULL;

	printf(1, "Referencing null pointer:\n");
	printf(1, "%d, %d\n", n, *p);

	exit();
}