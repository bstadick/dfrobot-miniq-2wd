/**
* @author Bryan Stadick - stadi012@umn.edu
* @version 1.0.0
*
* Controls.ino - Example PID control for the DFRobot MiniQ 2WD.
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

/**
* Performs simple PID control.
*/
void pid()
{

}

/**
* Follow a line.
*/
void followLine()
{
    
}

/**
* Avoid obstacles.
*/
void avoidWall()
{
    obs_t obs = robot.obstacleDetect();
    Serial.println((int)obs);
    switch(obs)
    {
        case RIGHT:
            robot.motorControl(BACKWARD, 100, BACKWARD, 100); // move backward;
            delay(300);
            robot.motorControl(FORWARD, 100, BACKWARD, 100); // turn left;
            delay(500);
            break;
        case LEFT:
            robot.motorControl(BACKWARD, 100, BACKWARD, 100); // move backward;
            delay(300);
            robot.motorControl(BACKWARD, 100, FORWARD, 100); // turn right;
            delay(500);
            break;
        case BOTH:
            robot.motorControl(BACKWARD, 100, BACKWARD, 100); // move backward;
            delay(300);
            robot.motorControl(BACKWARD, 100, FORWARD, 100); // turn 180;
            delay(1000);
            break;
        default:
            robot.motorControl(FORWARD, 100, FORWARD, 100); // move forward;
            break;
    }
}
