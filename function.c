#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "header.h"
#include <time.h>


int gotoPageTable(int page_number,int PAGE[PAGE_SIZE][2])
{
	if( PAGE[page_number][1] == 1 )//valid
	{
		return 1;

	}
	else//invalid
	{
		return 0;
	}

}
int backstore(int page_number,char value[], int page_offset)
{
	FILE* fp;
	int FrameNumber;
	char buffer[256];
	buffer[0] = '\0';
	fp = fopen("BACKING_STORE.bin","rb");
	//initilate the MainMemory of the linked list
	MainMemory *list = malloc(sizeof(MainMemory));
	list->next = NULL;
	list->buffer = NULL;

	if(fp == NULL)
		printf("read BACKING_STORE wrong\n");
	fseek(fp,page_number*256,SEEK_SET);//set the file pointer to specific position
	fread(buffer, sizeof(char), 256, fp);

	FrameNumber = push(list,buffer,page_number);
  value[page_number] = buffer[page_offset];
	fclose(fp);
	return FrameNumber;
}
int push(MainMemory * head, char buffer[],int page_number) 
{

    MainMemory * current = head;
    while (current->next != NULL) 
    {
        current = current->next;
    }
    /* now we can add a new variable */
    current->buffer = buffer;//save the buffer
    current->FrameNumber = rand() % 1000 +1000 + page_number;
    current->next = malloc(sizeof(MainMemory));
    current->next->buffer = NULL;
    current->next->next = NULL;
    //printf("==>%d\n",current->FrameNumber );
    //puts("4");
    return current->FrameNumber;

}
int LRU(int TLB_Entry,int Track[],int TLB[TLB_SIZE][2],int PAGE [PAGE_SIZE][2],int LRU_Index,int page_number)
{
	//printf("XXXXXpage number is%d\n",page_number );
			int i;
			int flag = 0;
            if(TLB_Entry < TLB_SIZE)
            {
              TLB[TLB_Entry][0] = page_number;
              TLB[TLB_Entry][1] = PAGE[page_number][0];//the frame number
              
              Track[TLB_Entry] = page_number;//keep track of the page number
              
              TLB_Entry++;

            }
            else//if it is full
            {
              for(i = TLB_Entry-1; i >= TLB_Entry-16;i--)
              {
                // if find in the track  do nothing

                if(Track[i] == page_number)
                {
                  flag = 1;
                  break;
                }
              }
                
                  //not find use LRU to update
                  // find the least recent used page_number, that is the last one of the window
                  //and update
              if(flag == 0)
               {
                  LRU_Index = (TLB_Entry-16)%16;
                  TLB[LRU_Index][0] = page_number;
                  TLB_Entry++;//move the window by one

               }

              
            }
      return TLB_Entry;

}

