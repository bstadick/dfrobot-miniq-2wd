#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#include<avr/eeprom.h>

#define FORW 1//前进
#define BACK 0//后退	
#define Vr   5//ad转换参考电压值5v

float  data[8]={0X00,0X00,0X00,0X00,0x00,0xff,0x00,0x00};//存储8个通道ad转换的值
float  data_;        //存储经过计算后的ad转换的值
char   channel=0x40; //ad转换通道的选择
char   key_1 = 0,key_2=0,key_3=0; //按键按下的次数计数
char   value[5]={0X00,0X00,0X00,0X00,0x00}; //红外传感器输出的电压值
unsigned char *p;

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
	ADCSRA = 0Xcb; 
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
    adc_init(channel);
}
//寻线函数
void huntline_deal(void)
{
    if(key_2==1)
	{
		int i;
		*p=0x0a;
	    //读取存在EEPROM中的电压值
	    for(i=0;i<5;i++)
		{
			eeprom_busy_wait();
		    value[i]=eeprom_read_byte(p+i);
		}
	    if(data[0]>(value[0]-1)&&data[1]>(value[1]-1)&&data[2]<(value[2]-1)&&data[3]>(value[3]-1)&&data[7]>(value[4]-1))//测一下实际值
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
//蜂鸣器
void buzzer(void)
{
     char k=0;
	 for(k=0;k<5;k++)
	 {
	     PORTB |= (1<<3);
		 _delay_us(10);
		 PORTB &= ~(1<<3);
		 _delay_us(40);
	 }
}
//按键扫描
void key_scan(void)//按键扫描
{
  int i;
  *p=0X0A;
  if(data[5]>4.50&&data[5]<6.00)//没有按键按下
    return;//返回
  else
  {
    if(data[5]>=0.00&&data[5]<0.50)//按键1按下
    {
       _delay_ms(5);//延时消抖
      if(data[5]>=0.00&&data[5]<0.50)//按键1确实按下
      {
        buzzer();//蜂鸣器响
        key_1++;//按键1计数
        if(key_1>=1&&key_1<=5) value_adjust(key_1);//寻线传感器的值调整
        if(key_1==6)
        {
          PORTB &=~(1<<5);//绿灯灭
          PORTB &=~(1<<4);//红灯灭
          //将设置好的电压值存入EEPROM，做到掉电保护
          for(i=0;i<5;i++)
		  {
            eeprom_busy_wait();
		    eeprom_write_byte(p+i,value[i]);
		  }
        }
      }
    }
    else if(data[5]>=0.50&&data[5]<1.5)
    {
       _delay_ms(5);//延时消抖
      if(data[5]>=0.50&&data[5]<1.5)
      {
	    buzzer();
        if(key_1>=1&&key_1<=5)
        {
          value[key_1-1]++;//传感器的分辨轨迹的界限值加加（调整）
          value_adjust(key_1);//跟实际值对比
        }
		else
		key_2++;
      }
    }
    else if(data[5]>=1.5&&data[5]<2.5)
    {
       _delay_ms(5);//延时消抖
      if(data[5]>=1.5&&data[5]<2.5)
      {
	    buzzer();
        if(key_1>=1&&key_1<=5)
        {
          value[key_1-1]--;//传感器的分辨轨迹的界限值减减（调整）
          value_adjust(key_1);//跟实际值对比
        }
      }
    }
  }
}

//主函数
int main(void)
{
    DDRD = 0XF2;//引脚配置
	DDRB = 0XFE;
	PORTD |=((1<<0)|(1<<2)|(1<<3));//设置输入引脚的上拉电阻有效
	pwm_init();//PWM脉冲初始化
	adc_init(channel);//ad初始化
	sei();
	while(1)
	{
	   key_scan();//按键扫描
	   huntline_deal();//寻线
	}
	
}
