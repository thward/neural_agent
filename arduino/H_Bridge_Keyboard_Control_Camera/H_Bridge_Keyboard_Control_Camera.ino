//Keyboard Controls:
//
// 1 -Motor 1 Left
// 2 -Motor 1 Stop
// 3 -Motor 1 Right
//
// 4 -Motor 2 Left
// 5 -Motor 2 Stop
// 6 -Motor 2 Right

// Declare L298N Dual H-Bridge Motor Controller directly since there is not a library to load.

// Motor 1
int dir1PinA = 2;
int dir2PinA = 3;
int speedPinA = 9; // Needs to be a PWM pin to be able to control motor speed

// Motor 2
int dir1PinB = 4;
int dir2PinB = 5;
int speedPinB = 10; // Needs to be a PWM pin to be able to control motor speed

// Light sensor
int lightLevel = 0;
int prevLightLevel = 0;

#include <SPI.h>  
#include <Pixy.h>
// This is the main Pixy object 
Pixy pixy;


// sound
int piezo = 8;
int duration = 250;

void setup() 
{  // Setup runs once per reset
    // initialize serial communication @ 9600 baud:
    Serial.begin(9600);
    Serial.print("Starting...\n");
    
    pixy.init();
    
    //Define L298N Dual H-Bridge Motor Controller Pins
    
    pinMode(dir1PinA,OUTPUT);
    pinMode(dir2PinA,OUTPUT);
    pinMode(speedPinA,OUTPUT);
    pinMode(dir1PinB,OUTPUT);
    pinMode(dir2PinB,OUTPUT);
    pinMode(speedPinB,OUTPUT);
    
    pinMode(piezo,OUTPUT);

}

void loop() 
{

   
    // Initialize the Serial interface:
    
    if (Serial.available() > 0) 
    {
        int inByte = Serial.read();
       
        
        switch (inByte) 
        {
        
            //______________Motor 1______________
            
            case '1': // Motor 1 Reverse
            analogWrite(speedPinA, 255);//Sets speed variable via PWM 
            digitalWrite(dir1PinA, LOW);
            digitalWrite(dir2PinA, HIGH);
            Serial.println("Motor 1 Reverse"); 
            break;
            
            case '2': // Motor 1 Stop (Freespin)
            analogWrite(speedPinA, 0);
            digitalWrite(dir1PinA, LOW);
            digitalWrite(dir2PinA, HIGH);
            Serial.println("Motor 1 Stop");
            break;
            
            case '3': // Motor 1 Forward
            analogWrite(speedPinA, 255);
            digitalWrite(dir1PinA, HIGH);
            digitalWrite(dir2PinA, LOW);
            Serial.println("Motor 1 Forward");
            break;
            
            //______________Motor 2______________
            
            case '4': // Motor 2 Reverse
            //analogWrite(speedPinB, 255);
            analogWrite(speedPinB, 200);
            digitalWrite(dir1PinB, LOW);
            digitalWrite(dir2PinB, HIGH);
            Serial.println("Motor 2 Reverse");
            break;
            
            case '5': // Motor 1 Stop (Freespin)
            analogWrite(speedPinB, 0);
            digitalWrite(dir1PinB, LOW);
            digitalWrite(dir2PinB, HIGH);
            Serial.println("Motor 2 Stop");
            break;
            
            case '6': // Motor 2 Forward
            //analogWrite(speedPinB, 255);
            analogWrite(speedPinB, 165);
            digitalWrite(dir1PinB, HIGH);
            digitalWrite(dir2PinB, LOW);
            Serial.println("Motor 2 Forward");
            break;
            
            default:
            // turn all the connections off if an unmapped key is pressed:
            for (int thisPin = 2; thisPin < 11; thisPin++) 
            {
                digitalWrite(thisPin, LOW);
            } 
        
        } // switch
        // Only do this on the second pass (after second motor command)..
        if ((inByte == '4')||(inByte == '5')||(inByte == '6')){

            //lightLevel = analogRead(A0); // Not using light meter currently

            uint16_t blocks;
            int exit = 0;
            int i = 0;
            int j;
            char buf[32]; 
 

            while (exit == 0)
            {
              
              // grab blocks!
              blocks = pixy.getBlocks();
              
              // Try finding blocks up to 1000 times, if none are found assume there really are none
              if (blocks || i==1000) 
              {
                sprintf(buf, "Detected %d:\n", blocks);
                Serial.print(buf);
                for (j=0; j<blocks; j++)
                {
                  sprintf(buf, "  block %d: ", j);
                  Serial.print(buf); 
// pixy.blocks[i].signature The signature number of the detected object (1-7)
// pixy.blocks[i].x The x location of the center of the detected object (0 to 319): left to right
// pixy.blocks[i].y The y location of the center of the detected object (0 to 199): top to bottom
// pixy.blocks[i].width The width of the detected object (1 to 320)
// pixy.blocks[i].height The height of the detected object (1 to 200)
// pixy.blocks[i].print() A member function that prints the detected object information to the serial port
                  pixy.blocks[j].print();
                }
                exit = 1;
              }
              
              if (blocks) 
              {
                tone(piezo,880,duration);
              }
              else 
              {
                tone(piezo,261,duration);
              }
              
              i++;
              
            }
           
            
            // right justify to make easier to read and convert back to int..
//            char buff[4];
//            sprintf(buff, "%04d", lightLevel);
//            Serial.println(buff);
            
 
            

        }
                   
    } // if serial available
    
} // loop
