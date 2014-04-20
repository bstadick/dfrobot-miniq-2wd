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

