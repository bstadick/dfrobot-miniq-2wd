#define EN1 6
#define IN1 7
#define EN2 5
#define IN2 4

#define FORW 1
#define BACK 0

#define IR_IN  8

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
void setup()
{
   Serial.begin(9600);
  unsigned char i;
  for(i=4;i<=7;i++)
  {
    pinMode(i,OUTPUT);
  }
  pinMode(IR_IN,INPUT);
}
void loop()
{  
  timer1_init();
  while(1)
  {
    remote_decode();
    //remote_deal();
    Serial.println(ir_code);
  }  
}


