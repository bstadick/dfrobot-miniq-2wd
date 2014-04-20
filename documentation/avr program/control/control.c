#include<avr/io.h>
#include<util/delay.h>	

#define FORW 1//前进
#define BACK 0//后退


//pwm初始化函数
void pwm_init(void)
{ 
	TCCR0A = 0XA3;//快速PWM模式      
	TCCR0B = 0X03;//时钟源设置为clk/64
	TCNT0 = 0X00; //初值设置为0
	TIMSK0 = 0X00;//禁止中断    
}
//控制电机转动
void Motor_Control(int M1_DIR,int M1_EN,int M2_DIR,int M2_EN)
{
  //////////M1////////////////////////
  if(M1_DIR==FORW)//M1电机方向  
    PORTD |=(1<<7);//设置向前
  else
    PORTD &=~(1<<7);//设置向后   
  if(M1_EN==0)//M1电机速度 
	OCR0A = 0;//停止
  else
    OCR0A = M1_EN;//设置给定的数值

  ///////////M2//////////////////////
  if(M2_DIR==FORW)//M2电机的方向  
    PORTD |=(1<<4);//设置向前 
  else
    PORTD &=~(1<<4);//设置向后   
  if(M2_EN==0) //M2电机的速度
    OCR0B = 0;//停止
  else
    OCR0B = M2_EN;//设置给定的数值
}
//主函数
int main(void)
{
	DDRD |=(1<<4)|(1<<5)|(1<<6)|(1<<7);//设置PORTD端口的4、5、6、7引脚为输出模式
	pwm_init();//pwm初始化
	while(1)
	{
	   int i;
	   Motor_Control(FORW,80,FORW,80);//前进
       for(i=0;i<50;i++)//延时
	   _delay_ms(10);
       Motor_Control(BACK,0,BACK,0);//停止
       for(i=0;i<50;i++)//延时
	   _delay_ms(10);
       Motor_Control(BACK,80,BACK,80);//后退
	   for(i=0;i<50;i++)//延时
	   _delay_ms(10);
       Motor_Control(BACK,0,BACK,0);//停止
       for(i=0;i<50;i++)//延时
	   _delay_ms(10);
	}
}
