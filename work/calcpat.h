// Specific to : 
// tminus1_9_input_4_output
// chaining works with bTminus1 = 'Y', but not when 'N' :this is as expected
#include           "calcOutputPat.h"
extern int	LOG_LEVEL ;

int isPattern(char *location, PATTERN_TYPE_P input_pattern_current_head)
{
  
  int isEqual = 0;
  
  if (
      location == "TOPLEFT" &&
      input_pattern_current_head -> value == 1 &&
      input_pattern_current_head -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> next -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> next -> next -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> next -> next -> next -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> next -> next -> next -> next -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> next -> next -> next -> next -> next -> next-> value  == 0 
  ){
    isEqual = 1;
  }
  else
  if (
      location == "TOPMIDDLE" &&
      input_pattern_current_head -> value == 0 &&
      input_pattern_current_head -> next -> value  == 1 &&
      input_pattern_current_head -> next -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> next -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> next -> next -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> next -> next -> next -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> next -> next -> next -> next -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> next -> next -> next -> next -> next -> next-> value  == 0 
  ){
    isEqual = 1;
  }
  else
  if (
      location == "TOPRIGHT" &&
      input_pattern_current_head -> value == 0 &&
      input_pattern_current_head -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> value  == 1 &&
      input_pattern_current_head -> next -> next -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> next -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> next -> next -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> next -> next -> next -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> next -> next -> next -> next -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> next -> next -> next -> next -> next -> next-> value  == 0 
  ){
    isEqual = 1;
  }
  else
  if (
      location == "MIDDLELEFT" &&
      input_pattern_current_head -> value == 0 &&
      input_pattern_current_head -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> next -> value  == 1 &&
      input_pattern_current_head -> next -> next -> next -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> next -> next -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> next -> next -> next -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> next -> next -> next -> next -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> next -> next -> next -> next -> next -> next-> value  == 0 
  ){
    isEqual = 1;
  }
  else
  if (
      location == "MIDDLEMIDDLE" &&
      input_pattern_current_head -> value == 0 &&
      input_pattern_current_head -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> next -> next -> value  == 1 &&
      input_pattern_current_head -> next -> next -> next -> next -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> next -> next -> next -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> next -> next -> next -> next -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> next -> next -> next -> next -> next -> next-> value  == 0 
  ){
    isEqual = 1;
  }
  else
  if (
      location == "MIDDLERIGHT" &&
      input_pattern_current_head -> value == 0 &&
      input_pattern_current_head -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> next -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> next -> next -> next -> value  == 1 &&
      input_pattern_current_head -> next -> next -> next -> next -> next -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> next -> next -> next -> next -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> next -> next -> next -> next -> next -> next-> value  == 0 
  ){
    isEqual = 1;
  }
  else
  if (
      location == "BOTTOMLEFT" &&
      input_pattern_current_head -> value == 0 &&
      input_pattern_current_head -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> next -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> next -> next -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> next -> next -> next -> next -> value  == 1 &&
      input_pattern_current_head -> next -> next -> next -> next -> next -> next -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> next -> next -> next -> next -> next -> next-> value  == 0 
  ){
    isEqual = 1;
  }
  else
  if (
      location == "BOTTOMMIDDLE" &&
      input_pattern_current_head -> value == 0 &&
      input_pattern_current_head -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> next -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> next -> next -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> next -> next -> next -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> next -> next -> next -> next -> next -> value  == 1 &&
      input_pattern_current_head -> next -> next -> next -> next -> next -> next -> next -> next-> value  == 0 
  ){
    isEqual = 1;
  }
  else
  if (
      location == "BOTTOMRIGHT" &&
      input_pattern_current_head -> value == 0 &&
      input_pattern_current_head -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> next -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> next -> next -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> next -> next -> next -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> next -> next -> next -> next -> next -> value  == 0 &&
      input_pattern_current_head -> next -> next -> next -> next -> next -> next -> next -> next-> value  == 1 
  ){
    isEqual = 1;
  }
  
  return isEqual;
  
}

void setPattern(char *location, PATTERN_TYPE_P input_pattern_current_head)
{
  
  
  
  if (location == "TOPLEFT"){
      input_pattern_current_head -> value = 1 ;
      input_pattern_current_head -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> next -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> next -> next -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> next -> next -> next -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> next -> next -> next -> next -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> next -> next -> next -> next -> next -> next-> value  = 0 ;  
  }
  else
  if (location == "TOPMIDDLE"){
      input_pattern_current_head -> value = 0 ;
      input_pattern_current_head -> next -> value  = 1 ;
      input_pattern_current_head -> next -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> next -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> next -> next -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> next -> next -> next -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> next -> next -> next -> next -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> next -> next -> next -> next -> next -> next-> value  = 0 ;  
  }
  else
  if (location == "TOPRIGHT"){
      input_pattern_current_head -> value = 0 ;
      input_pattern_current_head -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> value  = 1 ;
      input_pattern_current_head -> next -> next -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> next -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> next -> next -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> next -> next -> next -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> next -> next -> next -> next -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> next -> next -> next -> next -> next -> next-> value  = 0 ;  
  }
  else
  if (location == "MIDDLELEFT"){
      input_pattern_current_head -> value = 0 ;
      input_pattern_current_head -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> next -> value  = 1 ;
      input_pattern_current_head -> next -> next -> next -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> next -> next -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> next -> next -> next -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> next -> next -> next -> next -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> next -> next -> next -> next -> next -> next-> value  = 0 ;  
  }
  else
  if (location == "MIDDLEMIDDLE"){
      input_pattern_current_head -> value = 0 ;
      input_pattern_current_head -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> next -> next -> value  = 1 ;
      input_pattern_current_head -> next -> next -> next -> next -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> next -> next -> next -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> next -> next -> next -> next -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> next -> next -> next -> next -> next -> next-> value  = 0 ;  
  }
  else
  if (location == "MIDDLERIGHT"){
      input_pattern_current_head -> value = 0 ;
      input_pattern_current_head -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> next -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> next -> next -> next -> value  = 1 ;
      input_pattern_current_head -> next -> next -> next -> next -> next -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> next -> next -> next -> next -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> next -> next -> next -> next -> next -> next-> value  = 0 ;  
  }
  else
  if (location == "BOTTOMLEFT"){
      input_pattern_current_head -> value = 0 ;
      input_pattern_current_head -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> next -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> next -> next -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> next -> next -> next -> next -> value  = 1 ;
      input_pattern_current_head -> next -> next -> next -> next -> next -> next -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> next -> next -> next -> next -> next -> next-> value  = 0 ;  
  }
  else
  if (location == "BOTTOMMIDDLE"){
      input_pattern_current_head -> value = 0 ;
      input_pattern_current_head -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> next -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> next -> next -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> next -> next -> next -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> next -> next -> next -> next -> next -> value  = 1 ;
      input_pattern_current_head -> next -> next -> next -> next -> next -> next -> next -> next-> value  = 0 ;  
  }
  else
  if (location == "BOTTOMRIGHT"){
      input_pattern_current_head -> value = 0 ;
      input_pattern_current_head -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> next -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> next -> next -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> next -> next -> next -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> next -> next -> next -> next -> next -> value  = 0 ;
      input_pattern_current_head -> next -> next -> next -> next -> next -> next -> next -> next-> value  = 1 ;  
  }
  
  
}



float calcDesiredInputOutputPatterns(NODE_TYPE_P node_tail, int size_of_pattern, PATTERN_TYPE_P input_pattern_current_head,PATTERN_TYPE_P output_pattern_desired_head, char bTminus1)
{
  
  int x;
  int greatestNode;
  float greatestNodeValue;
  NODE_TYPE_P node_current;
  char Test;
  float patternThreshold;
  char bMove = 'N'; // (U)p, (D)own (L)eft, (R)ight, (N)owhere
  char bMotivated = 'N';
  int aboveThresholdCount = 0;
  float reward = 1.0 ;
    
  patternThreshold = 0.5;
  
  Test = 'N';
 
  // pattern 1: 1 0 0 0 0 0 0 0 0
  // pattern 2: 0 1 0 0 0 0 0 0 0
  // pattern 3: 0 0 1 0 0 0 0 0 0
  // pattern 4: 0 0 0 1 0 0 0 0 0
  // pattern 5: 0 0 0 0 1 0 0 0 0
  // pattern 6: 0 0 0 0 0 1 0 0 0
  // pattern 7: 0 0 0 0 0 0 1 0 0
  // pattern 8: 0 0 0 0 0 0 0 1 0
  // pattern 9: 0 0 0 0 0 0 0 0 1
  
  // which output node was greatest?
  node_current = node_tail;
  if (node_current->node_type == 'R'){ // skip the first node which is the motivator node (assigned as a bias node)
      if (node_current -> node_activation > 0.80){
	  bMotivated = 'Y';
	//  LOG_LEVEL = 1;
	  reward = node_current -> node_activation * 0.95 ;// decay
      }
      node_current = node_current -> prev;
  }
  
  if (bMotivated == 'N')
  {
    reward = 0;
  }
  
  greatestNode = node_current -> node_number;
  greatestNodeValue = node_current -> node_activation;
    
  for (x=0; x < (size_of_pattern -1) ; ++x)
  {  
      if (node_current -> node_activation > greatestNodeValue){
	  greatestNode = node_current -> node_number;
	  greatestNodeValue = node_current -> node_activation;
      }
    
      if (node_current -> node_activation > patternThreshold){
	  aboveThresholdCount ++;
      }
    
    
    node_current = node_current -> prev;
  }
  
  
  if (greatestNodeValue > patternThreshold && aboveThresholdCount == 1 ){ // we have a clear winner !
      if(greatestNode == 0){
	bMove = 'U';
      }else if(greatestNode == 1){
	bMove = 'D';
      }else if(greatestNode == 2){
	bMove = 'L';
      }else if(greatestNode == 3){
	bMove = 'R';
      }
  }
  
  
  ////////////////////////////////////////////////////
  // if input pattern 1
  ////////////////////////////////////////////////////
  if (isPattern("TOPLEFT", input_pattern_current_head) == 1)
  {
    
    if (bMove == 'N'){
      // Leave input pattern as is
    }
    else if (bMove == 'U'){
      // Leave input pattern as is
    }
    else if (bMove == 'D'){
      setPattern("MIDDLELEFT", input_pattern_current_head);   
    }
    else if (bMove == 'L'){
      // Leave input pattern as is
    }
    else if (bMove == 'R'){
      // Leave input pattern as is
      
    }
    
    if (bMotivated == 'N')
    {
    }
    else
    {
	fprintf(log_file_p,"bMotivated == True TOPLEFT");
    }
  
  }
  
  ////////////////////////////////////////////////////
  // if input pattern 2
  ////////////////////////////////////////////////////
  else 
  if (isPattern("TOPMIDDLE", input_pattern_current_head) == 1)
  {
    
    // This is the GOAL !! Always reward, send back to beginning..
    reward = 1.0;
   
   // Move to beginning
   //setPattern("TOPLEFT", input_pattern_current_head);
   
  // setPattern("TOPRIGHT", input_pattern_current_head);
   //setPattern("MIDDLERIGHT", input_pattern_current_head);
   //setPattern("MIDDLEMIDDLE", input_pattern_current_head);
   setPattern("MIDDLELEFT", input_pattern_current_head);
   //setPattern("BOTTOMLEFT", input_pattern_current_head);  // ToDo remove !!
   //setPattern("BOTTOMMIDDLE", input_pattern_current_head);  // ToDo remove !!
   //  setPattern("BOTTOMRIGHT", input_pattern_current_head);  // ToDo remove !!
      
   
  }
    
  ////////////////////////////////////////////////////
  // if input pattern 3
  ////////////////////////////////////////////////////
  else 
  if (isPattern("TOPRIGHT", input_pattern_current_head) == 1)
  {
    
    if (bMove == 'N'){
      // Leave input pattern as is
    }
    else if (bMove == 'U'){
      // Leave input pattern as is
    }
    else if (bMove == 'D'){
      setPattern("MIDDLERIGHT", input_pattern_current_head);   
    }
    else if (bMove == 'L'){
      setPattern("TOPMIDDLE", input_pattern_current_head);   
    }
    else if (bMove == 'R'){
      // Leave input pattern as is
    }

    
    if (bMotivated == 'N')
    {
    }
    else
    {
	fprintf(log_file_p,"bMotivated == True TOPRIGHT");
    }

  
  }
  
  ////////////////////////////////////////////////////
  // if input pattern 4
  ////////////////////////////////////////////////////
  else 
  if (isPattern("MIDDLELEFT", input_pattern_current_head) == 1)
  {
    
    if (bMove == 'N'){
      // Leave input pattern as is
    }
    else if (bMove == 'U'){
      setPattern("TOPLEFT", input_pattern_current_head);   
    }
    else if (bMove == 'D'){
      setPattern("BOTTOMLEFT", input_pattern_current_head);   
    }
    else if (bMove == 'L'){
      // Leave input pattern as is
    }
    else if (bMove == 'R'){
      setPattern("MIDDLEMIDDLE", input_pattern_current_head);   
    }

    
    if (bMotivated == 'N')
    {
    }
    else
    {
	fprintf(log_file_p,"bMotivated == True MIDDLELEFT");
    }
 
  }

  
  ////////////////////////////////////////////////////
  // if input pattern 5
  ////////////////////////////////////////////////////
  else 
  if (isPattern("MIDDLEMIDDLE", input_pattern_current_head) == 1)
  {
    
    if (bMove == 'N'){
      // Leave input pattern as is
    }
    else if (bMove == 'U'){
      // Leave input pattern as is
     // setPattern("TOPMIDDLE", input_pattern_current_head);   //ToDo Remove
    }
    else if (bMove == 'D'){
      setPattern("BOTTOMMIDDLE", input_pattern_current_head);   
    }
    else if (bMove == 'L'){
      setPattern("MIDDLELEFT", input_pattern_current_head);   
    }
    else if (bMove == 'R'){
      setPattern("MIDDLERIGHT", input_pattern_current_head);   
    }

    
    
    if (bMotivated == 'N')
    {
    }
    else
    {
	fprintf(log_file_p,"bMotivated == True MIDDLEMIDDLE");
    }
  }
  ////////////////////////////////////////////////////
  // if input pattern 6
  ////////////////////////////////////////////////////
  else 
  if (isPattern("MIDDLERIGHT", input_pattern_current_head) == 1)
  {
    
    if (bMove == 'N'){
      // Leave input pattern as is
    }
    else if (bMove == 'U'){
      setPattern("TOPRIGHT", input_pattern_current_head);   
    }
    else if (bMove == 'D'){
      setPattern("BOTTOMRIGHT", input_pattern_current_head);   
    }
    else if (bMove == 'L'){
      setPattern("MIDDLEMIDDLE", input_pattern_current_head);   
    }
    else if (bMove == 'R'){
      // Leave input pattern as is
    }

        
    if (bMotivated == 'N')
    {
    }
    else
    {
	fprintf(log_file_p,"bMotivated == True MIDDLERIGHT");
    }
 
  }

  ////////////////////////////////////////////////////
  // if input pattern 7
  ////////////////////////////////////////////////////
  else 
  if (isPattern("BOTTOMLEFT", input_pattern_current_head) == 1)
  {
    
    if (bMove == 'N'){
      // Leave input pattern as is
    }
    else if (bMove == 'U'){
      setPattern("MIDDLELEFT", input_pattern_current_head);   
    }
    else if (bMove == 'D'){
      // Leave input pattern as is
    }
    else if (bMove == 'L'){
      // Leave input pattern as is
    }
    else if (bMove == 'R'){
      setPattern("BOTTOMMIDDLE", input_pattern_current_head);   
    }

    
    if (bMotivated == 'N')
    {
    }
    else
    {
	fprintf(log_file_p,"bMotivated == True BOTTOMLEFT");
    }
 
  }

  
  ////////////////////////////////////////////////////
  // if input pattern 8
  ////////////////////////////////////////////////////
  else 
  if (isPattern("BOTTOMMIDDLE", input_pattern_current_head) == 1)
  {
    
    if (bMove == 'N'){
      // Leave input pattern as is
    }
    else if (bMove == 'U'){
      setPattern("MIDDLEMIDDLE", input_pattern_current_head);   
    }
    else if (bMove == 'D'){
      // Leave input pattern as is
    }
    else if (bMove == 'L'){
      setPattern("BOTTOMLEFT", input_pattern_current_head);   
    }
    else if (bMove == 'R'){
      setPattern("BOTTOMRIGHT", input_pattern_current_head);   
    }

    
    
    if (bMotivated == 'N')
    {
    }
    else
    {
	fprintf(log_file_p,"bMotivated == True BOTTOMMIDDLE");
    }
 
  }

  ////////////////////////////////////////////////////
  // if input pattern 9
  ////////////////////////////////////////////////////
  else 
  if (isPattern("BOTTOMRIGHT", input_pattern_current_head) == 1)
  {
    
    if (bMove == 'N'){
      // Leave input pattern as is
    }
    else if (bMove == 'U'){
      setPattern("MIDDLERIGHT", input_pattern_current_head);   
    }
    else if (bMove == 'D'){
      // Leave input pattern as is
    }
    else if (bMove == 'L'){
      setPattern("BOTTOMMIDDLE", input_pattern_current_head);   
    }
    else if (bMove == 'R'){
      // Leave input pattern as is
    }

    
    if (bMotivated == 'N')
    {
    }
    else
    {
	fprintf(log_file_p,"bMotivated == True BOTTOMRIGHT");
    }
 
  }
  
  calcOutputPattern(node_tail, size_of_pattern, output_pattern_desired_head, reward,0,Test,patternThreshold,bTminus1);
  fprintf(log_file_p,"%s%10.3f ","Reward: ", reward);
  return reward;
  
 
}




