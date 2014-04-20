
//      音符     计时值    频率值hz
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
#define ZERO     0        //休止符

#define BUZZER 11//定义蜂鸣器的引脚

int    initial_value; //发出每个音符时定时器的初值
char   time;//每个音符演奏的时间
char   ptr = 0x00;//指向music数组的
char   flag=0;
int    music[] = { 
  0XF1, 2, 0XF3, 2, 0XF4, 2, 0XF1, 1, 0, 1,   //两只老虎
  0XF1, 2, 0XF3, 2, 0XF4, 2, 0XF1, 1, 0, 1,
  0XF4, 2, 0XF5, 2, 0XF6, 2, 0, 2,
  0XF4, 2, 0XF5, 2, 0XF6, 2, 0, 2,
  0XF6, 1, 0XF7, 1, 0XF6, 1, 0XF5, 1, 0XF4, 2, 0XF1, 2,
  0XF6, 1, 0XF7, 1, 0XF6, 1, 0XF5, 1, 0XF4, 2, 0XF1, 2,
  0XF3, 2, 0XEC, 2, 0XF1, 2, 0, 2,
  0XF3, 2, 0XEC, 2, 0XF1, 2, 0, 2,0xff};

void timer2_init(void)//定时器2初始化
{
  TCCR2A = 0X00;
  TCCR2B = 0X07; //时钟源1024分频
  TCNT2 = initial_value;
  TIMSK2 = 0X01; //允许中断
}
ISR(TIMER2_OVF_vect)//定时器2中断
{
  TCNT2 = initial_value;//给定时器计时初值
  flag=~flag;
  if(flag)
     digitalWrite(BUZZER,HIGH);//置高，蜂鸣器响
  else
    digitalWrite(BUZZER,LOW);//置低，蜂鸣器不响
}
void play_music(void)//播放音乐
{
  if (music[ptr] != 0xFF && music[ptr] != 0x00) //判断是否是正常音符
  {
    TCCR2B = 0X07;//定时器开始工作
    initial_value = music[ptr]; //取设置定时器初值
    time = music[ptr + 1]; //取发声时间
    delay(time*200);//延时
    ptr += 2;//指向下一个音符
  }
  else if (music[ptr] == 0x00) //判断是否是休止符
  {
    time = music[ptr + 1];//取发声时间
    delay(time*200);//延时
    ptr += 2;//指向下一个音符
  }
  else//是结束符
  {
    TCCR2B = 0X00;//定时器停止工作
    digitalWrite(BUZZER,LOW);//置低，蜂鸣器不响
    delay(time*200);//延时
    ptr = 0;//清零，便于重新开始
  }
}
void setup()
{
   pinMode(BUZZER,OUTPUT);//设置连接蜂鸣器的引脚为输出模式
   timer2_init();//定时器初始化
   sei();//全局中断开启
}
void loop(void)
{
  while (1)
  {
    play_music();//播放音乐
  }

}


