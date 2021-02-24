#include "types.h"
#include "stat.h"
#include "user.h"
#include "pstat.h"

int main(int argc, char const *argv[])
{
	int t;
	if(argc == 2)
		t = atoi(argv[1]);
	else
		t = 1;
	settickets(t);

	while(1);
	// printf(1, "Main 2\n");

	// for(int i = 0; i < 10; i++)
	// {
	// 	printf(1,"Proc 2, Iter %d\n", i);
	// }

	exit();
}