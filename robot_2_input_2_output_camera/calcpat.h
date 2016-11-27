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


int getPixyValue(char* searchString, char* tempbuf)
{
  char *s;
  char miniTempbuf[3];
  strcpy(miniTempbuf, "   ");
  
  s = strstr(tempbuf, "x:"); 
  strncpy (miniTempbuf,s+3, 3);
  
  int ones = 0;
  int tens = 0;
  int hundreds = 0;
  
  if (miniTempbuf[1] == ' ') 
  // must be "n  " eg "7  "
  // get first char only
  {
    ones = miniTempbuf[0] - '0';
  }
  else if (miniTempbuf[2] == ' ') 
  // must be "nn " eg "78 "
  // get first and second char
  {
    tens = miniTempbuf[0] - '0';
    ones = miniTempbuf[1] - '0';
  }
  else 
  // Must be "nnn" eg "789"
  // get first, second and third char
  {
    hundreds = miniTempbuf[0] - '0';
    tens = miniTempbuf[1] - '0';
    ones = miniTempbuf[2] - '0';
  }
  
  
  int pixyValue = (ones) + (tens * 10) + (hundreds * 100) ;
  
  return pixyValue;
  
}

void calcDesiredInputOutputPatterns(NODE_TYPE_P node_tail, int size_of_pattern, PATTERN_TYPE_P input_pattern_current_head,PATTERN_TYPE_P output_pattern_desired_head, char bTminus1)
{
  
  
  NODE_TYPE_P node_current;
  char Test;
  float patternThreshold;
  
  char tempbuf[100];
  
  int w = 0;
  int n = 0;
  
  int pixyValue = -99;
  int pixyValueNormalized = -99;
  float pixyGovernorValue;
  
  
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
  strcpy(tempbuf, "                                                                                                   ");
  n=read(blueToothSerialConnection,tempbuf,99);/*printf("%d \n",n);*/
  tempbuf[99]= '\0';
  printf("tempbuf: %s ",tempbuf);
		
  strcpy(tempbuf, "                                                                                                   ");
  n=read(blueToothSerialConnection,tempbuf,99);/*printf("%d \n",n);*/
  tempbuf[99]= '\0';
  printf("tempbuf: %s ",tempbuf);
  
  strcpy(tempbuf, "                                                                                                   ");
  n=read(blueToothSerialConnection,tempbuf,99);/*printf("%d \n",n);*/
  tempbuf[99]= '\0';
  printf("tempbuf: %s ",tempbuf);
  int blocks_detected = tempbuf[9] - '0';
  int i;
      
  
  for( i = 1; i <= blocks_detected; i++ )
  {
    //strcpy (tempbuf,"  block 0: CC block! sig: 212 (138 decimal) x: 282 y: 158 width: 43 height: 15 angle 99");
    //strcpy (tempbuf,"  block 0: CC block! sig: 12 (10 decimal) x: 12 y: 73 width: 12 height: 5 angle -164");
    //strcpy (tempbuf,"  block 0: CC block! sig: 12 (10 decimal) x: 9 y: 73 width: 12 height: 5 angle -164");
 
    strcpy(tempbuf, "                                                                                                   ");
    n=read(blueToothSerialConnection,tempbuf,99);/*printf("%d \n",n);*/
    tempbuf[99]= '\0';
    printf("tempbuf: %s ",tempbuf); 
    
  
    pixyValue= getPixyValue("x:",tempbuf);
    
   
  }
                
 
 // 3) Determine desired output pattern for Move just made based on reward:
 // 	- reinforce if 'correct' (reward) 
 //	- randomize if 'wrong'(punish)
  if ((pixyValue > -1)) 
  {
    
    // pixy.blocks[i].x The x location of the center of the detected object (0 to 319): left to right
    if ((pixyValue > 0)&&(pixyValue <= 106 ))
    {
      pixyValueNormalized = -1;
      // + invert output
      calcOutputPattern(node_tail, size_of_pattern, output_pattern_desired_head, 0.0,1,Test,patternThreshold,bTminus1);
    }
    else if ((pixyValue >= 107 )&&(pixyValue <= 212 ))
    {
      pixyValueNormalized = 0;
      // + reinforce output
      calcOutputPattern(node_tail, size_of_pattern, output_pattern_desired_head, reward,1,Test,patternThreshold,bTminus1);
    }
    else // ((pixyValue >=213  )&&(pixyValue <= 319 ))
    {
      pixyValueNormalized = 1;
      // + invert output
      calcOutputPattern(node_tail, size_of_pattern, output_pattern_desired_head, 0.0,1,Test,patternThreshold,bTminus1);
    }
    
    
    pixyGovernorValue = 1;
  }
  else
  {
    pixyValueNormalized = 0;
    pixyGovernorValue = 0;
    // + invert output
    calcOutputPattern(node_tail, size_of_pattern, output_pattern_desired_head, 0.0,1,Test,patternThreshold,bTminus1);
  }
 
 
 // 4) save new input pattern (this gets loaded onto network later, after backpropagation (learning) phase ) 
  input_pattern_current_head -> value = pixyValueNormalized ;
  input_pattern_current_head -> next -> value = pixyGovernorValue ;
  usleep(500000);  //sleep 0.5 seconds
  
 
}




