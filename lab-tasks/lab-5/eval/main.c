#include <stdio.h>
#include <stdlib.h>
#include "mem.h"
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int main (int argc,char **argv) {
	
	int r;
      
  	printf("Enter size for heap memory initialization = ");
 	scanf("%d",&r);	
        Mem_Init(r);
	Mem_Dump();
	//to colaesce and free memory
    Mem_Free(NULL);
 
	int *a=Mem_Alloc(10,FIRSTFIT);    	// FF	
	Mem_Dump();

	int *b=Mem_Alloc(200,FIRSTFIT);    	// FF	
	Mem_Dump();
	
	int *c=Mem_Alloc(10,FIRSTFIT);    	// FF	
	Mem_Dump();

	int *d=Mem_Alloc(300,FIRSTFIT);    	// FF	
	Mem_Dump();

	int *e=Mem_Alloc(10,FIRSTFIT);    	// FF	
	Mem_Dump();

	Mem_Free(b);
	Mem_Dump();

    Mem_Free(d);
	Mem_Dump();


    int *f=Mem_Alloc(100, SECONDFIT);    	// FF	
	Mem_Dump();

	Mem_Free(a);

	Mem_Dump();
	
	Mem_Free(b);
	Mem_Free(c);
	Mem_Free(d);
	Mem_Free(e);
	Mem_Free(f);
	Mem_Dump();

  	return 0;
}
