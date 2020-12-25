#include<avr/io.h>
#include<util/delay.h>	

#define FORW 1//ǰ��
#define BACK 0//����


//pwm��ʼ������
void pwm_init(void)
{ 
	TCCR0A = 0XA3;//����PWMģʽ      
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
//������
int main(void)
{
	DDRD |=(1<<4)|(1<<5)|(1<<6)|(1<<7);//����PORTD�˿ڵ�4��5��6��7����Ϊ���ģʽ
	pwm_init();//pwm��ʼ��
	while(1)
	{
	   int i;
	   Motor_Control(FORW,80,FORW,80);//ǰ��
       for(i=0;i<50;i++)//��ʱ
	   _delay_ms(10);
       Motor_Control(BACK,0,BACK,0);//ֹͣ
       for(i=0;i<50;i++)//��ʱ
	   _delay_ms(10);
       Motor_Control(BACK,80,BACK,80);//����
	   for(i=0;i<50;i++)//��ʱ
	   _delay_ms(10);
       Motor_Control(BACK,0,BACK,0);//ֹͣ
       for(i=0;i<50;i++)//��ʱ
	   _delay_ms(10);
	}
}
