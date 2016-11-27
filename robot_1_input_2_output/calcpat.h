// Specific to : 
// robot_1_input_2_output
// Does not support tminus_1 currently
#include       "calcOutputPat.h"
extern int	LOG_LEVEL ;
extern int 	blueToothSerialConnection;

//For Bluetooth..
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include<fcntl.h>
#include<termios.h>


void calcDesiredInputOutputPatterns(NODE_TYPE_P node_tail, int size_of_pattern, PATTERN_TYPE_P input_pattern_current_head,PATTERN_TYPE_P output_pattern_desired_head, char bTminus1)
{
  
  
  NODE_TYPE_P node_current;
  char Test;
  float patternThreshold;
  
  char tempbuf[30];
  int w = 0;
  int n = 0;
  
  int lightLevel = -99;
  static int prevLightLevel = -99;
  int lightLevelNormalized = -99;
  
  
  float reward = 0.6 ;
    
  patternThreshold = 0.5;
  
  Test = 'N';
 
 // 1) Move (Write) (based on forward propagated activations)
  node_current = node_tail;
  tcflush(blueToothSerialConnection,TCIOFLUSH);
  if (node_current -> node_activation < 0.50) 		// Governor node
  {
    w=write(blueToothSerialConnection, "25", 2);  	// Stop !
  }
  else 
  { // Go !
    if (node_current -> prev -> node_activation < 0.50)
    {
      w=write(blueToothSerialConnection, "16", 2);  	// Left
    }
    else
    {
      w=write(blueToothSerialConnection, "34", 2);  	// Right
    }
  }
  
  usleep(500000);
	
               
  
 // 2) Read new input pattern, contains :
 // 	- reward for Move just made 
 //	- save new input pattern (this gets loaded onto network later, after backpropagation (learning) phase ) 
 // note : in (simple) light example the reward is the same as the input pattern
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
  printf("tempbuf: %s ",tempbuf); // we care about the light level only: 0-1023
  //lightLevel= atoi(tempbuf[1]);
  int result_ones = tempbuf[3] - '0';
  int result1_tens = tempbuf[2] - '0';
  int result1_hundreds = tempbuf[1] - '0';
  int result1_thousands = tempbuf[0] - '0';
  lightLevel = (result_ones) + (result1_tens * 10) + (result1_hundreds * 100) + (result1_thousands * 1000) ;
 
 // 3) Determine desired output pattern for Move just made based on reward:
 // 	- reinforce if 'correct' (reward) 
 //	- randomize if 'wrong'(punish)
  //if ((lightLevel > 100) || (lightLevel > prevLightLevel)) //light level must be more than 100 for reward
  if ((lightLevel > 100)) //light level must be more than 100 for reward
  {
    // + reinforce output
    calcOutputPattern(node_tail, size_of_pattern, output_pattern_desired_head, reward,1,Test,patternThreshold,bTminus1);
    lightLevelNormalized = 1;
  }
  else
  {
    // + invert output
    calcOutputPattern(node_tail, size_of_pattern, output_pattern_desired_head, 0.0,1,Test,patternThreshold,bTminus1);
    lightLevelNormalized = 0;
  }
 
 // save light reading for next turn..
  prevLightLevel = lightLevel;
 
 // 4) save new input pattern (this gets loaded onto network later, after backpropagation (learning) phase ) 
 
  input_pattern_current_head -> value = lightLevelNormalized ;
  
 
}




