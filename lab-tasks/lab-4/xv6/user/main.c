#include "types.h"
#include "stat.h"
#include "user.h"
#include "pstat.h"

int
main(int argc, char *argv[])
{
	// struct pstat* p;
	// p = (struct pstat*)malloc(sizeof(struct pstat));

	int a = fork();
	if (a < 0)
	{
		printf(1, "fork failed\n");
		exit();
	}
	else if (a == 0)
	{
		printf(1, "pid: %d\n", (int)getpid());
		// settickets(10);
		// while (1);
		int b = fork();
		if (b < 0)
		{
			printf(1, "fork failed\n");
			exit();
		}
		else if (b == 0)
		{
			printf(1, "pid: %d\n", (int)getpid());
			// settickets(10);
			// while (1);
			int c = fork();
			if (c < 0)
			{
				printf(1, "fork failed\n");
				exit();
			}
			else if (c == 0)
			{
				printf(1, "pid: %d\n", (int)getpid());
				// settickets(10);
				// while (1);
				int d = fork();
				if (d < 0)
				{
					printf(1, "fork failed\n");
					exit();
				}
				else if (d == 0)
				{
					printf(1, "pid: %d\n", (int)getpid());
					settickets(50);
					while (1);
				}
				else
				{
					printf(1, "pid: %d\n", (int)getpid());
					settickets(40);
					while (1);
				}
			}
			else
			{
				printf(1, "pid: %d\n", (int)getpid());
				settickets(30);
				while (1);
			}
		}
		else
		{
			printf(1, "pid: %d\n", (int)getpid());
			settickets(20);
			while (1);
		}
	}
	else
	{
		printf(1, "pid: %d\n", (int)getpid());
		settickets(10);
		while (1);
	}

	// if (1 == 0)
	// {
	// 	getpinfo(p);
	// 	printf(1, "pid\thticks\tlticks\n");

	// 	for(int i = 0; i < NPROC; i++)
	// 	{
	// 		if(p->inuse[i] != 0)
	// 		printf(1, "%d\t%d\t%d\n", p->pid[i], p->hticks[i], p->lticks[i]);
	// 	}
	// }

	exit();
}