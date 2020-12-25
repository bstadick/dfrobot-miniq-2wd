#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>

#define FORW 1//ǰ��
#define BACK 0//����

int count= 0;

//pwm��ʼ���Ӻ���
void pwm_init(void)
{ 
	TCCR0A = 0XA3;      
	TCCR0B = 0X03;//ʱ��Դ����Ϊclk/64
	TCNT0 = 0X00; //��ֵ����Ϊ0
	TIMSK0 = 0X00;//��ֹ�ж�    
}
//���Ƶ��ת��
void Motor_Control(int M1_DIR,int M1_EN,int M2_DIR,int M2_EN)
{
  //////////M1////////////////////////
  if(M1_DIR==FORW)//M1�������  
    PORTD |=(1<<7);//������ǰ
  else
    PORTD &=~(1<<7);//�������   
  if(M1_EN==0)//M1����ٶ� 
	OCR0A = 0;//ֹͣ
  else
    OCR0A = M1_EN;//���ø�������ֵ

  ///////////M2//////////////////////
  if(M2_DIR==FORW)//M2����ķ���  
    PORTD |=(1<<4);//������ǰ 
  else
    PORTD &=~(1<<4);//�������   
  if(M2_EN==0) //M2������ٶ�
    OCR0B = 0;//ֹͣ
  else
    OCR0B = M2_EN;//���ø�������ֵ
}
//����ⷢ��ܷ���Ƶ��Ϊ38khz����
void L_Send(void)
{
    int i;
	for(i=0;i<24;i++)
	{
    PORTB &=~(1<<1);//�õͣ���ͨ
	_delay_us(12);//��ʱ
    PORTB |=(1<<1);//�øߣ���ֹ
	_delay_us(12);//��ʱ
	}
}
//�Һ��ⷢ��ܷ���Ƶ��Ϊ38khz����
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
//���ű仯�жϳ�ʼ������
void pcint0_init(void)
{
	PCICR = 0X01;//ʹ�ܵ�0�����ű仯�ж�
	PCMSK0 = 0X01;//ʹ�ܵ�0��ĵ�0�����ű仯�ж�
}
//���ű仯�жϺ���

ISR(PCINT0_vect)
{
	count++;//�Խ��յ����������
}
//���Ϲ����Ӻ���
void Obstacle_Avoidance(void)
{
   char i,j;
   count=0;//��������
   for(i=0;i<20;i++)//��ܷ���20�����壬����40���ж�
   {
     L_Send();
     for(j=0;j<54;j++)
	 _delay_us(10);
   }
   if(count>20)//���յ�����20���ж�
   {
     Motor_Control(BACK,100,BACK,100);//����
	 for(j=0;j<50;j++)//��ʱ500ms
	 _delay_ms(10);
     Motor_Control(BACK,100,FORW,100);//��ת
	 for(j=0;j<50;j++)//��ʱ500ms
	 _delay_ms(10);
   }
   else
   {
	   count=0;//����
	   for(i=0;i<20;i++)//�ҷ���ܷ���20�����壬����40���ж�
	   {
	     R_Send();
	     for(j=0;j<54;j++)
		 _delay_us(10);
	   }
	   if(count>20)//���յ�����20���ж�
	   {
	     Motor_Control(BACK,100,BACK,100);//����
		 for(j=0;j<50;j++)//��ʱ
		 _delay_ms(10);
	     Motor_Control(FORW,100,BACK,100);//��ת
		 for(j=0;j<50;j++)//��ʱ
		 _delay_ms(10);
	   }
	   else
	   Motor_Control(FORW,100,FORW,100);//ǰ��
   }
}
//������
int main(void)
{

	DDRD = 0XF2;//PORTD����ģʽ����
	DDRB = 0XFE;//PORTB����ģʽ����
	//�����������ŵ�����������Ч
	PORTD |= ((1 << 0) | (1 << 2) | (1 << 3));
	PORTB |= (1 << 0);
	//���������ⷢ��ܸߵ�ƽ������ܽ�ֹ
	PORTB |=(1<<1);
    PORTB |=(1<<2);
	pwm_init();//pwm�����ʼ��
	pcint0_init();//�ⲿ�жϳ�ʼ��
	sei();//ȫ���ж�ʹ��
	while (1)
	{
	    Obstacle_Avoidance();//�����Ӻ���
	}
}
