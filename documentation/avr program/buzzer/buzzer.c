#include<avr/io.h>
#include<util/delay.h>


int main(void)
{
    char i;
    DDRB = (1<<3);//����PORTB�˿ڵ�PB3����Ϊ���ģʽ

	while(1)
	{
		//���һ��Ƶ�ʵ�����
	    for(i=0;i<80;i++)
	    {
	      PORTB |=(1<<3);//����PB3����Ϊ�ߵ�ƽ����������
	      _delay_ms(1);//��ʱ
	      PORTB &= ~(1<<3);//����PB3����Ϊ�͵�ƽ������������
	      _delay_ms(1);//��ʱ
	    }
	    //�����һ��Ƶ�ʵ�����
	    for(i=0;i<100;i++)
	    {
	      PORTB = (1<<3);//����PB3����Ϊ�ߵ�ƽ����������
	      _delay_ms(2);//��ʱ
	      PORTB &= ~(1<<3);//����PB3����Ϊ�͵�ƽ������������
	      _delay_ms(2);//��ʱ
	    }
	}
}
