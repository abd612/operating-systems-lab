#include "types.h"
#include "stat.h"
#include "user.h"
#include "pstat.h"
//#include "random.h"
#include "rand.h"

int main(int argc, char const *argv[])
{
	int max;
	if (argc == 2)
		max = atoi(argv[1]);
	else
		max = 10;
	// int t, r, s;
	// if(argc == 2)
	// 	t = atoi(argv[1]);
	// else
	// 	t = 1;

	// for (int i = 0; i < 10; i++)
	// {
	// 	r = genrand() % (t+1);
	// 	s = random_at_most(t);
	// 	printf(1, "%d\t%d\n", r,s);
	// }

	int queue[] = {0, 1, 2};
	int length = 3;
	int tickets[] = {5, 25, 30};
	int total = 60;
	int record[] = {0, 0, 0};

	for (int k = 0; k < max; k++)
	{
		int counter = 0;
		int winner = genrand() % (total);
		int i = 0;
		int current = 0;

		for(i = 0; i <= length; i++)
		{
			current = queue[i];
			counter += tickets[i];
			if(counter > winner)
			break;
		}
		record[current]++;
		//printf(1, "Winner: %d, Selected: %d\n", winner, current);
	}

	printf(1, "0's: %d, 1's: %d, 2's: %d\n", record[0], record[1], record[2]);

	exit();
}