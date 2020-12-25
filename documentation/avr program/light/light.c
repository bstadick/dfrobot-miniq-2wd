#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#include<avr/eeprom.h>

#define FORW 1//ǰ��
#define BACK 0//����
#define Vr   5//�ο���ѹֵ

float data[8]={0X00,0X00,0X00,0X00,0x00,0xff,0x00,0x00};//�洢8��ͨ��adת����ֵ
float data_;
char  channel=0x40;//adת��ͨ����ѡ��
float value[10]={0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
float val;
unsigned char sum=0;
unsigned char *p;
int   key_1=0,key_2=0,key_3=0;

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
//adת����ʼ������
void adc_init(int channel)
{
    //����adת���Ĵ���
	ADMUX = channel;
	ADCSRA = 0Xcb;
}
//adת���жϺ���
ISR(ADC_vect)
{
   ADCSRA &=~ (1<<ADEN);      //���ж�
   data_ = ADCL;               //���յڰ�λֵ
   data_  = data_+ADCH*256;    //����adc������ֵ
   data_  = ((data_*Vr)/1024); //ת����ģ��ֵ
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
   if(++channel==0x48)//��ͨ��
   	{
   		  channel = 0x40;
   	}
    adc_init(channel);
}
//Ѱ�⺯��
void hunt_light(void)
{
  int i;
  *p=0x0a;
  if(key_1==1)
  {
    key_1=0;
    //��ȡ10�λ�����ĵ�ѹֵ�������
    for(i=0;i<10;i++)
    {
      value[i]=data[4];
      sum +=value[i];
    }
    //���������ѹֵ�ĺʹ����EEPROM�����������Ա������ݲ���ʧ
    eeprom_busy_wait();
    eeprom_write_byte(p,sum);	
  }
  else
  {
    eeprom_busy_wait();
    val=eeprom_read_byte(p);//��ȡ��ѹֵ�ĺ�
    val=val/10;//��ƽ��ֵ
    if (data[4]<val-0.5)   //����ʵ�ʵ�ʵ�黷�����в���
      Motor_Control(FORW,100,BACK,100);//��ת
    else if (data[4]>val+1)
      Motor_Control(BACK,100,FORW,100);//��ת
    else
      Motor_Control(FORW,0,FORW,0);//ֹͣ
  }
}
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
//����ɨ��
void key_scan(void)
{
  if(data[5]>4.50&&data[5]<6.00)//û�а�������
    return;
  else
  {
    if(data[5]>=0.00&&data[5]<0.50)//����1����
    {
       _delay_ms(10);//��ʱ����
      if(data[5]>=0.00&&data[5]<0.50)//����1ȷʵ����
      {
        buzzer();//��������
		key_1++;//����1����                         
      }
    }
    else if(data[5]>=0.50&&data[5]<1.5)
    {
       _delay_ms(5);//��ʱ����
      if(data[5]>=0.50&&data[5]<1.5)
      {
	     buzzer();//��������
		 key_2++;//key2����
      }
    }
    else if(data[5]>=1.5&&data[5]<2.5)
    {
       _delay_ms(5);//��ʱ����
      if(data[5]>=1.5&&data[5]<2.5)
      {
	     buzzer();//��������
		 key_3++;//key3����
      }
    }
  }
}
//������
int main(void)
{
	DDRD = 0XF2;//��������
	DDRB =0XFE;
	PORTD |= ((1 << 0) | (1 << 2) | (1 << 3));//�����������ŵ�����������Ч
	pwm_init();//pwm��ʼ��
	adc_init(channel);//ad��ʼ��
	sei();//ȫ���ж�����
	while (1)
	{
	   key_scan();//����ɨ��
	   hunt_light();//Ѱ��
	}
}
