#include <sparki.h>

#define CYCLE_TIME .100  // seconds
#define LOOPCLOSURE 0 // if 1 then we do loop enclosure

// Program States
#define CONTROLLER_FOLLOW_LINE 1
#define CONTROLLER_DISTANCE_MEASURE 2

// state vars
int current_state = 1;

// the higher the threshold the whiter the surface, the lower the threshold, the darker the line
const int threshhold = 500;
int line_left = 1000;
int line_center = 1000;
int line_right = 1000;

// vars for loop, TIME IS OVERFLOW ERROR
unsigned long startTime, endTime, totalLoop;
int loopTime = 1000 * CYCLE_TIME; // desired loop time

// used to find the speed, used floats for decimals
float velocity, velDist = 30, velTime, vel = 0.03;
//vel is taken from velocity calculated at 0.28 (m/s) converted to (cm/ms)

// position variables
float pose_x = 0., pose_y = 0., pose_theta = 0.;

float rightWheel = 0.0; //right wheel start
float leftWheel = 0.0; // left wheel start

float D = 8.1; // distance between wheels (cm)
float halfLR = 0.0;

void setup() {
  // put your setup code here, to run once:
  sparki.RGB(RGB_RED); // red for starting setup
  sparki.servo(SERVO_CENTER);
  sparki.RGB(RGB_GREEN); // green for done setup
  // put your setup code here, to run once:
  //code for lab 2
  pose_x = 0.;
  pose_y = 0.;
  pose_theta = 0.;
}

void readSensors(){
  line_left = sparki.lineLeft();   // measure the left IR sensor
  line_right = sparki.lineRight();   // measure the right IR sensor
  line_center = sparki.lineCenter(); // measure the center IR sensor
}

void measure_30cm_speed() {
  // TODO
  startTime = millis(); // get the starting time to move
  sparki.moveForward(velDist); // getting sparki to move forward 30cm
  endTime = millis(); // get the final time to move
  sparki.moveStop();
  // by now sparki should have the start and end time of his moving forward
  velTime = endTime - startTime; // calc final time
  velocity = (velDist * 0.01) / (velTime * 0.0001); // v = d / t
  // display our odo
  sparki.print("current state: ");
  sparki.println(current_state);
  //tell sparki to print velocity  
  sparki.print("velocity (m/s): ");
  sparki.println(velocity);
  // tell sparki to print distance
  sparki.print("distance (m): ");
  sparki.println(velDist * 0.01);
  // tell sparki to print the time
  sparki.print("time (s) : ");
  sparki.println(velTime * 0.0001);
  current_state = CONTROLLER_FOLLOW_LINE;
}

void followLine() {
  if (line_left < threshhold){
    //the left censor is over the threshold
    sparki.moveLeft();
    rightWheel = vel;
    leftWheel = -1 * vel;
  } 
  if (line_right < threshhold) {
    // the right censor is over the threshold
    sparki.moveRight();
    rightWheel = -1 * vel;
    leftWheel = vel;
  }
  if (line_center < threshhold) {
    // the center censor is over the threshold
    sparki.moveForward();
    rightWheel = vel;
    leftWheel = vel;
  }
  
  if(LOOPCLOSURE){
    if ((line_center < threshhold) && (line_left < threshhold) && (line_right < threshhold)){
      //reset position to 0 - loop closure
      pose_x = 0.;
      pose_y = 0.;
      pose_theta = 0.;
    }
  }

  //calculating the odometry
  halfLR = (rightWheel + leftWheel) * 0.5; // x coordinate from Sparki's FoR
  pose_x += cos(pose_theta) * halfLR * loopTime; // x coord in global FoR
  pose_y += sin(pose_theta) * halfLR * loopTime; // y coord in global FoR
  pose_theta += (rightWheel - leftWheel) * (1 / (D * 100)) * loopTime * 10; // theta in global FoR
}

void displayOdometry() {
  // TODO
  // print the current state
  sparki.print("current state: ");
  sparki.println(current_state);
  // print x
  sparki.print("x: ");
  sparki.println(pose_x / 10);
  // print y
  sparki.print("y: ");
  sparki.println(pose_y / 10);
  // print theta
  sparki.print("(-): "); //its supposed to be a theta, ok?
  sparki.println(pose_theta * 57);
}

void loop() {
  readSensors(); // reads the line sensors
  sparki.clearLCD();
  switch (current_state) {
    case CONTROLLER_FOLLOW_LINE:
      // TODO
      followLine(); // tell sparki to follow the line
      displayOdometry(); // calculates the odometry
      break;
    case CONTROLLER_DISTANCE_MEASURE:
      measure_30cm_speed(); // going to measure the speed and display
      break;
    default:
      break;
  }
  sparki.updateLCD();
  // i rewrote it for a better style, but I think this should be it
  totalLoop = loopTime - (endTime - startTime);
  (totalLoop > 0) ? delay(totalLoop) : delay(0);
}