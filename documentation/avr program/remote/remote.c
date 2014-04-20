#include<avr/io.h>
#include<util/delay.h>

#define FORW 1//前进
#define BACK 0//后退

int  Pulse_Width=0;//存储脉宽 
int  ir_code=0x00;   //命令值

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
//设置定时器T1为定时模式
void timer1_init(void)
{
  TCCR1A = 0X00; 
  TCCR1B = 0X05;//给定时器时钟源  
  TCCR1C = 0X00;
  TCNT1 = 0X00;
  TIMSK1 = 0X00;//禁止定时器溢出中断
}
//执行译码结果
void remote_deal(void)
{ 
  switch(ir_code)
  {
    case 0xff00://停止
       Motor_Control(FORW,0,FORW,0);
      break;
    case 0xfe01://前进
	   Motor_Control(FORW,100,FORW,100);
       break;
    case 0xf609://后退
	   Motor_Control(BACK,100,BACK,100);
       break;
    case 0xfb04://左转
	   Motor_Control(FORW,100,BACK,100);
       break;
    case 0xf906://右转
      Motor_Control(BACK,100,FORW,100);
      break;
  }
}
//逻辑‘0’和‘1’判断函数
char logic_value()
{
    while(!(PINB&0X01)); //低等待
     Pulse_Width=TCNT1;
     TCNT1=0;
     if(Pulse_Width>=7&&Pulse_Width<=10)//低电平560us
     {
         while(PINB&0X01);//是高就等待
         Pulse_Width=TCNT1;
         TCNT1=0;
         if(Pulse_Width>=7&&Pulse_Width<=10)//接着高电平560us
           return 0;
         else if(Pulse_Width>=25&&Pulse_Width<=27) //接着高电平1.7ms
           return 1;
     }
     return -1;
}
//接收地址码和命令码
void pulse_deal()
{
  int i;
  //地址码
  //执行8个0
  for(i=0; i<8; i++)
  {
    if(logic_value() != 0) //不是0
      return;
  }
  //地址反码
  //执行6个1
  for(i=0; i<6; i++)
  {
     if(logic_value()!= 1) //不是1
      return;
  }
  //执行1个0
  if(logic_value()!= 0) //不是0
      return;
  //执行1个1
  if(logic_value()!= 1) //不是1
      return;
      
  
  //解析遥控器编码中的command指令
  ir_code=0x00;//清零
  for(i=0; i<16;i++ )
  {
    if(logic_value() == 1)
    {
      ir_code |=(1<<i);
    }
  }
}
//译码函数
void remote_decode(void)
{
     TCNT1=0X00;       
     while(PINB&0X01)//是高就等待
     {
        if(TCNT1>=1563)  //当高电平持续时间超过100ms，表明此时没有按键按下
        {
             ir_code = 0xff00;
             return;
        }  
     }
     
     //如果高电平持续时间不超过100ms
     TCNT1=0X00;
     
     while(!(PINB&0X01)); //低等待
     Pulse_Width=TCNT1;
     TCNT1=0;
     if(Pulse_Width>=140&&Pulse_Width<=141)//9ms
     {
         
         while(PINB&0X01);//是高就等待
         Pulse_Width=TCNT1;
         TCNT1=0;
         if(Pulse_Width>=68&&Pulse_Width<=72)//4.5ms
         {
            pulse_deal();
            return;
         }
         else if(Pulse_Width>=34&&Pulse_Width<=36)//2.25ms
         {
            while(!(PINB&0X01));//低等待
            Pulse_Width=TCNT1;
            TCNT1=0;
            if(Pulse_Width>=7&&Pulse_Width<=10)//560us
            {
               return; 
            }
         }
     }

}
//主函数
int main(void)
{

	DDRD = 0XF2; //引脚配置
	DDRB = 0XFE;
	PORTD |= ((1 << 0) | (1 << 2) | (1 << 3)); //设置输入引脚的上拉电阻有效
	PORTB |= (1 << 0);
	pwm_init();
	timer1_init();
	while (1)
	{  
	  remote_decode();  //译码
      remote_deal();   //执行译码结果
	}

}
