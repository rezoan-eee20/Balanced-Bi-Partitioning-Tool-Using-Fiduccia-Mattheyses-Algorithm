#include "circuit.h"
 
int main(int argc, char *argv[])
{

  	FILE	*fp_bench; //bench file
  	NODE	graph[MAX_NODES];// 

	CELL	noc[MAX_NODES];
	NET	con[MAX_NODES];

	
  	LIST	*in=NULL, *out=NULL;
	int	max_node_id=0,count=0, pmax, maxgainA,maxgainB,sizeA,sizeB,bcell,init_cut,cut;
  
	if(argc!=2)
  	{
     	printf("Insufficient arguments\n");
     	exit(0);
  	}
  
   fp_bench=fopen(argv[1],"r");
   max_node_id = read_circuit(fp_bench, graph, &in, &out);
   print_graph(graph);
   printf("MaxNodeID=%d\n", max_node_id);
   count=count_gates(graph,max_node_id);
   printf("Gate No:%d\n", count);
   adj_func( graph, noc, con, max_node_id,count);
   gain_calculate(graph, noc, con, max_node_id);	
   print_noc(noc, graph,max_node_id);
   print_con(con,graph, max_node_id);
   cut_state(con, graph,max_node_id,&cut);
   printf("Init_Cut=%d\n", cut);
   pmax = p_max(noc, graph,max_node_id);
   DLIST bktA[2*pmax+1], bktB[2*pmax+1];
   bucket_list_init(bktA,bktB,pmax);
   bucket_list(noc, graph, bktA, bktB, max_node_id,&maxgainA,&maxgainB, pmax);
   print_bucket_list(bktA,bktB,pmax);
   base_cell(graph, noc,con,max_node_id,bktA,bktB,maxgainA,maxgainB,&bcell,&sizeA,&sizeB,&cut);
   printf("CellA=%d\n", sizeA);
   printf("CellB=%d\n", sizeB);
   printf("Base_Cell=%d\n", bcell);
   print_noc(noc, graph,max_node_id);
   print_con(con,graph, max_node_id);
   printf("Cut=%d\n", cut);
}
























  
