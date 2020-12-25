#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#include<avr/eeprom.h>

#define FORW 1//ǰ��
#define BACK 0//����	
#define Vr   5//adת���ο���ѹֵ5v

float  data[8]={0X00,0X00,0X00,0X00,0x00,0xff,0x00,0x00};//�洢8��ͨ��adת����ֵ
float  data_;        //�洢����������adת����ֵ
char   channel=0x40; //adת��ͨ����ѡ��
char   key_1 = 0,key_2=0,key_3=0; //�������µĴ�������
char   value[5]={0X00,0X00,0X00,0X00,0x00}; //���⴫��������ĵ�ѹֵ
unsigned char *p;

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
	ADMUX =channel;
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
//Ѱ�ߺ���
void huntline_deal(void)
{
    if(key_2==1)
	{
		int i;
		*p=0x0a;
	    //��ȡ����EEPROM�еĵ�ѹֵ
	    for(i=0;i<5;i++)
		{
			eeprom_busy_wait();
		    value[i]=eeprom_read_byte(p+i);
		}
	    if(data[0]>(value[0]-1)&&data[1]>(value[1]-1)&&data[2]<(value[2]-1)&&data[3]>(value[3]-1)&&data[7]>(value[4]-1))//��һ��ʵ��ֵ
		{
			Motor_Control(FORW,100,FORW,100);//ǰ��
		} 
		else if(data[0]>(value[0]-1)&&data[1]>(value[1]-1)&&data[2]<(value[2]-1)&&data[3]<(value[3]-1)&&data[7]>(value[4]-1))
		{
			Motor_Control(BACK,0,FORW,100);//��ת
		}
		else if(data[0]>(value[0]-1)&&data[1]>(value[1]-1)&&data[2]>(value[2]-1)&&data[3]<(value[3]-1)&&data[7]>(value[4]-1))
		{
			Motor_Control(BACK,0,FORW,100);//��ת
		}
		else if(data[0]>(value[0]-1)&&data[1]>(value[1]-1)&&data[2]>(value[2]-1)&&data[3]<(value[3]-1)&&data[7]<(value[4]-1))
		{
			Motor_Control(BACK,100,FORW,100);//��ת
		}
		else if(data[0]>(value[0]-1)&&data[1]>(value[1]-1)&&data[2]>(value[2]-1)&&data[3]>(value[3]-1)&&data[7]<(value[4]-1))
		{
			Motor_Control(BACK,100,FORW,100);//��ת
		}
		else if(data[0]>(value[0]-1)&&data[1]<(value[1]-1)&&data[2]<(value[2]-1)&&data[3]>(value[3]-1)&&data[7]>(value[4]-1))
		{
			Motor_Control(FORW,100,BACK,0);//��ת
		}
		else if(data[0]>(value[0]-1)&&data[1]<(value[1]-1)&&data[2]>(value[2]-1)&&data[3]>(value[3]-1)&&data[7]>(value[4]-1))
		{
			Motor_Control(FORW,100,BACK,0);//��ת
		}
		else if(data[0]<(value[0]-1)&&data[1]<(value[1]-1)&&data[2]>(value[2]-1)&&data[3]>(value[3]-1)&&data[7]>(value[4]-1))
		{
			Motor_Control(FORW,100,BACK,100);//��ת
		}
		else if(data[0]<(value[0]-1)&&data[1]>(value[1]-1)&&data[2]>(value[2]-1)&&data[3]>(value[3]-1)&&data[7]>(value[4]-1))
		{
			Motor_Control(FORW,100,BACK,100);//��ת
		}
	}
}
//��ѹֵ��������
void value_adjust(char num)
{
     if(num==1)
	 {
	    if(data[0]>value[0])
        {
		   PORTB |=(1<<4); //��ǰֵС�����
		   PORTB &=~(1<<5);
        }
        else
        {
		   PORTB |=(1<<5); //��ǰֵ���̵���
		   PORTB &=~(1<<4);
        } 
	 }
	 if(num==2)
	 {
	    if(data[1]>value[1])
        {
		   PORTB |=(1<<4); //��ǰֵС�����
		   PORTB &=~(1<<5);
        }
        else
        {
		   PORTB |=(1<<5); //��ǰֵ���̵���
		   PORTB &=~(1<<4);
        } 
	 }
	 if(num==3)
	 {
	    if(data[2]>value[2])
        {
		   PORTB |=(1<<4); //��ǰֵС�����
		   PORTB &=~(1<<5);
        }
        else
        {
		   PORTB |=(1<<5); //��ǰֵ���̵���
		   PORTB &=~(1<<4);
        } 
	 }
	 if(num==4)
	 {
	    if(data[3]>value[3])
        {
		   PORTB |=(1<<4); //��ǰֵС�����
		   PORTB &=~(1<<5);
        }
        else
        {
		   PORTB |=(1<<5); //��ǰֵ���̵���
		   PORTB &=~(1<<4);
        } 
	 }
	 if(num==5)
	 {
	    if(data[4]>value[4])
        {
		   PORTB |=(1<<4); //��ǰֵС�����
		   PORTB &=~(1<<5);
        }
        else
        {
		   PORTB |=(1<<5); //��ǰֵ���̵���
		   PORTB &=~(1<<4);
        } 
	 }
}
//������
void buzzer(void)
{
     char k=0;
	 for(k=0;k<5;k++)
	 {
	     PORTB |= (1<<3);
		 _delay_us(10);
		 PORTB &= ~(1<<3);
		 _delay_us(40);
	 }
}
//����ɨ��
void key_scan(void)//����ɨ��
{
  int i;
  *p=0X0A;
  if(data[5]>4.50&&data[5]<6.00)//û�а�������
    return;//����
  else
  {
    if(data[5]>=0.00&&data[5]<0.50)//����1����
    {
       _delay_ms(5);//��ʱ����
      if(data[5]>=0.00&&data[5]<0.50)//����1ȷʵ����
      {
        buzzer();//��������
        key_1++;//����1����
        if(key_1>=1&&key_1<=5) value_adjust(key_1);//Ѱ�ߴ�������ֵ����
        if(key_1==6)
        {
          PORTB &=~(1<<5);//�̵���
          PORTB &=~(1<<4);//�����
          //�����úõĵ�ѹֵ����EEPROM���������籣��
          for(i=0;i<5;i++)
		  {
            eeprom_busy_wait();
		    eeprom_write_byte(p+i,value[i]);
		  }
        }
      }
    }
    else if(data[5]>=0.50&&data[5]<1.5)
    {
       _delay_ms(5);//��ʱ����
      if(data[5]>=0.50&&data[5]<1.5)
      {
	    buzzer();
        if(key_1>=1&&key_1<=5)
        {
          value[key_1-1]++;//�������ķֱ�켣�Ľ���ֵ�Ӽӣ�������
          value_adjust(key_1);//��ʵ��ֵ�Ա�
        }
		else
		key_2++;
      }
    }
    else if(data[5]>=1.5&&data[5]<2.5)
    {
       _delay_ms(5);//��ʱ����
      if(data[5]>=1.5&&data[5]<2.5)
      {
	    buzzer();
        if(key_1>=1&&key_1<=5)
        {
          value[key_1-1]--;//�������ķֱ�켣�Ľ���ֵ������������
          value_adjust(key_1);//��ʵ��ֵ�Ա�
        }
      }
    }
  }
}

//������
int main(void)
{
    DDRD = 0XF2;//��������
	DDRB = 0XFE;
	PORTD |=((1<<0)|(1<<2)|(1<<3));//�����������ŵ�����������Ч
	pwm_init();//PWM�����ʼ��
	adc_init(channel);//ad��ʼ��
	sei();
	while(1)
	{
	   key_scan();//����ɨ��
	   huntline_deal();//Ѱ��
	}
	
}
