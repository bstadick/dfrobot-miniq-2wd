/**
* @author Bryan Stadick - stadi012@umn.edu
* @version 1.1.0
*
* Micromouse.ino - Micromouse base code. Simply demos some of the functions available.
*/

/* This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
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
