#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>


//      ����     ��ʱֵ    Ƶ��ֵhz
#define DO_L     E2       //262
#define DOA_L    E4       //277
#define RE_L     E5       //294
#define REA_L    E7       //311
#define MI_L     E8       //330
#define FA_L     EA       //349
#define FAA_L    EB       //370
#define SO_L     EC       //392
#define SOA_L    ED       //415
#define LA_L     EE       //440
#define LAA_L    EF       //466
#define TI_L     F0       //494
#define DO       F1       //523
#define DOA      F2       //554
#define RE       F3       //587
#define REA      F3       //622
#define MI       F4       //659
#define FA       F5       //698
#define FAA      F5       //740
#define SO       F6       //784
#define SOA      F7       //831
#define LA       F7       //880
#define LAA      F8       //932
#define TI       F8       //988
#define DO_H     F9       //1046
#define DOA_H    F9       //1109
#define RE_H     F9       //1175
#define REA_H    FA       //1245
#define MI_H     FA       //1318
#define FA_H     FA       //1397
#define FAA_H    FB       //1480
#define SO_H     FB       //1568
#define SOA_H    FB       //1661
#define LA_H     FC       //1760
#define LAA_H    FC       //1865
#define TI_H     FC       //1976
#define ZERO     0        //��ֹ��



int initial_value;  //��ʱ���ĳ�ֵ
int ptr = 0x00;     //ָ��music�����ָ��
 
int music[] = { 0XF1, 2, 0XF3, 2, 0XF4, 2, 0XF1, 1, 0, 1,   //��ֻ�ϻ�
	            0XF1, 2, 0XF3, 2, 0XF4, 2, 0XF1, 1, 0, 1,
	            0XF4, 2, 0XF5, 2, 0XF6, 2, 0, 2,
	            0XF4, 2, 0XF5, 2, 0XF6, 2, 0, 2,
	            0XF6, 1, 0XF7, 1, 0XF6, 1, 0XF5, 1, 0XF4, 2, 0XF1, 2,
	            0XF6, 1, 0XF7, 1, 0XF6, 1, 0XF5, 1, 0XF4, 2, 0XF1, 2,
	            0XF3, 2, 0XEC, 2, 0XF1, 2, 0, 2,
	            0XF3, 2, 0XEC, 2, 0XF1, 2, 0, 2,
	            0xff};

//��ʱ��2��ʼ������
void timer2_init(void)
{
	TCCR2A = 0X00;
	TCCR2B = 0X07; //ʱ��Դ
	TCNT2 = initial_value;
	TIMSK2 = 0X01; //�����ж�
}
//��ʱ��/������2�жϺ���
ISR(TIMER2_OVF_vect)
{
	TCNT2 = initial_value;//����ʱ����ʱ��ֵ
	PORTB ^= (1 << 3);    
}
//�������ֺ���
void play_music(void)
{
	char time=0;
	int  i;
	if (music[ptr] != 0xFF && music[ptr] != 0x00) //�ж��Ƿ�����������
	{

		TCCR2B = 0X07;//��ʱ����ʼ����
		initial_value = music[ptr]; //ȡ����Ƶ����ֵ
		time = music[ptr + 1]; //ȡ����ʱ��
		for(i=0;i<time*15;i++)
		_delay_ms(10);//��ʱ
		ptr += 2;//ָ����һ������
	}
	else if (music[ptr] == 0x00) //�ж��Ƿ�����ֹ��
	{
		time = music[ptr + 1];
		for(i=0;i<time*15;i++)
		_delay_ms(10);//��ʱ
		ptr += 2;
	}
	else //������,ֹͣ��ʱ�����
	{
		TCCR2B = 0X00;//��ʱ��ֹͣ����
		PORTB |= (1 << 3);
		for(i=0;i<time*15;i++)
		_delay_ms(10);//��ʱ
		ptr = 0;
	}
}
//������
int main(void)
{
	DDRB = 0XFE;//��PORTB�˿ڸ����ŵ�ģʽ��������
	PORTB |= (1 << 0);//����PB0���ŵ���������
	timer2_init();//��ʱ��2��ʼ��
	sei();//����ȫ���ж�����
	while (1)
	{
		play_music();//��������
	}

}
