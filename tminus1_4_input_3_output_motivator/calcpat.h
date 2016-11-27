// Specific to : 
// tminus1_4_input_2_output
// chaining works with bTminus1 = 'Y', but not when 'N' :this is as expected
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
  char bMove = 'N'; //(L)eft, (R)ight, (N)owhere
  char bMotivated = 'N';
    
  patternThreshold = 0.5;
  
  Test = 'N';
 
  // pattern 1: 1 0 0 0 
  // pattern 2: 0 1 0 0 
  // pattern 3: 0 0 1 0
  // pattern 4: 0 0 0 1
  
  // which output node was greatest?
  node_current = node_tail;
  if (node_current->node_type == 'R'){ // skip the first node which is the motivator node (assigned as a bias node)
      if (node_current -> node_activation > 0.8){
	  bMotivated = 'Y';
      }
      node_current = node_current -> prev;
  }
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
  
  // if node 0 > 0.5 and node 1 < 0.5
  // 	move left
  // else if node 0 < 0.5 and node 1 > 0.5
  // 	move right
  // else dont move (if both < 0.5 or both > 0.5)
  //if (greatestNodeValue > 0.5 && greatestNodeValue < 0.7 && smallestNodeValue < 0.5){ // we have a clear winner !
  if (greatestNodeValue > 0.5 && smallestNodeValue < 0.5){ // we have a clear winner !
      if(greatestNode == 0){
	bMove = 'L';
      }else if(greatestNode == 1){
	bMove = 'R';
      }
  }
  
  
  ////////////////////////////////////////////////////
  // if input pattern 1
  ////////////////////////////////////////////////////
  if (input_pattern_current_head -> value == 1 &&
      input_pattern_current_head -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> next -> value  == 0 
  )
  {
    
    if (bMove == 'L') {
      // Leave input pattern as is
     
    }
    else if (bMove == 'N'){
      // Leave input pattern as is
      
    }
    else{
      // Move right
      input_pattern_current_head -> value = 0 ;
      input_pattern_current_head -> next -> value  = 1 ;
      input_pattern_current_head -> next -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> next -> value  = 0 ;
      
    }
    
    // + invert tminus1 output
    calcOutputPattern(node_tail, size_of_pattern, output_pattern_desired_head, 0.0,1,Test,patternThreshold,bTminus1);

  
  }
  
  ////////////////////////////////////////////////////
  // if input pattern 2
  ////////////////////////////////////////////////////
  else 
  if (input_pattern_current_head -> value == 0 &&
      input_pattern_current_head -> next -> value  == 1 &&
      input_pattern_current_head -> next -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> next -> value  == 0 
  )
  {
    
    if (bMove == 'L') {
    // Move Left
      input_pattern_current_head -> value = 1 ;
      input_pattern_current_head -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> next -> value  = 0 ;
      
    }
    else if (bMove == 'N'){
      // Leave input pattern as is

      
    }
    else{
      // Move right
      input_pattern_current_head -> value = 0 ;
      input_pattern_current_head -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> value  = 1 ;
      input_pattern_current_head -> next -> next -> next -> value  = 0 ;
      
    }

    
    if (bMotivated == 'N')
    {
    // + invert output
	calcOutputPattern(node_tail, size_of_pattern, output_pattern_desired_head, 0.0,1,Test,patternThreshold,bTminus1);
    }
    else
    {
	fprintf(log_file_p,"bMotivated == True 1");
      // + reinforce output
	calcOutputPattern(node_tail, size_of_pattern, output_pattern_desired_head, 1.0,1,Test,patternThreshold,bTminus1);
    }

  
  }
  
  
  ////////////////////////////////////////////////////
  // if input pattern 3
  ////////////////////////////////////////////////////
  else 
  if (input_pattern_current_head -> value == 0 &&
      input_pattern_current_head -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> value  == 1 &&
      input_pattern_current_head -> next -> next -> next -> value  == 0 
  )
  {
    
    if (bMove == 'L') {
    // Move Left
      input_pattern_current_head -> value = 0 ;
      input_pattern_current_head -> next -> value  = 1 ;
      input_pattern_current_head -> next -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> next -> value  = 0 ;
      
    }
    else if (bMove == 'N'){
      // Leave input pattern as is

      
    }
    else{
      // Move right
      input_pattern_current_head -> value = 0 ;
      input_pattern_current_head -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> next -> value  = 1 ;
      
    }

    
    if (bMotivated == 'N')
    {
    // + invert output
	calcOutputPattern(node_tail, size_of_pattern, output_pattern_desired_head, 0.0,1,Test,patternThreshold,bTminus1);
    }
    else
    {
	fprintf(log_file_p,"bMotivated == True 2");
      // + reinforce output
	calcOutputPattern(node_tail, size_of_pattern, output_pattern_desired_head, 1.0,1,Test,patternThreshold,bTminus1);
    }

  
  }
  
  ////////////////////////////////////////////////////
  // if input pattern 4
  ////////////////////////////////////////////////////
  else 
  if (input_pattern_current_head -> value == 0 &&
      input_pattern_current_head -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> next -> value  == 1 
  )
  {
   // This is the GOAL !! Always reward, send back to beginning.. 
   
   // Move to beginning
   input_pattern_current_head -> value = 1 ;
   input_pattern_current_head -> next -> value  = 0 ;
   input_pattern_current_head -> next -> next -> value  = 0 ;
   input_pattern_current_head -> next -> next -> next -> value  = 0 ;
      
   // + reinforce tminus1 output
   calcOutputPattern(node_tail, size_of_pattern, output_pattern_desired_head, 1.0,1,Test,patternThreshold, bTminus1);
  
  }

  
 
}


