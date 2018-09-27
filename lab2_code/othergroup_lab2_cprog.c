#include <sparki.h>

#define CYCLE_TIME .100  // seconds

// Program States
#define CONTROLLER_FOLLOW_LINE 1
#define CONTROLLER_DISTANCE_MEASURE 2


//global vars
int currentState = 0;
int distanceToObject = 30;
int distanceBeforeObject  = 7;
int threshhold = 500;

int startTime, endTime, totalLoop;
float pose_x = 0., pose_y = 0., pose_theta = 0.;

void readSensors(){
	cm_distance = 0; // Replace with code to read the distance sensor
	line_left = sparki.lineLeft();   // measure the left IR sensor
	line_right = sparki.lineRight();   // measure the right IR sensor
	line_center = sparki.lineCenter(); // measure the center IR sensor
}

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

void followLine(){
	if (line_left < threshhold){
		sparki.moveLeft();
	}
	if (line_right < threshhold) {
		sparki.moveRight();
	}
	if (line_center < threshhold) {
		sparki.moveForward();
	}
	if ((line_center < threshhold) && (line_left > threshhold) && (line_right > threshhold)){
		sparki.moveForward();
	}
	if ((line_center < threshhold) && (line_left < threshhold) && (line_right < threshhold)){
		/////reset position to 0
	}
}

void measure_30cm_speed() {
  // TODO
}


void updateOdometry() {
  // TODO
}

void displayOdometry() {
  // TODO
}

void loop() {
	readSensors(); // reads the line sensors
	sparki.clearLCD();
	startTime = millis(); // get the starting time of the loop

	switch (current_state) {
		case CONTROLLER_FOLLOW_LINE:
			// TODO
			break;
		case CONTROLLER_DISTANCE_MEASURE:
			measure_30cm_speed();
			break;
		default:
			break;
	}



	followLine(); // calls the following line function
	endTime = millis(); // get the final time of the loop
	sparki.updateLCD();

	totalLoop = loopTime - (endTime - startTime);
	if (totalLoop > 0) {
		delay(totalLoop);
	}
}