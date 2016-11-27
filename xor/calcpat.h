// Specific to : 
// XOR
// not coded to support chaining with bTminus1 = 'Y' or 'N' :but it does compile !
#include           "calcOutputPat.h"




float calcDesiredInputOutputPatterns(NODE_TYPE_P node_tail, int size_of_pattern, PATTERN_TYPE_P input_pattern_current_head,PATTERN_TYPE_P output_pattern_desired_head, char bTminus1)
{
  
  int x;
  int greatestNode;
  float greatestNodeValue;
  float reward;
  NODE_TYPE_P node_current;
  char Test;
  float patternThreshold;
  
  patternThreshold = 0.5;
  
  Test = 'N';
 
  // pattern 1: 0 0 
  // pattern 2: 0 1 
  // pattern 3: 1 0 
  // pattern 4: 1 1 
  
  // which output node was greatest?
  node_current = node_tail;
  greatestNode = node_current -> node_number;
  greatestNodeValue = node_current -> node_activation;
  node_current = node_current -> prev;
  
  for (x=0; x < (size_of_pattern -1) ; ++x)
  {  
    if (node_current -> node_activation > greatestNodeValue){
        greatestNode = node_current -> node_number;
	greatestNodeValue = node_current -> node_activation;
    }
    node_current = node_current -> prev;
  }
  
  
  ////////////////////////////////////////////////////
  // if input pattern 1
  ////////////////////////////////////////////////////
  if (input_pattern_current_head -> value == 0 &&
      input_pattern_current_head -> next -> value  == 0 
  )
  {
    input_pattern_current_head -> value = 0;
    input_pattern_current_head -> next -> value  = 1 ;
    // if output 0 
    if (greatestNodeValue > 0.5)
    {
      reward = 0.0;
    }
    else
    {
      reward = 1.0;

    }
          
  
  }
  
  ////////////////////////////////////////////////////
  // if input pattern 2
  ////////////////////////////////////////////////////
  else if (input_pattern_current_head -> value == 0 &&
      input_pattern_current_head -> next -> value  == 1 
  )
  {
    input_pattern_current_head -> value = 1;
    input_pattern_current_head -> next -> value  = 0 ;
    // if output 0 
    if (greatestNodeValue > 0.5)
    {
      reward = 1.0;
    }
    else
    {
      reward = 0.0;
    }
  
  }
  
  
  ////////////////////////////////////////////////////
  // if input pattern 3
  ////////////////////////////////////////////////////
  else if (input_pattern_current_head -> value == 1 &&
      input_pattern_current_head -> next -> value  == 0 
  )
  {
    input_pattern_current_head -> value = 1;
    input_pattern_current_head -> next -> value  = 1 ;
    // if output 0 
    if (greatestNodeValue > 0.5)
    {
      reward = 1.0;
    }
    else
    {
      reward = 0.0;
    }
  
  }
  
  ////////////////////////////////////////////////////
  // if input pattern 4
  ////////////////////////////////////////////////////
  else if (input_pattern_current_head -> value == 1 &&
      input_pattern_current_head -> next -> value  == 1 
  )
  {
    input_pattern_current_head -> value = 0;
    input_pattern_current_head -> next -> value  = 0 ;
    // if output 0 
    if (greatestNodeValue > 0.5)
    {
      reward = 0.0;    }
    else
    {
      reward = 1.0;
    }
  
  }  
  
  calcOutputPattern(node_tail, size_of_pattern, output_pattern_desired_head, reward,0,Test,patternThreshold,bTminus1);

  return reward;
 
}


