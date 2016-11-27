// Specific to : 
// three_point_world
#include           "struct.h"


// Calculate 'desired' output pattern
void genericCalcOutputPattern(NODE_TYPE_P node_tail, int size_of_pattern, PATTERN_TYPE_P output_pattern_current_head, float reward, float patternThreshold)
{
  int x;
  NODE_TYPE_P node_current;
  PATTERN_TYPE_P pattern_current_element;
  
  float i_rand;
  float i_float;
  
  char reinforce;
  reinforce = 'N';
  

  globalReward = reward;	
  
  
  if (reward > 0){
    reinforce = 'Y';
  }
  
  int greatestNode;
  float greatestNodeValue;
  
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
  
  
  
  node_current = node_tail;
  pattern_current_element = output_pattern_current_head;
  
  // Get to beginning of output layer..
  for (x=0; x < (size_of_pattern -1) ; ++x)
  {  
    node_current = node_current -> prev;
  }
  
  
  // For each node in output layer..
  for (x=0; x < (size_of_pattern) ; ++x)
  {  
    
    // number between 0..0.1
    i_rand = rand() % 100;
    i_float = (i_rand / 100) ;
  //  i_float = i_float - 0.05;
  
    if (reinforce == 'Y')
    {
      /*
      if (node_current->node_activation >= patternThreshold){
	pattern_current_element->value = 1.0;
      }
      else{
	pattern_current_element->value = 0.0;
      }
      */
      
      // Winner takes all..
      /*
      if (greatestNode == node_current -> node_number){
	pattern_current_element->value = 1.0;
      }
      else{
	pattern_current_element->value = 0.0;
      }
      */
      
     
      if (node_current->node_activation >= patternThreshold){
	//pattern_current_element->value = 0.9 + i_float ;
	pattern_current_element->value = 0.9 ;
      }
      else{
	//pattern_current_element->value = 0.1 + i_float;
	pattern_current_element->value = 0.1 ;
      }
     
      
    }
    else // reinforce = 'N'
    {
      /*
      if (node_current->node_activation >= patternThreshold){
	pattern_current_element->value = 0.0;
      }
      else{
	pattern_current_element->value = 1.0;
      }
      */
     
     // Winner takes all..
     /*
     if (greatestNode == node_current -> node_number){
	pattern_current_element->value = 0.0;
      }
      else{
	pattern_current_element->value = 1.0;
      }
      */
     
     /*
      if (node_current->node_activation >= patternThreshold){
	pattern_current_element->value = 0.1 + i_float;
	//pattern_current_element->value = 0.1 ;
      }
      else{
	pattern_current_element->value = 0.9 + i_float;
	//pattern_current_element->value = 0.9 ;
      }
      */
      pattern_current_element->value = i_float;

    }
    pattern_current_element = pattern_current_element ->next;  
    node_current = node_current -> next;
  }
  
}

// Calculate 'desired' output pattern
void specificOutputPattern(PATTERN_TYPE_P output_pattern_current_head, float val1,float val2, float val3)
{
  PATTERN_TYPE_P pattern_current_element;
    
  pattern_current_element = output_pattern_current_head;
  
  pattern_current_element -> value = val1;
  pattern_current_element -> next -> value  = val2 ;
  pattern_current_element -> next -> next -> value  = val3 ;
  
}


void calcOutputPattern(NODE_TYPE_P node_tail, int size_of_pattern, PATTERN_TYPE_P output_pattern_current_head, float reward,  float val1,float val2, float val3,char Test,float patternThreshold)
{
  if (Test =='Y'){
    specificOutputPattern(output_pattern_current_head, val1, val2, val3);
  }
  else{
    genericCalcOutputPattern(node_tail, size_of_pattern, output_pattern_current_head, reward, patternThreshold);
  }
  
}


void calcDesiredInputOutputPatterns(NODE_TYPE_P node_tail, int size_of_pattern, PATTERN_TYPE_P input_pattern_current_head,PATTERN_TYPE_P output_pattern_current_head)
{
  
  int x;
  int greatestNode;
  float greatestNodeValue;
  NODE_TYPE_P node_current;
  char Test;
  float patternThreshold;
  
  patternThreshold = 0.5;
  
  Test = 'N';
  //                    desired
  // pattern 1:  0 0 0   0 0 0
  // pattern 2:  0 0 1   0 0 1
  // pattern 3:  0 1 0   0 1 0  
  // pattern 3b: 0 1 1   0 1 1
  // pattern 4:  1 0 0   1 0 0
  // pattern 4b: 1 1 0   1 1 0
  // pattern 5:  1 1 1   1 1 1
  
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
      calcOutputPattern(node_tail, size_of_pattern, output_pattern_current_head, 1.0,0,0,0,Test,patternThreshold);
    }
    else
    {
      // + invert output
      calcOutputPattern(node_tail, size_of_pattern, output_pattern_current_head, 0.0,0,0,0,Test,patternThreshold);
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
      calcOutputPattern(node_tail, size_of_pattern, output_pattern_current_head, 1.0,0,0,1,Test,patternThreshold);
    }
    else
    {
      // + invert output
      calcOutputPattern(node_tail, size_of_pattern, output_pattern_current_head, 0.0,0,0,1,Test,patternThreshold);
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
    input_pattern_current_head -> value = 0;
    input_pattern_current_head -> next -> value  = 1 ;
    input_pattern_current_head -> next -> next -> value  = 1 ;
    
    // if output 0 
    if (node_tail -> node_activation < 0.5 &&
      node_tail ->prev ->node_activation > 0.5 &&
      node_tail ->prev ->prev-> node_activation < 0.5 
    )
    {
      // + reinforce output
      calcOutputPattern(node_tail, size_of_pattern, output_pattern_current_head, 1.0,0,1,0,Test,patternThreshold);
    }
    else
    {
      // + invert output
      calcOutputPattern(node_tail, size_of_pattern, output_pattern_current_head, 0.0,0,1,0,Test,patternThreshold);
    }
  }  
  
  ////////////////////////////////////////////////////
  // if input pattern 3b
  ////////////////////////////////////////////////////
  else 
  if (input_pattern_current_head -> value == 0 &&
      input_pattern_current_head -> next -> value  == 1 &&
      input_pattern_current_head -> next -> next -> value  == 1
  )
  {
    input_pattern_current_head -> value = 1;
    input_pattern_current_head -> next -> value  = 0 ;
    input_pattern_current_head -> next -> next -> value  = 0 ;
    
    // if output 0 
    if (node_tail -> node_activation > 0.5 &&
      node_tail ->prev ->node_activation > 0.5 &&
      node_tail ->prev ->prev-> node_activation < 0.5 
    )
    {
      // + reinforce output
      calcOutputPattern(node_tail, size_of_pattern, output_pattern_current_head, 1.0,0,1,1,Test,patternThreshold);
    }
    else
    {
      // + invert output
      calcOutputPattern(node_tail, size_of_pattern, output_pattern_current_head, 0.0,0,1,1,Test,patternThreshold);
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
    input_pattern_current_head -> value = 1;
    input_pattern_current_head -> next -> value  = 1;
    input_pattern_current_head -> next -> next -> value  = 0 ;
    
    // if output 0 
    if (node_tail -> node_activation < 0.5 &&
      node_tail ->prev ->node_activation < 0.5 &&
      node_tail ->prev ->prev-> node_activation > 0.5 
    )
    {
      // + reinforce output
      calcOutputPattern(node_tail, size_of_pattern, output_pattern_current_head, 1.0,1,0,0,Test,patternThreshold);
    }
    else
    {
      // + invert output
      calcOutputPattern(node_tail, size_of_pattern, output_pattern_current_head, 0.0,1,0,0,Test,patternThreshold);
    }
  }
  
  ////////////////////////////////////////////////////
  // if input pattern 4b
  ////////////////////////////////////////////////////
  else 
   if (input_pattern_current_head -> value == 1 &&
      input_pattern_current_head -> next -> value  == 1 &&
      input_pattern_current_head -> next -> next -> value  == 0
  )
  {
    input_pattern_current_head -> value = 1;
    input_pattern_current_head -> next -> value  = 1 ;
    input_pattern_current_head -> next -> next -> value  = 1 ;
    
    // if output 0 
    if (node_tail -> node_activation < 0.5 &&
      node_tail ->prev ->node_activation > 0.5 &&
      node_tail ->prev ->prev-> node_activation > 0.5 
    )
    {
      // + reinforce output
      calcOutputPattern(node_tail, size_of_pattern, output_pattern_current_head, 1.0,1,1,0,Test,patternThreshold);
    }
    else
    {
      // + invert output
      calcOutputPattern(node_tail, size_of_pattern, output_pattern_current_head, 0.0,1,1,0,Test,patternThreshold);
    }
  }  
  
  
  ////////////////////////////////////////////////////
  // if input pattern 5
  ////////////////////////////////////////////////////
  else 
   if (input_pattern_current_head -> value == 1 &&
      input_pattern_current_head -> next -> value  == 1 &&
      input_pattern_current_head -> next -> next -> value  == 1
  )
  {
    input_pattern_current_head -> value = 0;
    input_pattern_current_head -> next -> value  = 0 ;
    input_pattern_current_head -> next -> next -> value  = 0 ;
    
    // if output 0 
    if (node_tail -> node_activation > 0.5 &&
      node_tail ->prev ->node_activation > 0.5 &&
      node_tail ->prev ->prev-> node_activation > 0.5 
    )
    {
      // + reinforce output
      calcOutputPattern(node_tail, size_of_pattern, output_pattern_current_head, 1.0,1,1,1,Test,patternThreshold);
    }
    else
    {
      // + invert output
      calcOutputPattern(node_tail, size_of_pattern, output_pattern_current_head, 0.0,1,1,1,Test,patternThreshold);
    }
  }  
  
 
}


