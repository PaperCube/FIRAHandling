#include <pt.h> //ProtoThreads必须包含的头文件
int flame = A0;
int mq = A1;
int Beep = 3;
int val = 0;
int val1 = 0;
int hw = 5;
int val3;
static int counter1, counter2, counter3, state1 = 0, state2 = 0, state3 = 0; //counter为定时计数器，state为每个灯的状态
static int protothread1(struct pt *pt)                                       //线程1，控制灯1
{
    PT_BEGIN(pt); //线程开始
    while (1)     //每个线程都不会死
    {
        PT_WAIT_UNTIL(pt, counter1 == 1); //如果时间满了1秒，则继续执行，否则记录运行点，退出线程1
        val = analogRead(flame);
        Serial.print("flame=");
        Serial.println(val);
        delay(1000);
        if (val >= 500)
        {
            digitalWrite(Beep, LOW);
        }
        else
        {
            digitalWrite(Beep, HIGH);
        }
        counter1 = 0; //计数器置零
    }
    PT_END(pt); //线程结束
}

static int protothread2(struct pt *pt) //线程2，控制灯2
{
    PT_BEGIN(pt); //线程开始
    while (1)
    {                                     //每个线程都不会死
        PT_WAIT_UNTIL(pt, counter2 == 1); //如果时间满了5秒，则继续执行，否则记录运行点，退出线程2
        val1 = analogRead(mq);
        Serial.print("mq2=");
        Serial.println(val1);
        delay(1000);
        if (val1 >= 500)
        {
            digitalWrite(Beep, LOW);
        }
        else
        {
            digitalWrite(Beep, HIGH);
        }
        counter2 = 0; //计数清零
    }
    PT_END(pt); //线程结束
}
static int protothread3(struct pt *pt) //线程1，控制灯1
{
    PT_BEGIN(pt); //线程开始
    while (1)     //每个线程都不会死
    {
        PT_WAIT_UNTIL(pt, counter3 == 1); //如果时间满了1秒，则继续执行，否则记录运行点，退出线程1
        val = digitalRead(hw);            //将数字接口 7 的值读取赋给 val
        if (val == LOW)                   //当红外热释电传感器检测有信号时，LED 亮起
        {

            digitalWrite(Beep, HIGH);
        }
        else
        {
            digitalWrite(Beep, LOW);
        }
        counter3 = 0; //计数器置零
    }
    PT_END(pt); //线程结束
}
static struct pt pt1, pt2, pt3;
void setup()
{
    pinMode(Beep, OUTPUT);
    pinMode(hw, INPUT);
    Serial.begin(9600);
    PT_INIT(&pt1); //线程1初始化
    PT_INIT(&pt2); //线程2初始化
    PT_INIT(&pt3); //线程2初始化
}
void loop() //这就是进行线程调度的地方
{
    protothread2(&pt2); //执行线程2
    protothread1(&pt1); //执行线程1
    protothread3(&pt3); //执行线程1
    delay(10);          //时间片，每片1秒，可根据具体应用设置大小
    counter1++;
    counter2++;
    counter3++;
}
