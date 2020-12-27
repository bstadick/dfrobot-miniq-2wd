/**
* @author Bryan Stadick - stadi012@umn.edu
* @version 1.1.0
*
* Avoid.ino - Robot moves to avoid obstacles.
* <p>
* MIT License
* <p>
* Copyright (c) 2020 Bryan Stadick
* <p>
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* <p>
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* <p>
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
* <p>
*/

#include <DFRobot2WD.h>
#include <Notes.h>

DFRobot2WD robot = DFRobot2WD();

obs_t obstacle;

void setup() {
  // put your setup code here, to run once:

  while(!robot.getKeyOne());
}

void loop() {
  // put your main code here, to run repeatedly: 
  
  obstacle = robot.obstacleDetect(NULL, NULL);
  
  // obstacle in front
  if(obstacle == BOTH) {
      // turn around
      robot.motorControl(BACKWARD, 65, FORWARD, 65);
      delay(250);
      robot.motorStop();
  }
  // obstacle to left
  else if(obstacle == LEFT) {
      // turn right
      robot.motorControl(BACKWARD, 65, FORWARD, 65);
      delay(250);
      robot.motorStop();
  }
  else if(obstacle == RIGHT) {
      // turn left
      robot.motorControl(FORWARD, 65, BACKWARD, 65);
      delay(250);
      robot.motorStop();
  }
  else {
      // go forward
      robot.motorControl(FORWARD, 65, FORWARD, 65);
      delay(100);
  }
}
