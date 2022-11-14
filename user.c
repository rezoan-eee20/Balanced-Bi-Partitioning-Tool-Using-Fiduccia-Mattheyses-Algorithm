#include "circuit.h"


int count_gates(NODE graph[MAX_NODES],int max_node_id)
{
int count=0,i;

for(i=0;i <= max_node_id;i++)
{
  if(graph[i].type!=0)
   {
     count++;
     }

}

return count;
}



void adj_func( NODE graph[MAX_NODES], CELL noc[MAX_NODES], NET con[MAX_NODES], int max_node_id, int count)
{
	int n,i,m=0;
	LIST *temp_list;
	
	for (n=0; n<= max_node_id;n++)
	{noc[n].lck=0;
		if(graph[n].type!=0)
		{
			if(m<(int)(count/2)+(count%2))
			{
				noc[n].BLK=0;
				
			}
			else 
      {
				noc[n].BLK=1;
				
			}
			m++;		
		}
	}


	for (n=0; n<= max_node_id;n++)
	{
		
		if(graph[n].type!=0)
		{
 	    temp_list=graph[n].fanout;
			while(temp_list!=NULL)
		   	{
  				insert_list(&con[n].cells,temp_list->ID);
  				insert_list(&noc[temp_list->ID].nets,n);
     	    temp_list=temp_list->next;
		   	}
			if(graph[n].fanout!=NULL)
			{
				insert_list(&con[n].cells,n);
				insert_list(&noc[n].nets,n);
			}
	 	 }
	}
  
      
   for (i = 0; i<=max_node_id; i++) //moves from 1 to 23
   {
      if(graph[i].type!=0) //working with non empty cell or gate ID
  	{
   	    temp_list=con[i].cells; //for i=0 it will 
	  	while(temp_list!=NULL)
		{
		  if (noc[temp_list->ID].BLK ==0)
		     con[i].Na++;
		  else   
		    con[i].Nb++;

		  temp_list = temp_list->next;
		}
      	}
    }


}



void gain_calculate(NODE graph[MAX_NODES], CELL noc[MAX_NODES], NET con[MAX_NODES], int max_node_id)
{
int c,F,T,Fn,Tn,n,gain;
LIST *temp_list;

for (c = 0; c<=max_node_id; c++) //moves from 1 to 23
   {
      if(graph[c].type!=0) //working with non empty cell or gate ID
  	{
	   	temp_list=noc[c].nets;
		gain=0;
		F=noc[c].BLK;
		
		while(temp_list!=NULL)
		{
		n=temp_list->ID;
		
			if(F==0)
			{
				Fn=con[n].Na;
				Tn=con[n].Nb;
			}
			else
			{
				Fn=con[n].Nb;
				Tn=con[n].Na;
			}
			if(Fn==1)
			gain++;
			if(Tn==0)
			gain--;
			noc[c].gain=gain;
			temp_list = temp_list->next;
				
		} 
		
	  	
      	}
    }
}



void move_cell(NODE graph[MAX_NODES], CELL noc[MAX_NODES], NET con[MAX_NODES], int max_node_id,int bcell)
{
int c,F,T,Fn,Tn,n,gain;
LIST *temp_noc, *temp_con;

		
		temp_noc=noc[bcell].nets;
		
		F=noc[bcell].BLK;
		T=1-F;
		noc[bcell].lck=1;
		noc[bcell].BLK=T;
		printf("Base_Cell(Move_Cell)=%d\n", bcell);
		while(temp_noc!=NULL)
		{
			n=temp_noc->ID;
			temp_con=con[n].cells;
			if(F==0)
			{
				Fn=con[n].Na;
				Tn=con[n].Nb;
			}
			else
			{//printf("F=%d\n", F);
				Fn=con[n].Nb;
				Tn=con[n].Na;
			}

			//F=noc[bcell].BLK;
			if(Tn==0)
			{
				while(temp_con!=NULL)
					{
						c=temp_con->ID;
						if(noc[c].lck==0)					
							noc[c].gain++;	
						temp_con = temp_con->next;				
					}

			}
			if(Tn==1)
			{	while(temp_con!=NULL)
					{c=temp_con->ID;
						if((noc[c].lck==0)  &&  (noc[c].BLK=T))				
							noc[c].gain--;	
						temp_con = temp_con->next;				
					}
			}
			
			//Change net distribution
			if(F==0)
			{
				con[n].Na--;
				con[n].Nb++;
			}
			else
			{
				con[n].Nb--;
				con[n].Na++;
			}
			
			

			// Check critical nets after the move
			temp_con=con[n].cells;
			if(Fn==0)
			{
				while(temp_con!=NULL)
					{c=temp_con->ID;
						if(noc[c].lck==0)					
							noc[c].gain--;	
						temp_con = temp_con->next;				
					}
			}

			if(Fn==1)
			{	while(temp_con!=NULL)
					{c=temp_con->ID;
						if((noc[c].lck==0)  &&  (noc[c].BLK==F))				
							noc[c].gain++;
						temp_con = temp_con->next;					
					}
			}
		temp_noc = temp_noc->next;		
		} 
		
	  	
      	
}


void base_cell(NODE graph[MAX_NODES], CELL noc[MAX_NODES],NET con[MAX_NODES], int max_node_id,DLIST *bktA,DLIST *bktB,int maxgainA,int maxgainB,int *bcell,int *sizeA,int *sizeB,int *cut)
{
	int n,init_cut;
	
	
	for (n=0; n<= max_node_id;n++)
	{
		if(graph[n].type!=0)
		{
			if(noc[n].BLK==0)
			{
				
				*sizeA += 1;
			}
			else 
      			{
				
				*sizeB +=1;
			}
					
		}
	}
	

	if(sizeA>sizeB)
	{
		*bcell=bktA[maxgainA].next->ID;

	}
	else if (sizeA>sizeB)
	{
		*bcell=bktB[maxgainB].next->ID;
		
	}
 	else if (sizeA==sizeB)   	
	{
		if (maxgainA>maxgainB)
		*bcell=bktA[maxgainA].next->ID;
		if (maxgainA<maxgainB)
		*bcell=bktB[maxgainB].next->ID;
	}

move_cell(graph, noc, con,max_node_id, *bcell);
cut_state(con, graph,max_node_id,cut);


}


void cut_state(NET con[MAX_NODES], NODE graph[MAX_NODES],int max_node_id,int *cut)
{
int Na,Nb,n;
*cut=0;

for(n=0;n<= max_node_id;n++)
	{
		Na=con[n].Na;
		Nb=con[n].Nb;
		if(Na!=0&&Nb!=0)
		{
			*cut+=1;

			

		}

	}


}



void update_bucket(CELL noc[MAX_NODES], NODE graph[MAX_NODES],DLIST *bktA,DLIST *bktB, int max_node_id,int *maxgainA,int *maxgainB, int pmax)
{
	int c,gain,i;
	*maxgainA=*maxgainB=0;
	DLIST *temp_dlist, *max_gain_pointer;
	
	

	for (c=0; c<= max_node_id;c++)
	{
		
		if(graph[c].type!=0)
		{	
			

			gain=noc[c].gain;
			if(noc[c].BLK==0)
			{
			if(*maxgainA<(gain+pmax))
				*maxgainA=gain+pmax;
			insert_dList(&(bktA[gain+pmax].next),c);
			}
			else {
			if(*maxgainB<(gain+pmax))
				*maxgainB=gain+pmax;
	                insert_dList(&(bktB[gain+pmax].next),c);
	                	
			}
 	    	}
	}

}



void print_noc(CELL noc[MAX_NODES], NODE graph[MAX_NODES], int max_node_id)
{
int i,count;

printf("Cellarray:\n");
printf("ID\tNet_List\tBLOCK\t\t\tGAIN\n");
for(i=0;i <= max_node_id;i++)
	{
		if(graph[i].type!=0)
		{	
			printf("%d\t",i);//Cells
			
			print_list(noc[i].nets);
			printf("\t\t%d", noc[i].BLK);
			printf("\t\t\t%d\n", noc[i].gain);
			
		}

	}

}


int p_max(CELL noc[MAX_NODES], NODE graph[MAX_NODES], int max_node_id)
{
int pmax=0;
int i,count;
for(i=0;i <= max_node_id;i++)
	{
		if(graph[i].type!=0)
		{	
			
			count = count_list(noc[i].nets);
			if(pmax<=count)
			{
			pmax=count;
			}
			
		}

	}

return pmax;
}


void bucket_list(CELL noc[MAX_NODES], NODE graph[MAX_NODES],DLIST *bktA,DLIST *bktB, int max_node_id,int *maxgainA,int *maxgainB, int pmax)
{
	int c,gain,i;
	*maxgainA=*maxgainB=0;
	DLIST *temp_dlist, *max_gain_pointer;
	//pmax=p_max(noc, graph, max_node_id);
	

	for (c=0; c<= max_node_id;c++)
	{
		
		if(graph[c].type!=0)
		{	
			

			gain=noc[c].gain;
			if(noc[c].BLK==0)
			{
			if(*maxgainA<(gain+pmax))
				*maxgainA=gain+pmax;
			insert_dList(&(bktA[gain+pmax].next),c);
			}
			else {
			if(*maxgainB<(gain+pmax))
				*maxgainB=gain+pmax;
	                insert_dList(&(bktB[gain+pmax].next),c);
	                	
			}
 	    	}
	}

}



void bucket_list_init(DLIST *bktA,DLIST *bktB,int p_max)
{	int i;
	for(i=0;i<(2*p_max+1);i++)
	{
	bktA[i].next=bktA[i].next=NULL;
	bktA[i].ID=0;
	bktB[i].next=bktA[i].next=NULL;
	bktB[i].ID=0;
	}

}


void print_bucket_list(DLIST *bktA,DLIST *bktB, int pmax)
{
	int i;
	printf("Bucket List A\n");
	printf("Index\tGain\tCells\n");
	for(i=0;i<(2*pmax+1);i++)
	{
	printf("%d\t%d\t",i,i-pmax);
	print_dlist(bktA[i].next);
	printf("\n");
	}
	printf("Bucket List B\n");
	printf("Index\tGain\tCells\n");
	for(i=0;i<(2*pmax+1);i++)
	{
	printf("%d\t%d\t",i,i-pmax);
	print_dlist(bktB[i].next);
	printf("\n");
	}

}




void print_con(NET con[MAX_NODES], NODE graph[MAX_NODES],int max_node_id)
{
int i;

printf("Netarray:\n");
printf("ID\tCell_List\tNa\tNb\n");
for(i=0;i <= max_node_id;i++)
	{
		if(graph[i].type!=0)
		{	printf("%d\t",i);
			print_list(con[i].cells);
			printf("\t\t%d\t%d\n", con[i].Na,con[i].Nb);
		}
	}

}
