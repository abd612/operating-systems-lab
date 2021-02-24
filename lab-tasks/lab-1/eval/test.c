#include <stdio.h>
#include <stdlib.h>

int main()

{
	int x = 'z';
	//97 to 122
	int r = 97+rand()/100000000;
	int s = 97+rand()/100000000;
	
	printf("%d %d\n", r, s);
}