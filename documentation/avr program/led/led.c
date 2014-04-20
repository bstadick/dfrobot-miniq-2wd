#include<avr/io.h>
#include<util/delay.h>

int main(void)
{
    DDRB = (1<<4);//设置PORTB端口的PB4脚为输出模式
	while(1)
	{
	    int i;
	    PORTB |= (1<<4);//置PB4引脚为高电平，红灯亮
		//延时1s
		for(i=0;i<100;i++)
		_delay_ms(10);//延时10ms

		PORTB &= ~(1<<4);//置PB4引脚为低电平，红灯灭
		//延时1s
		for(i=0;i<100;i++)
	    _delay_ms(10);//延时10ms
	}
}
