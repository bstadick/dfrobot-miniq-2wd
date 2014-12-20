/**
* @author Bryan Stadick - stadi012@umn.edu
* @version 1.1.0
*
* Avoid.ino - Robot moves to avoid obstacles.
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
