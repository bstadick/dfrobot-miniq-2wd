#define LED_RED 12//led pin
void setup()
{
  pinMode(LED_RED,OUTPUT);//init red led
}
void loop()
{
  digitalWrite(LED_RED,HIGH);//turn on the led
  delay(1000);
  digitalWrite(LED_RED,LOW);//then turn it off
  delay(1000);
}
