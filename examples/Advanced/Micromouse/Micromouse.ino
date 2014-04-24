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

