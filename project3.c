#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "header.h"
#include <time.h>

void main()
{
	int TLBhit = 0;
	int TLBmiss = 0;
	int pagehit = 0;
	int pagefault = 0;
	int i = 0;
  int TLB_Entry = 0;//keep track of update index of TLB
  int TLB[TLB_SIZE][2];
  int PAGE[PAGE_SIZE][2];
  int Track[2000];//keep track of all the page number
  int TLB_index = 0;
  int TLB_Hit_Index = 0;
  int FrameNumber = 0;//the FrameNumber of Main Memory that we put the data in
  int TLB_Hit_Flag = 0;
  int LRU_Index;
  int AddrNumber = 0;
  int test = 0;
  char value[256];
    //initizate the TLB table
  for(i = 0; i < TLB_SIZE; i++)
  {
      TLB[i][0] = -1;//-1 means no page number
      TLB[i][1] = -1;//-1 means no frame number
      //TLB save the page number that we already asscess
  }
   //initizate the Page table
  for(i = 0; i < PAGE_SIZE; i++)
  {
    PAGE[i][0] = -1;//-1 means no frame number
    PAGE[i][1] = -1;//1 is valid; -1 is invalid
  }
	//READ A FILE
	
	FILE* fp;
	char* line =NULL;
	size_t len = 0;
	ssize_t read;
	fp = fopen("addresses.txt", "r");
    	
	while(read = getline(&line,&len,fp) != -1)
	{
                          
		    int virtual_address = atoi(line);
      
        int page_offset = atoi(line) & 255;
        int page = atoi(line) & 65280;
        int page_number = page >> 8;
      
        //printf("page_table_number is %d \t offset is %d \n ",page_number,page_offset);
  //check LRU has the virtual address or not
        for(i = 0;i < TLB_SIZE;i++)
        {
          //printf("TLB[%d][0]%d\n",i,TLB[i][0]);
           if (TLB[i][0] == page_number)
           {
              TLB_Hit_Flag = 1;
              TLB_Hit_Index = i;
              break;
           }
        }
       	//check TLB hit or miss
       	if(TLB_Hit_Flag == 1)//TLB bit
       	{
       		TLBhit++;
          //TLB HIT , get frame number and translate to physical address
       		printf("TLBHIT : Virtual_address is %d Physical address is %d Value is %hhd\n",virtual_address,(TLB[TLB_Hit_Index][1]*256+page_offset),value[page_number]);
          TLB_Hit_Flag = -1;
          TLB_Hit_Index = -1;
       	}
       	else
       	{
       		TLBmiss++;
       		//check the page table
       		if (gotoPageTable(page_number,PAGE) == 1)
       		{
       			pagehit++;

            //Use LRU to update TLB
            //if TLB is not full, update until it is full
            //0-15
            TLB_Entry=LRU(TLB_Entry,Track,TLB,PAGE,LRU_Index,page_number);
            //page hit translate to physical address
       			printf("PAGEHIT : Virtual_address is %d Physical address is %d Value is %hhd\n",virtual_address,(PAGE[page_number][0]*256+page_offset),value[page_number]);


       		}
       		else
       		{
       		//check the back and store
       			pagefault++;
       		//find the frame number to backstore
       			FrameNumber = backstore(page_number,value,page_offset);
       		//update page table
       			PAGE[page_number][0] = FrameNumber;
       			PAGE[page_number][1] = 1;
       			printf("PAGEFAULT : Virtual_address is %d Physical address is %d Value is %hhd\n",virtual_address,FrameNumber*256+page_offset,value[page_number]);
            //update the TLB
            TLB_Entry=LRU(TLB_Entry,Track,TLB,PAGE,LRU_Index,page_number);
       		}
       	}
        ++AddrNumber;

	}


       	//Statistics
        printf("Number of Translated Addresses = %d\n",AddrNumber);
       	printf("Page Fault = %d\n",pagefault );
       	printf("Page Fault Rate = %f\n", (float) pagefault/ (float) (pagefault+pagehit+TLBhit) )  ;
       	printf("TLB Hit = %d\n",TLBhit );
       	printf("TLB Hit Rate = %f\n", (float) TLBhit/ (float) (TLBhit+TLBmiss)  );




}


    