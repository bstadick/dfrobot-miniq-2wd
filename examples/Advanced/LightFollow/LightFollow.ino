/**
* @author Bryan Stadick - stadi012@umn.edu
* @version 1.1.0
*
* LightFollow.ino - Has the robot follow a light source around.
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

#define THRESH 1.0

DFRobot2WD robot = DFRobot2WD();

float light;

void setup() {
  // put your setup code here, to run once:

  // wait for key one to be pressed before beginning
  while(!robot.getKeyOne()) ;
}

void loop() {
  // put your main code here, to run repeatedly:
  
  // get surface refelectivity
  light = robot.getLight();
  
  // light is in the center
  if(light >= 2.5 - THRESH && light <= 2.5 + THRESH) {
      // drive forward
      robot.motorControl(FORWARD, 75, FORWARD, 75);
  }
  // light is slightly to left
  else if(light > 2.5 + THRESH) {
      robot.motorStop();
      // turn to left
      robot.motorRight(FORWARD, 65);
      // turn until center is over line
      robot.getLight();
      while(light > 2.5 + THRESH)
          robot.getLight();
      // drive forward
      robot.motorControl(FORWARD, 75, FORWARD, 75);
  }
  // light is slightly to right
  else {
      robot.motorStop();
      // turn to right
      robot.motorLeft(FORWARD, 65);
      // turn until center is over line
      robot.getLight();
      while(light < 2.5 - THRESH)
          robot.getLight();
      // drive forward
      robot.motorControl(FORWARD, 75, FORWARD, 75);
  }
}

