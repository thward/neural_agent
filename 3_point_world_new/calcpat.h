// Specific to : 
// 3_point_world_new
// only supports bTminus1 = 'N'
#include           "calcOutputPat.h"




void calcDesiredInputOutputPatterns(NODE_TYPE_P node_tail, int size_of_pattern, PATTERN_TYPE_P input_pattern_current_head,PATTERN_TYPE_P output_pattern_desired_head, char bTminus1)
{
  
  int x;
  int greatestNode;
  float greatestNodeValue;
  float smallestNodeValue;
  NODE_TYPE_P node_current;
  char Test;
  float patternThreshold;
  char bMotivated = 'N';
    
  patternThreshold = 0.5;
  
  Test = 'N';
 
   //                    desired
  // pattern 1:  0 0 0   0 0 0
  // pattern 2:  0 0 1   0 0 1
  // pattern 3:  0 1 0   0 1 0
  // pattern 4:  1 0 0   1 0 0  
  
  
  // which output node was greatest?
  node_current = node_tail;

  greatestNode = node_current -> node_number;
  greatestNodeValue = node_current -> node_activation;
  smallestNodeValue = node_current -> node_activation;
  node_current = node_current -> prev;
  
  for (x=0; x < (size_of_pattern -1) ; ++x)
  {  
      if (node_current -> node_activation > greatestNodeValue){
	  greatestNode = node_current -> node_number;
	  greatestNodeValue = node_current -> node_activation;
      }
    
      if (node_current -> node_activation < smallestNodeValue){
	  smallestNodeValue = node_current -> node_activation;
      }
    
    node_current = node_current -> prev;
  }
  
  
  
  
    ////////////////////////////////////////////////////
  // if input pattern 1
  ////////////////////////////////////////////////////
  if (input_pattern_current_head -> value == 0 &&
      input_pattern_current_head -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> value  == 0
  )
  {
    input_pattern_current_head -> value = 0;
    input_pattern_current_head -> next -> value  = 0 ;
    input_pattern_current_head -> next -> next -> value  = 1 ;
    
    if (node_tail -> node_activation < 0.5 &&
      node_tail ->prev ->node_activation < 0.5 &&
      node_tail ->prev ->prev-> node_activation < 0.5 
    )
    {
      // + reinforce output
      calcOutputPattern(node_tail, size_of_pattern, output_pattern_desired_head, 1.0,1,Test,patternThreshold,bTminus1);
    }
    else
    {
      // + invert output
      calcOutputPattern(node_tail, size_of_pattern, output_pattern_desired_head, 0.0,1,Test,patternThreshold,bTminus1);
    }
  
  }
  
  ////////////////////////////////////////////////////
  // if input pattern 2
  ////////////////////////////////////////////////////
  else 
  if (input_pattern_current_head -> value == 0 &&
      input_pattern_current_head -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> value  == 1
  )
  {
    input_pattern_current_head -> value = 0;
    input_pattern_current_head -> next -> value  = 1 ;
    input_pattern_current_head -> next -> next -> value  = 0 ;
    
    if (node_tail -> node_activation > 0.5 &&
      node_tail ->prev ->node_activation < 0.5 &&
      node_tail ->prev ->prev-> node_activation < 0.5 
    )
    {
      // + reinforce output
      calcOutputPattern(node_tail, size_of_pattern, output_pattern_desired_head, 1.0,1,Test,patternThreshold,bTminus1);
    }
    else
    {
      // + invert output
      calcOutputPattern(node_tail, size_of_pattern, output_pattern_desired_head, 0.0,1,Test,patternThreshold,bTminus1);
    }
  
  }
  
  
  ////////////////////////////////////////////////////
  // if input pattern 3
  ////////////////////////////////////////////////////
  else 
  if (input_pattern_current_head -> value == 0 &&
      input_pattern_current_head -> next -> value  == 1 &&
      input_pattern_current_head -> next -> next -> value  == 0
  )
  {
    input_pattern_current_head -> value = 1;
    input_pattern_current_head -> next -> value  = 0 ;
    input_pattern_current_head -> next -> next -> value  = 0 ;
    
    // if output 0 
    if (node_tail -> node_activation < 0.5 &&
      node_tail ->prev ->node_activation > 0.5 &&
      node_tail ->prev ->prev-> node_activation < 0.5 
    )
    {
      // + reinforce output
      calcOutputPattern(node_tail, size_of_pattern, output_pattern_desired_head, 1.0,1,Test,patternThreshold,bTminus1);
    }
    else
    {
      // + invert output
      calcOutputPattern(node_tail, size_of_pattern, output_pattern_desired_head, 0.0,1,Test,patternThreshold,bTminus1);
    }
  }  
  
  
  
  ////////////////////////////////////////////////////
  // if input pattern 4
  ////////////////////////////////////////////////////
  else 
   if (input_pattern_current_head -> value == 1 &&
      input_pattern_current_head -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> value  == 0
  )
  {
    input_pattern_current_head -> value = 0;
    input_pattern_current_head -> next -> value  = 0;
    input_pattern_current_head -> next -> next -> value  = 0 ;
    
    // if output 0 
    if (node_tail -> node_activation < 0.5 &&
      node_tail ->prev ->node_activation < 0.5 &&
      node_tail ->prev ->prev-> node_activation > 0.5 
    )
    {
      // + reinforce output
      calcOutputPattern(node_tail, size_of_pattern, output_pattern_desired_head, 1.0,1,Test,patternThreshold,bTminus1);
    }
    else
    {
      // + invert output
      calcOutputPattern(node_tail, size_of_pattern, output_pattern_desired_head, 0.0,1,Test,patternThreshold,bTminus1);
    }
  }
  


  
 
}


