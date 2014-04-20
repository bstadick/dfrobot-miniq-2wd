#include <EEPROM.h>

#define EN1 6//pin for run the right motor 
#define IN1 7//pin for control right motor direction
#define EN2 5//pin for run the left motor 
#define IN2 4//pin for control left motor direction

#define FORW 1
#define BACK 0
#define BUZZER 11//buzzer io pin
#define Vr   5

float data[10]={
  0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
unsigned char sum=0;
float data_1=0x01,data_2=0x01;
float val;
int   key_1=0,key_2=0,key_3=0;

void Motor_Control(int M1_DIR,int M1_EN,int M2_DIR,int M2_EN)//control motor
{
  //////////M1////////////////////////
  if(M1_DIR==FORW)//M1 motor direction
    digitalWrite(IN1,FORW);//forward
  else
    digitalWrite(IN1,BACK);//back
  if(M1_EN==0)//M1电机速度 
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

void Read_Value(void)//read button input
{
  data_1=analogRead(4);
  data_1 = ((data_1 * Vr) / 1024);
  data_2=analogRead(5);
  data_2 = ((data_2 * Vr) / 1024);
}

void buzzer(void)
{
  digitalWrite(BUZZER,HIGH);
  delay(1);
  digitalWrite(BUZZER,LOW);
  delay(10);
}
void key_scan(void)//scan keyboard input
{
  if(data_2>4.50&&data_2<6.00)//no input
    return;//返回
  else
  {
    if(data_2>=0.00&&data_2<0.50)//press button1
    {
      delay(180);//remove noise
      if(data_2>=0.00&&data_2<0.50)//confirm the button pressed
      {
        buzzer();
        key_1++;
      }
    }
    else if(data_2>=0.50&&data_2<1.5)
    {
      delay(180);
      if(data_2>=0.50&&data_2<1.5)
      {
        buzzer();
        key_2++;
      }
    }
    else if(data_2>=1.5&&data_2<2.5)
    {
      delay(180);
      if(data_2>=1.5&&data_2<2.5)
      {
        buzzer();
        key_3++;
      }
    }
  }
}
void hunt_light(void)//follow the light
{
  int i;
  if(key_1==1)
  {
    key_1=0;
    
    for(i=0;i<10;i++)
    {
      Read_Value();
      data[i]=data_1;
      sum +=data[i];
    }
    //save the ambient light value
    EEPROM.write(0x0a,sum);
  }
  else
  {
    val=EEPROM.read(0x0a);
    val=val/10;
    Read_Value();
    if (data_1<val-0.5)   
      Motor_Control(FORW,100,BACK,100);//turn left
    else if (data_1>val+1)
      Motor_Control(BACK,100,FORW,100);//turn right
    else
      Motor_Control(FORW,0,FORW,0);//stop it
  }
}
void setup()
{ 
  unsigned char j;
  for(j=4;j<=7;j++)
    pinMode(j,OUTPUT);
}
void loop()
{
  Motor_Control(FORW,0,FORW,0);
  while(1)
  {
    Read_Value();
    key_scan();
    hunt_light();
  }
}




