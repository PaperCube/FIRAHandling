#include "InitFIRA.h"
const int d1=A1;
const int d2=A2;
unsigned long duration;
int f=0;
int dist;
int dist2;
unsigned long time1;
unsigned long time2;
bool fs=false;
void setup()
{
  InitFIRA(1, 0);
  Serial.begin(9600);

}

void loop()
{
  dispscreen();
  //等待按钮被按下
  while(1)
  {
    if(GetKey(1)==1)
    {
      duration=10;
      f=1;
      break;
    }
    if(GetKey(2)==1)
    {
      duration=30;
      f=2;
      break;
    }
    if(GetKey(3)==1)
    {
      duration=50;
      f=3;
      break;
    }
    if(GetKey(4)==1)
    {
      duration=60;
      f=4;
      break;
    }
  }
  //提示计时开始
  sound01();
  //计时函数；
  GLCD.clrScr();
  durxx();
}
void testR()
{
   //检测眼睛是否距离书本太近，太近则蜂鸣器长鸣
   dist2=pow(3027.4/analogRead(d2),1.2134);
   if(dist2<50)
    {
        BuzzerOn();
    }
    else
    {
      BuzzerOff();
    }
}
void durxx()
{
   //获取当前时间
   time1=millis(); 
   //GLCD.print(String(duration),100,20,90);
   duration=duration*60*1000;
   //开始计时
   while((millis()-time1)<duration)
   {
     testR();
     //显示倒计时；
     //GLCD.print(String(t),80,20,90);
     //Serial.println(t);
     GLCD.setFont(BigFont);
     unsigned long timex=(duration-(millis()-time1))/1000;
     int timeM=timex/60;
     int timeS=timex-timeM*60;
     if(timeM>=10)
        GLCD.print(String(timeM),80,20,90);
     else
     {
        GLCD.print("0",80,20,90);
        GLCD.print(String(timeM),80,36,90);
     }
     GLCD.print(":",80,52,90);
     if(timeS>=10)
        GLCD.print(String(timeS),80,68,90);
     else
     {
        GLCD.print("0",80,68,90);
        GLCD.print(String(timeS),80,84,90);
     } 
     
     
     //如果中途离开，计时结束
     dist=pow(3027.4/analogRead(d1),1.2134);  
     //Serial.println(dist);   
     if(dist>55)
     {
        break;
     }
   }
   //提示休息时间到。
   sound00();
   delay(1000);
}
//屏显内容
//按钮1，10分钟计时
//按钮2，30分钟计时
//按钮3，50分钟计时
//按纽斯，60分钟计时
void dispscreen()
{
     GLCD.clrScr();
     GLCD.setColor(255,0,0);
     GLCD.setFont(SmallFont);
     GLCD.print("Choose duration :",110,5,90);
     GLCD.print(" 1 10Minutes ",90,30,90);
     GLCD.print(" 2 30Minutes",70,30,90);
     GLCD.print(" 3 50Minutes ",50,30,90);
     GLCD.print(" 4 60Minutes",30,30,90);    
}

void sound01()
{//开始计时的声音。 
     BuzzerOn();
     delay(200);
     BuzzerOff();
     delay(100);
     BuzzerOn();
     delay(200);
     BuzzerOff();
     delay(100);  
}
void sound00()
{//结束计时的声音
  for(int i=0;i<4;i++)
  {
     BuzzerOn();
    delay(300);
    BuzzerOff();
    delay(100);
  }
}
