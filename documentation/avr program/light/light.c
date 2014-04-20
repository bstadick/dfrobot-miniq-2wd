#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#include<avr/eeprom.h>

#define FORW 1//前进
#define BACK 0//后退
#define Vr   5//参考电压值

float data[8]={0X00,0X00,0X00,0X00,0x00,0xff,0x00,0x00};//存储8个通道ad转换的值
float data_;
char  channel=0x40;//ad转换通道的选择
float value[10]={0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
float val;
unsigned char sum=0;
unsigned char *p;
int   key_1=0,key_2=0,key_3=0;

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
	ADMUX = channel;
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
//寻光函数
void hunt_light(void)
{
  int i;
  *p=0x0a;
  if(key_1==1)
  {
    key_1=0;
    //读取10次环境光的电压值，并求和
    for(i=0;i<10;i++)
    {
      value[i]=data[4];
      sum +=value[i];
    }
    //将环境光电压值的和存放在EEPROM里，这样掉电可以保护数据不丢失
    eeprom_busy_wait();
    eeprom_write_byte(p,sum);	
  }
  else
  {
    eeprom_busy_wait();
    val=eeprom_read_byte(p);//读取电压值的和
    val=val/10;//求平均值
    if (data[4]<val-0.5)   //根据实际的实验环境进行测量
      Motor_Control(FORW,100,BACK,100);//左转
    else if (data[4]>val+1)
      Motor_Control(BACK,100,FORW,100);//右转
    else
      Motor_Control(FORW,0,FORW,0);//停止
  }
}
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
//按键扫描
void key_scan(void)
{
  if(data[5]>4.50&&data[5]<6.00)//没有按键按下
    return;
  else
  {
    if(data[5]>=0.00&&data[5]<0.50)//按键1按下
    {
       _delay_ms(10);//延时消抖
      if(data[5]>=0.00&&data[5]<0.50)//按键1确实按下
      {
        buzzer();//蜂鸣器响
		key_1++;//按键1计数                         
      }
    }
    else if(data[5]>=0.50&&data[5]<1.5)
    {
       _delay_ms(5);//延时消抖
      if(data[5]>=0.50&&data[5]<1.5)
      {
	     buzzer();//蜂鸣器响
		 key_2++;//key2计数
      }
    }
    else if(data[5]>=1.5&&data[5]<2.5)
    {
       _delay_ms(5);//延时消抖
      if(data[5]>=1.5&&data[5]<2.5)
      {
	     buzzer();//蜂鸣器响
		 key_3++;//key3计数
      }
    }
  }
}
//主函数
int main(void)
{
	DDRD = 0XF2;//引脚配置
	DDRB =0XFE;
	PORTD |= ((1 << 0) | (1 << 2) | (1 << 3));//设置输入引脚的上拉电阻有效
	pwm_init();//pwm初始化
	adc_init(channel);//ad初始化
	sei();//全局中断允许
	while (1)
	{
	   key_scan();//按键扫描
	   hunt_light();//寻光
	}
}
