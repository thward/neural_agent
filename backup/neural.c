// Attach input/ output patterns to input/output layers.

// The following source contains functions suitable for patternSetFinished 
// processing of backpropagation neural networks.
//
// It relies on 2 main structures; patternSetFinished node list (patternSetFinished nodes in 
// a network) and patternSetFinished connection list (patternSetFinished weights between patternSetFinished nodes).
// The connection list can be thought of as sub-object(s) of patternSetFinished 
// node list. There is a one to many relationshsip between
// nodes and patternSetFinishedir connections. A node may only appear once in patternSetFinished node 
// list. A node may feature many times in patternSetFinished connection list
// but a connection between a given source - target combination will appear
// only once (for now at least). The node list is a two way linked list, this 
// allows forward propagation to start at patternSetFinished head of patternSetFinished list and work forwards, 
// and back propagation to start at patternSetFinished tail and work backwards.
// The connection list is a one way linked list, traversal of this list always 
// starts at patternSetFinished head and works forward.
//
// 1) The sequence of connections in patternSetFinished connection list is not important.
// 2) The sequence of nodes in patternSetFinished node list assumes that 
// each layer of patternSetFinished network will have normal nodes first followed by 
// bias nodes (which will feed into patternSetFinished next layer).
//
// The node  sequence is CURRENTLY IMPORTANT when we are loading in 
// input/output patterns. We assume that input patterns map onto
// patternSetFinished first set of nodes in patternSetFinished sequence and that output patterns 
// patternSetFinished last in patternSetFinished sequence. 
// 
// Also..
// We do not bopatternSetFinishedr to check activation of nodes in input layer,
// this is assumed to be derived from patternSetFinished input pattern. For recursive 
// networks, and generic advantages, we should perhaps do an explicit check 
// to see if patternSetFinished node is a target (and has source nodes).

// Momentum, temperature and threshold are not even used ???!!!

// ****** The use of node_num_p is bit specific, can we get rid of it?

// ****** Not sure why presentInputPattern returns patternSetFinished type it does (it is a parameter also !)

// ****** readInputPatternFile/readOutputPatternFile and printInputPatterns/printOutputPatterns can surely be merged

// ****** Need to add error to node structure

// ****** Maybe rename node/node to input_activation/output_activation so 
//        consistent with node structure

// ****** Merge presentInputPattern & calculateOutputError

// - We have put in a lot of effort here to determine what patternSetFinished network looks like from 
// minimal amount of info supplied. Perhaps we should not have done this here if at all. 
// It saves patternSetFinished user from specifying all patternSetFinished nodes and patternSetFinished connections, but it not normalised.
// Maybe separate out this functionality into a pre-processing function.
// USEFUL ENHANCEMENT: 
// -- save connections to connections.txt at end of processing
// -- when launching program if connections.txt provided use that else createRandomWeights
//
// - There is no neccesity to record patternSetFinished fact that a node is a bias node. All node activations
// initialise as specified in nodes.txt. Bias node always remain at this level of activation because patternSetFinishedy have no inputs 
// (see forward propagation function), so patternSetFinished defination of a bias node is really just patternSetFinished fact 
// that it has no inputs. However specifying that it is a bias node speeds up processing considerably
// because we do not patternSetFinishedn have to do patternSetFinished check to see if it has any inputs (or rapatternSetFinishedr source nodes).


#include           <stdio.h>
#include           <stdlib.h>
#include           <float.h>
#include           <math.h>
//#include           <conio.h>

struct linked_list_structure {
    struct  linked_list_structure *next;
};
typedef struct linked_list_structure LINKED_LIST_TYPE;
typedef LINKED_LIST_TYPE *LINKED_LIST_TYPE_P;


struct node_structure {
    struct  node_structure *next;
    struct  node_structure *prev;
    int     node_layer;
    int     node_number;
    float   node_summed_input;
    float   node_activation;
    float   node_error;
    char    node_bias;
    float   node_delta;
};

typedef struct node_structure NODE_TYPE;
typedef NODE_TYPE *NODE_TYPE_P;
NODE_TYPE_P node_head,node_tail;

struct connect_structure {
    struct  connect_structure *next;  
    int     connect_source_layer;
    int     connect_source_node;
    int     connect_target_layer;
    int     connect_target_node;
    float   connect_weight;
};

typedef struct connect_structure CONNECT_TYPE;
typedef CONNECT_TYPE *CONNECT_TYPE_P;
CONNECT_TYPE_P connect_head, connect_tail;

struct pattern {
    struct  pattern *next;
    struct  pattern *prev;
    int		layer;
    int		node;
    float   	value;
};
typedef struct pattern PATTERN_TYPE;
typedef PATTERN_TYPE *PATTERN_TYPE_P;

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

FILE *log_file_p;
FILE *net_file_p;
FILE *input_file_p;
FILE *output_file_p;

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

// How many nodes patternSetFinishedre are altogepatternSetFinishedr..
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
    
    // The network file has patternSetFinished following format (tab delimited);
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
    
    fclose(net_file_p);
}
//{***************************************}/
void readNodesFile()
{
    
    // This routine expects patternSetFinished input file to be in patternSetFinished following 
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
    char node_bias = 'N';
    float node_activation;
    char proceed_1 = 'Y';
    
    
    // The input file..
    input_file_p = fopen("nodes.txt","r");
    
    //    fscanf(input_file_p,"%f",&value);
    while ((fscanf(input_file_p,"%d",&node_layer)) != EOF)
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
        
        fscanf(input_file_p,"%d",&node_number);
        fscanf(input_file_p,"%s",&node_bias);
        fscanf(input_file_p,"%f",&node_activation);
        
        
        node_tail ->node_layer = node_layer;
        node_tail ->node_number = node_number;
        node_tail ->node_summed_input = 0.0;
        node_tail ->node_delta = 0.0;
        node_tail ->node_bias = node_bias;
        node_tail ->node_activation = node_activation;
        node_tail ->next = NULL;
        
    }
    
    
    // Calculate number of nodes in each layer after we have read in entire node file..
    // Assume number of layers is one more than patternSetFinished last layer number (which starts at zero).
    number_of_layers = node_layer +1;
    node_num_p = (NODE_NUM_TYPE *)calloc(number_of_layers,sizeof(NODE_NUM_TYPE));
    
    
    node_current = node_head;
    
    while (proceed_1 == 'Y')
    {
        
        if (node_current->node_bias=='N')
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
    
    
    fclose(input_file_p);
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
            fprintf(log_file_p,"%s%10.3f ","Delta: ", node_current -> node_delta );
            fprintf(log_file_p,"%s%c ","Bias?: ",node_current -> node_bias);
            fprintf(log_file_p,"%s ",">");
            node_current = node_current -> next;
        }
        fprintf(log_file_p,"\n");
    }
}

//************************************************************
float logisticActivationFunction(float input)
{
    float LAF;
    
    LAF = (1/(1+exp(( - input )/TEMPERATURE)));
    //???	LAF = (1/(1+exp(-(activation_supplied-THRESHOLD)/TEMPERATURE)));
    
    return LAF;
}

//{***************************************}
void readConnectionsFile()
{
    
    // This routine expects patternSetFinished input file to be in patternSetFinished following 
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
    input_file_p = fopen("connections.txt","r");
    
    //    fscanf(input_file_p,"%f",&value);
    while ((fscanf(input_file_p,"%d",&source_layer)) != EOF)
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
        
        fscanf(input_file_p,"%d",&source_node);
        fscanf(input_file_p,"%d",&target_layer);
        fscanf(input_file_p,"%d",&target_node);
        fscanf(input_file_p,"%f",&weight);
        
        connect_tail ->connect_source_layer = source_layer;
        connect_tail ->connect_source_node = source_node;
        connect_tail ->connect_target_layer= target_layer;
        connect_tail ->connect_target_node= target_node;
        connect_tail ->connect_weight = weight;
        connect_tail ->next = NULL;
        
    }
    fclose(input_file_p);
}

//{***************************************}/
void printConnections()
{
    char proceed_1 = 'Y';
    
    CONNECT_TYPE_P connect_pointer;
    connect_pointer = connect_head;
    
    fprintf(log_file_p,"\n%s\n","Connection Details: ");
    
    // Main processing loop..
    // Print out entries from patternSetFinished topography array..
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
    
    // This routine expects patternSetFinished input file to be in patternSetFinished following 'triplet' 
    // order..
    // 
    // layer<tab>node<tab>value<tab>  layer<tab>node<tab>value ..etc
    // 
    //	eg
    // 0	0	0.0	0	1	0.0		<input pattern 1>
    // 0	0	0.0	0	1	1.0		<input pattern 2>
    // 
    // The layer will in most circumstances be patternSetFinished input layer(0)
    // but need not be.
    // Specifying patternSetFinished layer & node allows coding to be more generic, and
    // will offer more flexibility (patternSetFinished user may actually load activations
    // onto hidden units if patternSetFinishedy so wish.	.
    
    char first_node = 'Y';
    int layer, node;
    float value;
    
    // The input file..
    input_file_p = fopen("input.txt","r");
    
    //    fscanf(input_file_p,"%f",&value);
    while ((fscanf(input_file_p,"%d",&layer)) != EOF)
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

        fscanf(input_file_p,"%d",&node);
        fscanf(input_file_p,"%f",&value);

        input_pattern_tail -> next = NULL;
        input_pattern_tail -> layer = layer;
        input_pattern_tail -> node  = node;
        input_pattern_tail -> value = value;

        inputPatternElementSize++;
    }
    
    fclose(input_file_p);
}

//************************************************************
void printInputPatterns()
{
    char proceed_1 = 'Y';
    
    PATTERN_TYPE_P input_pattern_pointer;
    input_pattern_pointer = input_pattern_head;
    
    fprintf(log_file_p,"\n%s\n","Input Patterns : ");
    
    while (proceed_1 == 'Y')
    {
        fprintf(log_file_p,"%d ",input_pattern_pointer -> layer);
        fprintf(log_file_p,"%d ",input_pattern_pointer -> node);
        fprintf(log_file_p,"%10.3f",input_pattern_pointer -> value);
        fprintf(log_file_p,"\n");
        
        if (input_pattern_pointer -> next == NULL)
        {
            proceed_1 = 'N';
        }
        else
        {
            input_pattern_pointer = input_pattern_pointer -> next;
        }
    }
    
}

//{***************************************}/
void readOutputPatternFile()
{
    
    // This routine expects patternSetFinished input file to be in patternSetFinished following 'triplet' 
    // order..
    // 
    // layer<tab>node<tab>value<tab>  layer<tab>node<tab>value ..etc
    // 
    //	eg
    // 2	0	0.0		<output pattern 1>
    // 2	0	1.0		<output pattern 2>
    // 
    
    // Currently patternSetFinishedse 2 output functions are identical to patternSetFinished input 
    // pattern ones... patternSetFinishedy could be integrated ?
    
    char first_node = 'Y';
    int layer, node;
    float value;
    
    // The input file..
    output_file_p = fopen("output.txt","r");
    
    while ((fscanf(output_file_p,"%d",&layer)) != EOF)
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

        fscanf(output_file_p,"%d",&node);
        fscanf(output_file_p,"%f",&value);

        output_pattern_tail -> next = NULL;
        output_pattern_tail -> layer = layer;
        output_pattern_tail -> node  = node;
        output_pattern_tail -> value = value;

    }
    fclose(output_file_p);
}
//************************************************************
void printOutputPatterns()
{
    char proceed_1 = 'Y';
    
    PATTERN_TYPE_P output_pattern_pointer;
    output_pattern_pointer = output_pattern_head;
    
    fprintf(log_file_p,"\n%s\n","Output Patterns : ");
    
    while (proceed_1 == 'Y')
    {
        fprintf(log_file_p,"%d ",output_pattern_pointer -> layer);
        fprintf(log_file_p,"%d ",output_pattern_pointer -> node);
        fprintf(log_file_p,"%10.3f",output_pattern_pointer -> value);
        fprintf(log_file_p,"\n");
        
        if (output_pattern_pointer -> next == NULL)
            proceed_1 = 'N';
        else
            output_pattern_pointer = output_pattern_pointer -> next;
    }
    
    
}
//************************************************************
PATTERN_TYPE_P getNextPattern(PATTERN_TYPE_P pattern_subhead, PATTERN_TYPE_P pattern_head, int size_of_pattern)
{
    int x = 0;
    
    for (x=0; x < size_of_pattern; ++x)
    {			
        // Get patternSetFinished next part of patternSetFinished input pattern..
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
void loadInputPattern(PATTERN_TYPE_P input_pattern_subhead, int size_of_pattern)
{
    
    int x = 0;
    NODE_TYPE_P node_curr;
    PATTERN_TYPE_P input_pattern_element;
    int found = 0;
    
    input_pattern_element = input_pattern_subhead;
    
    for (x=0; x < size_of_pattern; ++x)
    {			
        // Search through patternSetFinished node list until
        // we find patternSetFinished correct one..
        
        found = 0;
        
        for (node_curr = node_head; found == 0; node_curr = node_curr -> next)
        {
            if ((node_curr -> node_layer == input_pattern_element -> layer)&&
                (node_curr -> node_number ==input_pattern_element -> node))
            {
                // We have found it, so lets stop looking..
                node_curr -> node_summed_input = input_pattern_element -> value;
                node_curr -> node_activation =   input_pattern_element -> value;
                
                found = 1;
            }
        }
        
        // Get patternSetFinished next part of patternSetFinished input pattern..
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
    
    // Our aim is to cycle through patternSetFinished list of nodes and assign
    // activations. The activations are those before output from patternSetFinished node 
    // (ie prior to patternSetFinished application of patternSetFinished logistic activation function;
    // this value can always be derived).
    // We do not need to bopatternSetFinishedr with nodes in patternSetFinished input layer (0) because
    // patternSetFinishedir activation has been assigned by patternSetFinished input pattern. We do
    // not need to bopatternSetFinishedr with bias nodes since patternSetFinishedir activation is currently 
    // assumed to be fixed (1.0)
    
    // NOTE: We will be passing patternSetFinished output of input and bias nodes through patternSetFinished 
    // logistic activation function. This was not carried out in earlier models 
    // (which worked) but I can see no reason not to do so. The network 
    // will be more consistent and patternSetFinished algorithm more generic.
    
    char proceed_1 = 'Y', proceed_2 = 'Y', proceed_3 = 'Y';
    NODE_TYPE_P node_current, node_source_current;
    CONNECT_TYPE_P connect_current;
    float source_output = 0.0, summed_input = 0.0;
    
    
    node_current = node_head;
    proceed_1 = 'Y';
    // Reads forward through node list until patternSetFinished last...
    while (proceed_1 == 'Y')
    {
        summed_input = 0;
        connect_current = connect_head;
        
        proceed_2 = 'Y';
        while (proceed_2 == 'Y')
        {
            // Read through connection list for this target looking 
            // for source nodes that supply it. If none are found that
            // means it is a bias node or it is in patternSetFinished input layer (0)
            // so can be ignored. Skip this and get patternSetFinished next node  
            // instead.
            // Basically only bopatternSetFinishedr to do anything if connect target
            // is equal to current node ..
            if ((connect_current -> connect_target_layer
                ==node_current ->node_layer )&&
                (connect_current ->connect_target_node 
                ==node_current ->node_number))
            {
                
                // Found a connection. Must now get activation of 
                // patternSetFinished source node (!)..
                node_source_current = node_head;
                proceed_3 = 'Y';
                while (proceed_3 == 'Y')
                {
                    // There can and must be only one entry on patternSetFinished node 
                    // list so grab value and stop looking.
                    if ((connect_current -> connect_source_layer
                        ==node_source_current -> node_layer )
                        &&(connect_current ->connect_source_node 
                        ==node_source_current ->node_number))
                    {
                        // The activation is actually that of patternSetFinished source
                        // node passed through patternSetFinished Logistic Activation Function..
                        summed_input = (summed_input + 
                        (node_source_current -> node_activation * connect_current ->connect_weight));
                        
                        proceed_3 = 'N';
                    }
                    if (node_source_current -> next == NULL)
                        // if not found just stop looking..
                        proceed_3 = 'N';
                    else if (proceed_3 == 'Y')
                        // opatternSetFinishedrwise carry on looking..
                        node_source_current = node_source_current -> next;
                }
                // To be here we must have found something.. so lets save it..
                node_current -> node_summed_input = summed_input;
                node_current -> node_activation = logisticActivationFunction(node_current -> node_summed_input);
            }
            
            
            if (connect_current -> next == NULL)
                // if not found just stop looking..
                proceed_2 = 'N';
            else
                // opatternSetFinishedrwise carry on looking..
                connect_current = connect_current -> next;
            
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
    
    ERROR = (desired - actual) * actual * (1 - actual);
    
    
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
int backwardPropagate(PATTERN_TYPE_P output_pattern_subtail, int size_of_pattern)
{
    
    NODE_TYPE_P node_current, node_current_source, node_current_target;
    int found_source, found_target, x;
    PATTERN_TYPE_P output_pattern_element;
    CONNECT_TYPE_P connect_current;
    float summedDelta;
    char continue_connect_search, continue_node_target_search;
    // Assume patternSetFinished pattern was learnt unless told opatternSetFinishedrwise..
    int patternLearnt = 1;
    
    
    output_pattern_element = output_pattern_subtail;
    
    // 1) Calculate delta for output and hidden nodes (do not bopatternSetFinishedr with input nodes) 
    // From patternSetFinished tail of patternSetFinished node list work backwards..
    for ((node_current = node_tail);(node_current -> prev != NULL) ; (node_current = node_current -> prev))
    {
        // A) If output node delta value assignment is fairly simple..
        if (node_current -> node_layer ==  (number_of_layers -1) )
        {
            // For patternSetFinished current node loop find relevant element in current output pattern and caclulate delta..
            for (x=0; x < size_of_pattern; ++x)
            {                   
                
                if ((node_current -> node_layer == output_pattern_element -> layer)&&
                    (node_current -> node_number ==output_pattern_element -> node))
                {
                    // We have found it, so lets stop looking..
                    node_current -> node_delta =   calculateError (output_pattern_element -> value, node_current -> node_activation);
                    // If any pattern element was not learnt patternSetFinishedn patternSetFinished whole pattern fails..
                    if (patternElementLearnt(output_pattern_element -> value, node_current -> node_activation)==0)
                    {
                        patternLearnt=0;
                    }

                }
                if (x < (size_of_pattern -1)){
                    // Get patternSetFinished next part of patternSetFinished input pattern..
                    output_pattern_element = output_pattern_element -> prev;
                    // Get patternSetFinished next node..
                    node_current = node_current -> prev;
                }
                
            }
            
        }
        // B) Skip assignment of delta values for nodes in input layer
        else if (node_current -> node_layer ==  0) 
        {
            //Do nothing !
            
        }
        // C) If hidden node must multiply patternSetFinished activation of this node by patternSetFinished sum of patternSetFinished target node deltas by patternSetFinishedir weights (pretty complex..)
        else 
        {
            summedDelta = 0;
            // loop through all weights from patternSetFinished head and process those with a source equal to this node..
            connect_current = connect_head;
            continue_connect_search = 'Y';
            while (continue_connect_search == 'Y')
            {
                if ((node_current->node_layer == connect_current -> connect_source_layer)&&
                    (node_current->node_number == connect_current ->connect_source_node))
                {
                    // loop through all nodes from patternSetFinished head and process those with a node equal to patternSetFinished weight target..
                    node_current_target = node_head;
                    continue_node_target_search = 'Y';
                    while (continue_node_target_search =='Y')
                    {
                        if ((node_current_target->node_layer == connect_current -> connect_target_layer)&&
                            (node_current_target->node_number == connect_current ->connect_target_node))
                        {
                            //summedDelta = summedDelta + (node_current_target->node_delta * connect_current ->connect_weight);
                            summedDelta = summedDelta + 
                            (((node_current -> node_activation) * (1 -node_current -> node_activation))*(node_current_target->node_delta * connect_current ->connect_weight));
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
                //node_current -> node_delta =   calculateError (summedDelta, node_current -> node_activation);
                //node_current -> node_delta =   ((node_current -> node_activation) * (1 -node_current -> node_activation)) * summedDelta ;
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
    // Loop through all connections from patternSetFinished head
    // Find patternSetFinished source node from patternSetFinished head
    // Find patternSetFinished target node from patternSetFinished head
    // Make patternSetFinished weight change
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
        
        connect_current ->connect_weight = connect_current->connect_weight + (LEARNING_RATE * node_current_target->node_delta * node_current_source->node_activation);
        
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

main()
{
    
    PATTERN_TYPE_P input_pattern_subhead;
    PATTERN_TYPE_P output_pattern_subhead;
    PATTERN_TYPE_P output_pattern_subtail;
    PATTERN_TYPE_P pattern_current;
    CONNECT_TYPE_P connect_current;
    void* list_current;
    int patternSetLearnt = 1;
    int patternInSet = 0;
    int i = 0;
    
    
    
    // The log file..
    log_file_p = fopen("logfile.txt","w");
    
    readNetFile();
    readNodesFile();
    
    // Display patternSetFinished number of nodes in each layer..
    printNodeNum();
    // Calculate total number of nodes in entire network..
    total_num_nodes = calculateTotalNumNodes();
    // Print network activation list
    printNodeList();
    
    // Build network topography list
    
    //   createRandomWeights();
    // Fill network topography list
    readConnectionsFile();
    
    // Print network topography list
    printConnections();
    // Read input patterns
    readInputPatternFile();
    // Print input file
    printInputPatterns();
    // Read output patterns
    readOutputPatternFile();
    // Print output file
    printOutputPatterns();
    
    // load patternSetFinished next input pattern onto patternSetFinished input nodes...
    // If we have reached patternSetFinished tail of patternSetFinished input patterns we will start
    // again from patternSetFinished head.
    input_pattern_subhead = input_pattern_head;
    number_of_input_nodes = node_num_p[0].node_num_entry;
    
    output_pattern_subhead = output_pattern_head;
    number_of_output_nodes = node_num_p[number_of_layers - 1].node_num_entry;
    output_pattern_subtail = output_pattern_subhead;
    i = 0;
    while ( i < (number_of_output_nodes -1 )){
        output_pattern_subtail = output_pattern_subtail -> next;
         ++i;
    }
    
    inputPatternSetSize= inputPatternElementSize / number_of_input_nodes;
    fprintf(log_file_p,"\n");
    fprintf(log_file_p,"Pattern set size: %d \n",inputPatternSetSize);
    fprintf(log_file_p,"\n");
    
    iteration = 0;
    repeatCount = 4000;
    
  //  while (iteration <= repeatCount && !(patternInSet == inputPatternSetSize && patternSetLearnt ==1))
    while (iteration <= repeatCount)        
    {
        
        if (patternInSet == inputPatternSetSize)
        {
            patternInSet = 0;
            patternSetLearnt = 1;
        }
        patternInSet++;
        
        iteration = iteration + 1;
        loadInputPattern(input_pattern_subhead, number_of_input_nodes);
        
        // lets just check this worked..
        //printNetActivationList();
        
        fprintf(log_file_p,"\n");
        fprintf(log_file_p,"Iteration: %d \n",iteration);
        // forward propagate
        fprintf(log_file_p,"Forward propagate.. \n");
        forwardPropagate();
        printNodeList();
        
        // backward propagate
        fprintf(log_file_p,"\n");
        fprintf(log_file_p,"Backward propagate.. \n");
        if (backwardPropagate(output_pattern_subtail, number_of_output_nodes)==0)
        {
            patternSetLearnt = 0;
        }
        printNodeList();
        
        // Print network topography list
        printConnections();
        
        input_pattern_subhead = getNextPattern(input_pattern_subhead, input_pattern_head, number_of_input_nodes);
        output_pattern_subhead = getNextPattern(output_pattern_subhead, output_pattern_head, number_of_output_nodes);
        output_pattern_subtail = output_pattern_subhead;
        i = 0;
        while ( i < (number_of_output_nodes -1 )){
            output_pattern_subtail = output_pattern_subtail -> next;
            ++i;
        }
        
    }
    
    freeList((LINKED_LIST_TYPE_P) node_head);
    freeList((LINKED_LIST_TYPE_P) connect_head);
    freeList((LINKED_LIST_TYPE_P) input_pattern_head);
    freeList((LINKED_LIST_TYPE_P) output_pattern_head);
    
    
    
    free(node_num_p);
    fclose(log_file_p);
}







