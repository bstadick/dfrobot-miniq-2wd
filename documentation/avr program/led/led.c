#include<avr/io.h>
#include<util/delay.h>

int main(void)
{
    DDRB = (1<<4);//����PORTB�˿ڵ�PB4��Ϊ���ģʽ
	while(1)
	{
	    int i;
	    PORTB |= (1<<4);//��PB4����Ϊ�ߵ�ƽ�������
		//��ʱ1s
		for(i=0;i<100;i++)
		_delay_ms(10);//��ʱ10ms

		PORTB &= ~(1<<4);//��PB4����Ϊ�͵�ƽ�������
		//��ʱ1s
		for(i=0;i<100;i++)
	    _delay_ms(10);//��ʱ10ms
	}
}
