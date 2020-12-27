/**
* @author Bryan Stadick - stadi012@umn.edu
* @version 1.1.0
*
* Control.ino - Example PID control for the DFRobot MiniQ 2WD.
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

// PID gains (tune these to your liking)
#define kp 1.0
#define ki 1.0
#define kd 0.0

#define THRESH 3.0
#define PIDTHRESH 3 
float reflect[5];

int32_t prevCountR = 0, prevCountL = 0, countR, countL, powR, powL;
unsigned long time, prevTime;
dir_t dirR, dirL;

/**
* Performs simple PID control on position
*/
void pid(int setPointR, int setPointL)
{
    // get current values
    countR = robot.getEncRight();
    countL = robot.getEncLeft();
    time = micros();
    
    // check if setpoint is within threshold, then skip PID
    if(abs(countR - setPointR) <= PIDTHRESH && abs(countL - setPointL) <= PIDTHRESH) {
        // set current values as previous values for next iteration
        prevTime = time;
        prevCountR = countR;
        prevCountL = countL;
        return;
    }
    
    // calculate proportional term, in cm
    powR = (int)((setPointR - countR) * kp);
    powL = (int)((setPointL - countL) * kp);
    
    // calculate the integral term (use rectangular method), in cm * us
    powR += (int)((countR - prevCountR) * (time - prevTime) * ki);
    powL += (int)((countL - prevCountL) * (time - prevTime) * ki);
    
    // calculate the differential term, in cm / us
    powR += (int)((countR - prevCountR) / (time - prevTime) * kd);
    powL += (int)((countL - prevCountL) / (time - prevTime) * kd);
    
    // set current values as previous values for next iteration
    prevTime = time;
    prevCountR = countR;
    prevCountL = countL;
    
    // set right or left wheel power to 0 if it alone is in threshold
    if(abs(countR - setPointR) <= PIDTHRESH)
        powR = 0;
    if(abs(countL - setPointL) <= PIDTHRESH)
        powL = 0;
    
    // set direction
    if(powR > 0)
        dirR = FORWARD;
    else
        dirR = BACKWARD;
    if(powL > 0)
        dirL = FORWARD;
    else
        dirL = BACKWARD;
    
    // peform action
    robot.motorControl(dirR, constrain(abs(powR), 0, 255), dirL, constrain(abs(powL), 0, 255));
}

/**
* Follow a line.
*/
void followLine()
{
    // get surface refelectivity
    robot.getReflectivity(reflect);
    
    // center is over line
    if(reflect[3] >= THRESH)
        // drive forward
        robot.motorControl(FORWARD, 75, FORWARD, 75);
    // line slightly to left
    else if(reflect[2] >= THRESH || reflect[0] >= THRESH) {
        robot.motorStop();
        // turn to left
        robot.motorRight(FORWARD, 65);
        // turn until center is over line
        robot.getReflectivity(reflect);
        while(reflect[3] < THRESH)
            robot.getReflectivity(reflect);
        // drive forward
        avoidWall();
    }
    // line slightly to right
    else if(reflect[4] >= THRESH || reflect[5] >= THRESH) {
        robot.motorStop();
        // turn to right
        robot.motorLeft(FORWARD, 65);
        // turn until center is over line
        robot.getReflectivity(reflect);
        while(reflect[3] < THRESH)
            robot.getReflectivity(reflect);
        // drive forward
        avoidWall();
    }
    // line is not in sight
    else {
        // search for the line
        if(!search()) {
            // line not found, give up
            robot.playNote(NOTE_P_C1, 250);
            delay(250);
            robot.playNote(NOTE_P_C1, 250);
            // wait for key one to be pressed before beginning again
            while(!robot.getKeyOne()) ;
        }
        else
            avoidWall();
    }
}
/**
* Search for line if lost completely
*/
boolean search() {
    
    for(int i = 5; i < 53; i += 2) {
        // sweep left
        robot.motorRight(FORWARD, 65);
        while(robot.getEncRight() < i){
            robot.getReflectivity(reflect);
            if(reflect[0] >= THRESH ||
               reflect[1] >= THRESH ||
               reflect[2] >= THRESH ||
               reflect[3] >= THRESH ||
               reflect[4] >= THRESH) {
                robot.motorStop();
                return true;
            }
        }
        robot.motorStop();
        
        // go back to original position
        robot.motorRight(BACKWARD, 65);
        while(robot.getEncRight() < i) ;
        robot.motorStop();
        
        // sweep right
        robot.motorLeft(FORWARD, 65);
        while(robot.getEncLeft() < i){
            robot.getReflectivity(reflect);
            if(reflect[0] >= THRESH ||
               reflect[1] >= THRESH ||
               reflect[2] >= THRESH ||
               reflect[3] >= THRESH ||
               reflect[4] >= THRESH) {
                robot.motorStop();
                return true;
            }
        }
        robot.motorStop();
        
        // go back to original position
        robot.motorLeft(BACKWARD, 65);
        while(robot.getEncLeft() < i) ;
        robot.motorStop();
    }
    return false;
}

/**
* Avoid obstacles.
*/
void avoidWall()
{
    obs_t obs = robot.obstacleDetect(NULL, NULL);
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
