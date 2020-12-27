/**
* @author Bryan Stadick - stadi012@umn.edu
* @version 1.1.0
*
* IRCommand.ino - Allows for control of the robot using the included IR remote.
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
