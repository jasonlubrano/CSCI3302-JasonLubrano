#include <Sparki.h> // include the sparki library

// 50.45 diameter of the wheel
// 85.86 axle length

int startTime, endTime, lineLeft, lineCenter, lineRight;
const int THRESHOLD = 700;

float halfRadius = 12.6125;
float radiusOverAxle = 0.29379;

float theta = 0.0;
float x = 0.0;
float y = 0.0;

float rightWheel = 0.0;
float leftWheel = 0.0;

float distance = 8.58
int COM = 0.0;

int loopTime = 100;
int leftturn = -17;
int righturn = 17;

void setup()
{
  sparki.print("");
  sparki.updateLCD();
  sparki.clearLCD();
}

void loop()
{
  startTime = millis();
  line_Left   = sparki.lineLeft();   // measure the left IR sensor
  line_Center = sparki.lineCenter(); // measure the center IR sensor
  line_Right  = sparki.lineRight();  // measure the right IR sensor

  if ( lineCenter < THRESHOLD ) // if line is below left line sensor
  {
    rightwheel = .03;
    leftwheel = .03;
    sparki.moveForward(); // move forward
  }
  else if ( lineLeft < THRESHOLD ) // if line is below left line sensor
    {
      rightwheel = .03;
      leftwheel = -.03;
      theta += leftturn;
      sparki.moveLeft(); // turn left
    }

  else if ( lineRight < THRESHOLD ) // if line is below right line sensor
    {
      rightwheel = -.03;
      leftwheel = .03;
      theta += righturn;
      sparki.moveRight(); // turn right
    }
  else if ((line_center < threshhold) && (line_left < threshhold) && (line_right < threshhold))
    {
      theta = 0;
      x = 0;
      y = 0;
    }
COM = (rightWheel + leftWheel) * 0.5;
x += cos(theta) * COM * loopTime;
y += sin(theta) * COM * loopTime;


sparki.println("X: " , x / 10);
sparki.println("Y: " , y/10);
sparki.println("Theta: " , theta);
sparki.updateLCD();

endTime = millis();

if (loopTime - (endTime - startTime) > 0)
  {
    delay(loopTime - (endTime - startTime));
  }
}
