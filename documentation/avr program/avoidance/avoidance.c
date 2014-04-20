#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>

#define FORW 1//前进
#define BACK 0//后退

int count= 0;

//pwm初始化子函数
void pwm_init(void)
{ 
	TCCR0A = 0XA3;      
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
//左红外发射管发射频率为38khz脉冲
void L_Send(void)
{
    int i;
	for(i=0;i<24;i++)
	{
    PORTB &=~(1<<1);//置低，导通
	_delay_us(12);//延时
    PORTB |=(1<<1);//置高，截止
	_delay_us(12);//延时
	}
}
//右红外发射管发射频率为38khz脉冲
void R_Send(void)
{
    int i;
	for(i=0;i<24;i++)
	{
    PORTB &=~(1<<2);
	_delay_us(12);
    PORTB |=(1<<2);
	_delay_us(12);
	}
}
//引脚变化中断初始化函数
void pcint0_init(void)
{
	PCICR = 0X01;//使能第0组引脚变化中断
	PCMSK0 = 0X01;//使能第0组的第0个引脚变化中断
}
//引脚变化中断函数

ISR(PCINT0_vect)
{
	count++;//对接收到的脉冲计数
}
//避障功能子函数
void Obstacle_Avoidance(void)
{
   char i,j;
   count=0;//计数清零
   for(i=0;i<20;i++)//左管发射20个脉冲，会有40个中断
   {
     L_Send();
     for(j=0;j<54;j++)
	 _delay_us(10);
   }
   if(count>20)//接收到大于20个中断
   {
     Motor_Control(BACK,100,BACK,100);//后退
	 for(j=0;j<50;j++)//延时500ms
	 _delay_ms(10);
     Motor_Control(BACK,100,FORW,100);//右转
	 for(j=0;j<50;j++)//延时500ms
	 _delay_ms(10);
   }
   else
   {
	   count=0;//清零
	   for(i=0;i<20;i++)//右发射管发射20个脉冲，会有40个中断
	   {
	     R_Send();
	     for(j=0;j<54;j++)
		 _delay_us(10);
	   }
	   if(count>20)//接收到大于20个中断
	   {
	     Motor_Control(BACK,100,BACK,100);//后退
		 for(j=0;j<50;j++)//延时
		 _delay_ms(10);
	     Motor_Control(FORW,100,BACK,100);//左转
		 for(j=0;j<50;j++)//延时
		 _delay_ms(10);
	   }
	   else
	   Motor_Control(FORW,100,FORW,100);//前进
   }
}
//主函数
int main(void)
{

	DDRD = 0XF2;//PORTD引脚模式设置
	DDRB = 0XFE;//PORTB引脚模式设置
	//设置输入引脚的上拉电阻有效
	PORTD |= ((1 << 0) | (1 << 2) | (1 << 3));
	PORTB |= (1 << 0);
	//给两个红外发射管高电平，红外管截止
	PORTB |=(1<<1);
    PORTB |=(1<<2);
	pwm_init();//pwm脉冲初始化
	pcint0_init();//外部中断初始化
	sei();//全局中断使能
	while (1)
	{
	    Obstacle_Avoidance();//避障子函数
	}
}
