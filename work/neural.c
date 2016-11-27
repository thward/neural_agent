// Attach input/ output patterns to input/output layers.

// The following source contains functions suitable for the 
// processing of backpropagation neural networks.
//
// It relies on 2 main structures; the node list (the nodes in 
// a network) and the connection list (the weights between the nodes).
// The connection list can be thought of as sub-object(s) of the 
// node list. There is a one to many relationshsip between
// nodes and their connections. A node may only appear once in the node 
// list. A node may feature many times in the connection list
// but a connection between a given source - target combination will appear
// only once (for now at least). The node list is a two way linked list, this 
// allows forward propagation to start at the head of the list and work forwards, 
// and back propagation to start at the tail and work backwards.
// The connection list is a one way linked list, traversal of this list always 
// starts at the head and works forward.
//
// 1) The sequence of connections in the connection list is not important.
// 2) The sequence of nodes in the node list assumes that 
// each layer of the network will have normal nodes first followed by 
// bias nodes (which will feed into the next layer).
//
// The node  sequence is CURRENTLY IMPORTANT when we are loading in 
// input/output patterns. We assume that input patterns map onto
// the first set of nodes in the sequence and that output patterns 
// the last in the sequence. 
// 
// Also..
// We do not bother to check activation of nodes in input layer,
// this is assumed to be derived from the input pattern. For recursive 
// networks, and generic advantages, we should perhaps do an explicit check 
// to see if the node is a target (and has source nodes).

// Momentum, temperature and threshold are not even used ???!!!

// ****** The use of node_num_p is bit specific, can we get rid of it?

// ****** Not sure why presentInputPattern returns the type it does (it is a parameter also !)

// ****** readInputPatternFile/readOutputPatternFile and printInputPatterns/printOutputPatterns can surely be merged

// ****** Need to add error to node structure

// ****** Maybe rename node/node to input_activation/output_activation so 
//        consistent with node structure

// ****** Merge presentInputPattern & calculateOutputError

// - We have put in a lot of effort here to determine what the network looks like from 
// minimal amount of info supplied. Perhaps we should not have done this here if at all. 
// It saves the user from specifying all the nodes and the connections, but it not normalised.
// Maybe separate out this functionality into a pre-processing function.
// USEFUL ENHANCEMENT: 
// -- save connections to connections.txt at end of processing
// -- when launching program if connections.txt provided use that else createRandomWeights
//
// - There is no neccesity to record the fact that a node is a bias node. All node activations
// initialise as specified in nodes.txt. Bias node always remain at this level of activation because they have no inputs 
// (see forward propagation function), so the defination of a bias node is really just the fact 
// that it has no inputs. However specifying that it is a bias node speeds up processing considerably
// because we do not then have to do the check to see if it has any inputs (or rather source nodes).


#include           <stdio.h>
#include           <stdlib.h>
#include           <float.h>
#include           <math.h>
#include           "calcpat.h"
//#include           <conio.h>

//For Bluetooth..
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include<fcntl.h>
#include<termios.h>


struct linked_list_structure {
    struct  linked_list_structure *next;
};
typedef struct linked_list_structure LINKED_LIST_TYPE;
typedef LINKED_LIST_TYPE *LINKED_LIST_TYPE_P;



NODE_TYPE_P node_head,node_tail;



PATTERN_TYPE_P input_pattern_head, input_pattern_tail;
PATTERN_TYPE_P output_pattern_head, output_pattern_tail;

int 	CURRENT_PATTERN;
int	CONTINUE;
int	iteration = 0;
int	repeatCount = 0;

float	LEARNING_RATE;
float	MOMENTUM;
float	THRESHOLD;
float	TEMPERATURE;
int	ITERATIONS;
int	LOG_LEVEL = 1;
int     blueToothSerialConnection=0;
char 	BLUETOOTH_ENABLED;
char 	LEARNING_MODE;
char 	REINFORCEMENT_LEARNING = 'b';


// How many layers of nodes - MAX?
int     number_of_layers;

// A row exists for each layer in what will be an array..
struct NODE_NUM_STRUCT {
    int     node_num_entry; // Normal nodes in this layer
    int     node_num_total; // Normal + Bias nodes in this layer
};

typedef struct NODE_NUM_STRUCT NODE_NUM_TYPE;
typedef NODE_NUM_TYPE *NODE_NUM_TYPE_P;
NODE_NUM_TYPE_P node_num_p;

// How many nodes there are altogether..
int     total_num_nodes;

int     inputPatternElementSize = 0;
int     inputPatternSetSize = 0;

int number_of_input_nodes, number_of_output_nodes;

float logisticActivationFunction(float input);

int patternElementLearnt(float desired, float actual);



//---------------------------------------------------------------------------
//{***************************************}/
void readNetFile()
{
    int i;
    // The network architecture file..
    net_file_p = fopen("netfile.txt","r");
    
    // The network file has the following format (tab delimited);
    // learning rate, momentum, threshold, temperature <newline>
    // number of layers <newline>
    // nodes per layer 1 <newline>
    // ..
    // nodes per layer n <newline>
    //
    // Suggested network variables settings..
    //    float	LEARNING_RATE = 1.0;
    //    float	MOMENTUM  =	0.6;
    //    float	THRESHOLD =	0.01;
    //    float	TEMPERATURE	= 0.2;
    fscanf(net_file_p,"%f",&LEARNING_RATE);
    fscanf(net_file_p,"%f",&MOMENTUM);
    fscanf(net_file_p,"%f",&THRESHOLD);
    fscanf(net_file_p,"%f",&TEMPERATURE);
    fscanf(net_file_p,"%i",&ITERATIONS);
    fscanf(net_file_p,"%i",&LOG_LEVEL);
    fscanf(net_file_p,"%s",&BLUETOOTH_ENABLED);
    fscanf(net_file_p,"%s",&LEARNING_MODE);
    fscanf(net_file_p,"%s",&REINFORCEMENT_LEARNING);
    
    
    fclose(net_file_p);
}
//{***************************************}/
void readNodesFile()
{
    
    // This routine expects the input file to be in the following 
    // order..
    // 
    // node layer<tab>node number<tab>Bias node Y/N<tab>activation
    //
    // Note; activation is set here because for bias node it is fixed and must be specified at creation.
    // 
    // eg
    // 0       0       N	0.0
    // 0       1       N	0.0
    // 0       2       N	0.0
    // 0       3       Y	0.9
    // 0       4       Y	0.9
    // 
    
    char first_node = 'Y';
    int node_layer, node_number;
    NODE_TYPE_P node_current;
    char node_type = 'N';
    float node_activation;
    char proceed_1 = 'Y';
    
    
    // The input file..
    file_p = fopen("nodes.txt","r");
    
    //    fscanf(input_file_p,"%f",&value);
    while ((fscanf(file_p,"%d",&node_layer)) != EOF)
    {
        if (first_node == 'Y')
        {
            node_head = (NODE_TYPE *)malloc(sizeof(NODE_TYPE));
            node_tail = node_head;
            node_tail -> prev = NULL;
            
            first_node = 'N';
        }
        else
        {
            node_tail -> next = (NODE_TYPE *)malloc(sizeof(NODE_TYPE));
            node_tail -> next -> prev = node_tail;
            node_tail = node_tail->next;
        }
        
        fscanf(file_p,"%d",&node_number);
        fscanf(file_p,"%s",&node_type);
        fscanf(file_p,"%f",&node_activation);
        
        
        node_tail ->node_layer = node_layer;
        node_tail ->node_number = node_number;
        node_tail ->node_summed_input = 0.0;
        node_tail ->node_delta = 0.0;
        node_tail ->node_type = node_type;
        node_tail ->node_activation = node_activation;
	node_tail ->node_tminus1_activation = node_activation;
        node_tail ->next = NULL;
        
    }
    
    
    // Calculate number of nodes in each layer after we have read in entire node file..
    // Assume number of layers is one more than the last layer number (which starts at zero).
    number_of_layers = node_layer +1;
    node_num_p = (NODE_NUM_TYPE *)calloc(number_of_layers,sizeof(NODE_NUM_TYPE));
    
    
    node_current = node_head;
    
    while (proceed_1 == 'Y')
    {
        
        if (node_current->node_type=='N')
        {
            node_num_p[node_current->node_layer].node_num_entry = node_current->node_number +1;
        }
        node_num_p[node_current->node_layer].node_num_total = node_current->node_number +1;
        
        if (node_current -> next == NULL)
        {
            proceed_1 = 'N';
        }
        else
        {
            node_current = node_current -> next;
        }
    }
    
    
    fclose(file_p);
}
//{***************************************}/
void printNodeNum()
{
    int i;
    
    fprintf(log_file_p,"%s%d\n","Number of layers: ",number_of_layers);
    
    for (i=0; i < number_of_layers; ++i){
        fprintf(log_file_p,"%s%d %s%d\n","Layer: ",i,"Number of (non-bias) nodes: ", node_num_p[i].node_num_entry);
        fprintf(log_file_p,"%s%d %s%d\n","Layer: ",i,"Number of (non-bias + bias) nodes: ", node_num_p[i].node_num_total);
    }
    
}
//{***************************************}/
int calculateTotalNumNodes ()
{
    int i, total_num = 0;
    
    for (i=0; i < number_of_layers; ++i){
        total_num = total_num + node_num_p[i].node_num_total;
    }
    return total_num;
}
//{***************************************}/
void printNodeList()
{
    
    int x=0, y=0;
    NODE_TYPE_P node_current;
    
    node_current = node_head;
    
    fprintf(log_file_p,"\n%s\n","Node Details: ");
    
    for (x=0; x < number_of_layers; ++x)
    {
        for (y=0; y < node_num_p[x].node_num_total; ++y)
        {
            fprintf(log_file_p,"%s ","<");
            fprintf(log_file_p,"%s%d ","Layer: ",node_current -> node_layer);
            fprintf(log_file_p,"%s%d ","Node: ",node_current -> node_number);
            fprintf(log_file_p,"%s%10.3f ","Input: ", node_current -> node_summed_input);
            fprintf(log_file_p,"%s%10.3f ","Act: ", node_current -> node_activation );
	    fprintf(log_file_p,"%s%10.3f ","t-1: ", node_current -> node_tminus1_activation );
            //fprintf(log_file_p,"%s%10.3f ","Delta: ", node_current -> node_delta );
            fprintf(log_file_p,"%s%c ","Type: ",node_current -> node_type);
            fprintf(log_file_p,"%s ",">");
            node_current = node_current -> next;
        }
        fprintf(log_file_p,"\n");
    }
}

//************************************************************
float activationFunction(float input)
{
    float AF;
    
// Logistic Activation Function

    AF = (1/(1+exp(( - input )/TEMPERATURE)));
    //???	LAF = (1/(1+exp(-(activation_supplied-THRESHOLD)/TEMPERATURE)));

    
    
    return AF;
}

//{***************************************}
int readConnectionsFile()
{
    
    // This routine expects the file to be in the following 
    // order..
    // 
    // source layer<tab>source node<tab>target layer<tab>target node<tab> weight
    // 
    // eg
    // 0       0       1       0       0.43 
    // 0       0       1       1       -0.03 
    // 
    
    char first_element = 'Y';
    int source_layer, source_node, target_layer, target_node;
    float weight;
    
    // The input file..
    file_p = fopen("connections.txt","r");
    
    if(!file_p)
      return 0;
    else{
    //    fscanf(input_file_p,"%f",&value);
      while ((fscanf(file_p,"%d",&source_layer)) != EOF)
      {
        if (first_element == 'Y')
        {
            connect_head = (CONNECT_TYPE *)malloc(sizeof(CONNECT_TYPE));
            connect_tail = connect_head;
            connect_tail ->next = NULL;
            
            first_element = 'N';
        }
        else
        {
            connect_tail -> next = (CONNECT_TYPE *)malloc(sizeof(CONNECT_TYPE));
            connect_tail = connect_tail->next;
        }
        
        fscanf(file_p,"%d",&source_node);
        fscanf(file_p,"%d",&target_layer);
        fscanf(file_p,"%d",&target_node);
        fscanf(file_p,"%f",&weight);
        
        connect_tail ->connect_source_layer = source_layer;
        connect_tail ->connect_source_node = source_node;
        connect_tail ->connect_target_layer= target_layer;
        connect_tail ->connect_target_node= target_node;
        connect_tail ->connect_weight = weight;
        connect_tail ->next = NULL;
        
      }
    }
    fclose(file_p);
    return 1;
}
//{***************************************}/
createRandomWeights()
{
    // Autogenerate connections with weights:
    //
    // NORMAL source node in current layer connects to all NORMAL target nodes in subsequent layer
    // BIAS source node in current layer connects to next NORMAL target node in subsequent layer (that does not already have a bias node connected to it)
    // Note; each target normal node has only one source bias node assigned to it.
    //
    // All weights random between -0.5..+0.5
  
  
    // source node list 
    // target node list
    // lastAssignedTarget
    
    NODE_TYPE_P node_source_current, node_target_current;
    
    char proceed_1, proceed_2; 
    
    int lastBiasTargetFoundNodeLayer = -1;
    int lastBiasTargetFoundNodeNum = -1;
    
    float i_rand;
    float i_float;
    
    node_source_current = node_head;
  
  
    // The output file..
    file_p = fopen("connections.txt","w");
    
    proceed_1 = 'Y';
    
    // loop source node list while source layer less than number of layers
    while (proceed_1 == 'Y'){
      
      if(node_source_current -> node_type=='N')
      // if normal source node
      {
	// loop target node list and create a connection for every NORMAL target node in the next layer
	node_target_current = node_head;
	proceed_2 = 'Y';
	
	while (proceed_2 == 'Y'){
	  
	  if ((node_target_current ->node_layer ==  (node_source_current ->node_layer + 1 ))
	    && (node_target_current->node_type =='N' || node_target_current->node_type =='R') ){
	    fprintf(file_p,"%s%d ","",node_source_current ->node_layer);
	    fprintf(file_p,"%s%d "," ",node_source_current->node_number );
	    fprintf(file_p,"%s%d "," ",node_target_current->node_layer);
	    fprintf(file_p,"%s%d "," ",node_target_current->node_number);
	    i_rand = rand() % 100;     	// 0..100
	    i_float = (i_rand / 100) - 0.5 ; 	// -0.5..+0.5
	    fprintf(file_p,"%s%10.3f "," ", i_float); //ToDo - randomise
	    fprintf(file_p,"\n");
	  }
	  
	  if (node_target_current -> next == NULL  )
	    proceed_2 = 'N';
	  else
	    node_target_current = node_target_current -> next;
	}
      }
      else if(node_source_current -> node_type=='B')
      // if bias source node
      {
	// loop target node list and create a connection for one (ie next unassigned) normal target node in the subsequent layer
	node_target_current = node_head;
	proceed_2 = 'Y';
	
	char currBiasTargetFound = 'N'; 
	
	while (proceed_2 == 'Y'){
	  
	  if (
	    (node_target_current ->node_layer ==  (node_source_current ->node_layer + 1 ))
	    && (node_target_current->node_type =='N'|| node_target_current->node_type =='R') 
	    && ((node_target_current ->node_layer > lastBiasTargetFoundNodeLayer) || 
	      (node_target_current ->node_layer == lastBiasTargetFoundNodeLayer && node_target_current ->node_number > lastBiasTargetFoundNodeNum))
	    ){
	    fprintf(file_p,"%s%d ","",node_source_current ->node_layer);
	    fprintf(file_p,"%s%d "," ",node_source_current->node_number );
	    fprintf(file_p,"%s%d "," ",node_target_current->node_layer);
	    fprintf(file_p,"%s%d "," ",node_target_current->node_number);
 	    i_rand = rand() % 100;     	// 0..100
	    i_float = (i_rand / 100) - 0.5 ; 	// -0.5..+0.5
	    fprintf(file_p,"%s%10.3f "," ", i_float); //ToDo - randomise
	    fprintf(file_p,"\n");
	  
	    lastBiasTargetFoundNodeLayer = node_target_current->node_layer;
	    lastBiasTargetFoundNodeNum = node_target_current->node_number;
	  
	    currBiasTargetFound = 'Y'; 
	  }
	  
	  if (node_target_current -> next == NULL  || currBiasTargetFound == 'Y')
	    proceed_2 = 'N';
	  else
	    node_target_current = node_target_current -> next;
	}

      }
      
      if (node_source_current -> next == NULL || node_source_current->next->node_layer == (number_of_layers -1) )
          proceed_1 = 'N';
      else
          node_source_current = node_source_current -> next;

      
    }
    
    fclose(file_p);

}

//{***************************************}/
void saveConnections()
{
  
    // This routine expects the file to be in the following 
    // order..
    // 
    // source layer<tab>source node<tab>target layer<tab>target node<tab> weight
    // 
    // eg
    // 0       0       1       0       0.43 
    // 0       0       1       1       -0.03 
    // 
    
    char proceed_1 = 'Y';
    
    CONNECT_TYPE_P connect_pointer;
    connect_pointer = connect_head;
    
    file_p = fopen("connections_out.txt","w");
    
    // Main processing loop..
    // Print out entries from the topography array..
    while (proceed_1 == 'Y')
    {
        fprintf(file_p,"%s%d ","",connect_pointer -> connect_source_layer);
        fprintf(file_p,"%s%d "," ",connect_pointer -> connect_source_node);
        fprintf(file_p,"%s%d "," ",connect_pointer -> connect_target_layer);
        fprintf(file_p,"%s%d "," ",connect_pointer -> connect_target_node);
        fprintf(file_p,"%s%10.3f "," ",connect_pointer -> connect_weight);
        fprintf(file_p,"\n");
        
        if (connect_pointer -> next == NULL)
        {
            proceed_1 = 'N';
        }
        else
        {
            connect_pointer = connect_pointer -> next;
        }
    }
    fclose(file_p);
}

//{***************************************}/
void printConnections()
{
    char proceed_1 = 'Y';
    
    CONNECT_TYPE_P connect_pointer;
    connect_pointer = connect_head;
    
    fprintf(log_file_p,"\n%s\n","Connection Details: ");
    
    // Main processing loop..
    // Print out entries from the topography array..
    while (proceed_1 == 'Y')
    {
        fprintf(log_file_p,"%s%d ","Source layer: ",connect_pointer -> connect_source_layer);
        fprintf(log_file_p,"%s%d ","Source node: ",connect_pointer -> connect_source_node);
        fprintf(log_file_p,"%s%d ","Target layer: ",connect_pointer -> connect_target_layer);
        fprintf(log_file_p,"%s%d ","Target node: ",connect_pointer -> connect_target_node);
        fprintf(log_file_p,"%s%10.3f ","Weight: ",connect_pointer -> connect_weight);
        fprintf(log_file_p,"\n ");
        
        if (connect_pointer -> next == NULL)
        {
            proceed_1 = 'N';
        }
        else
        {
            connect_pointer = connect_pointer -> next;
        }
    }
}

//{***************************************}/
void readInputPatternFile()
{
    
    // This routine expects the input file to be in the following 'triplet' 
    // order..
    // 
    // layer<tab>node<tab>value<tab>  layer<tab>node<tab>value ..etc
    // 
    //	eg
    // 0	0	0.0	0	1	0.0		<input pattern 1>
    // 0	0	0.0	0	1	1.0		<input pattern 2>
    // 
    // The layer will in most circumstances be the input layer(0)
    // but need not be.
    // Specifying the layer & node allows coding to be more generic, and
    // will offer more flexibility (the user may actually load activations
    // onto hidden units if they so wish.	.
    
    char first_node = 'Y';
    int layer, node;
    float value;
    
    // The input file..
    file_p = fopen("input.txt","r");
    
    //    fscanf(input_file_p,"%f",&value);
    while ((fscanf(file_p,"%d",&layer)) != EOF)
    {
        if (first_node == 'Y')
        {
            input_pattern_head = (PATTERN_TYPE *)malloc(sizeof(PATTERN_TYPE));
            input_pattern_tail = input_pattern_head;
            input_pattern_tail -> prev = NULL;
            
            first_node = 'N';
        }
        else
        {
            input_pattern_tail -> next=(PATTERN_TYPE *)malloc(sizeof(PATTERN_TYPE));
            input_pattern_tail -> next -> prev = input_pattern_tail;
            input_pattern_tail = input_pattern_tail -> next;
        }

        fscanf(file_p,"%d",&node);
        fscanf(file_p,"%f",&value);

        input_pattern_tail -> next = NULL;
        input_pattern_tail -> layer = layer;
        input_pattern_tail -> node  = node;
        input_pattern_tail -> value = value;

        inputPatternElementSize++;
    }
    
    fclose(file_p);
}



//{***************************************}/
void readOutputPatternFile()
{
    
    // This routine expects the input file to be in the following 'triplet' 
    // order..
    // 
    // layer<tab>node<tab>value<tab>  layer<tab>node<tab>value ..etc
    // 
    //	eg
    // 2	0	0.0		<output pattern 1>
    // 2	0	1.0		<output pattern 2>
    // 
    
    // Currently these 2 output functions are identical to the input 
    // pattern ones... they could be integrated ?
    
    char first_node = 'Y';
    int layer, node;
    float value;
    
    // The input file..
    file_p = fopen("output.txt","r");
    
    while ((fscanf(file_p,"%d",&layer)) != EOF)
    {
        if (first_node == 'Y')
        {
            output_pattern_head = (PATTERN_TYPE *)malloc(sizeof(PATTERN_TYPE));
            output_pattern_tail = output_pattern_head;
            output_pattern_tail -> prev = NULL;
            
            first_node = 'N';
        }
        else
        {
            output_pattern_tail -> next=(PATTERN_TYPE *)malloc(sizeof(PATTERN_TYPE));
            output_pattern_tail -> next -> prev = output_pattern_tail;
            output_pattern_tail = output_pattern_tail -> next;
        }

        fscanf(file_p,"%d",&node);
        fscanf(file_p,"%f",&value);

        output_pattern_tail -> next = NULL;
        output_pattern_tail -> layer = layer;
        output_pattern_tail -> node  = node;
        output_pattern_tail -> value = value;

    }
    fclose(file_p);
}

//************************************************************
PATTERN_TYPE_P printPattern(PATTERN_TYPE_P pattern_pointer, int pattern_size, char input_pattern)
{
    
    int i = 0;
    
    if (input_pattern == 'Y'){
        fprintf(log_file_p,"\n%s\n","Input Pattern : ");
    }
    else if (input_pattern == 'N'){
        fprintf(log_file_p,"\n%s\n","Output Pattern : ");
    }
    else{
        // do nothing
    }
        
    while (i < pattern_size)
    {
        fprintf(log_file_p,"%d ",pattern_pointer -> layer);
        fprintf(log_file_p,"%d ",pattern_pointer -> node);
        fprintf(log_file_p,"%10.3f",pattern_pointer -> value);
        fprintf(log_file_p,"\n");
        
        pattern_pointer = pattern_pointer -> next;
        i++;
    }
    
    return pattern_pointer;
}
//************************************************************
void printAllPatterns(PATTERN_TYPE_P pattern_pointer, int pattern_size, char input_pattern, int patternSetSize)
{
     int i = 0;
    
    if (input_pattern == 'Y'){
        fprintf(log_file_p,"\n%s\n","Input Patterns : ");
    }
    else{
        fprintf(log_file_p,"\n%s\n","Output Patterns : ");
    }
        
    while (i < patternSetSize)
    {
        pattern_pointer = printPattern(pattern_pointer, pattern_size, 'x');
        i++;
    }
    
}

//************************************************************
PATTERN_TYPE_P getNextPattern(PATTERN_TYPE_P pattern_subhead, PATTERN_TYPE_P pattern_head, int size_of_pattern)
{
    int x = 0;
    
    for (x=0; x < size_of_pattern; ++x)
    {			
        // Get the next part of the input pattern..
        if (pattern_subhead -> next != NULL)
        {
            pattern_subhead = pattern_subhead -> next;
        }
        else
        {
            pattern_subhead = pattern_head;
        }
    }
    
    return pattern_subhead;
    
}
//************************************************************
void loadInputPattern(PATTERN_TYPE_P input_pattern_current_head, int size_of_pattern)
{
    
    int x = 0;
    NODE_TYPE_P node_curr;
    PATTERN_TYPE_P input_pattern_element;
    int found = 0;
    float tminus1activation;
    
    input_pattern_element = input_pattern_current_head;
    
    for (x=0; x < size_of_pattern; ++x)
    {			
        // Search through the node list until
        // we find the correct one..
        
        found = 0;
        
        for (node_curr = node_head; found == 0; node_curr = node_curr -> next)
        {
            if ((node_curr -> node_layer == input_pattern_element -> layer)&&
                (node_curr -> node_number ==input_pattern_element -> node))
            {
                // We have found it, so lets stop looking..
                node_curr -> node_summed_input = input_pattern_element -> value;
                tminus1activation = node_curr -> node_activation;
                node_curr -> node_tminus1_activation = tminus1activation;
		 //node_curr -> node_activation = node_curr -> node_summed_input;
		 node_curr -> node_activation = activationFunction(node_curr -> node_summed_input);
                
                found = 1;
            }
        }
        
        // Get the next part of the input pattern..
        if (input_pattern_element -> next != NULL)
        {
            input_pattern_element = input_pattern_element -> next;
        }
        else
        {
            //error !
        }
        
    }
}

//************************************************************

forwardPropagate()
{
    
    // Our aim is to cycle through the list of nodes and assign
    // activations. 
    // We do not need to bother with nodes in the input layer (0) because
    // their activation has been assigned by the input pattern. We do
    // not need to bother with bias nodes since their activation is currently 
    // assumed to be fixed (1.0)
    
    // NOTE: We will be passing the output of input and bias nodes through the 
    // logistic activation function. This was not carried out in earlier models 
    // (which worked) but I can see no reason not to do so. The network 
    // will be more consistent and the algorithm more generic.
    
    char proceed_1 = 'Y', proceed_2 = 'Y', proceed_3 = 'Y';
    NODE_TYPE_P node_current, node_source_current;
    CONNECT_TYPE_P connect_current;
    float source_output = 0.0, summed_input = 0.0;
    float tminus1activation = 0;
    
    
    node_current = node_head;
    proceed_1 = 'Y';
    // Reads forward through node list until the last...
    while (proceed_1 == 'Y')
    {
      summed_input = 0;
      
      if (node_current ->node_layer == 0) 
      {// input layer
	
	// already applied Logistic Activation Function to get activation in function loadInputPattern
	
      }// input layer
      else 
      {// not input layer
	
        connect_current = connect_head;
        
        proceed_2 = 'Y';
        while (proceed_2 == 'Y')
        {
            // Read through connection list for this target looking 
            // for source nodes that supply it. If none are found that
            // means it is a bias node or it is in the input layer (0)
            // so can be ignored. Skip this and get the next node  
            // instead.
            // Basically only bother to do anything if connect target
            // is equal to current node ..
            if ((connect_current -> connect_target_layer
                ==node_current ->node_layer )&&
                (connect_current ->connect_target_node 
                ==node_current ->node_number))
            {
                
                // Found a connection. Must now get activation of 
                // the source node (!)..
                node_source_current = node_head;
                proceed_3 = 'Y';
                while (proceed_3 == 'Y')
                {
                    // There can and must be only one entry on the node 
                    // list so grab value and stop looking.
                    if ((connect_current -> connect_source_layer
                        ==node_source_current -> node_layer )
                        &&(connect_current ->connect_source_node 
                        ==node_source_current ->node_number))
                    {
                        // The activation is actually that of the source
                        // node passed through the Logistic Activation Function..
                        
                        // For REINFORCEMENT_LEARNING apply a threshold..
                        //Adding a threshold is not mandatory, but it massively improves performance for RewardLearning.
                        //We have to place the threshold here (after the activation has been calculated) to preserve the activation value if it is below 0.5. If we don't then sigmoid activation function does not work (but flat active still does work).
                        //Adding threshold here may make final output 0.5 for sigmoid activation function, becuase it has set input 
                        // to zero therefore output is 0.5. Also we have not applied rule to final output layer so 0.5 has
                        // not been transformed to zero.

                        if (REINFORCEMENT_LEARNING == 'r')
			{
			  if (node_source_current -> node_activation > 0.5)
			  {
			    summed_input = (summed_input + 
                           (node_source_current -> node_activation * connect_current ->connect_weight));
			  }
			  else
			  {
			    // add nothing
			  }
			}
			else
			{
                        summed_input = (summed_input + 
                        (node_source_current -> node_activation * connect_current ->connect_weight));
			}
                        
                        proceed_3 = 'N';
                    }
                    if (node_source_current -> next == NULL)
                        // if not found just stop looking..
                        proceed_3 = 'N';
                    else if (proceed_3 == 'Y')
                        // otherwise carry on looking..
                        node_source_current = node_source_current -> next;
                }
                
                //??? node_activation code used to go here !!
      
                
            }
            
            
            if (connect_current -> next == NULL)
                // if not found just stop looking..
                proceed_2 = 'N';
            else
                // otherwise carry on looking..
                connect_current = connect_current -> next;
            
	}// not input layer
	
	if (node_current->node_type != 'Y') // Leave Bias node activations alone..
	{
	  tminus1activation = node_current -> node_activation;
	  node_current -> node_tminus1_activation = tminus1activation;
	  node_current -> node_summed_input = summed_input;
	  node_current -> node_activation = activationFunction(node_current -> node_summed_input);
	}
	
      }

      if (node_current -> next == NULL)
          proceed_1 = 'N';
      else
          node_current = node_current -> next;
        
    }
    
    return 0;
    
}


//************************************************************
float calculateError(float desired, float actual)
{
    float ERROR;
    // (desired - actual) 		: this calculates error
    // actual * (1 - actual)		: this 'normalises' the error mid range values (eg 0.5) are increased the most but low 
    // and high values the least (or virtually not at all!). Presumably so that massive weights are not increased more.
    // eg
    // 0.5 * (1-0.5) = 0.25
    // 0.9 * (1-0.9) = 0.09
    // 0.1 * (1-0.1) = 0.09
    //ERROR = (desired - actual) * actual * (1 - actual);
    // Just for fun .. 
    ERROR = (desired - actual) *  0.20; // This actually appears to work, but not tested with XOR
    
    
    return ERROR;
}
//************************************************************
int patternElementLearnt(float desired, float actual)
{
    if (fabs(desired - actual)< 0.1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
    
}
//************************************************************
int backwardPropagate(PATTERN_TYPE_P output_pattern_desired_tail, int size_of_pattern, char bTminus1)
{
    
    NODE_TYPE_P node_current, node_current_source, node_current_target;
    int found_source, found_target, x;
    PATTERN_TYPE_P output_pattern_element;
    CONNECT_TYPE_P connect_current;
    float summedDelta;
    char continue_connect_search, continue_node_target_search;
    // Assume the pattern was learnt unless told otherwise..
    int patternLearnt = 1;
    
    
    output_pattern_element = output_pattern_desired_tail;
    
    // 1) Calculate delta for output and hidden nodes (do not bother with input nodes) 
    // From the tail of the node list work backwards..
    for ((node_current = node_tail);(node_current -> prev != NULL) ; (node_current = node_current -> prev))
    {
        // A) If output node delta value assignment is fairly simple..
        if (node_current -> node_layer ==  (number_of_layers -1) )
        {
            // For the current node loop find relevant element in current output pattern and caclulate delta..
            for (x=0; x < size_of_pattern; ++x)
            {                   
                
                if ((node_current -> node_layer == output_pattern_element -> layer)&&
                    (node_current -> node_number ==output_pattern_element -> node))
                {
                    // We have found it, so lets stop looking..
                    if (bTminus1 == 'N'){
		      node_current -> node_delta =   calculateError (output_pattern_element -> value, node_current -> node_activation);
		    }
		    else{
		      node_current -> node_delta =   calculateError (output_pattern_element -> value, node_current -> node_tminus1_activation);
		    }
                    
                    // If any pattern element was not learnt then the whole pattern fails..
                    /*if (patternElementLearnt(output_pattern_element -> value, node_current -> node_activation)==0)
                    {
                        patternLearnt=0;
                    } */

                }
                if (x < (size_of_pattern -1)){
                    // Get the next part of the input pattern..
                    output_pattern_element = output_pattern_element -> prev;
                    // Get the next node..
                    node_current = node_current -> prev;
                }
                
            }
            
        }
        // B) Skip assignment of delta values for nodes in input layer
        else if (node_current -> node_layer ==  0) 
        {
            //Do nothing !
            
        }
        // C) If hidden node must multiply the activation of this node by the sum of the target node deltas by their weights (pretty complex..)
        else 
        {
            summedDelta = 0;
            // loop through all weights from the head and process those with a source equal to this node..
            connect_current = connect_head;
            continue_connect_search = 'Y';
            while (continue_connect_search == 'Y')
            {
                if ((node_current->node_layer == connect_current -> connect_source_layer)&&
                    (node_current->node_number == connect_current ->connect_source_node))
                {
                    // loop through all nodes from the head and process those with a node equal to the weight target..
                    node_current_target = node_head;
                    continue_node_target_search = 'Y';
                    while (continue_node_target_search =='Y')
                    {
                        if ((node_current_target->node_layer == connect_current -> connect_target_layer)&&
                            (node_current_target->node_number == connect_current ->connect_target_node))
                        {
                           if (bTminus1 == 'N'){
			      summedDelta = summedDelta + (((node_current -> node_activation) * (1 -node_current -> node_activation))*(node_current_target->node_delta * connect_current ->connect_weight));
			    }else{
			      summedDelta = summedDelta + (((node_current -> node_tminus1_activation) * (1 -node_current -> node_tminus1_activation))*(node_current_target->node_delta * connect_current ->connect_weight));
			    }
			    
                        }
                        if (node_current_target->next != NULL)
                        {
                            node_current_target = node_current_target ->next;
                        }
                        else
                        {
                            continue_node_target_search = 'N';
                        }
                    }
                }
                node_current -> node_delta = summedDelta ;
                
                if (connect_current -> next != NULL)
                {
                    connect_current = connect_current -> next;
                }
                else
                {
                    continue_connect_search = 'N';
                }
                
            }
            
        }
        
    }
    
    // 2) Adjust weights according to delta values...
    // Loop through all connections from the head
    // Find the source node from the head
    // Find the target node from the head
    // Make the weight change
    connect_current = connect_head;
    continue_connect_search = 'Y';
    while (continue_connect_search == 'Y')
    {
        found_source = 0 ;
        found_target = 0 ;
        
        node_current_source = node_head;
        node_current_target = node_head;
        
        while (found_source == 0)
        {
            if ((node_current_source->node_layer == connect_current -> connect_source_layer)&&
                (node_current_source->node_number == connect_current -> connect_source_node))
            { 
                found_source = 1;
            }
            
            if (node_current_source -> next != NULL && found_source == 0)
            {
                node_current_source = node_current_source -> next;
            }
        }
        
        while (found_target == 0)
        {
            if ((node_current_target->node_layer == connect_current -> connect_target_layer)&&
                (node_current_target->node_number == connect_current -> connect_target_node))
            { 
                found_target = 1;
            }
            
            if (node_current_target -> next != NULL && found_target == 0)
            {
                node_current_target = node_current_target -> next;
            }
        }
        if (bTminus1 == 'N'){
	  connect_current ->connect_weight = connect_current->connect_weight + (LEARNING_RATE * node_current_target->node_delta * node_current_source->node_activation);
	}else{
	  connect_current ->connect_weight = connect_current->connect_weight + (LEARNING_RATE * node_current_target->node_delta * node_current_source->node_tminus1_activation);
	}
        
        if (connect_current -> next != NULL)
        {
            connect_current = connect_current -> next;
        }
        else
        {
            continue_connect_search = 'N';
        }
        
    }
    
    return patternLearnt;
}
//************************************************************
void rewardLearning(float REWARD, char bTminus1)
{
    
    float desired;
    float i_rand;
    float i_float;
    
    
 
    NODE_TYPE_P node_current_source, node_current_target;
    int found_source, found_target, x;
    
    CONNECT_TYPE_P connect_current;
    
    char continue_connect_search;

    // Loop through all connections from the head
    // Find the source node from the head
    // Find the target node from the head
    // Make the weight change
    connect_current = connect_head;
    continue_connect_search = 'Y';
    while (continue_connect_search == 'Y')
    {
        found_source = 0 ;
        found_target = 0 ;
        
        node_current_source = node_head;
        node_current_target = node_head;
        
        while (found_source == 0)
        {
            if ((node_current_source->node_layer == connect_current -> connect_source_layer)&&
                (node_current_source->node_number == connect_current -> connect_source_node))
            { 
                found_source = 1;
            }
            
            if (node_current_source -> next != NULL && found_source == 0)
            {
                node_current_source = node_current_source -> next;
            }
        }
        
        while (found_target == 0)
        {
            if ((node_current_target->node_layer == connect_current -> connect_target_layer)&&
                (node_current_target->node_number == connect_current -> connect_target_node))
            { 
                found_target = 1;
            }
            
            if (node_current_target -> next != NULL && found_target == 0)
            {
                node_current_target = node_current_target -> next;
            }
        }
        
        // *****************************************************************************************
        // 1) Calculate desired
        
        if (REWARD > 0.0 )
	{
	  
	  if (node_current_target->node_type == 'R' && bTminus1 =='Y') {
	    desired = REWARD ;
	  }
	  else
	  {
	    if (bTminus1 == 'N')
	    {
	      if (node_current_target -> node_activation <= 0.5)
	      {
		desired = 0.0;
	      }
	      else //(node_current -> node_activation > 0.5)
	      {
		desired = 1.0;
	      }
	    }
	    else
	    {
	      if (node_current_target -> node_tminus1_activation <= 0.5)
	      {
		desired = 0.0;
	      }
	      else //(node_current -> node_activation > 0.5)
	      {
		desired = 1.0;
	      }
	    }
	  }
	  
	}
	else
	{
	  if (node_current_target->node_type == 'R' && bTminus1 =='Y') {
	    //desired = 0.0 ;
	    desired = 0.65 ;
	  }
	  else
	  {
	    // number between 0.45..0.55
	    i_rand = rand() % 100;
	    desired = 0.45 + (i_rand / 1000) ;	
	    
	    if (desired <= 0.5)
	    {
	      desired = 0.0;
	    }
	    else //(node_current -> node_activation > 0.5)
	    {
	      desired = 1.0;
	    }
	  }
	}
	
        // *****************************************************************************************
        // 2) Calculate delta


	if (bTminus1 == 'N')
	{
	  node_current_target -> node_delta =   calculateError (desired, node_current_target -> node_activation);
	}
	else
	{
	  node_current_target -> node_delta =   calculateError (desired, node_current_target -> node_tminus1_activation);
	}
	
        // *****************************************************************************************
        // 3) Update weight

	
	if (bTminus1 == 'N')
	{
	  if (node_current_source->node_activation > 0.5)
	  { 
	    if (REWARD > 0.0){
	      connect_current ->connect_weight = connect_current->connect_weight + (LEARNING_RATE * node_current_target->node_delta * node_current_source->node_activation);
	    }
	    else{
	      connect_current ->connect_weight = connect_current->connect_weight + (LEARNING_RATE * node_current_target->node_delta * node_current_source->node_activation);
	    }
	  }
	}
	else
	{
	  if (node_current_source->node_tminus1_activation > 0.5)
	  { 
	    if (REWARD > 0.0){
	      connect_current ->connect_weight = connect_current->connect_weight + (LEARNING_RATE * node_current_target->node_delta * node_current_source->node_tminus1_activation);
	    }
	    else{
	      connect_current ->connect_weight = connect_current->connect_weight + (LEARNING_RATE * node_current_target->node_delta * node_current_source->node_tminus1_activation);
	    }
	  }
  
	}
	  
        
        if (connect_current -> next != NULL)
        {
            connect_current = connect_current -> next;
        }
        else
        {
            continue_connect_search = 'N';
        }
        
    }
    
    
}
//************************************************************
// Simply determines if a reward condition or not, before calling rewardLearning. Specific to LEARNING_MODE == (s)upervised
void calcRewardLearning(PATTERN_TYPE_P output_pattern_desired_tail,int size_of_pattern)
{
  float REWARD = 1.0;
  int x;
  PATTERN_TYPE_P output_pattern_element;
  NODE_TYPE_P node_current;    

  output_pattern_element = output_pattern_desired_tail;
  node_current = node_tail;
  
  
  for (x=0; x < size_of_pattern; ++x)
  {                   
    if ((output_pattern_element->value == 0.0 && node_current->node_activation > 0.5)||(output_pattern_element->value == 1.0 && node_current->node_activation <= 0.5))
    {
      REWARD = 0.0;
    }
     
    if (x < (size_of_pattern -1))
    {
      // Get the next part of the input pattern..
      output_pattern_element = output_pattern_element -> prev;
      // Get the next node..
      node_current = node_current -> prev;
    }

  }
  
  rewardLearning(REWARD,'N');
}

//************************************************************
void freeList(LINKED_LIST_TYPE_P list_current)
{
    LINKED_LIST_TYPE_P list_next;
    
    while (list_current != NULL)
    {
        list_next = list_current -> next;
        free(list_current);
        list_current= list_next;
    }
    list_current = NULL;
}
//************************************************************
supervisedLoop(char bTminus1, PATTERN_TYPE_P input_pattern_current_head, PATTERN_TYPE_P output_pattern_desired_head, PATTERN_TYPE_P output_pattern_desired_tail)
{
    int i = 0;
    
    while (iteration <= repeatCount)        
    {
        if(LOG_LEVEL > 0){
	  printPattern(input_pattern_current_head, number_of_input_nodes, 'Y');
	  printPattern(output_pattern_desired_head, number_of_output_nodes, 'N');
	}

        
        iteration = iteration + 1;
        loadInputPattern(input_pattern_current_head, number_of_input_nodes);
        
        if(LOG_LEVEL > 0){
	  fprintf(log_file_p,"\n");
	  fprintf(log_file_p,"Iteration: %d \n",iteration);
        // forward propagate
	  fprintf(log_file_p,"Forward propagate.. \n");
	}
        forwardPropagate();
	
	if(LOG_LEVEL > 0){
	  printNodeList();
	}
	
       
        // backward propagate
        if(LOG_LEVEL > 0){
	  fprintf(log_file_p,"\n");
	  fprintf(log_file_p,"Backward propagate.. \n");
	}

    //Adding a threshold here is not mandatory, but it massively improves performance.
	if (REINFORCEMENT_LEARNING == 'r')
	{
	  calcRewardLearning(output_pattern_desired_tail,number_of_output_nodes);
	}else{
	  backwardPropagate(output_pattern_desired_tail, number_of_output_nodes, bTminus1);	//Note: backwardPropagate() returns 1 if pattern set learnt
	}
        
        //debug
        if(LOG_LEVEL > 1){
        printConnections();
	}
        

	input_pattern_current_head = getNextPattern(input_pattern_current_head, input_pattern_head, number_of_input_nodes);
	output_pattern_desired_head = getNextPattern(output_pattern_desired_head, output_pattern_head, number_of_output_nodes);
	output_pattern_desired_tail = output_pattern_desired_head;
	for (i=0; i < (number_of_output_nodes -1 ); ++i){
	    output_pattern_desired_tail = output_pattern_desired_tail -> next;
	}

       
    }

}


//************************************************************

unSupervisedLoop(char bTminus1, PATTERN_TYPE_P input_pattern_current_head, PATTERN_TYPE_P output_pattern_desired_head, PATTERN_TYPE_P output_pattern_desired_tail)
{
    float reward;
    
    while (iteration <= repeatCount)        
    {
        if(LOG_LEVEL > 0){
	  printPattern(input_pattern_current_head, number_of_input_nodes, 'Y');
	  printPattern(output_pattern_desired_head, number_of_output_nodes, 'N');
	}

        
        iteration = iteration + 1;
        loadInputPattern(input_pattern_current_head, number_of_input_nodes);
        
        if(LOG_LEVEL > 0){
	  fprintf(log_file_p,"\n");
	  fprintf(log_file_p,"Iteration: %d \n",iteration);
        // forward propagate
	  fprintf(log_file_p,"Forward propagate.. \n");
	}
        forwardPropagate();
	
	if(LOG_LEVEL > 0){
	  printNodeList();
	}
	
	reward = calcDesiredInputOutputPatterns(node_tail, number_of_output_nodes,input_pattern_current_head,output_pattern_desired_head,bTminus1);
	
        if (REINFORCEMENT_LEARNING == 'r')
	{
	  rewardLearning(reward,bTminus1);
	}else{
	   // backward propagate
	  if(LOG_LEVEL > 0){
	    fprintf(log_file_p,"\n");
	    fprintf(log_file_p,"Backward propagate.. \n");
	  }
	  //Note: backwardPropagate() returns 1 if pattern set learnt
	  backwardPropagate(output_pattern_desired_tail, number_of_output_nodes, bTminus1);
	}
	
               
         //debug
        if(LOG_LEVEL > 1){
        printConnections();
	}
        
               
    }


}


//************************************************************
initialiseBluetooth()
{

    char tempbuf[30];
    int w = 0;
    int n = 0;
    int iterator = 0;
    
    struct termios tty;

    blueToothSerialConnection=open("/dev/rfcomm0",O_RDWR | O_NOCTTY);
    if(blueToothSerialConnection == -1)
    {
      printf("Unable to open /dev/rfcomm0\n");
    }
    else
    {
      if(tcgetattr(blueToothSerialConnection, &tty)!=0){
	perror("tcgetatt() error");
      }
      else{
	
	cfsetospeed(&tty, B9600);
	cfsetispeed(&tty, B9600);

	tty.c_cflag &= ~PARENB;
	tty.c_cflag &= ~CSTOPB;
	tty.c_cflag &= ~CSIZE;
	tty.c_cflag |= CS8;
	tty.c_cflag &= ~CRTSCTS; 
	tty.c_cflag |= CLOCAL | CREAD;

	tty.c_iflag |= IGNPAR | IGNCR;
	tty.c_iflag &= ~(IXON | IXOFF | IXANY);
	tty.c_lflag |= ICANON;
	tty.c_oflag &= ~OPOST;
	tcsetattr(blueToothSerialConnection, TCSANOW, &tty);

	while (iterator < 2){
	
	tcflush(blueToothSerialConnection,TCIOFLUSH);
	w=write(blueToothSerialConnection, "36", 2);/*printf("%d\n",w);
	fprintf(stderr, "fd = %d.\n", fd);*/
	usleep(500000);
	//sleep(1);
               
	strcpy(tempbuf, "                             ");
	n=read(blueToothSerialConnection,tempbuf,29);/*printf("%d \n",n);*/
	tempbuf[29]= '\0';
	printf("tempbuf: %s ",tempbuf);
		
	strcpy(tempbuf, "                             ");
	n=read(blueToothSerialConnection,tempbuf,29);/*printf("%d \n",n);*/
	tempbuf[29]= '\0';
	printf("tempbuf: %s ",tempbuf);
                
	strcpy(tempbuf, "                             ");
	n=read(blueToothSerialConnection,tempbuf,29);/*printf("%d \n",n);*/
	tempbuf[29]= '\0';
	printf("tempbuf: %s ",tempbuf);
	
	
	//sleep(1);
	
		
	tcflush(blueToothSerialConnection,TCIOFLUSH);
	w=write(blueToothSerialConnection, "25", 2);/*printf("%d\n",w);
	fprintf(stderr, "fd = %d.\n", fd);*/
	usleep(500000);
	//sleep(1);
		
	strcpy(tempbuf, "                             ");
	n=read(blueToothSerialConnection,tempbuf,29);/*printf("%d \n",n);*/
	tempbuf[29]= '\0';
	printf("tempbuf: %s ",tempbuf);
            
	strcpy(tempbuf, "                             ");
	n=read(blueToothSerialConnection,tempbuf,29);/*printf("%d \n",n);*/
	tempbuf[29]= '\0';
	printf("tempbuf: %s ",tempbuf);
		
	strcpy(tempbuf, "                             ");
	n=read(blueToothSerialConnection,tempbuf,29);/*printf("%d \n",n);*/
	tempbuf[29]= '\0';
	printf("tempbuf: %s ",tempbuf);
	
	
	iterator ++;
	}
		
		
	
		
	
		
		
      }
    }
}
//************************************************************    
    
main()
{
    
    PATTERN_TYPE_P input_pattern_current_head;
    PATTERN_TYPE_P output_pattern_desired_head;
    PATTERN_TYPE_P output_pattern_desired_tail;
    PATTERN_TYPE_P pattern_current;
    CONNECT_TYPE_P connect_current;
    void* list_current;
    
    int i = 0;
    
    // The log file..
    log_file_p = fopen("logfile.txt","w");
    
    readNetFile();
    
       
    readNodesFile();
    
    // Display the number of nodes in each layer..
    printNodeNum();
    // Calculate total number of nodes in entire network..
    total_num_nodes = calculateTotalNumNodes();
    // Print network activation list
    printNodeList();
    
    
    // Load connections..
    if (readConnectionsFile() == 0){
          fprintf(log_file_p,"\n");
	  fprintf(log_file_p,"No connections file found !! Autogenerating... \n");
	  fprintf(log_file_p,"\n");
	  createRandomWeights();
	  // Load connections..
	  if (readConnectionsFile() == 0){
	    fprintf(log_file_p,"\n");
	    fprintf(log_file_p,"ERROR: Unable to auto generate connections. \n");
	    fprintf(log_file_p,"\n");
	  }
    }
    
    // Print connections..
    printConnections();
    // Read input patterns WE ALWAYS CALL THIS, EVEN FOR UNSUPERVISED LEARNING, JUST TO CREATE STORAGE FOR THE PATTERN !!
    readInputPatternFile();
    
    // Read output patterns WE ALWAYS CALL THIS, EVEN FOR UNSUPERVISED LEARNING, JUST TO CREATE STORAGE FOR THE PATTERN !!
    readOutputPatternFile();
    
    // load the next input pattern onto the input nodes...
    // If we have reached the tail of the input patterns we will start
    // again from the head.
    input_pattern_current_head = input_pattern_head;
    number_of_input_nodes = node_num_p[0].node_num_entry;
    

    output_pattern_desired_head = output_pattern_head;
    number_of_output_nodes = node_num_p[number_of_layers - 1].node_num_total;
    output_pattern_desired_tail = output_pattern_desired_head;
    for (i=0; i < (number_of_output_nodes -1 ); ++i){
      output_pattern_desired_tail = output_pattern_desired_tail -> next;
    }

    
    inputPatternSetSize= inputPatternElementSize / number_of_input_nodes;
    fprintf(log_file_p,"\n");
    fprintf(log_file_p,"Pattern set size: %d \n",inputPatternSetSize);
    fprintf(log_file_p,"\n");
    
    printAllPatterns(input_pattern_current_head, number_of_input_nodes, 'Y',inputPatternSetSize);
    printAllPatterns(output_pattern_desired_head, number_of_output_nodes, 'N',inputPatternSetSize);

    
    iteration = 0;
    repeatCount = ITERATIONS;
    
  
    if (LEARNING_MODE=='s')
    {
      supervisedLoop('N', input_pattern_current_head, output_pattern_desired_head, output_pattern_desired_tail);
    }
    else{
	if (BLUETOOTH_ENABLED == 'y')
	{
	  initialiseBluetooth(blueToothSerialConnection);
	}
            
	if (LEARNING_MODE=='p')
	{
	  unSupervisedLoop('N', input_pattern_current_head, output_pattern_desired_head, output_pattern_desired_tail);
	}
	else if (LEARNING_MODE=='c')
	{
	  unSupervisedLoop('Y', input_pattern_current_head, output_pattern_desired_head, output_pattern_desired_tail);
	}
	
	if (BLUETOOTH_ENABLED == 'y')
	{
	  close(blueToothSerialConnection);
	}
	

    }
    
        
    saveConnections();
    
    freeList((LINKED_LIST_TYPE_P) node_head);
    freeList((LINKED_LIST_TYPE_P) connect_head);
    freeList((LINKED_LIST_TYPE_P) input_pattern_head);
    freeList((LINKED_LIST_TYPE_P) output_pattern_head);
    
    
    
    free(node_num_p);
    fclose(log_file_p);
}







