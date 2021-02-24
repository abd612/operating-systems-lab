#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	int i = 0; //offset
	int j = 0; //reserved space
	char *msg = (char *)malloc(j*sizeof(char)); //allocate space for one character

	printf("Enter input: "); //prompt for input
	while(scanf("%c", (msg + i)) == 1 && *(msg + i++) != '\n') //store input until enter is pressed
		msg = realloc(msg, ++j*sizeof(char)); //reallocate memory with additional space for another character

	printf("Echo output: "); //display output
	for(i = 0; i < j; i++) //loop till message length
		printf("%c", *(msg + i)); //print out the message character by  character
	
	printf("\n");

	free(msg); //deallocate the used memory

	return 0;
}