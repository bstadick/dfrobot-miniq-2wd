/**
* @author Bryan Stadick - stadi012@umn.edu
* @version 1.1.0
*
* IRCommand.ino - Allows for control of the robot using the included IR remote.
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

void setup() {
  // put your setup code here, to run once:

  // wait for key one to be pressed before beginning
  while(!robot.getKeyOne()) ;
}

void loop() {
    int code = robot.getIRRemote();
    switch(code) {
        case 0xff00: // No IR command = Stop
            robot.motorStop();
            break;
        case 0xfe01: // Vol+ = Forward
            robot.motorControl(FORWARD,100,FORWARD,100);
            break;
        case 0xf609: // Vol- = Backward
            robot.motorControl(BACKWARD,100,BACKWARD,100);
            break;
        case 0xfb04:  // Prev? = Left?
            robot.motorControl(FORWARD,100,BACKWARD,100);
            break;
        case 0xf906: // Next? = Right?
            robot.motorControl(BACKWARD,100,FORWARD,100);
            break;
        default:
            robot.motorStop();
            break;
    }
    Serial.println(code);
}
