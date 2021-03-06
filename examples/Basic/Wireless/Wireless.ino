/**
* @author Bryan Stadick - stadi012@umn.edu
* @version 1.1.0
*
* Wireless.ino - Takes drive commands from the wireless serial port.
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
