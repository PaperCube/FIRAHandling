# FIRAHandler Official Documention :: <Gmotor.h> & <Grobot.h> 
---
Author VisualDust , 201907092259LastUpdate  
<font face="等线" size=5>
《FIRAHandlerG库从入门到放弃全教程》 
</font>   
<font face="等线" size=4>
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
<font face ="等线" size=5 color=grey>  
看完入门部分有什么感想？是不是完全没看懂？那就对了。我自己也看不懂。接下来我们进入放弃部分 (没错这才是有用的部分上面是用来撑字数的)

---

<font face ="等线" size=5 color=C00000>  

# FBI Warning : 前方核能

</font>

<font face ="等线" size=4 color = black>  

* 以下是放弃部分

> Getting Start 入门  

来试试看在脑子里想象一个机器人首先我们需要一个便于我们想象一个机器人的环境。所以....
```cpp
#include "Grobot.h"
```
好了，包含这个头文件能够构造时空断裂(我瞎扯的)以便于我们的脑海中生成一台机器人，机器人长什么样子来着.....那么我们新建一个机器人...  
```cpp
Grobot *robot = new Grobot();
```
好了我们创建了一个机器人。如你所见，robot的意思就是机器人，怎么样是不是很简单？就像是你新建一个int变量时写的[ int i = 0 ]一样。  
emmmmm我想想好像有两个轮子来着，所以就有控制这两个轮子的电机。所以我们再想象两个电机...  
```cpp
Gmotor *lmt = new Gmotor(3, 2), *rmt = new Gmotor(5, 4);
```
lmt啥玩意啊你逗我呢？！  
别别别急啊Left(左)MoTor(电机)缩写不就变成lmt了吗，谁没有个手懒的时候用个缩写咋了你有意见啊？同理RightMotor(右电机)的缩写也就是rmt。由于左电机是由3，2号PWM口控制的，右电机是由5，4号PWM口控制的，你至少告诉它自己是那个口控制的吧？奥对忘告诉你了...  
我们先看一下FIRA官方提供的开发板长什么样子...
![FIRABoard](FIRABoard.jpg)
对对对！没错就是这个！
刚才我们提到控制电机的pwm口...emmmm....你看左下方隐约写着PWM D2-D7 PWM字样的那一排，蓝色的就是PWM接口了。因为过一会我们还要往机器人身上安装传感器，而传感器用的是analog口，也就是左侧好几排黄色的接口---所以在这里我们把它们加以区分 :  
蓝色的这个pwm口可以输出pwm信号，所以能向电机驱动模块传递转速信号。  
黄色的analog接口可以读取pwm信号，所以能够读取传感器传来的信息。  
总之，蓝色的能输出，黄色的能读入。嘿嘿，想想看如果把这两个接在一起.....  
啊呸我在想些什么！  
回归正题，我们说到....奥对，我们新建了两个电机！光新建了不行啊我们还得把它们安装到你的机器人上! void setup你应该知道吧，那么试试看在setup里写下这句话。什么你不知道什么是setup？  

---
啊....好吧，当你新建一个arduino空程序时，会自动生成这两个方法(,method,也叫函数,function):
```cpp
void setup(){}
```
```cpp
void loop(){}
```
这两个方法从字面意思理解,setup就是"建立"的意思。当你的arduino开机时，"会首先运行初始化也代码"，也就是会首先运行setup里面的内容。  
而loop字面上是循环的意思。当arduino执行完setup之后，会进入loop，并循环运行里面的代码。唯一的跳出条件是return。
所以，我们进行电机配置、显示屏配置等工作都需要在"初始化"也就是setup的时候做。

---

好了我们回到正题。刚才说怎么把电机链接到你的机器人上来着：
```cpp
robot->configMotor(lmt, rmt);
```  
config意思是配置，motor意思是电机。(可别写错了大小写昂，Motor的M要大写。以后我们写什么都要注意大小写别弄错了...)连起来就是配置电机的意思。也就是，我们把刚才创建的左电机lmt和右电机rmt配置到了机器人上。至于中间那个箭头(->)....那个是指针...呸，别想指针，你可以理解成机器人接下来要做一件事的时候用一个箭头为它指明方向！比如说"机器人->吃屎..." 呸我什么也没说。  

好的...我再看看...你看这个板子上是不是有个显示屏啊，我们得把这个也安装上，才显得我们足够狂飙炫酷吊炸天。  
```cpp
 robot->configTFT(QD_TFT180A, 51, 52, 32, 34, 0, 33);
```  
还记得config是什么意思吧，对是配置的意思。至于这个TFT，是显示器的一种。configTFT就是给机器人搞一个TFT显示屏。这个显示屏的具体型号是QD_TFT180A，我给你填好了你记住就行。  

那么接下来是遥控器的接收器。我们知道，这个比赛项目有自动和手动两个部分。手动部分当然是要遥控的，所以需要一个遥控器和一个遥控器的接收器。我们试着在setup里为你的机器人配置一个接收器:   
```cpp
 robot->configController(A14, A7, A13, A6, true, true);
 ```
 再复习一次config什么意思来着？什么你忘了？来这位同学出去站着去。  
 咳咳剩下的同学我们继续。不想出去陪他站着你们就好好背单词。是的Controller是控制器的意思。词根是control(控制)，加个er表示控制器。所以configController连一起就是"配置控制器"的意思。至于里面填了啥，你也不用管，都给你填好了复制粘贴就行。想了解参数含义的可以进PS2XLIB库里面暗中观察一下。  

 搞完了控制器我们来看看各个传感器。你的机器人上应该有5个传感器用来巡线以及进行各种判定，我们习惯上直接插在analog口的，从上往下看，五个传感器从左到右依次是S1(也就是Sensor1),S2,S3,S4,S5,分别插在A1,A2,A3,A4,A5上。那么代码就是：  
```cpp
    robot->configSensor(A1, A2, A3, A4, A5);
```  
会有老师教你们接线的，不用记住。这些config有关的你们都可以复制粘贴。  

那么，这个机器人差不多成型了，接下来我们要为它注入灵魂，欸嘿嘿...  
```cpp
    robot->initialRobot();
```
initial是初始化的意思，Robot我们再复习一次是机器人的意思。在setup里吟唱这句咒语会为你的机器人注入灵魂。一定不要忘了加上这句话！否则你的机器人将会成为一个没有灵魂的灭世者的存在  

这么强大的存在，当然要据为己有！现在我们来构筑灵魂链接！！！
```cpp
    robot->pair();
```  
呃...好中二啊我...其实就是跟遥控器配个对....pair的意思就是配对啦...这句话一写上你的机器人就会等待你的遥控器配对，当你的机器人屏幕上显示正在配对时，把遥控器打开按下上面的SELECT按钮，你的机器人显示屏上就会显示配对提示。注意，这个方法会在内部用到显示屏和遥控器接收器，所以在配对之前必须已经为机器人配置了显示屏和控制器，并且你的机器人已经注入了灵魂(滑稽)。
  
<font color=C00000>

* 这时候你的所有代码应该已经长成这个样子了：
</font>  

```cpp
#include "Grobot.h"

Grobot *robot = new Grobot();
Gmotor *lmt = new Gmotor(3, 2), *rmt = new Gmotor(5, 4);

void setup()
{
    robot->configMotor(lmt, rmt);
    robot->configTFT(QD_TFT180A, 51, 52, 32, 34, 0, 33);
    robot->configController(A14, A7, A13, A6, true, true);
    robot->configSensor(A1, A2, A3, A4, A5);
    robot->initialRobot();
    robot->pair();
}

void loop()
{

}
``` 
<font color=C00000>

* 如果不一样的话，请立即检查自己哪里理解错了并立马报警

</font>   

嗯？这位同学把手放下，你想问什么？奥，奥好的你是说为什么没有在setup里看到熟悉的pinMode之类的对吧？好的关于这个....你可以理解成其实你的机器人帮你做了这件事情。在你配置各个部分的时候你的机器人就会自行处理这些，用不着您大驾光临自己写了。是不是很方便？  

---

## 现在摆在你们面前的，有一个坏消息和一个更坏的消息。坏消息是接下来我们才开始真正的学习。更坏的消息是上面这一堆看上去复杂的东西你们都白学了，因为它们都是固定不变的，到时候直接复制粘贴就行的。  
怎么样是不是很想打我？欸嘿嘿嘿......

---

# 好的前排这位已经快吐了的同学，请你立马打起精神来，我们现在开始真正的学习！也就是到时候你们要写的，void loop(){} 里面的内容！

> 这个标题是用来充字数的并表示我们即将开始正式课程

咳咳....(喝水的声音)好的欢迎回来。在刚才的课程中我们已经学习了没有用的内容...(低头躲过飞过来的香蕉皮和拖鞋)，那么现在...啊！(被西红柿爆头)  
保安请把这位同学带走出去站着去  
好的我们继续。首先我们来熟悉一下场地。  

![空场地](PlayGround_Empty.png)

这是场地和场地上可能出现的物体的尺寸。我们来把物体随机乱放....

![有目标物的场地](PlayGround_WithObj.png)

绿色实心是目标物，黄色的当然就是目标区域了。什么你听不明白？？？给我出去站着再看一遍比赛规则去。  
我们先试着把场地清空：

![空场地](PlayGround.png)

现在我们将通过完成一些小任务来学习。

---

> 任务 I : 超速行驶

![Mission2](Mission_1.png)

现在，你需要让你的机器人疯狂往前跑1秒！越快越好！快到有人指控你超速为止！实现这个任务，只需要让机器人以一定速度直行一秒即可。  
```cpp
    robot->walkTime(255,1000);
```

欸？什么意思？walk就是行走的意思，Time是时间的意思。连起来就是：行走一段时间。其命令具体格式为：
```cpp
机器人->walkTime(速度,时间);
```
速度是0~255里选一个数字，时间是毫秒。1000毫秒就是一秒，255就是最大速度。这样的话你的全部代码大概是长这个样子:
```cpp
#include "Grobot.h"

Grobot *robot = new Grobot();
Gmotor *lmt = new Gmotor(3, 2), *rmt = new Gmotor(5, 4);

void setup()
{
    robot->configMotor(lmt, rmt);
    robot->configTFT(QD_TFT180A, 51, 52, 32, 34, 0, 33);
    robot->configController(A14, A7, A13, A6, true, true);
    robot->configSensor(A1, A2, A3, A4, A5);
    robot->initialRobot();
    robot->pair();
}

void loop()
{
     robot->walkTime(255,1000);
     robot->stop(0);
}
```
欸？怎么又多了个stop？  
stop就是停止的意思。你的机器人执行完这个任务后需要停下来。stop命令的格式为:
```cpp
机器人 -> stop(停止模式);
```
那我该如何填写停止模式？

![停止模式选择](Tutorial_2.png)

也许你看完了就想全部用stop(1)得了，误差小。其实不是的。在一点点实践的过程中，你会发现，流畅的动作更需要stop(0)，而需要高精度的操作则需要stop(1)。两者各有千秋，当你在调试时发现其中一种有问题时可以尝试另外一种。  
好了！现在链接下载线，把这段代码下载到你的机器人里吧！  
下载完你会发现，你的机器人立马就开始了动作，甚至不等你把它放到场地上。我的妈呀这么急切你去投胎啊！  
这个好解决，只要你在代码中loop的第一行插入一个wait方法：
```cpp
void loop()
{
    robot->wait(6000);
    .....
    .....
```
这啥意思？wait就是等待的意思。里面传入参数为一个毫秒值，意思是等待多少毫秒。6000的话就是6000毫秒也就是六秒，如果你改成10000你的机器人在开始运行前就会先等10秒。这些事件足够你把它放到合适的位置。  
那么，再试一次吧！祝武运昌隆！  
(￣▽￣*)你咋又回来了....啥？你说你的两个电机转速不一样导致你的直行实际上是在走弧线？没关系。我们来调整一下这个问题。我猜你现在正在想要不要把转的慢的电机调快一点，把转的快的电机调慢一点。嘛，差不多嘛。其实你可以在void setup中configMotor以后再加两句话。我们在这里假设你的左半边轮子比右半边轮子转的快。
```cpp
    lmt->setProp(0.8);
    rmt->setProp(1);
```
什么意思呢？set是设置的意思，Prop是参数、属性(properties)的缩写。这句话连起来的意思大概就是为电机设置一个转速参数。其具体格式为：  
```cpp
机器人 -> setProp(参数);
```
这样写的效果，就会使你为电机设置转速的时候会被后台乘上一个参数。所以左半边电机速度会为原来速度的0.8倍。  
这时候你的代码应该是：

```
#include "Grobot.h"

Grobot *robot = new Grobot();
Gmotor *lmt = new Gmotor(3, 2), *rmt = new Gmotor(5, 4);

void setup()
{
    robot->configMotor(lmt, rmt);
    lmt->setProp(0.8);  //新加入的一行
    rmt->setProp(1);    //新加入的一行
    robot->configTFT(QD_TFT180A, 51, 52, 32, 34, 0, 33);
    robot->configController(A14, A7, A13, A6, true, true);
    robot->configSensor(A1, A2, A3, A4, A5);
    robot->initialRobot();
    robot->pair();
}

void loop()
{
     robot->walkTime(255,1000);
     robot->stop(0);
}
```
好了，再试试看吧！如果发现你的电机转的方向干脆不一样，说明你该重新检查接线了。如果实在懒得检查，可以试试在转反了的那个电机setProp()里面把参数加个负号。比如，你发现左电机转的方向不对，可以：
```cpp
 lmt->setProp(-0.8);  //改成了负数
```
这样你的电机速度就会被后台乘以-0.8，也就是不但会减速还会反转。其实，有一个专门的办法处理反转：

```cpp
 lmt->exchangePin();
 lmt->setProp(0.8); //用了上面这行，就不用加负号了。
```
其具体格式为：
```cpp
电机 -> exchangePin();
```
exchange的意思是交换，Pin的意思是针脚，也就是电机的端口。连起来就是交换电机的两个控制端口，也就会导致它反转。该方法(函数)不需要传入任何参数。  
再试试去吧，这次可别回来再问了昂~


---

> 任务 II : Z字走位 ！

![Mission2](Mission_2.png)

现在，你需要让你的机器人按照如图所示的轨迹进行移动。怎么办呢？  
你的大脑应该已经开始思考完成这个任务需要几个动作了。很明显，大概就是：  
```
巡线一格  
左转  
巡线一格  
右转  
巡线一格
```    
对应到代码上就是：  
```cpp
robot->huntLine(40,50,1);   //巡线一格  
robot->turnLeft(90,1);      //左转  
robot->huntLine(40,50,1);   //巡线一格  
robot->turnRight(90,1);     //右转  
robot->huntLine(40,50,1);   //巡线一格
```  
(前排的同学此时看的一脸懵逼)我们来讲一下这些方法(函数)的具体含义和传入参数的含义。  
hunt是打猎，追捕的意思。Line就是线的意思。连在一起的huntLine当然就是追捕线...呸，巡线的意思。其具体格式是：
```cpp
机器人->huntLine(基准速度,差速度,巡线格数);
```
其中，速度为0~255的一个值。其运作原理是:  
![巡线工作原理](Tutorial_1.png)
这下有人问了：欸？巡线一格后的撞线判定之类的不用自己写吗？当然不用。这些琐碎你的机器人已经帮你处理好了，你只要发号施令，想想思路，剩下的全部交给你的机器人。  奥对了，提示你一下，你的机器人将会巡线左(也就是在线的左侧巡线)。  
至于turnLeft....就是左转的意思。当然turnRight就是右转的意思。其格式为：  
```cpp
机器人 -> turnLeft(速度,转弯模式);
```
速度大家都是知道的，至于这个转弯模式参数...我们来看图:  
这样的话你的全部代码应该是长这个样子：
```cpp
#include "Grobot.h"

Grobot *robot = new Grobot();
Gmotor *lmt = new Gmotor(3, 2), *rmt = new Gmotor(5, 4);

void setup()
{
    robot->configMotor(lmt, rmt);
    robot->configTFT(QD_TFT180A, 51, 52, 32, 34, 0, 33);
    robot->configController(A14, A7, A13, A6, true, true);
    robot->configSensor(A1, A2, A3, A4, A5);
    robot->initialRobot();
    robot->pair();
    robot->testSensors(); //注意，多了这一行
}

void loop()
{
    robot->wait(6666);
    robot->huntLine(40,50,1);   //巡线一格  
    robot->turnLeft(90,1);      //左转  
    robot->huntLine(40,50,1);   //巡线一格  
    robot->turnRight(90,1);     //右转  
    robot->huntLine(40,50,1);   //巡线一格
    robot->stop(1);
}
```
什么？多了的那行
```cpp
robot->testSensors();
```
是干嘛用的？其实大家知道，你的机器人要是需要巡线，就需要知道黑线和白色区域上传感器的数值。其具体格式为：
```cpp
机器人 -> testSensors();
```
这个方法不需要传入任何参数。当你的机器人运行到这个方法(函数)时，你会看到以下画面：  

![SensorTest_1](SensorTest_1.jpg)

下面写着"Now test sensors"就是现在测试传感器值的意思  
再看"Value for S 2W"这一行，S 2意思是2号传感器，W是White的缩写，意思是白色。整句话的意思就是"二号传感器在白色区域的值"，这时候你需要把你的机器人放在场地上，并保证二号传感器是在白色区域。  
我们复习一下，从上往下看传感器从左到右为S1,S2,S3,S4,S5,从车底看从右到左为S1,S2,S3,S4,S5。在白色区域的传感器值应该很大，一般800以上。  
准备好了以后长按1秒遥控器上的START按钮。你会看到画面变了：

![SensorTest_2](SensorTest_2.jpg)

没错，屏幕上方出现了一行小字"White:197"，意思就是2号传感器的白色值为197(当然，你自己测试的时候肯定不是这个数)，同时原来下方的的"2W"变成了"2B"。B是Black的缩写，意思为黑色。有的同学已经明白了，现在该测试2号传感器在黑线上的值了！于是你把2号传感器放到了黑线上，稍作位置上的调整并找到了 最小值 。注意，是尽量找最小值，这样更加精确。再次按下1秒遥控器上的START按钮后，你发现画面又变了：

![SensorTest_3](SensorTest_3.jpg)

看看，3W代表什么？这下不用我教了吧...你可以自行完成剩下的测试内容，你的机器人将会自动计算出黑白阈值并记住。    
根据程序内容，你的机器人将会在6秒多以后开始运行，注意把它放到正确的位置哦  

好了，插上下载线然后试试看！

---

> 任务 III :  蛇皮走位

![Mission3](Mission_3.png)

啊哈！看上去和上面的一个难度啊好简单...  
等等，为什么这个是无限循环走圈圈？  
也许很多人已经想到了，外面套一个无限循环就可以了。然而其实你们可以不用循环。之前我们说到loop的意思是什么来着？对，循环。所以说void loop里面的内容实际上本来就会被循环执行。所以只要...  
```cpp
#include "Grobot.h"

Grobot *robot = new Grobot();
Gmotor *lmt = new Gmotor(3, 2), *rmt = new Gmotor(5, 4);

void setup()
{
    robot->configMotor(lmt, rmt);
    robot->configTFT(QD_TFT180A, 51, 52, 32, 34, 0, 33);
    robot->configController(A14, A7, A13, A6, true, true);
    robot->configSensor(A1, A2, A3, A4, A5);
    robot->initialRobot();
    robot->pair();
    robot->testSensors();
}
//上面这些代码就像我说的，一点都没变，复制粘贴就行。

void loop()
{
    robot->huntLine(40,50,1);   //巡线一格
    robot->turnLeft(130,1);      //左转
    robot->huntLine(40,50,1);   //巡线一格
    robot->turnLeft(130,1);      //左转
    robot->huntLine(40,50,1);   //巡线一格
    robot->turnLeft(130,1);      //左转
    robot->huntLine(40,50,1);   //巡线一格
    robot->turnLeft(130,1);      //左转
}
```
试试看，它是不是在原地转圈？让它再转一会。如果转了好半天都没问题，说明你的程序已经足够稳定了。如果转了一会出现了穿线、找不着线等故障，你就需要调试你的巡线和转向方法的参数了。

> 任务 IV : 调试

行了别抱怨了，出现故障是正常的事情。我们接下来将会试着调试你在任务III里遇到的问题。  
首先，调试的时候你更愿意节省时间，所以我们可以暂时删除没有用的消耗时间的内容。  
比如，你发现启动画面时间较长，而且还要等遥控器配对，还要测试传感器之类的，反正测试一次要花大量时间等待，这不合算。然而只是巡线的话其实可以不连接遥控器，测试传感器也可以跳过并手动设置阈值。所以，你在void setup()里更改了如下内容 :  
```cpp
    //robot->pair();
    //robot->testSensors();
```
注意了吗？你在这两行前面加了两条"/"，这叫代码注释，加了//的内容在机器人运行时会被跳过，也就是不会运行。但是这样的话机器人就不知道黑白线的值了啊？别着急，我们将为它手动设置。  
在上一次测试的过程中，你应该大概记住了各个传感器黑白值，这里我们以3号传感器为例。假设其白色值为1003，黑色值为150，那么我们为了高灵敏度不再使用平均值，而是认为阈值为900。其大致算法为：
```cpp
传感器阈值 = 传感器白色值 - (传感器白色值 - 传感器黑色值) / 5
```
差不多就是这样一个数值。这时候我们需要为传感器手动设置这个值：
```cpp
robot->setThreshold(3,900);
```
什么意思呢？set我们复习一下就是设置的意思，Threshold就是阈值的意思。连起来就是设置阈值。该方法的具体格式为：
```cpp
机器人 -> setThreshold(传感器编号，阈值);
```
由于我们这次只调试巡线，因此只会用到2,3,4号传感器。所以我们在原来代码的void setup()最后添加如下代码: 
```cpp
......
void setup(){}
......
......
robot->setThreshold(2,900);
robot->setThreshold(3,900);
robot->setThreshold(4,900);
}
......
......
```
这样一来我们就为传感器手动设置了阈值。现在你的全部代码应该是：
```cpp
#include "Grobot.h"

Grobot *robot = new Grobot();
Gmotor *lmt = new Gmotor(3, 2), *rmt = new Gmotor(5, 4);

void setup()
{
    robot->configMotor(lmt, rmt);
    robot->configTFT(QD_TFT180A, 51, 52, 32, 34, 0, 33);
    robot->configController(A14, A7, A13, A6, true, true);
    robot->configSensor(A1, A2, A3, A4, A5);
    robot->initialRobot();
    // robot->pair();
    // robot->testSensors();
    robot->setThreshold(2,900);
    robot->setThreshold(3,900);
    robot->setThreshold(4,900);
}
//上面这些代码就像我说的，一点都没变，复制粘贴就行。

void loop()
{
    robot->huntLine(40,50,1);   //巡线一格
    robot->turnLeft(130,1);      //左转
    robot->huntLine(40,50,1);   //巡线一格
    robot->turnLeft(130,1);      //左转
    robot->huntLine(40,50,1);   //巡线一格
    robot->turnLeft(130,1);      //左转
    robot->huntLine(40,50,1);   //巡线一格
    robot->turnLeft(130,1);      //左转
}
```
好了，这样我们就为漫长的调试节约了大量时间。  
那么以后干脆一直手动设置阈值得了，还省时间啊..  
其实不然。测试的时候我们的目的就是省时间，而现场比赛时我们更倾向于更加精准的实时值。电池电压降低和现场光照强度都可能影响光电传感器的值。所以调试的时候手动，比赛的时候还是自动的好。  
那么就开始调试吧！机器人转弯速度过快、巡线速度差过大或过小都可能导致丢线等问题，可以具体分析问题，调整速度或比例，再试一次。不断尝试，总能找到稳定的状态。

---

> 任务 V :奖励关卡[秋名山の车神]  

小伙子我看你有开车的天赋！经历了漫长而苦逼的调试之后，我们终于迎来了第一个奖励关卡！  
其实我们的比赛分为两个部分，第二部分是手动模式！要不然我们要遥控器干嘛？ 
其实进入手动模式的程序也就是一行:  
```cpp
robot->enterManualMode();
```  
enter是进入的意思，Manual是手动，人工的意思，Mode我们之前讲过是模式的意思。这一句话会让你的机器人丧失意识成为你的忠实走狗！  
该方法的的格式为：
```cpp
机器人 -> enterManualMode();
```
呸我开玩笑的。你的全部代码应该是这样的 :

```cpp
#include "Grobot.h"

Grobot *robot = new Grobot();
Gmotor *lmt = new Gmotor(3, 2), *rmt = new Gmotor(5, 4);

void setup()
{
    robot->configMotor(lmt, rmt);
    robot->configTFT(QD_TFT180A, 51, 52, 32, 34, 0, 33);
    robot->configController(A14, A7, A13, A6, true, true);
    robot->configSensor(A1, A2, A3, A4, A5);
    robot->initialRobot();
    robot->pair();
    // robot->testSensors(); //因为是遥控模式所以并不需要传感器
}

void loop()
{
    enterManualMode();
}
```  
在这里再次提醒大家大小写是有区分的。  
现在，下载程序试试看。开机后，你会照常看到启动和配对画面。在配对完成之后，按下遥控器的start按钮，你会发现你的机器人主板上的指示灯闪烁了起来，这说明电波biubiubiu地到达了那里并且你的机器人收到了。现在我们来看一下遥控器 :  

![PS2X](PS2X.jpg)

这是一个PS2X手柄。至于我们为什么用这个...emmm因为比较便宜(划掉)因为PS2X有好心大闲人给写了现成的库可以直接用方便极了。  
现在再推动推杆...  
没错！你的机器人动起来了！！！试试看来个漂移!  
看上去很简单，就一行的程序，为什么要单独拿出来说呢？因为遥控这个事情需要多练。不是我开玩笑，多练一段时间你会发现自己的遥控能力得到了巨大的提升。这样才配得起你秋名山车神的称号！真的，多练出奇迹。  
现在我们试一下一个难度高一点的动作：  
![Mission_5](Mission_5.png)  
先让机器人自动按照如图所示的轨迹运行，再进入手动模式。这次的代码就不单独拿出来分析了....毕竟只要把以前的代码改改再加上进入手动模式的这一行就行了...  
什么？你说车速太快你跟不上？呃..那好吧，看到你遥控器上，有一个X按钮和一个⚪按钮。在遥控过程中按下X按钮速度会下降，按下⚪按钮速度会升高，这下总行了吧...  

---


实话跟你讲我还没写完....

</font>

<font color=black>

---

> Contributors  

报告，作者已失去生理活性。

<table><tr><td align="center"><a href="https://github.com/VisualDust"><img src="Icon.VisualDust.png" width="200px;" alt="VisualDust"/><br /><sub><b>VisualDust</b></sub></a><br /><a href="https://github.com/VisualDust" title="Code">💻</a> <a href="#design-jakedex" title="Design">🎨</a> <a href="https://github.com/VisualDust/FIRAHandling/commits?author=VisualDust" title="Documentation">📖</a> <a href="#question-mfix22" title="Answering Questions">💬</a> <a href="#infra-briandennis" title="Infrastructure (Hosting, Build-Tools, etc)">🚇</a> <a href="#review-VisualDust" title="Reviewed Pull Requests">👀</a></td></table>

---

> 附录

[![ReportBugsForDoc](ReportBugsForDoc.png)](https://github.com/visualDust/FIRAHandling/issues/2)  
[![RequestFuncForLib.h](RequestFuncForLib.png)](https://github.com/visualDust/FIRAHandling/issues/1)  
[![Grobot.h](View_GrobotH_Button.png)](https://github.com/visualDust/FIRAHandling/blob/master/Libiaries/Grobot.h)  
[![Gmotor.h](View_GmotorH_Button.png)](https://github.com/visualDust/FIRAHandling/blob/master/Libiaries/Gmotor.h)
[![ReportBugsForLib](ReportBugsForLib.png)](https://github.com/visualDust/FIRAHandling/issues/3)  
[![ChatGroup](ChatGroup.png)](https://github.com/visualDust/FIRAHandling/issues/4)
[![BackToTop](BackTo_DocTop_Button.png)](https://github.com/visualDust/FIRAHandling/blob/master/Documents/LibDoc.markdown)
[![BackToMainPage](BackTo_MainPage_Button.png)](https://github.com/visualDust/FIRAHandling)

---

[![Donate](Donate.png)]()

[![NobleList](NobleList.png)](https://github.com/visualDust/FIRAHandling#donate)
