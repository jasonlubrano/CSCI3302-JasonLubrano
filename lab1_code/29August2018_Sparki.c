#include <sparki.h>

#define STATE_SEARCH_OBJ 1
#define STATE_MOV_TO_OBJ 2
#define STATE_CAPTURE_ATTEMPT 3
#define STATE_FOLLOWING_LINE 4
#define STATE_DRIVING_TO_LINE 5
#define STATE_ON_START 6// Set up some global variables with default values to be replaced during operation
#define STATE_FINISHED 7

int current_state = STATE_SEARCH_OBJ;
const int threshold = 750; // IR reading threshold to detect whether there's a black line under the sensor
//the higher the threshold the whiter the surface, the lower the threshold, the darker the line
int cm_distance = 1000;
int line_left = 1000;
int line_center = 1000;
int line_right = 1000;
int count = 0;

void setup(){
  // put your setup code here, to run once:
  sparki.RGB(RGB_RED); // Turn on the red LED
  sparki.servo(SERVO_CENTER); // Center the ultrasonic readSensors
  delay(1000); // Give the motor time to turn
  sparki.gripperOpen(); // Open the gripper
  delay(5000); // Give the motor time to open the griper
  sparki.gripperStop(); // 5 seconds should be long enough
  sparki.RGB(RGB_GREEN); // Change LED to green so we know1 the robot's setup is done!
}

void readSensors(){
  cm_distance = 0; // Replace with code to read the distance sensor
  line_left = sparki.lineLeft();   // measure the left IR sensor
  line_right = sparki.lineRight();   // measure the right IR sensor
  line_center = sparki.lineCenter(); // measure the center IR sensor
}

void searchingForObject(){
  sparki.println("trying to rotate but im a stupid robot\n");
  sparki.motorRotate(MOTOR_RIGHT, DIR_CW, 25); // make sparki rotate clockwise with the right motor with speed 25
  delay(150);// trying to get him to turn slightly mlonger?
  cm_distance = sparki.ping(); // finds the distance of an object
  sparki.print("distance: ");
  sparki.println(cm_distance);
  if(cm_distance <= 30 && cm_distance >= 0){
    sparki.motorStop(MOTOR_RIGHT); //this would be to stop the right motor
    sparki.moveLeft(5); //turn right
    current_state = STATE_MOV_TO_OBJ; // moving to an object
  }
  delay(100);
}

void movingToObject(){
  cm_distance = sparki.ping(); // Replace with code to read the distance sensor
  sparki.println("moving to the object\n i am a stupid robot\n");
  sparki.print("distance: ");
  sparki.println(cm_distance);
  sparki.moveForward(); // moving forward
  if(cm_distance < 5 && cm_distance >= 0){
    sparki.moveStop();
    current_state = STATE_CAPTURE_ATTEMPT; //moving to caputre the thing
  }
  delay(100);
}

void gripObject(){
  sparki.println("gripping the object with my stupid arms");
  sparki.moveStop();
  sparki.gripperClose();
  delay(4000);
  sparki.gripperStop();
  sparki.moveRight(180); //testing to see if it turns 
  current_state = STATE_DRIVING_TO_LINE;
}

void drivingToLine(){
  sparki.println("driving to the line with my stupid wheels");
  // if the center line sensor is the only one reading a line
  sparki.moveForward(); // move backwards for some reason movig forward he coundt do
  sparki.print("Line Left: "); // show left line sensor on screen
  sparki.println(line_left);
  sparki.print("Line Center: "); // show center line sensor on screen
  sparki.println(line_center);
  sparki.print("Line Right: "); // show right line sensor on screen
  sparki.println(line_right);
  if(line_center < threshold || line_right < threshold || line_left < threshold){
    // check to see if we found a line on any sensor
    sparki.moveStop();
    delay(25);
    if(line_left < threshold){
      while(line_center > threshold){
        sparki.moveRight(1);
        delay(200);
        sparki.moveForward(1);
        delay(200);
      }
    }
    if(line_right < threshold){
      while(line_center > threshold){
        sparki.moveLeft(1);
        delay(200);
        sparki.moveForward(1);
        delay(200);
      }
    }    
    current_state = STATE_FOLLOWING_LINE;
  }
}

void followingLine(){
  //test to see if he needs to go left
  if(line_left < threshold){
    //the left censor is over the threshold
    sparki.moveLeft(3); //turn left
  }
  if (line_right < threshold){
    //the right is over the threshold
    sparki.moveRight(3); //turn right
  }
  if((line_right > threshold) && (line_left > threshold) && (line_center < threshold)){
    //the ceter is over the threshold
    sparki.moveForward(1);//move forward
  }
  if ((line_right > threshold) && (line_left > threshold) && (line_center > threshold)){
    //the ceter is over the threshold
    sparki.moveBackward(1);
    sparki.moveStop();
    //sparki.moveBackward(2);
    //sparki.moveStop();
  } 
  if((line_right < threshold) && (line_left < threshold) && (line_center < threshold)){
    sparki.moveBackward(1);
    sparki.moveLeft(1); //turn left
    sparki.moveStop(); //stop to change state
    current_state = STATE_ON_START; //change the state
   }
  sparki.print("Threshold: ");// show the treshold
  sparki.println(threshold);
  sparki.print("Line Left: "); // show left line sensor on screen
  sparki.println(line_left);
  sparki.print("Line Center: "); // show center line sensor on screen
  sparki.println(line_center);
  sparki.print("Line Right: "); // show right line sensor on screen
  sparki.println(line_right);
  //test to see if sparki is at the start
}

void onStart(){
  sparki.println("I have found the start!");
  sparki.gripperOpen(); // Open the gripper
  delay(5000); // Give the motor time to open the griper
  sparki.gripperStop(); // 5 seconds should be long enough
  sparki.moveStop();
  sparki.println("BEEP!");
  sparki.beep();
  current_state = STATE_FINISHED;
}

void finished(){
  sparki.println("Give my genius creators an A please");
}


void loop() {
  // put your main code here, to run repeatedly:
  readSensors();
  sparki.clearLCD();
  sparki.print("STATE: ");
  sparki.println(current_state);
  sparki.updateLCD();
  delay(100);
  // Your state machine code goes here
  switch(current_state){
    case STATE_SEARCH_OBJ: searchingForObject(); break; //1
    case STATE_MOV_TO_OBJ: movingToObject(); break; //2
    case STATE_CAPTURE_ATTEMPT: gripObject(); break; //3
    case STATE_DRIVING_TO_LINE: drivingToLine(); break; //4
    case STATE_FOLLOWING_LINE: followingLine(); break; //5
    case STATE_ON_START: onStart(); break; //6
    case STATE_FINISHED: finished(); break; //7
    default: break;
  }
  sparki.updateLCD();
  delay(500); // Only run controller at 10Hz
}