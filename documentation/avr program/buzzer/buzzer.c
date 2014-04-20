#include<avr/io.h>
#include<util/delay.h>


int main(void)
{
    char i;
    DDRB = (1<<3);//设置PORTB端口的PB3引脚为输出模式

	while(1)
	{
		//输出一个频率的声音
	    for(i=0;i<80;i++)
	    {
	      PORTB |=(1<<3);//设置PB3引脚为高电平，蜂鸣器响
	      _delay_ms(1);//延时
	      PORTB &= ~(1<<3);//设置PB3引脚为低电平，蜂鸣器不响
	      _delay_ms(1);//延时
	    }
	    //输出另一个频率的声音
	    for(i=0;i<100;i++)
	    {
	      PORTB = (1<<3);//设置PB3引脚为高电平，蜂鸣器响
	      _delay_ms(2);//延时
	      PORTB &= ~(1<<3);//设置PB3引脚为低电平，蜂鸣器不响
	      _delay_ms(2);//延时
	    }
	}
}
