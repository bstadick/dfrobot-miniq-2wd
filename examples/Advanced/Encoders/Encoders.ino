/**
* @author Bryan Stadick - stadi012@umn.edu
* @version 1.1.0
*
* Encoders.ino - Moves robot set number of encoder ticks and prints encoder
* counts to the serial port.
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
  // wait for key one to be pressed before beginning
  while(!robot.getKeyOne()) ;
}

void loop() {
    while(robot.getEncRight() < 10 && robot.getEncLeft() < 10)
    {
        robot.motorRight(FORWARD, 60);
        robot.motorLeft(FORWARD, 60);
    }

    robot.motorRight(FORWARD, 0);
    robot.motorLeft(FORWARD, 0);
    
    Serial.print(robot.getEncRight());
    Serial.print(", ");
    Serial.println(robot.getEncLeft());

    robot.setEnc(0, 0);

    delay(1000);

    while(!robot.getKeyOne())
        delay(100);
}
