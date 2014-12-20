/**
* @author Bryan Stadick - stadi012@umn.edu
* @version 1.1.0
*
* Wireless.ino - Takes drive commands from the wireless serial port.
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
    Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly: 
  
  // wireless communication simply uses the serial port
  if(Serial.available() > 1) {
      switch(Serial.read()) {
          case 'f':
              robot.motorControl(FORWARD, 75, BACKWARD, 75);
              delay(500);
              robot.motorStop();
              break;
          case 'b':
              robot.motorControl(BACKWARD, 75, BACKWARD, 75);
              delay(500);
              robot.motorStop();
              break;
          case 'r':
              robot.motorControl(BACKWARD, 75, FORWARD, 75);
              delay(500);
              robot.motorStop();
              break;
          case 'l':
              robot.motorControl(FORWARD, 75, BACKWARD, 75);
              delay(500);
              robot.motorStop();
              break;
      }
  }
}
