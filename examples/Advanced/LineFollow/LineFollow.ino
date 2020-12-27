/**
* @author Bryan Stadick - stadi012@umn.edu
* @version 1.1.0
*
* LineFollow.ino - Has the robot follow a line.
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

#define THRESH 3.0

DFRobot2WD robot = DFRobot2WD();

float reflect[5];

void setup() {
  // put your setup code here, to run once:

  // wait for key one to be pressed before beginning
  while(!robot.getKeyOne()) ;
}

void loop() {
  // put your main code here, to run repeatedly:
  
  // get surface refelectivity
  robot.getReflectivity(reflect);
  
  // center is over line
  if(reflect[3] >= THRESH)
      // drive forward
      robot.motorControl(FORWARD, 75, FORWARD, 75);
  // line slightly to left
  else if(reflect[2] >= THRESH || reflect[0] >= THRESH) {
      robot.motorStop();
      // turn to left
      robot.motorRight(FORWARD, 65);
      // turn until center is over line
      robot.getReflectivity(reflect);
      while(reflect[3] < THRESH)
          robot.getReflectivity(reflect);
      // drive forward
      robot.motorControl(FORWARD, 75, FORWARD, 75);
  }
  // line slightly to right
  else if(reflect[4] >= THRESH || reflect[5] >= THRESH) {
      robot.motorStop();
      // turn to right
      robot.motorLeft(FORWARD, 65);
      // turn until center is over line
      robot.getReflectivity(reflect);
      while(reflect[3] < THRESH)
          robot.getReflectivity(reflect);
      // drive forward
      robot.motorControl(FORWARD, 75, FORWARD, 75);
  }
  // line is not in sight
  else
      // search for the line
      if(!search()) {
          // line not found, give up
          robot.playNote(C1, 250);
          delay(250);
          robot.playNote(C1, 250);
          // wait for key one to be pressed before beginning again
          while(!robot.getKeyOne()) ;
      }
      else
          robot.motorControl(FORWARD, 75, FORWARD, 75);
}

// search for line if lost completely
boolean search() {
    
    for(int i = 5; i < 53; i += 2) {
        // sweep left
        robot.motorRight(FORWARD, 65);
        while(robot.getEncRight() < i){
            robot.getReflectivity(reflect);
            if(reflect[0] >= THRESH ||
               reflect[1] >= THRESH ||
               reflect[2] >= THRESH ||
               reflect[3] >= THRESH ||
               reflect[4] >= THRESH) {
                robot.motorStop();
                return true;
            }
        }
        robot.motorStop();
        
        // go back to original position
        robot.motorRight(BACKWARD, 65);
        while(robot.getEncRight() < i) ;
        robot.motorStop();
        
        // sweep right
        robot.motorLeft(FORWARD, 65);
        while(robot.getEncLeft() < i){
            robot.getReflectivity(reflect);
            if(reflect[0] >= THRESH ||
               reflect[1] >= THRESH ||
               reflect[2] >= THRESH ||
               reflect[3] >= THRESH ||
               reflect[4] >= THRESH) {
                robot.motorStop();
                return true;
            }
        }
        robot.motorStop();
        
        // go back to original position
        robot.motorLeft(BACKWARD, 65);
        while(robot.getEncLeft() < i) ;
        robot.motorStop();
    }
    return false;
}
