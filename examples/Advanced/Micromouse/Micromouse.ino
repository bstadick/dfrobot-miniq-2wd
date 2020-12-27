/**
* @author Bryan Stadick - stadi012@umn.edu
* @version 1.1.0
*
* Micromouse.ino - Micromouse base code. Simply demos some of the functions
* available.
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

DFRobot2WD robot = DFRobot2WD();

#define ENC
#define REFLECT
#define OBSIR
#define OBSTACT
#define REMOTE
#define IO
#define AVOID

float* buf;
int i = 0, r, l;
char toggle = 0;

void setup()
{
    Serial.begin(9600);
#ifdef OBSTACT
    robot.initTactile();
#endif
    while(!robot.getKeyOne()) ;
}

void loop()
{
    
#ifdef ENC
    while(robot.getEncRight() < 10 && robot.getEncLeft() < 10)
    {
        robot.motorRight(FORWARD, 60);
        robot.motorLeft(FORWARD, 60);
    }

    robot.motorStop();
    
    Serial.print(robot.getEncRight());
    Serial.print(", ");
    Serial.println(robot.getEncLeft());

    robot.setEnc(0, 0);

    delay(1000);

    while(!robot.getKeyOne())
        delay(100);
#endif    

#ifdef REFLECT
    robot.getReflectivity(buf);

    for(i = 0; i < 5; i++)
    {
        Serial.print(buf[i]);
        Serial.print(", ");
    }
    Serial.println("");
#endif

#ifdef OBSIR
    Serial.print("O: ");
    Serial.print(robot.obstacleDetect(&r, &l));
    Serial.print(" r: ");
    Serial.print(r);
    Serial.print(" l: ");
    Serial.println(l);
#endif

#ifdef OBSTACT
    Serial.print("T: ");
    Serial.print(robot.getTactileState());
#endif

#ifdef REMOTE
    Serial.print("R: ");
    Serial.println(robot.getIRRemote());
#endif

#ifdef IO
    digitalWrite(LED_RED, !(toggle & 1));
    digitalWrite(LED_GREEN, toggle & 1);
    if(toggle & 2)
        robot.playNote(NOTE_P_C1, 600);
    toggle = toggle ^ 3;
    delay(400);
#endif

#ifdef AVOID
    while(!robot.getKeyOne()) ;
    while(!robot.getKeyOne())
        avoidWall();
#endif

    delay(100);
    
}

