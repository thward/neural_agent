struct node_structure {
    struct  node_structure *next;
    struct  node_structure *prev;
    int     node_layer;
    int     node_number;
    float   node_summed_input;
    float   node_activation;
    float   node_tminus1_activation;
    float   node_error;
    char    node_type;
    float   node_delta;
};

typedef struct node_structure NODE_TYPE;
typedef NODE_TYPE *NODE_TYPE_P;


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

FILE *log_file_p;
FILE *net_file_p;
FILE *file_p;


float globalReward;