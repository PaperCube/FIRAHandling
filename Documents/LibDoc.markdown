# FIRAHandler Official Documention :: <Gmotor.h> & <Grobot.h> 
---
Author VisualDust , 201907092259LastUpdate  
<font face="等线" size=5>
《FIRAHandlerG库从入门到放弃全教程》 
</font>   
* 以下是入门部分
> <Gmotor.h>的参数说明  
```cpp
int cnt1,cnt2; //链接L298N驱动芯片的两个pwm控制端口
int    speed; //电机的记忆速度
int    solid = 0; //电机在停转时是否有功率
double prop  = 1.0; //电机速度比例矫正，当出现转速过慢或两个电机转速不一时进行调整
int    direc = 0; //用来记忆电机目前的正反转情况

```
> <Gmotor.h>方法说明  
```cpp
Gmotor(int cnt1, int cnt2); //初始化方法，传入参数为链接L298N驱动芯片的两个pwm控制端口的端口号
void stop(int mode); //即刻生效的电机停止方法，传入参数停止模式，0为立即停止，非零为渐停
void setSolid(int solid); //用以更改solid参数，传入一个新值作为solid值 
void setSpeed(int speed); //设置电机速度并立刻生效
int  getSpeed(); //返回一个整数值，为电机当前功率
void setProp(double prop); //设置电机矫正值
void exchangePin(); //一个内部方法，用以交换两个控制端口。由于调试可见性为public
void gradientTo(int speed, int ms); //使电机转速从当前值耗时传入参数ms毫秒渐变到传入参数speed
void walk(int speed, int ms); //使该电机以传入参数speed的速度旋转传入参数ms毫秒
```
> <Gmotor.h>的使用样例
```cpp
Gmotor* myMotor = new Gmotor(3, 4); //使用3、4号口建立一个电机控制。注意，传入参数顺序3,4和4,3不一样，两者电机旋转方向不同，即当你发现写3,4转的不对，可以改成4,3
myMotor->exchangePin(); //如果开机后我发现这个电机旋转方向不是我想要的，调用这个方法更改方向
myMotor->setSpeed(120); //将电机速度设置为120，立即生效，电机开始疯狂旋转 
myMotor->setProp(0.8); //经历一波调试后我发现电机总体转速太高了，将prop参数设置为0.8，这时如果再setSpeed(120)实际速度会是120*0.8=96，记住，setProp方法不能传入大于1的数，如果传入大于1的数会按照1处理
myMotor->walk(120,1000); //我的这个电机以120的速度走了1秒
myMotor->gradientTo(50,1000); //我的这个电机在1秒内从转速120渐变成了转速50
myMotor->walk(120,1000); //我的这个电机以50的速度走了1秒

```
---
什么？？你说你的车车上有两个电机，一下控制俩太麻烦？好的没问题。接下来我们看一下<Grobot.h>这个新工具。当然，很明显，从字面意思上理解，Gmotor.h一看就是用来控制电机的，那么Grobot.h当然就是用来控制机器人的。

> <Grobot.h>的参数说明  
```cpp
//这部分参数可见性为全局可见
extern uint8_t SmallFont[]; //声明小字体矩阵，用于TFT显示屏
extern uint8_t BigFont[];//声明大字体矩阵，用于TFT显示屏
extern uint8_t SevenSegNumFont[];//声明特大数字字体字体矩阵，用于TFT显示屏
byte           vibrate = 0;//用于控制遥控手柄震动

//下面的参数可见性为private
Gmotor *mtl, *mtr; //这个机器人包含两个电机
uc      s1, s2, s3, s4, s5; //五个链接光电传感器的analog端口
UTFT *  GLCD       = new UTF(QD_TFT180A, 51, 52, 32, 34, 33); //默认初始化一个TFT显示屏
PS2X *  controller = new PS2X(); //默认初始化一个遥控手柄接收器
int     threshold[6]; //用来记忆几个光电传感器的阈值
int     lineCrossTime = 70; //大致穿线一次的时间，单位ms
int     negativeSpeed = 100,negativeRuntime = 50; //急停反转功率和反转时间
```
> <Gmotor.h>的方法说明  
```cpp
    Grobot(); //默认初始化方法，其实里面什么也没有
    void configMotor((Gmotor *leftMotor, Gmotor *rightMotor); //为你的机器人链接两个电机
    void configSensor(uc s1, uc s2, uc s3, uc s4, uc s5); //为你的机器人设置5个传感器
    void configTFT(byte pos); //为你的机器人定义一个显示屏，使用默认端口初始化并规定显示方向pos
    void configTFT( byte model, int RS, int WR, int CS, int RST, byte pos, int SER = 0); //为你的机器人定义一个显示屏，使用自定义端口初始化并规定显示方向pos
    byte configController(uc clk, uc cmd, uc att, uc dat); //为你的机器人初始化一个遥控器接收器，该方法返回一个byte，0表示已连接，1表示未连接
    void configController(uc clk, uc cmd, uc att, uc dat, bool pressures, bool rumble);//为你的机器人初始化一个遥控器接收器，并设置是否允许模拟压感和震动
    void configServo(int armPin, int handPin);用这个方法为你的机器人添加两个舵机以控制机械手，armPin为链接机械臂控制舵机的端口号，handPin为链接机械手控制舵机的端口号。
    void testSensors(); //以引导的形式测量场地传感器值并自行计算阈值。该方法将使用TFT显示屏辅助矫正，所以在这之前必须已经完成了显示器配置
    void setThreshold(int s1, int s2, int s3, int s4, int s5); //这个方法为5个传感器设置阈值
    void setThreshold(int sensorNum, int val); //这个方法为端口号为sensorNum的传感器设置阈值val
    int  getThreshold(int sensorNum); //这个方法用来获得记忆中端口号为sensorNum的传感器阈值，返回一个整数值。
    void setLineCrossTime(int); //设置大致穿线时间
    void setHunterSensor(int, int, int);//设置巡线用到哪三个传感器

    int     getSensorVal(uc pin); //用来获得链接于端口pin的传感器当前值
    Gmotor *getMotor(uc motor); //用来得到一个电机指针，传入参数'l'或'r'获得左边或右边电机的返回指针

    void walkTime(int speed, int ms); //让你的机器人以speed的电机转速往前走ms毫秒，注意，这与Gmotor中的walk不同，Grobot中的walk会使两个电机同时转动
    void walkBlock(int speed, int lineCnt); //以speed的速度在场地上直行lineCnt个格
    void turnLeft(int speed, int mode); //机器人以speed的电机转速左转一格
    void turnRight(int speed, int mode);//机器人以speed的电机转速右转一格
    void huntLine(int baseSpeed, int subSpeed, int lineCnt);//机器人以speed的电机转速左转一格
    void stop(int mode); //以mode模式停止，0为自然停止，1为瞬间停止

    void waitForButtonPress(ui BUTTON); //等待遥控手柄上的BUTTON键被按下
    void waitForButtonRelease(ui BUTTON); //等待遥控手柄上的BUTTON键被松开

    void initialRobot(); //用这个方法初始化机器人，否则机器人无法正常运行
    void pair(); //用这个方法使遥控器配对
    void enterManualMode(); //调用这个方法进入手动遥控模式
```
---
看完入门部分有什么感想？是不是完全没看懂？那就对了。我自己也看不懂。接下来我们进入放弃部分  

---
* 以下是放弃部分
> Getting Start 入门  

来试试看在脑子里想象一个机器人首先我们需要一个便于我们想象一个机器人的环境。所以....
```cpp
#include "Grobot.h"
```
好了，我们可以想一下了，机器人长什么样子来着.....我们新建一个机器人...  
```cpp
Grobot *robot = new Grobot();
```
好了我们创见了一个机器人。如你所见，robot的意思就是机器人，怎么样是不是很简单？就像是你新建一个int变量时写的[ int i = 0 ]一样。  
emmmmm我想想好像有两个轮子来着，所以就有控制这两个轮子的电机。所以我们再想象两个电机...  
```cpp
Gmotor *lmt = new Gmotor(3, 2), *rmt = new Gmotor(5, 4);
```
lmt啥玩意啊你逗我呢？！  
别别别急啊Left(左)MoTor(电机)缩写不就变成lmt了吗，谁没有个手懒的时候用个缩写咋了你有意见啊？同理RightMotor(右电机)的缩写也就是rmt。由于左电机是由3，2号PWM口控制的，右电机是由5，4号PWM口控制的，你至少告诉它自己是那个口控制的吧？

</font>
