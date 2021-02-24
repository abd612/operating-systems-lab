#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include "sort.h"
#include <sys/types.h>
#include <sys/stat.h>

void usage(char *prog) 
{
    fprintf(stderr, "usage: %s <-i input file> <-o output file>\n", prog);
    exit(1);
}

int main(int argc, char *argv[])
{
    // program assumes a 4-byte key in a 100-byte record
    assert(sizeof(rec_t) == 100);

    // arguments
    char *inFile = "/no/such/file";
    char *outFile = "/no/such/file";

    // input parameters
    int c;
    opterr = 0;
    while ((c = getopt(argc, argv, "i:o:")) != -1)
    {
		switch (c)
			{
			case 'i':
			    inFile = strdup(optarg);
			    break;
			case 'o':
			    outFile = strdup(optarg);
			    break;
			default:
			    usage(argv[0]);
			}
    }

    struct stat st;
    if (stat(inFile, &st))
    {
		perror("stat");
		exit(1);
    }
    size_t size = st.st_size;
    int items = size/100;
    printf("\nSize: %lu bytes, Items: %d structures\n\n", size, items);

	// open and create input file
    int fdi = open(inFile, O_RDONLY);
    if (fdi < 0)
    {
		perror("open");
		exit(1);
    }

    printf("\n%s\n\n", "** Input Data (Unsorted) **");
    

    // copy input from file to array
    rec_t *r = (rec_t *)malloc(items*sizeof(rec_t));
    if(r == NULL)
    {
    	perror("malloc");
    	exit(1);
    }
    //rec_t r[items];
    int i  = 0;
    int j = 0;

    while (1)
    {	
		int rc;
		rc = read(fdi, &r[i], sizeof(rec_t));
		if (rc == 0) // 0 indicates EOF
		    break;
		if (rc < 0)
		{
		    perror("read");
		    exit(1);
		}

		printf("key: %c rec:", r[i].key);
		for (j = 0; j < NUMRECS; j++) 
		    printf("%u ", r[i].record[j]);
		printf("\n");
		i++;
    }

	int len = i;    

	//printf("%s: %i\n", "length", len);
    // close the input file
    (void) close(fdi);

    // sort the structure array
    for(i = 0; i < len; i++)
    {
    	for (j = i+1; j < len; j++)
    	{
    		if (r[i].key > r[j].key)
    		{
    			rec_t temp = r[i];
    			r[i] = r[j];
    			r[j] = temp;
    		}
    	}
    }

    // open and create output file
    int fdo = open(outFile, O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU);
    if (fdo < 0)
    {
	perror("open");
	exit(1);
    }

    printf("\n%s\n\n", "** Output Data (Sorted) **");
    
    // copy output from array to file
    for (i = 0; i < len; i++)
    {
		printf("key: %c rec:", r[i].key);
		for (j = 0; j < NUMRECS; j++) 
		    printf("%u ", r[i].record[j]);
		printf("\n");
		

		int rc = write(fdo, &r[i], sizeof(rec_t));
		if (rc != sizeof(rec_t))
		{
		    perror("write");
		    exit(1);
		    // should probably remove file here but ...
		}
    }

    //close the output file
	(void) close(fdo);

    return 0;
}