#ifndef HEADER_FILE
#define HEADER_FILE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define PAGE_SIZE 256
#define TLB_SIZE 16

typedef struct node
{
    char* buffer;
	int FrameNumber;
	struct node* next;

}MainMemory
;

int gotoPageTable(int page_number,int PAGE [PAGE_SIZE][2]);
int backstore(int page_number,char value[], int page_offset);
int LRU(int TLB_Entry,int Track[],int TLB[TLB_SIZE][2],int PAGE [PAGE_SIZE][2],int LRU_Index,int page_number);
int push(MainMemory * head, char buffer[],int page_number);

#endif