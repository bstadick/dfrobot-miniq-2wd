#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>

#define FORW 1//ǰ��
#define BACK 0//����
#define Vr   5//�ο���ѹֵ5v

float  data[8]={0X00,0X00,0X00,0X00,0x00,0xff,0x00,0x00};//�洢8��ͨ��adת����ֵ
float  data_;
char   channel=0x40;//adת��ͨ����ѡ��
char   key_1 = 0,key_2=0,key_3=0;//�������µĴ�������
char   value[5]={0X00,0X00,0X00,0X00,0x00};//���⴫��������ĵ�ѹֵ
int    count=0;
int    count_r=0,count_l=0;//�������ַ��ص�������м���
int    Pulse_Width=0;//�洢���� 
int    ir_code=0x00;   //����ֵ
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
	ADCSRA =0xcb; 
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
    adc_init(channel);//���³�ʼ��
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
     if(key_2==1||key_3==3)//������Ϊ����С����Ϊ
	 {
     Motor_Control(BACK,100,BACK,100);//����
	 for(j=0;j<50;j++)//��ʱ500ms
	 _delay_ms(10);
     Motor_Control(BACK,100,FORW,100);//��ת
	 for(j=0;j<50;j++)//��ʱ500ms
	 _delay_ms(10);
	 }
	 if(key_3==1)//Ϯ����ģʽ
     {
      Motor_Control(FORW,100,FORW,100);//ǰ��
	  for(j=0;j<30;j++)//��ʱ
      _delay_ms(10);//��ʱ
     }
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
	     if(key_2==1||key_3==3)//������Ϊ����С����Ϊ
		 {
	     Motor_Control(BACK,100,BACK,100);//����
		 for(j=0;j<50;j++)//��ʱ
		 _delay_ms(10);
	     Motor_Control(FORW,100,BACK,100);//��ת
		 for(j=0;j<50;j++)//��ʱ
		 _delay_ms(10);
		 }
		 if(key_3==1)//Ϯ����ģʽ
	     {
	      Motor_Control(FORW,100,FORW,100);//ǰ��
		  for(j=0;j<30;j++)//��ʱ
	      _delay_ms(10);//��ʱ
	     }
	   }
	   else
	   if(key_2==1||key_3==3)//����ģʽ����С��ģʽ
       Motor_Control(FORW,100,FORW,100);//ǰ��
       if(key_3==1)//Ϯ����ģʽ
       Motor_Control(FORW,0,FORW,0);//ǰ��
   }
}
//Ѱ�ߺ���
void huntline_deal(void)
{
    if(data[0]>(value[0]-1)&&data[1]>(value[1]-1)&&data[2]<(value[2]-1)&&data[3]>(value[3]-1)&&data[7]>(value[4]-1))
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
//Ѱ�⺯��
void hunt_light(void)
{

	if (data[4]> 1 && data[4]< 2) 
	{
	     Motor_Control(BACK,100,FORW,100);//��ת
	}
	else if (data[4]> 4 && data[4]< 5)
	{
	    Motor_Control(FORW,100,BACK,100);//��ת
	}
	else
	{
		Motor_Control(FORW,0,FORW,0);//ֹͣ
	}

}
//ң��

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

char logic_value()//�߼���0���͡�1���жϺ���
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

void pulse_deal()//���յ�ַ���������
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
void remote_decode(void)//���뺯��
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
       _delay_ms(5);//��ʱ����
      if(data[5]>=0.00&&data[5]<0.50)//����1ȷʵ����
      {
        buzzer();//��������
		key_1++;//����1����
        if(key_1>=1&&key_1<=5) 
		value_adjust(key_1);//Ѱ�ߴ�������ֵ����
      }
    }
    else if(data[5]>=0.50&&data[5]<2.00)
    {
       _delay_ms(5);//��ʱ����
      if(data[5]>=0.50&&data[5]<2.00)
      {
	     buzzer();//��������
		 if(key_1>=1&&key_1<=5)
        {
          value[key_1-1]++;//�������ķֱ�켣�Ľ���ֵ�Ӽӣ�������
          value_adjust(key_1);//��ʵ��ֵ�Ա�
        }
     	else
		key_2++;//key2����
      }
    }
    else if(data[5]>=2.00&&data[5]<3.00)
    {
       _delay_ms(5);//��ʱ����
      if(data[5]>=2.00&&data[5]<3.00)
      {
	     buzzer();//��������
		 if(key_1>=1&&key_1<=5)
        {
          value[key_1-1]--;//�������ķֱ�켣�Ľ���ֵ������������
          value_adjust(key_1);//��ʵ��ֵ�Ա�
        }
		else
		key_3++;//key3����
      }
    }
  }
}
//����ֵ������
void key_deal(void)
{

    if(key_1==6)//Ѱ��
	{
	   PORTB &=~(1<<5);//�̵���
       PORTB &=~(1<<4);//�����
	   huntline_deal();//Ѱ��
	}
	if(key_2==1||key_3==1||key_3==3)//key_2=1������Ϊ��key_3==1Ϯ���ߣ�key_3==3С����Ϊ
	{
	    ADCSRA=0XC3;
	    PORTB |=(1<<4);
	    pcint0_init();//���ű仯�жϳ�ʼ��
	    Obstacle_Avoidance();
        ADCSRA=0XCB;
	}
	if(key_2==3)//key_2==3Ѱ����Ϊ
	{
	   PORTB |=(1<<4);
	   hunt_light();//Ѱ��
	}  
	if(key_3==5)//ң��
    {
	    ADCSRA=0XC3;
	    PORTB |=(1<<4);
	    timer1_init();//��ʱ����ʼ������
	    remote_decode();  //����
	    remote_deal();   //ִ��������
		ADCSRA=0XCB;
    } 
	if(key_1==7||key_2==2||key_2==4||key_3==2||key_3==4||key_3==6)
	{
	   //����ֵ����
	    key_1=0;
	    if(key_2==4)
	    key_2=0;
	    if(key_3==6)
	    key_3=0;
        PORTB &=~(1<<4);
		Motor_Control(FORW,0,FORW,0);//ֹͣ
	}
}
//������
int main(void)
{
    DDRD = 0XF2;//����PORTD������
	PORTD |= ((1<<0)|(1<<2)|(1<<3));//����PORTD���������ŵ�����������Ч
	DDRB = 0XFE;//����PORTB������
	PORTB |= (1<<0);//����PORTB���������ŵ�����������Ч
    pwm_init();//PWM�����ʼ��
	adc_init(channel);//adת����ʼ��
	sei();//ȫ���ж�����
	while(1)
	{
	   key_scan();//ɨ�谴��
	   key_deal();//��������
    }
}
