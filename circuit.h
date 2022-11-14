#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>

 /* NODE TYPE CONSTANTS */
#define INPT	1				/* Primary Input*/
#define AND   	2				/* AND       */
#define NAND 	3				/* NAND      */
#define OR  	4				/* OR        */
#define NOR 	5				/* NOR       */
#define NOT  	6				/* INVERTER  */
#define OUTPUT  7				/* OUTPUT    */ 
#define BUFF    8				/* BUFFER    */

#define MAX_NODES   25000

typedef struct list
{
   int ID;
   struct list *next;
} LIST;

typedef struct dlist_struct
{
   int ID;
   struct dlist_struct *next;
   struct dlist_struct *previous;
} DLIST;

typedef struct node
{
  int type;			//Gate Type
  int fanout_num;	//Number of fan-outs from a gate
  int fanin_num;	// Number of fan-ins to a gate
  int output;
  LIST *fanin;		//pointer to Linked list which represent the Fan-ins to a gate.
  LIST *fanout;	//pointer to Linked list which represent the Fan-outs from a gate.
 
} NODE;

/*-----------------------------------------------------------------
                  New Structures
------------------------------------------------------------------*/   

typedef struct cell
{
LIST *nets;//nets connected to that cell
int BLK,gain,lck;//block partition id 0 or 1

}CELL;

typedef struct net
{
LIST *cells;//cells connected to that nets
int Nb,Na;//Nb=number of cells connected from nets to partition B; Na=number of cells connected to partition A

}NET;

typedef struct free
{
LIST *cells;//nets connected to that cell
int bcut;//block partition id 0 or 1

}FREE;






/*---------------------------------------------------------------------------
 			Function Prototypes                                                       
---------------------------------------------------------------------------*/

int read_circuit(FILE*,NODE[MAX_NODES],LIST**,LIST**);
void insert_list(LIST **, int);  
void initialize_graph(NODE *); 
void charcat(char * ,char );  
int assign_type(char *);
void print_graph(NODE [MAX_NODES]);
void print_list(LIST *);
int count_list(LIST *); 




/*-----------------------------------------------------------------
                  New Function Prototype
------------------------------------------------------------------*/   

void adj_func( NODE[MAX_NODES], CELL[MAX_NODES], NET[MAX_NODES], int,int);
void print_noc(CELL [MAX_NODES], NODE [MAX_NODES],int);
void print_con(NET [MAX_NODES], NODE [MAX_NODES],int);
int count_gates(NODE [MAX_NODES],int);
void gain_calculate(NODE[MAX_NODES], CELL[MAX_NODES], NET[MAX_NODES], int);
int p_max(CELL [MAX_NODES], NODE [MAX_NODES],int);
void bucket_list(CELL [MAX_NODES], NODE[MAX_NODES],DLIST *,DLIST *, int,int *,int *, int);
void print_dlist(DLIST *);
void bucket_list_init(DLIST *bktA,DLIST *bktB,int);
void print_bucket_list(DLIST *bktA,DLIST *bktB, int);
void base_cell(NODE[MAX_NODES], CELL[MAX_NODES],NET con[MAX_NODES],int,DLIST *bktA,DLIST *bktB,int,int,int *,int *,int*,int *);
void move_cell(NODE [MAX_NODES], CELL [MAX_NODES], NET [MAX_NODES], int ,int);
void cut_state(NET [MAX_NODES], NODE [MAX_NODES],int ,int *);











 

