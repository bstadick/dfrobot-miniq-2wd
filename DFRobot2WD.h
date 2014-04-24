/**
* @author Bryan Stadick - stadi012@umn.edu
* @version 1.1.0
*
* DFRobot2WD.h - Library of functions for the DFRobot MiniQ 2WD
*
* DFRobot is a trademark of DFRobot. The author of this library does not claim
* any ownership or association with DFRobot or their affiliates.
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


#ifndef DFRobot2WD_h_
#define DFRobot2WD_h_

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#elif defined(WIRING)
#include <Wiring.h>
#else
#include "WProgram.h"
#endif

#include "Notes.h"

// Constants
#define VR 5.0 // voltage reference (5V)
#define REF_SIZE 5 // number of reflectivity sensors

// Motor pins
#define EN1 6 // pin to run the right motor (digital w/ analog write)
#define IN1 7 // pin to control right motor direction (digital)
#define EN2 5 // pin to run the left motor (digital w/ analog write)
#define IN2 4 // pin to control left motor direction (digital)

// IR pins
#define IR_IN 8 // IR receiver pin (digital)
#define L_IR 9 // left ir transmitter pin (digital)
#define R_IR 10 // right ir transmitter pin (digital)
#define L_TACT 9 // optional tactile sensor pin (digital)
#define R_TACT 10 // optional tactile sensor pin (digital)

// Analog pins
#define VREF 6 // supply voltage (analog)
#define PB 5 // push buttons (analog)
#define LIGHT 4 // left/right light intensity sensors
#define IR0 0 // bottom left IR reflectivity sensor
#define IR1 1
#define IR2 2 // bottom middle IR reflectivity sensor
#define IR3 3
#define IR4 7 // bottom right IR reflectivity sensor

// Other pins
#define BUZZER 11 // Buzzer pin (digital w/ analog write)
#define LED_RED 12 // Red LED pin (digital)
#define LED_GREEN 13 // Green LED pin (digital)

/**
* Direction of wheel travel.
*/
enum dir_t {BACKWARD = 0, FORWARD = 1};
typedef enum dir_t dir_t;

/**
* Location of obstacle.
*/
enum obs_t {NONE = 0, IND = 0, RIGHT = 1, FRONT = 1, 
			SWITCH1 = 1, LEFT = 2, BACK = 2, SWITCH2 = 2, BOTH = 3};
typedef enum obs_t obs_t;

class DFRobot2WD
{
    public:
        DFRobot2WD();
        void motorLeft(dir_t dir, unsigned char pwr);
        void motorRight(dir_t dir, unsigned char pwr);
        void motorControl(dir_t dirR, unsigned char pwrR, dir_t dirL, unsigned char pwrL);

        /**
        * Stops both motors.
        */
        inline void motorStop(){ analogWrite(EN1, LOW); analogWrite(EN2, LOW); }

        /**
        * Gets the value of the right encoder.
        * One tick is about 1cm distance traveled.
        *
        * @return the tick count of the right encoder
        */
        inline int32_t getEncRight(){ return count_r; }

        /**
        * Gets the value of the left encoder.
        * One tick is about 1cm distance traveled.
        *
        * @return the tick count of the left encoder.
        */
        inline int32_t getEncLeft(){ return count_l; }

        /**
        * Sets the value of the right encoder. Meant for resetting the count.
        *
        * @param count - the tick count to set the right encoder at
        */
        inline void setEncRight(unsigned int count) { count_r = count; }

        /**
        * Sets the value of the left encoder. Meant for resetting the count.
        *
        * @param count - the tick count to set the left encoder at
        */
        inline void setEncLeft(unsigned int count) { count_l = count; }

        /**
        * Sets the values of the right and left encoders. Meant for resetting the count.
        *
        * @param countR - the tick count to set the right encoder at
        * @param countL - the tick count to set the left encoder at
        */
        inline void setEnc(uint32_t countR, uint32_t countL){ count_r = countR; count_l = countL; }

		void initIRDetect();
        obs_t obstacleDetect(int* countR, int* countL);
		void initTactile();
		obs_t getSwitchState();
        boolean getKeyOne();
        boolean getKeyTwo();
        boolean getKeyThree();
        void getReflectivity(float reflectivity[]);
        void playNote(float period, uint16_t time);

        /**
        * Gets the supply voltage. Useful for determining battery health and 
        * power needs of the motor (lower voltage = less motor output).
        *
        * @return the voltage level of the supply voltage [0.0, 5.0]V
        */
        inline float getVoltage(){ return ((analogRead(VREF) * VR * 1.5) / 1024.0); }

        /**
        * Gets the left and right light intensity ratio.
        *
        * @return the relative proportion of light intensity to the right versus the left
        */
        inline float getLight(){ return ((analogRead(LIGHT) * VR) / 1024.0); }

        /**
        * Gets the code sent over from the IR remote. Requires to be called in a
        * polling routine to ensure the code was sent (non-blocking).
        *
        * @return the code sent, otherwise 0xFF00 if no code was sent
        */
        int16_t getIRRemote(){ remoteDecode(); return irCode; }
        
        /**
        * Sets the state of the red led.
        *
        * @param state - the state to change the led to (either HIGH or LOW)
        */
        inline void ledRed(boolean state) { digitalWrite(LED_RED, state); }
        
        /**
        * Sets the state of the green led.
        *
        * @param state - the state to change the led to (either HIGH or LOW)
        */
        inline void ledGreen(boolean state) {digitalWrite(LED_GREEN, state); }

        // ISR functions
        static void encLISR();
        static void encRISR();
        static void obsISR();
        static void noteISR();
        
    private:
        void dfRobotInit();

        /**
        * Enable external interrupts on rising edge for the encoders.
        */
        void interrupt01_init(){ EICRA = 0X0F; EIMSK = 0X03; }

        /**
        * Enable interrupt on PB0 (pin 12), for use with the IR receiver.
        */
        void pcint0_init(){ PCICR = 0X01; PCMSK0 = 0X01; }

        void obsSendRPulse();
        void obsSendLPulse();
        void timer1_init();
        void remoteDecode();
        void pulseDeal();
        char logicValue();
        void timer2_init();

    private:
        int16_t pulseWidth; // the width of the IR pulse for IR signal reception
        int16_t irCode; // the IR signal code received
        float reflectivity[5]; // values of bottom reflectivity sensors
		boolean useTactile;

    protected:
        // encoder tick counts
        static int32_t count_r;
        static int32_t count_l;
        // current wheel direction
        static dir_t direction_r;
        static dir_t direction_l;
        // number of obstacle pulses received
        static int count_obs;
        static int  note; // value for each note timer
        static char time; // each note playing time
        static char flag;
};

// static variable definitions
int32_t DFRobot2WD::count_l = 0;
int32_t DFRobot2WD::count_r = 0;
dir_t DFRobot2WD::direction_l = FORWARD;
dir_t DFRobot2WD::direction_r = FORWARD;
int DFRobot2WD::count_obs = 0;
int DFRobot2WD::note = 0;
char DFRobot2WD::time = 0;
char DFRobot2WD::flag = 0;
void EncRISR();
void EncLISR();


//**************************** ISRs ****************************

/*
* Two functions are declared for the user to call custom code when
* the right encoder and left encoder interrupts are triggered.
* To use them, the user can define the following functions:
*
* void EncRISR(void) // right encoder ISR call
* void EncLISR(void) // left encoder ISR call
* 
* To use these functions, the user must include
* 
* #define ENCISR
* 
* at the very top of the file (before the includes).
*/

// interrupt routines
ISR(INT0_vect) // right motor encoder interrupt
{
    DFRobot2WD::encRISR();
	#ifdef ENCISR
	EncRISR();
	#endif
}

ISR(INT1_vect) // left motor encoder interrupt
{
    DFRobot2WD::encLISR();
	#ifdef ENCISR
	EncLISR();
	#endif
}

ISR(PCINT0_vect) // obstacle detection interrupt
{
    DFRobot2WD::obsISR();
}

ISR(TIMER2_OVF_vect) // buzzer note interrupt
{
  DFRobot2WD::noteISR();
}

void DFRobot2WD::encRISR()
{
    if(DFRobot2WD::direction_r == FORWARD)
        DFRobot2WD::count_r++;
    else
        DFRobot2WD::count_r--;
}

void DFRobot2WD::encLISR()
{
    if(DFRobot2WD::direction_l == FORWARD)
        DFRobot2WD::count_l++;
    else
        DFRobot2WD::count_l--;
}

void DFRobot2WD::obsISR()
{
    DFRobot2WD::count_obs++;
}

void DFRobot2WD::noteISR()
{
    TCNT2 = DFRobot2WD::note;
    DFRobot2WD::flag = ~(DFRobot2WD::flag);
    if(DFRobot2WD::flag)
        digitalWrite(BUZZER, HIGH);
    else
        digitalWrite(BUZZER, LOW);
}

//**************************** Constructors ****************************

/**
* Crates a new robot instance.
*/
DFRobot2WD::DFRobot2WD()
{
    dfRobotInit();
}

/**
* Initializes the new robot instance.
*/
void DFRobot2WD::dfRobotInit()
{
    
    DFRobot2WD::count_r = 0;
    DFRobot2WD::count_l = 0;
    irCode = 0;
    pulseWidth = 0;
	useTactile = false;
    
    char i;
    for(i = 4; i <= 13; i++)
    {
        pinMode(i, OUTPUT);
    }
    pinMode(IR_IN, INPUT);
    digitalWrite(R_IR,HIGH);
    digitalWrite(L_IR,HIGH);
    
    interrupt01_init(); // enable interrupts for encoders
    pcint0_init();
    
    sei(); // enable global interrupts
}

//**************************** Motor Control ****************************

/**
* The encoders on the wheels are gear tooth counters. They trigger an external
* interrupt every time a gear tooth passes by. There are a total of 12 teeth
* on each wheel. Each wheel is about 4cm in diameter making the resolution of
* each tick on the counter to be about 1.05cm. The count is handled in a separate
* ISR that catches every tick and uses the current direction of the motor
* control to determine if the count should be incremented or decremented.
* <p>
* Since the tick count will change as the wheel spins, it is important to check
* the count often to make sure you have moved the distance you would like and correct
* accordingly. You can either either poll the count number and check for changes,
* or you can add your own code to the ISR. It is important that you make ISRs
* fast, as they will interrupt any other code running and will not relinquish control
* until done. This means long running operations in an ISR may cause other ISRs
* to not run when needed (and thus miss ticks on the other wheel).
*/

/**
* Controls the power and direction of the left motor.
*
* @param dir - the direction of the left motor
* @param pwr - the power of the left motor
*/
void DFRobot2WD::motorLeft(dir_t dir, unsigned char pwr)
{
    cli(); // critical section
    direction_l = dir;
    if(dir == FORWARD) // motor direction     
        digitalWrite(IN2, FORWARD); // forward
    else
        digitalWrite(IN2, BACKWARD); // back
    if(pwr == 0)
        analogWrite(EN2, LOW); // stop
     else
        analogWrite(EN2, pwr); // set speed
    sei();
}

/**
* Controls the power and direction of the right motor.
*
* @param dir - the direction of the right motor
* @param pwr - the power of the right motor
*/
void DFRobot2WD::motorRight(dir_t dir, unsigned char pwr)
{
    cli(); // critical section
    direction_r = dir;
    if(dir == FORWARD) // motor direction
        digitalWrite(IN1, FORWARD); // forward
    else
        digitalWrite(IN1, BACKWARD); // back
    if(pwr == 0)
        analogWrite(EN1, LOW); // stop
    else
        analogWrite(EN1, pwr); // set speed
    sei();
}

/**
* Controls the power and direction of both motors.
*
* @param dirR - the direction of the right motor
* @param pwrR - the power of the right motor
* @param dirL - the direction of the left motor
* @param pwrL - the power of the left motor
*/
void DFRobot2WD::motorControl(dir_t dirR, unsigned char pwrR, dir_t dirL, unsigned char pwrL)
{
    motorRight(dirR, pwrR);
    motorLeft(dirL, pwrL);
}

//**************************** Push Keys ****************************

/**
* The keys are all attached to an analog pin and create different level voltage
* dividers to determine which key is pressed based on the voltage level.
* Because of this, if all three keys are pressed at once then key one will be
* detected, while keys two and three will not be detected. If keys two and three
* are pressed, key two will be detected. If keys one and three are pressed, then
* key one will be detected. The order of precedence is: k_one > k_two > k_three.
*/

/**
* Gets if key one is pressed.
* <p>
* Key one takes precedence over key two and three.
*
* @return if key one is pressed
*/
boolean DFRobot2WD::getKeyOne()
{
    if(analogRead(PB) < 102)
    {
        delay(100); // debounce
        return true;
    }
    delay(100); // debounce
    return false;
}

/**
* Gets if key two is pressed.
* <p>
* Key two takes precedence over key three.
*
* @return if key two is pressed
*/
boolean DFRobot2WD::getKeyTwo()
{
    uint16_t value = analogRead(PB);
    if(value >= 102 && value < 307)
    {
        delay(100); // debounce
        return true;
    }
    delay(100); // debounce
    return false;
}

/**
* Gets if key three is pressed.
*
* @return if key three is pressed
*/
boolean DFRobot2WD::getKeyThree()
{
    uint16_t value = analogRead(PB);
    if(value >= 307 && value < 512)
    {
        delay(100); // debounce
        return true;
    }
    delay(100); // debounce
    return false;
}

//**************************** Obstacle Detection ****************************

/**
* Potentiometer W2 is used to attenuate the amount of light output by the IR LEDs.
* This allows for tuning of the amount of light emitted to reduce glare or possible
* over exposure and allows for an overall more tunable experience. The IR receiver
* (also used for the IR Remote), is found on the front center of the robot.
* It is used to pick up the pulses sent out by the IR LEDs to determine if an
* object is present based on how much light is reflected back.
*/

/**
* Initializes the IR sensors (default) to be used for obstacle detection.
* Disables the tactile sensors.
*/
void DFRobot2WD::initIRDetect() {
	pinMode(L_IR, OUTPUT);
	pinMode(R_IR, OUTPUT);
	pcint0_init();
	useTactile = false;
}

/**
* Sends carrier pulses on the left IR LED.
*/
void DFRobot2WD::obsSendLPulse()
{
    char i;
    for(i = 0; i < 24; i++)
    {
        digitalWrite(L_IR, LOW); // set right IR LED on
        delayMicroseconds(8);
        digitalWrite(L_IR, HIGH); // set right IR LED off
        delayMicroseconds(8);
    }
}

/**
* Sends carrier pulses on the right IR LED.
*/
void DFRobot2WD::obsSendRPulse()
{
    char i;
    for(i = 0; i < 24; i++)
    {
        digitalWrite(R_IR, LOW); // set right IR LED on
        delayMicroseconds(8);
        digitalWrite(R_IR, HIGH); // set right IR LED off
        delayMicroseconds(8);
    }
}

/**
* Uses the two IR LEDs on the front of the robot and the IR receiver in the middle
* to detect obstacles.
*
* @param countR - returns in it the count value of the right led pulse
* @param countL - returns in it the count value of the left led pulse
* @return if an obstacle is present to either the left, the right, both, or neither side
*/
obs_t DFRobot2WD::obstacleDetect(int* countR, int* countL)
{   
    obs_t obs = NONE;
    char i;
	
	if(useTactile)
		return IND;

    DFRobot2WD::count_obs = 0;
    for(i = 0; i < 20; i++) // right transmitter sends 20 pulses
    {
        obsSendRPulse();
        delayMicroseconds(600);
    }
    if(DFRobot2WD::count_obs > 15) // more than 15 pulses detected = obstacle
        obs = RIGHT;
    if(countR != NULL)
        *countR = DFRobot2WD::count_obs;
        
    delay(50);
    
    DFRobot2WD::count_obs = 0;
    for(i = 0; i < 20; i++) // left transmitter sends 20 pulses
    {
        obsSendLPulse();
        delayMicroseconds(600);
    }
    if(DFRobot2WD::count_obs > 15) // more than 15 pulses detected = obstacle
    {
        if(obs == RIGHT)
            obs = BOTH;
        else
            obs = LEFT;
    }
    if(countL != NULL)
        *countL = DFRobot2WD::count_obs;
    
    return obs;
}

//**************************** Obstacle Detection ****************************

/**
The IR obstacle detection above tends not to be the most robust method for detecting objects in your path. Instead it is recommended that you wire up a pair of micro switches, one on the front and one on the back. This will give a more robust way of telling if a wall is in your way.
<p>
The wiring is as described. You wire the normally open (NO) node of the micro switch to the cathode of the IR LED (negative terminal). You can wire switch one to the right IR LED and switch two to the left IR LED. You do not need to remove the LEDs for this to work. The common node of each micro switch is wired to ground through a 1Kohm or 10Kohm pull down resistor. Both can be wired to the same resistor. The pull down resistor is then wired to the ground on the robot. There are several options for doing this. The easiest is to connect the resistor to a single standard 0.1" pitch receptacle, and then plug the receptacle onto the bottom-left pin of the ICSP header (the pin closest to the batteries and wheel).
<p>
This setup will allow you to tell when you hit a wall or other object. Both IR LED pins will be pulled low when you hit either switch one or switch two. This means you can't detect direction, except by inferring it through your current direction of travel. In theory, you could add more micro switches and determine which one was pressed based on your current direction of travel.
<p>
To use the switches, you will need to set the IR LED pins to inputs and detect when they are false. To regain use of your IR LEDs, simple change the pins to outputs and unplug the pull-down resistor from the ICSP ground pin.
*/

/**
* Initializes the optional push buttons. Disables IR object detection.
*/
void DFRobot2WD::initTactile() {
	PCICR = PCICR & 0XFE;
	pinMode(L_IR, INPUT);
	pinMode(R_IR, INPUT);
	useTactile = true;
}

/**
* Gets the state of the switches, based off current direction of travel.
*/
obs_t DFRobot2WD::getSwitchState() {

	if(!useTactile)
		return IND;
		
	if(direction_r == FORWARD && direction_l == FORWARD && (!digitalRead(L_IR) || !digitalRead(R_IR)))
		return FRONT;
	else if(direction_r == BACKWARD && direction_l == BACKWARD && (!digitalRead(L_IR) || !digitalRead(R_IR)))
		return BACK;
	else if(digitalRead(L_IR) && digitalRead(R_IR))
		return NONE;
	else
		return IND;
}

//**************************** Line Following ****************************

/**
* Line following uses five IR reflectivity sensors on the bottom of the robot.
* The reflectivity will depend on the surface texture and color, thus making
* it possible to detect different colors and thus lines.
* <p>
* The higher the reflectivity, the higher the value (e.g. black is around 2.0 while white is closer to 4.8).
* The values for any given surface will range significantly between sensors based on many different factors.
* Always use a range of values when doing comparisons for detecting different surfaces.
* For best results, detect either a black line on a white surface or a white line on a black surface.
*/

/**
* Gets the reflectivity of the surface below the robot over five different points.
*
* @param reflectivity - an array of the five sensor values, in the range of [0.0, 5.0]V, from left to right
*/
void DFRobot2WD::getReflectivity(float reflectivity[])
{
    reflectivity[0] = ((analogRead(IR0) * VR) / 1024.0); // left most
    reflectivity[1] = ((analogRead(IR1) * VR) / 1024.0);
    reflectivity[2] = ((analogRead(IR2) * VR) / 1024.0); // middle
    reflectivity[3] = ((analogRead(IR3) * VR) / 1024.0);
    reflectivity[4] = ((analogRead(IR4) * VR) / 1024.0); // right most
}

//**************************** IR Remote ****************************

/**
* The center sensor on the front of the robot is the IR receiver (also used for
* the obstacle detection). It can pick up pulses sent out by IR sources.
* This can include the on-board IR LEDs or a separate IR remote control 
* (such as the one that comes with the kit). With a remote, the robot can be 
* given commands, which is what this section uses the IR receiver for.
*/

/**
* Sets up timer 1 for operation to be used with IR remote signal decoding.
*/
void DFRobot2WD::timer1_init()
{
  TCCR1A = 0X00; // don't use and of the compare or waveform functions
  TCCR1B = 0X05; // set-up with a 1024 prescaler
  TCCR1C = 0X00; // don't use output compare
  TCNT1 = 0X00; // set the timer counter to 0
  TIMSK1 = 0X00; // no interrupts
}

/**
* Demodulates the carrier from the data to determine if the sent IR value is a one or a zero.
*
* @return the logic value detected, or -1 if failed
*/
char DFRobot2WD::logicValue()
{

    while(!(digitalRead(IR_IN))) ;

    pulseWidth = TCNT1;
    TCNT1 = 0;

    if(pulseWidth >= 7 && pulseWidth <= 10)
    {
        while(digitalRead(IR_IN)) ;

        pulseWidth = TCNT1;
        TCNT1 = 0;

        if(pulseWidth >= 7 && pulseWidth <= 10)
            return 0;
        else if(pulseWidth >= 25 && pulseWidth <= 27)
            return 1;
    }
    return -1;
}

/**
* Determines the overall data value sent in the message.
*/
void DFRobot2WD::pulseDeal()
{
    char i;

    for(i = 0; i < 8; i++)
    {
        if(logicValue() != 0)
            return;
    }
  
    for(i = 0; i < 6; i++)
    {
        if(logicValue() != 1)
            return;
    }
  
    if(logicValue() != 0)
        return;
      
    if(logicValue() != 1)
        return;


    //decode command
    irCode = 0x00;
    for(i = 0; i < 16; i++ )
    {
        if(logicValue() == 1)
            irCode = irCode | (1 << i);
    }
}

/**
* Decodes the IR message into its numerical value.
*/
void DFRobot2WD::remoteDecode()
{
    cli(); // disable external interrupts
    timer1_init(); // turn on timer 1 for IR reciever

    TCNT1 = 0X00;

    while(digitalRead(IR_IN))
    {
        if(TCNT1 >= 1563)
        {
            irCode = 0xff00;

            TCCR1B = TCCR1B & 0xF8; // turn timer 1 off
            sei();
            return;
        }  
    }


    TCNT1=0X00;

    while(!(digitalRead(IR_IN))) ;

    pulseWidth = TCNT1;
    TCNT1 = 0;

    if(pulseWidth >= 140 && pulseWidth <= 141) // 9ms
    {

        while(digitalRead(IR_IN)) ;

        pulseWidth = TCNT1;
        TCNT1 = 0;

        if(pulseWidth >= 68 && pulseWidth <= 72) // 4.5ms
        {
            pulseDeal();

            TCCR1B = TCCR1B & 0xF8; // turn timer 1 off
            sei();
            return;
        }
        else if(pulseWidth >= 34 && pulseWidth <= 36) // 2.25ms
        {
            while(!(digitalRead(IR_IN))) ;

            pulseWidth = TCNT1;
            TCNT1 = 0;

            if(pulseWidth >= 7 && pulseWidth <= 10) // 560us
            {
                TCCR1B = TCCR1B & 0xF8; // turn timer 1 off
                sei();
                return;
            }
        }
    }

    TCCR1B = TCCR1B & 0xF8; // turn timer 1 off
    sei();
}

//**************************** Wireless Communication ****************************

/**
* No wireless communication functions are present in this library. The DFRobot
* MiniQ 2WD robot platform uses standard Xbee compatible radios to communicate
* wirelessly. The robot use the Rx/Tx USART module of the microcontroller to send
* and receive data. For those more familiar with the Arduino libraries, this
* functionality is obtained through the Serial library (most commonly associated
* with the USB port).
* <p>
* The DFRobot MiniQ 2WD has two points where the USART is found: in the USART header
* at the front of the robot and at the wireless headers in the back. Both of these
* ports use the same Rx/Tx pins (as this chip has only one USART module), therefore
* both wired and wireless serial communication cannot be used at the same time.
* <p>
* Compatible radios include any Xbee based radio (such as those made by Digi International).
* These radios are usually 3.3V (instead of 5V), come in several different communication
* protocols (such as Bluetooth, ZigBee, DigiMesh, IEEE 802.15.4, and IEEE 802.11 b/g/n (aka Wi-Fi)),
* and have a similar/compatible pin layout (so they're interchangeable).
* You must be careful thought to make sure all the radios you are networking are
* using the same protocol. While they are pin compatible, they can only communicate
* to other radios using the same protocol.
* <p>
* The wireless port on the DFRobot MiniQ 2WD should be able to accept any Xbee
* based radio, as it converts voltages to 3.3V levels and is pin compatible with
* most common Xbee based radios.
* <p>
* For more information and to make sure you are using compatible radios, see
* {@link https://www.sparkfun.com/pages/xbee_guide}
*/

//**************************** On-board LEDs & Other ****************************

/**
* The two black sensors on the front are light intensity sensors. They are wired
* together in a voltage divider through the W1 potentiometer. This configuration
* allows for the determining the direction of a light source. The higher the light
* intensity to the right, the lower the voltage ratio and vice-versa. This value is
* tunable using the W1 potentiometer to correct for any bias or create bias to 
* one side or the other.
*/


/**
* The blue LEDs under the robot cannot be controlled. They are hardwired to power.
* <p>
* The red and green LEDs on top can be controlled through pins LED_RED and LED_GREEN.
* You use digitalWrite to turn them on and off or you can use analogWrite to 
* create a dimming effect.
* <p>
* The buzzer is connected to the pin BUZZER. It can create different tones by
* writing a PWM signal to it.
*/

/**
* Initialize timer 2 for the buzzer
*/
void DFRobot2WD::timer2_init()
{
    TCCR2A = 0X00; // no compare or waveform functions
    TCCR2B = 0X07; // set to 1024 prescaler
    TCNT2 = DFRobot2WD::note;
    TIMSK2 = 0X01; // enable timer
}

/**
* Plays the note at the given frequency and for the given time (blocking)
*
* @param period - the note frequency to play
* @param time - the time to play the note for (ms)
*/
void DFRobot2WD::playNote(float period, uint16_t time)
{
    timer2_init();
    DFRobot2WD::note = (int)(period / 0.128); // set note value
    DFRobot2WD::time = time; // set the time to hold
    Serial.println(note);
    delay(time);
    digitalWrite(BUZZER,LOW);
    TCCR2B = TCCR2B & 0XF8; // turn timer 2 off
}

/**
* The supply voltage can be read using the getVoltage function.
* <p>
* The power button is a latching button that allows for click-on, click-off operation.
* <p>
* The reset button will reset the microcontroller when pressed. This is the equivalent
* of power cycling. All RAM will be cleared and the program will be started from
* the beginning.
* <p>
* If desired, rechargeable AA batteries can be used (such as NiMH). A 6V dc power
* supply with a 3.5mm outer diameter plug can be used to charge the batteries while
* in the robot. Simultaneous operation and charging cannot and should not occur.
* Power off the robot before charging. Do not run the robot from the charger directly.
*/

#endif
