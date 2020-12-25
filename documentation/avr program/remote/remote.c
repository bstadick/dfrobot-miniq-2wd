#include<avr/io.h>
#include<util/delay.h>

#define FORW 1//ǰ��
#define BACK 0//����

int  Pulse_Width=0;//�洢���� 
int  ir_code=0x00;   //����ֵ

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
//���ö�ʱ��T1Ϊ��ʱģʽ
void timer1_init(void)
{
  TCCR1A = 0X00; 
  TCCR1B = 0X05;//����ʱ��ʱ��Դ  
  TCCR1C = 0X00;
  TCNT1 = 0X00;
  TIMSK1 = 0X00;//��ֹ��ʱ������ж�
}
//ִ��������
void remote_deal(void)
{ 
  switch(ir_code)
  {
    case 0xff00://ֹͣ
       Motor_Control(FORW,0,FORW,0);
      break;
    case 0xfe01://ǰ��
	   Motor_Control(FORW,100,FORW,100);
       break;
    case 0xf609://����
	   Motor_Control(BACK,100,BACK,100);
       break;
    case 0xfb04://��ת
	   Motor_Control(FORW,100,BACK,100);
       break;
    case 0xf906://��ת
      Motor_Control(BACK,100,FORW,100);
      break;
  }
}
//�߼���0���͡�1���жϺ���
char logic_value()
{
    while(!(PINB&0X01)); //�͵ȴ�
     Pulse_Width=TCNT1;
     TCNT1=0;
     if(Pulse_Width>=7&&Pulse_Width<=10)//�͵�ƽ560us
     {
         while(PINB&0X01);//�Ǹ߾͵ȴ�
         Pulse_Width=TCNT1;
         TCNT1=0;
         if(Pulse_Width>=7&&Pulse_Width<=10)//���Ÿߵ�ƽ560us
           return 0;
         else if(Pulse_Width>=25&&Pulse_Width<=27) //���Ÿߵ�ƽ1.7ms
           return 1;
     }
     return -1;
}
//���յ�ַ���������
void pulse_deal()
{
  int i;
  //��ַ��
  //ִ��8��0
  for(i=0; i<8; i++)
  {
    if(logic_value() != 0) //����0
      return;
  }
  //��ַ����
  //ִ��6��1
  for(i=0; i<6; i++)
  {
     if(logic_value()!= 1) //����1
      return;
  }
  //ִ��1��0
  if(logic_value()!= 0) //����0
      return;
  //ִ��1��1
  if(logic_value()!= 1) //����1
      return;
      
  
  //����ң���������е�commandָ��
  ir_code=0x00;//����
  for(i=0; i<16;i++ )
  {
    if(logic_value() == 1)
    {
      ir_code |=(1<<i);
    }
  }
}
//���뺯��
void remote_decode(void)
{
     TCNT1=0X00;       
     while(PINB&0X01)//�Ǹ߾͵ȴ�
     {
        if(TCNT1>=1563)  //���ߵ�ƽ����ʱ�䳬��100ms��������ʱû�а�������
        {
             ir_code = 0xff00;
             return;
        }  
     }
     
     //����ߵ�ƽ����ʱ�䲻����100ms
     TCNT1=0X00;
     
     while(!(PINB&0X01)); //�͵ȴ�
     Pulse_Width=TCNT1;
     TCNT1=0;
     if(Pulse_Width>=140&&Pulse_Width<=141)//9ms
     {
         
         while(PINB&0X01);//�Ǹ߾͵ȴ�
         Pulse_Width=TCNT1;
         TCNT1=0;
         if(Pulse_Width>=68&&Pulse_Width<=72)//4.5ms
         {
            pulse_deal();
            return;
         }
         else if(Pulse_Width>=34&&Pulse_Width<=36)//2.25ms
         {
            while(!(PINB&0X01));//�͵ȴ�
            Pulse_Width=TCNT1;
            TCNT1=0;
            if(Pulse_Width>=7&&Pulse_Width<=10)//560us
            {
               return; 
            }
         }
     }

}
//������
int main(void)
{

	DDRD = 0XF2; //��������
	DDRB = 0XFE;
	PORTD |= ((1 << 0) | (1 << 2) | (1 << 3)); //�����������ŵ�����������Ч
	PORTB |= (1 << 0);
	pwm_init();
	timer1_init();
	while (1)
	{  
	  remote_decode();  //����
      remote_deal();   //ִ��������
	}

}
