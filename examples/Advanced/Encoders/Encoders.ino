/**
* @author Bryan Stadick - stadi012@umn.edu
* @version 1.1.0
*
* Encoders.ino - Moves robot set number of encoder ticks and prints encoder
* counts to the serial port.
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
