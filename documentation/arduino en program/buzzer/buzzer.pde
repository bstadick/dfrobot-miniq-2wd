#define BUZZER 11//the buzzer pin
void setup()
{
  pinMode(BUZZER,OUTPUT);//init buzzer
}
void loop()
{
  unsigned char i,j;
  while(1)
  {
    //output a sound
    for(i=0;i<80;i++)
    {
      digitalWrite(BUZZER,HIGH);
      delay(1);
      digitalWrite(BUZZER,LOW);
      delay(1);
    }
    //output another sound
    for(i=0;i<100;i++)
    {
      digitalWrite(BUZZER,HIGH);
      delay(2);
      digitalWrite(BUZZER,LOW);
      delay(2);
    }
  }
}
