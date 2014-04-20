#define EN1 6//pin for run the right motor 
#define IN1 7//pin for control right motor direction
#define EN2 5//pin for run the left motor 
#define IN2 4//pin for control left motor direction

#define FORW 1//go forward
#define BACK 0//go back

#define IR_IN  8//IR receiver pin
#define L_IR 9//left ir transmitter pin
#define R_IR 10//right ir transmitter pin

#define BUZZER 11
#define LED_RED 12
#define LED_GREEN  13

#define Vr   5//参考电压值

int   count;//count the motor speed pulse

float data[8]={
  0X00,0X00,0X00,0X00,0x00,0xff,0x00,0x00};
char  value[5]={
  0x00,0x00,0x00,0x00,0x00};
char  key_1=0x00,key_2=0x00,key_3=0x00;

int    count_r=0,count_l=0;

int Pulse_Width=0;
int  ir_code=0x00;


void Motor_Control(int M1_DIR,int M1_EN,int M2_DIR,int M2_EN)
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
//encoder value
void interrupt01_init(void)
{
  EICRA = 0X0F;
  EIMSK = 0X03;
}
ISR(INT0_vect)//motor encoder interrupt
{
  if(++count_r==120)
    count_r=0;
}
ISR(INT1_vect)
{
  if(++count_l==120)
    count_l=0;
}
//Obstacle Avoidance

void L_Send40KHZ(void)//左发射管发射频率为40kHZ脉冲
{
  int i;
  for(i=0;i<24;i++)
  {
    digitalWrite(L_IR,LOW);//设置左发射管为高电平
    delayMicroseconds(8);//延时
    digitalWrite(L_IR,HIGH);//设置左发射管为低电平
    delayMicroseconds(8);//延时
  }
}
void R_Send40KHZ(void)//右发射管发射频率为40kHZ脉冲
{
  int i;
  for(i=0;i<24;i++)
  {
    digitalWrite(R_IR,LOW);//设置右发射管为高电平
    delayMicroseconds(8);//延时
    digitalWrite(R_IR,HIGH);//设置右发射管为低电平
    delayMicroseconds(8);//延时
  }
}

void pcint0_init(void)
{
  PCICR = 0X01;
  PCMSK0 = 0X01;
}
ISR(PCINT0_vect)
{
  count++;
}
void Obstacle_Avoidance(void)
{   
  char i;
  count=0;
  for(i=0;i<20;i++)//left transmitter sends 20 pulses
  {
    L_Send40KHZ();
    delayMicroseconds(600);    
  }
  if(count>20)//if recieved a lot pulse , it means there's a obstacle
  {
    if(key_2==1||key_3==3)
    {
      Motor_Control(BACK,100,BACK,100);
      delay(500);
      Motor_Control(BACK,100,FORW,100);
      delay(500);
    }
    if(key_3==1)
    {
      Motor_Control(FORW,100,FORW,100);
      delay(300);
    }
  }
  else
  {
    if(key_2==1||key_3==3)
      Motor_Control(FORW,100,FORW,100);
    if(key_3==1)
      Motor_Control(FORW,0,FORW,0);
  }


  count=0;
  for(i=0;i<20;i++)//right transmitter sends 20 pulses
  {
    R_Send40KHZ(); 
    delayMicroseconds(600);        
  }
  if(count>20)
  {
    if(key_2==1||key_3==3)
    {
      Motor_Control(BACK,100,BACK,100);
      delay(500);
      Motor_Control(FORW,100,BACK,100);
      delay(500);
    }
    if(key_3==1)
    {
      Motor_Control(FORW,100,FORW,100);
      delay(300);
    }
  }
  else
  {
    if(key_2==1||key_3==3)
      Motor_Control(FORW,100,FORW,100);
    if(key_3==1)
      Motor_Control(FORW,0,FORW,0);
  }
}

void Read_Value(void)
{  
  int i;
  for(i=0;i<8;i++)
  {
    data[i]=analogRead(i);//
    data[i]= ((data[i]*Vr)/1024); //
  }
}
//trace line
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
void huntline_deal(void)
{
  if(data[0]>(value[0]-1)&&data[1]>(value[1]-1)&&data[2]<(value[2]-1)&&data[3]>(value[3]-1)&&data[7]>(value[4]-1))//测一下实际值
    Motor_Control(FORW,100,FORW,100);
  else if(data[0]>(value[0]-1)&&data[1]>(value[1]-1)&&data[2]<(value[2]-1)&&data[3]<(value[3]-1)&&data[7]>(value[4]-1))
    Motor_Control(BACK,20,FORW,100);
  else if(data[0]>(value[0]-1)&&data[1]>(value[1]-1)&&data[2]>(value[2]-1)&&data[3]<(value[3]-1)&&data[7]>(value[4]-1))
    Motor_Control(BACK,100,FORW,100);
  else if(data[0]>(value[0]-1)&&data[1]>(value[1]-1)&&data[2]>(value[2]-1)&&data[3]<(value[3]-1)&&data[7]<(value[4]-1))
    Motor_Control(BACK,100,FORW,100);
  else if(data[0]>(value[0]-1)&&data[1]>(value[1]-1)&&data[2]>(value[2]-1)&&data[3]>(value[3]-1)&&data[7]<(value[4]-1))
    Motor_Control(BACK,100,FORW,100);
  else if(data[0]>(value[0]-1)&&data[1]<(value[1]-1)&&data[2]<(value[2]-1)&&data[3]>(value[3]-1)&&data[7]>(value[4]-1))
    Motor_Control(FORW,100,BACK,20);
  else if(data[0]>(value[0]-1)&&data[1]<(value[1]-1)&&data[2]>(value[2]-1)&&data[3]>(value[3]-1)&&data[7]>(value[4]-1))
    Motor_Control(FORW,100,BACK,100);
  else if(data[0]<(value[0]-1)&&data[1]<(value[1]-1)&&data[2]>(value[2]-1)&&data[3]>(value[3]-1)&&data[7]>(value[4]-1))
    Motor_Control(FORW,100,BACK,100);
  else if(data[0]<(value[0]-1)&&data[1]>(value[1]-1)&&data[2]>(value[2]-1)&&data[3]>(value[3]-1)&&data[7]>(value[4]-1))
    Motor_Control(FORW,100,BACK,100);
}
//trace light
void hunt_light(void)
{
  if (data[4] >1&&data[4]< 2)
    Motor_Control(BACK,100,FORW,100);
  else if (data[4] > 4 && data[4] < 5)
    Motor_Control(FORW,100,BACK,100);
  else
    Motor_Control(FORW,0,FORW,0);
}
//ir remote control using controller from ir kit
void timer1_init(void)
{
  TCCR1A = 0X00; 
  TCCR1B = 0X05;
  TCCR1C = 0X00;
  TCNT1 = 0X00;
  TIMSK1 = 0X00;
}

void remote_deal(void)
{ 
  switch(ir_code)
  {
  case 0xff00:
    Motor_Control(FORW,0,FORW,0);
    break;
  case 0xfe01:
    Motor_Control(FORW,100,FORW,100);
    break;
  case 0xf609:
    Motor_Control(BACK,100,BACK,100);
    break;
  case 0xfb04:
    Motor_Control(FORW,100,BACK,100);
    break;
  case 0xf906:
    Motor_Control(BACK,100,FORW,100);
    break;
  }
}

char logic_value()
{
  while(!(digitalRead(8)));
  Pulse_Width=TCNT1;
  TCNT1=0;
  if(Pulse_Width>=7&&Pulse_Width<=10)
  {
    while(digitalRead(8));
    Pulse_Width=TCNT1;
    TCNT1=0;
    if(Pulse_Width>=7&&Pulse_Width<=10)
      return 0;
    else if(Pulse_Width>=25&&Pulse_Width<=27)
      return 1;
  }
  return -1;
}
void pulse_deal()
{
  int i;


  for(i=0; i<8; i++)
  {
    if(logic_value() != 0)
        return;
  }
  
  for(i=0; i<6; i++)
  {
    if(logic_value()!= 1)
        return;
  }
  
  if(logic_value()!= 0)
      return;
      
  if(logic_value()!= 1)
      return;


  //decode command
  ir_code=0x00;
  for(i=0; i<16;i++ )
  {
    if(logic_value() == 1)
    {
      ir_code |=(1<<i);
    }
  }
}

void remote_decode(void)
{
  TCNT1=0X00;       
  while(digitalRead(8))
  {
    if(TCNT1>=1563)
    {
      ir_code = 0xff00;
      return;
    }  
  }


  TCNT1=0X00;

  while(!(digitalRead(8)));
  Pulse_Width=TCNT1;
  TCNT1=0;
  if(Pulse_Width>=140&&Pulse_Width<=141)//9ms
  {

    while(digitalRead(8));
    Pulse_Width=TCNT1;
    TCNT1=0;
    if(Pulse_Width>=68&&Pulse_Width<=72)//4.5ms
    {
      pulse_deal();
      return;
    }
    else if(Pulse_Width>=34&&Pulse_Width<=36)//2.25ms
    {
      while(!(digitalRead(8)));
      Pulse_Width=TCNT1;
      TCNT1=0;
      if(Pulse_Width>=7&&Pulse_Width<=10)//560us
      {
        return; 
      }
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

void key_scan(void)
{
  if(data[5]>4.50&&data[5]<6.00)
    return;
  else
  {
    if(data[5]>=0.00&&data[5]<0.50)
    {
      delay(200);
      if(data[5]>=0.00&&data[5]<0.50)
      {
        buzzer();
        key_1++;
        if(key_1>=1&&key_1<=5) 
          value_adjust(key_1);
      }
    }
    else if(data[5]>=0.50&&data[5]<1.5)
    {
      delay(200);
      if(data[5]>=0.50&&data[5]<1.5)
      {
        buzzer();
        if(key_1>=1&&key_1<=5)
        {
          value[key_1-1]++;
          value_adjust(key_1);
        }
        else
        {
          key_2++;
        }
      }
    }
    else if(data[5]>=1.5&&data[5]<2.5)
    {
      delay(200);
      if(data[5]>=1.5&&data[5]<2.5)
      {
        buzzer();
        if(key_1>=1&&key_1<=5)
        {
          value[key_1-1]--;
          value_adjust(key_1);
        }
        else
        {
          key_3++;
        }
      }
    }
  }
}
void key_deal()
{
  if(key_1==6)
  {
    digitalWrite(LED_RED,LOW);
    digitalWrite(LED_GREEN,LOW);
    huntline_deal();
  }
  if(key_2==1||key_3==1||key_3==3)
  {
    digitalWrite(LED_RED,HIGH);
    pcint0_init();
    interrupt01_init();
    sei();
    Obstacle_Avoidance();
  }
  if(key_2==3)
  {
    digitalWrite(LED_RED,HIGH);
    hunt_light();
  }
  if(key_3==5)
  {
    digitalWrite(LED_RED,HIGH);
    timer1_init();
    remote_decode();
    remote_deal();
  }
  if(key_1==7||key_2==2||key_2==4||key_3==2||key_3==4||key_3==6)
  {
    
    key_1=0;
    if(key_2==4)
      key_2=0;
    if(key_3==6)
      key_3=0;
    digitalWrite(LED_RED,LOW);
    Motor_Control(FORW,0,FORW,0);
  }
}

void low_voltage_check(void)
{ 
  if(data[6]<2.5)
  {
    buzzer();
  }
}
void setup()
{
  int i;
  for(i=4;i<=13;i++)
  {
    pinMode(i,OUTPUT);
  }
  pinMode(IR_IN,INPUT);
}
void loop()
{
  Motor_Control(FORW,0,FORW,0);
  while(1)
  {
    Read_Value();
    key_scan();
    key_deal();
    low_voltage_check();
  }
}






















