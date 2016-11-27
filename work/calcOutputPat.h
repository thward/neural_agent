#include           "struct.h"


// Calculate 'desired' output pattern
void genericCalcOutputPattern(NODE_TYPE_P node_tail, int size_of_pattern, PATTERN_TYPE_P output_pattern_desired_head, float reward, float patternThreshold, char bTminus1)
{
  int x;
  NODE_TYPE_P node_current;
  PATTERN_TYPE_P pattern_current_element;
  
  float i_rand;
  float i_float;
  
  char reinforce;
  reinforce = 'N';
  
 
  if (reward > 0){
    reinforce = 'Y';
  }
  
  
  node_current = node_tail;
  pattern_current_element = output_pattern_desired_head;
  
  // Get to beginning of output layer..
  for (x=0; x < (size_of_pattern -1) ; ++x)
  {  
    node_current = node_current -> prev;
  }
  
  
  // For each node in output layer..
  for (x=0; x < (size_of_pattern) ; ++x)
  {  
    
    // number between 0.45..0.55
    i_rand = rand() % 100;
    //i_float = (i_rand / 100) ;
    i_float = 0.45 + (i_rand / 1000) ;
  
  
    if (reinforce == 'Y')
    {
      if (node_current->node_type == 'R') {
	//pattern_current_element->value = 1.0 ;
	pattern_current_element->value = reward ;
      }
      else{
	if (bTminus1 == 'N'){
	  if (node_current->node_activation >= patternThreshold){
	    //pattern_current_element->value = 0.9 + i_float ;
	    //pattern_current_element->value = 0.9 ;
	    pattern_current_element->value = reward ;
	  }
	  else{
	    //pattern_current_element->value = 0.1 + i_float;
	    //pattern_current_element->value = 0.1 ;
	    pattern_current_element->value = 1.0 - reward ;
	  }
	}
	else
	{
	  if (node_current->node_tminus1_activation >= patternThreshold){
	    //pattern_current_element->value = 0.9 + i_float ;
	    //pattern_current_element->value = 0.9 ;
	    pattern_current_element->value = reward ;
	  }
	  else{
	    //pattern_current_element->value = 0.1 + i_float;
	    //pattern_current_element->value = 0.1 ;
	    pattern_current_element->value = 1.0 - reward ;
	  }
	}
      }
      
    }
    else // reinforce = 'N'
    {
      if (node_current->node_type == 'R') {
	pattern_current_element->value = 0.0 ;
      }
      else{
	pattern_current_element->value = i_float;
      }

    }
    pattern_current_element = pattern_current_element ->next;  
    node_current = node_current -> next;
  }
  
}

// Calculate 'desired' output pattern
void specificOutputPattern(PATTERN_TYPE_P output_pattern_desired_head, float val1)
{
  PATTERN_TYPE_P pattern_current_element;
    
  pattern_current_element = output_pattern_desired_head;
  
  pattern_current_element -> value = val1;
  
  
}


void calcOutputPattern(NODE_TYPE_P node_tail, int size_of_pattern, PATTERN_TYPE_P output_pattern_desired_head, float reward,  float val1,char Test,float patternThreshold, char bTminus1)
{
  if (Test =='Y'){
    //specificOutputPattern(output_pattern_desired_head, val1);
  }
  else{
    genericCalcOutputPattern(node_tail, size_of_pattern, output_pattern_desired_head, reward, patternThreshold, bTminus1);
  }
  
}


