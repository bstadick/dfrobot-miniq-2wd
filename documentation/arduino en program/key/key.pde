#define BUZZER 11//buzzer io pin
#define LED_RED 12//red led io pin
#define LED_GREEN 13//green led io pin
#define Vr   5//reference voltage

float data=0xff;//save the button input value

void buzzer(void)//buzzer sound
{
  digitalWrite(BUZZER,HIGH);//turn it on
  delay(1);
  digitalWrite(BUZZER,LOW);//turn it off
  delay(10);
}

void Read_Value(void)//read voltage
{
  data=analogRead(5);
  data = ((data * Vr) / 1024);
  key_scan();
}

void key_scan(void)//scan the keyboard input
{
  if(data>4.50&&data<6.00)//no input
    return;
  else
  {
    if(data>=0.00&&data<0.50)//press button1
    {
      delay(180);//remove noise
      if(data>=0.00&&data<0.50)//confirm the button pressed
      {
        buzzer();
        digitalWrite(LED_RED,HIGH);//turn on the red led
      }
    }
    else if(data>=0.50&&data<1.5)
    {
      delay(180);
      if(data>=0.50&&data<1.5)
      {
        buzzer();
        digitalWrite(LED_GREEN,HIGH);//turn on the green led
      }
    }
    else if(data>=1.5&&data<2.5)
    {
      delay(180);
      if(data>=1.5&&data<2.5)
      {
        buzzer();
        digitalWrite(LED_RED,LOW);//turn off the red led
        digitalWrite(LED_GREEN,LOW);//turn off the green led
      }
    }
  }
}
void setup()
{
  pinMode(BUZZER,OUTPUT);//init buzzer
  pinMode(LED_RED,OUTPUT);//init red led
  pinMode(LED_GREEN,OUTPUT);//init green led
}
void loop()
{
  Read_Value();//read button input
}

