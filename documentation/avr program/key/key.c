#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>

#define Vr  5//ad转换参考电压值5v

float data=0xff;

//蜂鸣器发声函数
void buzzer(void)
{
     char k=0;
	 for(k=0;k<5;k++)
	 {
	     PORTB |= (1<<3);
		 _delay_us(10);
		 PORTB &= ~(1<<3);
		 _delay_us(40);//蜂鸣器响
	 }
}
//AD转换初始化
void adc_init(void)
{
    //设置相应寄存器
	ADMUX =0x45;
	ADCSRA = 0Xcb;
}
//ad转换中断函数
ISR(ADC_vect)
{
   
   ADCSRA &=~ (1<<ADEN);    //关中断
   data = ADCL;            //接收低八位值
   data  = data+ADCH*256;  //整个adc的数字值
   data  = ((data*Vr)/1024);//转换成模拟值
   adc_init();//重新初始化
}
//按键扫描
void key_scan(void)
{
  if(data>4.50&&data<6.00)//无按键按下
     return;//返回
  else
  {
    if(data>=0.00&&data<0.50)//按键1按下
    { 
      _delay_ms(5);//延时消抖
      if(data>=0.00&&data<0.50)//按键1确实按下
      {
        buzzer();//蜂鸣器响
        PORTB |=(1<<4);//红灯亮
        PORTB &=~(1<<5);//绿灯灭
	  }
    }
    else if(data>=0.50&&data<1.5)//按键2按下
    {
       _delay_ms(5);//延时消抖
      if(data>=0.50&&data<1.5)//按键2确实按下
      {
        buzzer();//蜂鸣器响
        PORTB |=(1<<5);//绿灯亮
        PORTB &=~(1<<4);//红灯灭
      }
    }
    else if(data>=1.5&&data<2.5)//按键3按下
    {
      _delay_ms(5);//延时
      if(data>=1.5&&data<2.5)//按键3确实按下
      {
        buzzer();//蜂鸣器响
        PORTB &=~(1<<5);//绿灯灭
        PORTB &=~(1<<4);//红灯灭
      }
    }
  }
}
int main(void)
{
   DDRB =0XFE;//设定PB4、PB5引脚为输出模式
   adc_init();//AD初始化
   sei();//全局中断允许
   while(1)
   {
     key_scan();//按键扫描
   }
}
