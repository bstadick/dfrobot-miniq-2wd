#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>

#define FORW 1//前进
#define BACK 0//后退
#define Vr   5//参考电压值5v

float  data[8]={0X00,0X00,0X00,0X00,0x00,0xff,0x00,0x00};//存储8个通道ad转换的值
float  data_;
char   channel=0x40;//ad转换通道的选择
char   key_1 = 0,key_2=0,key_3=0;//按键按下的次数计数
char   value[5]={0X00,0X00,0X00,0X00,0x00};//红外传感器输出的电压值
int    count=0;
int    count_r=0,count_l=0;//对左右轮返回的脉冲进行计数
int    Pulse_Width=0;//存储脉宽 
int    ir_code=0x00;   //命令值
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
//ad转换初始化函数
void adc_init(int channel)
{
    //设置ad转换寄存器
	ADMUX =channel;
	ADCSRA =0xcb; 
}
//ad转换中断函数
ISR(ADC_vect)
{
   ADCSRA &=~ (1<<ADEN);      //关中断
   data_ = ADCL;               //接收第八位值
   data_  = data_+ADCH*256;    //整个adc的数字值
   data_  = ((data_*Vr)/1024); //转换成模拟值
   switch(ADMUX)
   {
        case 0x40:
		          data[0] = data_;
				  break;
	    case 0x41:
		          data[1] = data_;
				  break;
	    case 0x42:
		          data[2] = data_;
				  break;
	    case 0x43:
		          data[3] = data_;
				  break;
		case 0x44:
		          data[4] = data_;
				  break;
		case 0x45:
		          data[5] = data_; 
				  break;
		case 0x46:
		          data[6] = data_;
				  break;
		case 0x47:
		          data[7] = data_;
				  break;
   }
   if(++channel==0x48)//换通道
   	{
   		  channel = 0x40;
   	}
    adc_init(channel);//重新初始化
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
     if(key_2==1||key_3==3)//避障行为或者小鸡行为
	 {
     Motor_Control(BACK,100,BACK,100);//后退
	 for(j=0;j<50;j++)//延时500ms
	 _delay_ms(10);
     Motor_Control(BACK,100,FORW,100);//右转
	 for(j=0;j<50;j++)//延时500ms
	 _delay_ms(10);
	 }
	 if(key_3==1)//袭击者模式
     {
      Motor_Control(FORW,100,FORW,100);//前进
	  for(j=0;j<30;j++)//延时
      _delay_ms(10);//延时
     }
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
	     if(key_2==1||key_3==3)//避障行为或者小鸡行为
		 {
	     Motor_Control(BACK,100,BACK,100);//后退
		 for(j=0;j<50;j++)//延时
		 _delay_ms(10);
	     Motor_Control(FORW,100,BACK,100);//左转
		 for(j=0;j<50;j++)//延时
		 _delay_ms(10);
		 }
		 if(key_3==1)//袭击者模式
	     {
	      Motor_Control(FORW,100,FORW,100);//前进
		  for(j=0;j<30;j++)//延时
	      _delay_ms(10);//延时
	     }
	   }
	   else
	   if(key_2==1||key_3==3)//避障模式或者小鸡模式
       Motor_Control(FORW,100,FORW,100);//前进
       if(key_3==1)//袭击者模式
       Motor_Control(FORW,0,FORW,0);//前进
   }
}
//寻线函数
void huntline_deal(void)
{
    if(data[0]>(value[0]-1)&&data[1]>(value[1]-1)&&data[2]<(value[2]-1)&&data[3]>(value[3]-1)&&data[7]>(value[4]-1))
	{
		Motor_Control(FORW,100,FORW,100);//前进
	} 
	else if(data[0]>(value[0]-1)&&data[1]>(value[1]-1)&&data[2]<(value[2]-1)&&data[3]<(value[3]-1)&&data[7]>(value[4]-1))
	{
		Motor_Control(BACK,0,FORW,100);//右转
	}
	else if(data[0]>(value[0]-1)&&data[1]>(value[1]-1)&&data[2]>(value[2]-1)&&data[3]<(value[3]-1)&&data[7]>(value[4]-1))
	{
		Motor_Control(BACK,0,FORW,100);//右转
	}
	else if(data[0]>(value[0]-1)&&data[1]>(value[1]-1)&&data[2]>(value[2]-1)&&data[3]<(value[3]-1)&&data[7]<(value[4]-1))
	{
		Motor_Control(BACK,100,FORW,100);//右转
	}
	else if(data[0]>(value[0]-1)&&data[1]>(value[1]-1)&&data[2]>(value[2]-1)&&data[3]>(value[3]-1)&&data[7]<(value[4]-1))
	{
		Motor_Control(BACK,100,FORW,100);//右转
	}
	else if(data[0]>(value[0]-1)&&data[1]<(value[1]-1)&&data[2]<(value[2]-1)&&data[3]>(value[3]-1)&&data[7]>(value[4]-1))
	{
		Motor_Control(FORW,100,BACK,0);//左转
	}
	else if(data[0]>(value[0]-1)&&data[1]<(value[1]-1)&&data[2]>(value[2]-1)&&data[3]>(value[3]-1)&&data[7]>(value[4]-1))
	{
		Motor_Control(FORW,100,BACK,0);//左转
	}
	else if(data[0]<(value[0]-1)&&data[1]<(value[1]-1)&&data[2]>(value[2]-1)&&data[3]>(value[3]-1)&&data[7]>(value[4]-1))
	{
		Motor_Control(FORW,100,BACK,100);//左转
	}
	else if(data[0]<(value[0]-1)&&data[1]>(value[1]-1)&&data[2]>(value[2]-1)&&data[3]>(value[3]-1)&&data[7]>(value[4]-1))
	{
		Motor_Control(FORW,100,BACK,100);//左转
	}

}
//电压值调整函数
void value_adjust(char num)
{
     if(num==1)
	 {
	    if(data[0]>value[0])
        {
		   PORTB |=(1<<4); //当前值小红灯亮
		   PORTB &=~(1<<5);
        }
        else
        {
		   PORTB |=(1<<5); //当前值大绿灯亮
		   PORTB &=~(1<<4);
        } 
	 }
	 if(num==2)
	 {
	    if(data[1]>value[1])
        {
		   PORTB |=(1<<4); //当前值小红灯亮
		   PORTB &=~(1<<5);
        }
        else
        {
		   PORTB |=(1<<5); //当前值大绿灯亮
		   PORTB &=~(1<<4);
        } 
	 }
	 if(num==3)
	 {
	    if(data[2]>value[2])
        {
		   PORTB |=(1<<4); //当前值小红灯亮
		   PORTB &=~(1<<5);
        }
        else
        {
		   PORTB |=(1<<5); //当前值大绿灯亮
		   PORTB &=~(1<<4);
        } 
	 }
	 if(num==4)
	 {
	    if(data[3]>value[3])
        {
		   PORTB |=(1<<4); //当前值小红灯亮
		   PORTB &=~(1<<5);
        }
        else
        {
		   PORTB |=(1<<5); //当前值大绿灯亮
		   PORTB &=~(1<<4);
        } 
	 }
	 if(num==5)
	 {
	    if(data[4]>value[4])
        {
		   PORTB |=(1<<4); //当前值小红灯亮
		   PORTB &=~(1<<5);
        }
        else
        {
		   PORTB |=(1<<5); //当前值大绿灯亮
		   PORTB &=~(1<<4);
        } 
	 }
}
//寻光函数
void hunt_light(void)
{

	if (data[4]> 1 && data[4]< 2) 
	{
	     Motor_Control(BACK,100,FORW,100);//右转
	}
	else if (data[4]> 4 && data[4]< 5)
	{
	    Motor_Control(FORW,100,BACK,100);//左转
	}
	else
	{
		Motor_Control(FORW,0,FORW,0);//停止
	}

}
//遥控

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

char logic_value()//逻辑‘0’和‘1’判断函数
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

void pulse_deal()//接收地址码和命令码
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
void remote_decode(void)//译码函数
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
//蜂鸣器
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
//按键扫描
void key_scan(void)
{
  if(data[5]>4.50&&data[5]<6.00)//没有按键按下
    return;
  else
  {
    if(data[5]>=0.00&&data[5]<0.50)//按键1按下
    {
       _delay_ms(5);//延时消抖
      if(data[5]>=0.00&&data[5]<0.50)//按键1确实按下
      {
        buzzer();//蜂鸣器响
		key_1++;//按键1计数
        if(key_1>=1&&key_1<=5) 
		value_adjust(key_1);//寻线传感器的值调整
      }
    }
    else if(data[5]>=0.50&&data[5]<2.00)
    {
       _delay_ms(5);//延时消抖
      if(data[5]>=0.50&&data[5]<2.00)
      {
	     buzzer();//蜂鸣器响
		 if(key_1>=1&&key_1<=5)
        {
          value[key_1-1]++;//传感器的分辨轨迹的界限值加加（调整）
          value_adjust(key_1);//跟实际值对比
        }
     	else
		key_2++;//key2计数
      }
    }
    else if(data[5]>=2.00&&data[5]<3.00)
    {
       _delay_ms(5);//延时消抖
      if(data[5]>=2.00&&data[5]<3.00)
      {
	     buzzer();//蜂鸣器响
		 if(key_1>=1&&key_1<=5)
        {
          value[key_1-1]--;//传感器的分辨轨迹的界限值减减（调整）
          value_adjust(key_1);//跟实际值对比
        }
		else
		key_3++;//key3计数
      }
    }
  }
}
//按键值处理函数
void key_deal(void)
{

    if(key_1==6)//寻线
	{
	   PORTB &=~(1<<5);//绿灯灭
       PORTB &=~(1<<4);//红灯灭
	   huntline_deal();//寻线
	}
	if(key_2==1||key_3==1||key_3==3)//key_2=1避障行为，key_3==1袭击者，key_3==3小鸡行为
	{
	    ADCSRA=0XC3;
	    PORTB |=(1<<4);
	    pcint0_init();//引脚变化中断初始化
	    Obstacle_Avoidance();
        ADCSRA=0XCB;
	}
	if(key_2==3)//key_2==3寻光行为
	{
	   PORTB |=(1<<4);
	   hunt_light();//寻光
	}  
	if(key_3==5)//遥控
    {
	    ADCSRA=0XC3;
	    PORTB |=(1<<4);
	    timer1_init();//定时器初始化函数
	    remote_decode();  //译码
	    remote_deal();   //执行译码结果
		ADCSRA=0XCB;
    } 
	if(key_1==7||key_2==2||key_2==4||key_3==2||key_3==4||key_3==6)
	{
	   //按键值清零
	    key_1=0;
	    if(key_2==4)
	    key_2=0;
	    if(key_3==6)
	    key_3=0;
        PORTB &=~(1<<4);
		Motor_Control(FORW,0,FORW,0);//停止
	}
}
//主函数
int main(void)
{
    DDRD = 0XF2;//设置PORTD口引脚
	PORTD |= ((1<<0)|(1<<2)|(1<<3));//设置PORTD口输入引脚的上拉电阻有效
	DDRB = 0XFE;//设置PORTB口引脚
	PORTB |= (1<<0);//设置PORTB口输入引脚的上拉电阻有效
    pwm_init();//PWM脉冲初始化
	adc_init(channel);//ad转换初始化
	sei();//全局中断允许
	while(1)
	{
	   key_scan();//扫描按键
	   key_deal();//按键处理
    }
}
