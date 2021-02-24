#include "mem.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>

int page_size=0;
int m_init_flag=0;
int m_error;

typedef struct __list_t{
int size;
_Bool type;
struct __list_t *next;
}*MemNodePtr;

MemNodePtr g_head;

int g_addr_space;


//***************** MEM_INIT *******************//

int Mem_Init(int sizeOfRegion)
{
m_error=0;
page_size=getpagesize();

if((sizeOfRegion<=0)||m_init_flag){
	printf("ERROR\n");
	m_error=E_BAD_ARGS;
	return -1;
	}
if(sizeOfRegion%page_size!=0){
	sizeOfRegion+=(page_size -(sizeOfRegion%page_size));
	}

g_addr_space=sizeOfRegion;

int fd=open("/dev/zero",O_RDWR);
MemNodePtr head = mmap(NULL, sizeOfRegion, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);

if (head == MAP_FAILED){
	perror("mmap"); 
	exit (1); 
	}
g_head=head;
head->size=sizeOfRegion-sizeof(MemNodePtr*);
head->next=NULL;

close(fd);
m_init_flag=1;
return 0;
}

//***************** MEM_Alloc *******************//

void *Mem_Alloc(int size, int style)
{
m_error=0;
MemNodePtr head = g_head;
MemNodePtr allotted=NULL;
int service_complete=0;

if(size%8!=0){
	size+=(8-(size%8));
}

switch(style){

case FIRSTFIT:
	while((!service_complete)&&(head!=NULL)){
		if(size==head->size){
			head->size=0;	
			service_complete=1;
		}
		else if(size < head->size){
			MemNodePtr temp = head->next;

			if(head->size-size >= sizeof(MemNodePtr*)){

			head->next=(void *)head+size+sizeof(MemNodePtr*);
			head->next->size=head->size-(size+sizeof(MemNodePtr*));
			head->next->next=temp;
		 	head->size=0;
			head->type = 1;
			allotted=head;
			head=head->next;
			}
			else{}
			service_complete=1;
		}
		else{
			head=head->next;
		}
	}
	if(service_complete){
		return ((void*)allotted+sizeof(MemNodePtr*));
	}
	else{
		m_error=E_NO_SPACE;
		return NULL;
	}
	break;

case WORSTFIT:
	while(head!=NULL){
		if(size<=head->size)
		{
			if(allotted==NULL)
			{
				allotted=head;
			}
			else
			{
				if(head->size>allotted->size)
					allotted=head;
			}
		}
		head=head->next;
	}
	if(allotted==NULL){
		m_error=E_NO_SPACE;
		return NULL;
	}
	else{
		if(allotted->size==size){
			allotted->size=0;
		}
		else if(allotted->size>size){
			MemNodePtr temp = allotted->next;

			if(allotted->size-size>=sizeof(MemNodePtr*)){

			allotted->next=(void *)allotted+size+sizeof(MemNodePtr*);
//printf("%d\n",allotted->size);
			allotted->next->size=allotted->size-(size+sizeof(MemNodePtr*));
			allotted->next->next=temp;
		 	allotted->size=0;
			allotted->type = 1;
			}
			else{
			allotted=allotted->next;
			}
		}

	
	return ((void*) allotted + sizeof(MemNodePtr*));
	}
	break;

default:
	break;
}
return NULL;
}

//***************** MEM_Free *******************//

int Mem_Free(void *ptr)
{
m_error=0;

if(ptr==NULL)
	return -1;
MemNodePtr head=g_head;
while(head!=NULL){

	if(((void*)head->next+sizeof(MemNodePtr*)==ptr)&&(head->size!=0)){               
		if(head->next->next==NULL){
                        head->size=(int)((void*)g_head+g_addr_space-(void*)head)-sizeof(MemNodePtr*);
			head->next=NULL;
                        return 0;
                }
                else{
                        if(head->next->next->size!=0){
                                head->size=(int)((void*)head->next->next-(void*)head)+head->next->next->size;//-sizeof(node_t);
                                head->next=head->next->next->next;
                        }
                        else{
                                head->size=(int)((void*)head->next->next-(void*)head)-sizeof(MemNodePtr*);
				head->next=head->next->next;
			}
                        return 0;
		}		
	}
	else if(((void*)head+sizeof(MemNodePtr*))==ptr){
		if(head->next==NULL){
			head->size=(int)((void*)g_head+g_addr_space-(void*)head)-sizeof(MemNodePtr*);
			return 0;
		}
		else{
			if(head->next->size!=0){
				head->size=(int)((void*)head->next-(void*)head)+head->next->size;
				head->next=(head->next)->next;
			}
			else
				head->size=(int)((void*)head->next-(void*)head)-sizeof(MemNodePtr*);
			return 0;
		}
	}
	head=head->next;
   }
m_error= E_BAD_POINTER;
return -1;
}

//***************** MEM_Dump *******************//

void Mem_Dump()
{
MemNodePtr head=g_head;
printf("Regions below\n");
while(head!=NULL){
	printf("Starting address of the chunk: %p Size available: %d\n",(void*)head+sizeof(MemNodePtr*),head->size);
	head=head->next;
}
}
