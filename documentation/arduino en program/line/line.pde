#include <EEPROM.h>

#define EN1 6//pin for run the right motor 
#define IN1 7//pin for control right motor direction
#define EN2 5//pin for run the left motor 
#define IN2 4//pin for control left motor direction

#define FORW 1
#define BACK 0

#define BUZZER 11
#define LED_RED 12
#define LED_GREEN 13
#define Vr   5

float data[8]={
  0X00,0X00,0X00,0X00,0x00,0xff,0x00,0x00};//save the analog value
unsigned char value[5]={
  0x00,0x00,0x00,0x00,0x00};//get the line tracing value
unsigned char key_1=0x00,key_2=0x00;//button input

void Motor_Control(int M1_DIR,int M1_EN,int M2_DIR,int M2_EN)//control the motor
{
  //////////M1////////////////////////
  if(M1_DIR==FORW)//M1 motor direction
    digitalWrite(IN1,FORW);//forward
  else
    digitalWrite(IN1,BACK);//back
  if(M1_EN==0)
    analogWrite(EN1,LOW);//stop
  else
    analogWrite(EN1,M1_EN);//set speed

  ///////////M2//////////////////////
  if(M2_DIR==FORW)
    digitalWrite(IN2,FORW);
  else
    digitalWrite(IN2,BACK);
  if(M2_EN==0)
    analogWrite(EN2,LOW);
  else
    analogWrite(EN2,M2_EN);
}

void Read_Value(void)//read the analog value
{  
  char i;
  for(i=0;i<8;i++)
  {
    data[i]=analogRead(i);
    data[i]= ((data[i]*Vr)/1024);
  }
}

void huntline_deal(void)//trace line
{
  if(key_2==1)
  {
    int i;
    //save the value
    for(i=0;i<5;i++)
      value[i]=EEPROM.read(0x0a+i);
    if(data[0]>(value[0]-1)&&data[1]>(value[1]-1)&&data[2]<(value[2]-1)&&data[3]>(value[3]-1)&&data[7]>(value[4]-1))//测一下实际值
    {
      Motor_Control(FORW,100,FORW,100);//go forward
    } 
    else if(data[0]>(value[0]-1)&&data[1]>(value[1]-1)&&data[2]<(value[2]-1)&&data[3]<(value[3]-1)&&data[7]>(value[4]-1))
    {
      Motor_Control(BACK,50,FORW,100);//turn right
    }
    else if(data[0]>(value[0]-1)&&data[1]>(value[1]-1)&&data[2]>(value[2]-1)&&data[3]<(value[3]-1)&&data[7]>(value[4]-1))
    {
      Motor_Control(BACK,100,FORW,100);//turn right fast
    }
    else if(data[0]>(value[0]-1)&&data[1]>(value[1]-1)&&data[2]>(value[2]-1)&&data[3]<(value[3]-1)&&data[7]<(value[4]-1))
    {
      Motor_Control(BACK,100,FORW,100);//turn right fast
    }
    else if(data[0]>(value[0]-1)&&data[1]>(value[1]-1)&&data[2]>(value[2]-1)&&data[3]>(value[3]-1)&&data[7]<(value[4]-1))
    {
      Motor_Control(BACK,100,FORW,100);//turn right fast
    }
    else if(data[0]>(value[0]-1)&&data[1]<(value[1]-1)&&data[2]<(value[2]-1)&&data[3]>(value[3]-1)&&data[7]>(value[4]-1))
    {
      Motor_Control(FORW,100,BACK,50);//turn left
    }
    else if(data[0]>(value[0]-1)&&data[1]<(value[1]-1)&&data[2]>(value[2]-1)&&data[3]>(value[3]-1)&&data[7]>(value[4]-1))
    {
      Motor_Control(FORW,100,BACK,100);//turn left fast
    }
    else if(data[0]<(value[0]-1)&&data[1]<(value[1]-1)&&data[2]>(value[2]-1)&&data[3]>(value[3]-1)&&data[7]>(value[4]-1))
    {
      Motor_Control(FORW,100,BACK,100);//turn left fast
    }
    else if(data[0]<(value[0]-1)&&data[1]>(value[1]-1)&&data[2]>(value[2]-1)&&data[3]>(value[3]-1)&&data[7]>(value[4]-1))
    {
      Motor_Control(FORW,100,BACK,100);//turn left fast
    }
  }
}
void key_scan(void)//scan keyboard
{
  int i;
  if(data[5]>4.50&&data[5]<6.00)///no input
    return;
  else
  {
    if(data[5]>=0.00&&data[5]<0.50)//press button1
    {
      delay(180);//remove noise
      if(data[5]>=0.00&&data[5]<0.50)//confirm the button pressed
      {
        buzzer();
        key_1++;
        if(key_1>=1&&key_1<=5) value_adjust(key_1);
        if(key_1==6)
        {
          digitalWrite(LED_RED,LOW);
          digitalWrite(LED_GREEN,LOW);
          
          for(i=0;i<5;i++)
            EEPROM.write(0x0a+i,value[i]);
        }
      }
    }
    else if(data[5]>=0.50&&data[5]<2.00)
    {
      delay(180);
      if(data[5]>=0.50&&data[5]<2.00)
      {
        buzzer();
        if(key_1>=1&&key_1<=5)
        {
          value[key_1-1]++;
          value_adjust(key_1);
        }
        else
          key_2++;
      }
    }
    else if(data[5]>=2.00&&data[5]<3.00)
    {
      delay(180);
      if(data[5]>=2.00&&data[5]<3.00)
      {
        if(key_1>=1&&key_1<=5)
        {
          buzzer();
          value[key_1-1]--;
          value_adjust(key_1);
        }
      }
    }
  }
}
void value_adjust(unsigned char num)//
{
  if(num==1)
  {
    if(data[0]>value[0])
    {
      digitalWrite(LED_RED,HIGH);
      digitalWrite(LED_GREEN,LOW);
    }
    else
    {
      digitalWrite(LED_RED,LOW); 
      digitalWrite(LED_GREEN,HIGH);
    } 
  }
  if(num==2)
  {
    if(data[1]>value[1])
    {
      digitalWrite(LED_RED,HIGH);
      digitalWrite(LED_GREEN,LOW);
    }
    else
    {
      digitalWrite(LED_RED,LOW); 
      digitalWrite(LED_GREEN,HIGH);
    } 
  }
  if(num==3)
  {
    if(data[2]>value[2])
    {
      digitalWrite(LED_RED,HIGH);
      digitalWrite(LED_GREEN,LOW);
    }
    else
    {
      digitalWrite(LED_RED,LOW); 
      digitalWrite(LED_GREEN,HIGH);
    } 
  }
  if(num==4)
  {
    if(data[3]>value[3])
    {
      digitalWrite(LED_RED,HIGH);
      digitalWrite(LED_GREEN,LOW);
    }
    else
    {
      digitalWrite(LED_RED,LOW); 
      digitalWrite(LED_GREEN,HIGH);
    } 
  }
  if(num==5)
  {
    if(data[4]>value[4])
    {
      digitalWrite(LED_RED,HIGH);
      digitalWrite(LED_GREEN,LOW);
    }
    else
    {
      digitalWrite(LED_RED,LOW); 
      digitalWrite(LED_GREEN,HIGH);
    } 
  }
}

void buzzer(void)
{
  digitalWrite(BUZZER,HIGH);
  delay(1);
  digitalWrite(BUZZER,LOW);
  delay(10);
}

void setup()
{
  char i;
  for(i=4;i<=7;i++)//init motor
  {
    pinMode(i,OUTPUT);
  }
  pinMode(BUZZER,OUTPUT);//init the buzzer
  pinMode(LED_RED,OUTPUT);//init red led
  pinMode(LED_GREEN,OUTPUT);//init green led
}

void loop()
{
  Motor_Control(FORW,0,FORW,0);//stop move
  while(1)
  {
    Read_Value();
    key_scan();
    huntline_deal();//trace line
  }
}









