#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>

#define Vr  5//adת���ο���ѹֵ5v

float data=0xff;

//��������������
void buzzer(void)
{
     char k=0;
	 for(k=0;k<5;k++)
	 {
	     PORTB |= (1<<3);
		 _delay_us(10);
		 PORTB &= ~(1<<3);
		 _delay_us(40);//��������
	 }
}
//ADת����ʼ��
void adc_init(void)
{
    //������Ӧ�Ĵ���
	ADMUX =0x45;
	ADCSRA = 0Xcb;
}
//adת���жϺ���
ISR(ADC_vect)
{
   
   ADCSRA &=~ (1<<ADEN);    //���ж�
   data = ADCL;            //���յͰ�λֵ
   data  = data+ADCH*256;  //����adc������ֵ
   data  = ((data*Vr)/1024);//ת����ģ��ֵ
   adc_init();//���³�ʼ��
}
//����ɨ��
void key_scan(void)
{
  if(data>4.50&&data<6.00)//�ް�������
     return;//����
  else
  {
    if(data>=0.00&&data<0.50)//����1����
    { 
      _delay_ms(5);//��ʱ����
      if(data>=0.00&&data<0.50)//����1ȷʵ����
      {
        buzzer();//��������
        PORTB |=(1<<4);//�����
        PORTB &=~(1<<5);//�̵���
	  }
    }
    else if(data>=0.50&&data<1.5)//����2����
    {
       _delay_ms(5);//��ʱ����
      if(data>=0.50&&data<1.5)//����2ȷʵ����
      {
        buzzer();//��������
        PORTB |=(1<<5);//�̵���
        PORTB &=~(1<<4);//�����
      }
    }
    else if(data>=1.5&&data<2.5)//����3����
    {
      _delay_ms(5);//��ʱ
      if(data>=1.5&&data<2.5)//����3ȷʵ����
      {
        buzzer();//��������
        PORTB &=~(1<<5);//�̵���
        PORTB &=~(1<<4);//�����
      }
    }
  }
}
int main(void)
{
   DDRB =0XFE;//�趨PB4��PB5����Ϊ���ģʽ
   adc_init();//AD��ʼ��
   sei();//ȫ���ж�����
   while(1)
   {
     key_scan();//����ɨ��
   }
}
