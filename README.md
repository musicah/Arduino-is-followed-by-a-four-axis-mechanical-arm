# Arduino is followed by a four-axis mechanical arm
 Arduino随动四轴机械臂，机械臂实现步骤记录、复现功能

制作这个机械臂是机器人课程老师留下的作业，任务要求是夹取印章，然后在指定的地方盖章。程序要求能记录步骤，并且复现出来。

先大概看下丑陋的成品：
![机械臂成品图](https://img-blog.csdnimg.cn/20191227005810718.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE1MjU1OTI=,size_16,color_FFFFFF,t_70)
说实话确实很丑，因为临近毕业了，事情又多，没时间搞那么好看了

**原材料：**
电位器 * 4
9g舵机 * 4
轻触开关 *  1
Arduin UNO * 1
热熔胶
杜邦线多根（建议用质量好的杜邦线）
连接用的木棍（可以用其他代替）

![原材料](https://img-blog.csdnimg.cn/20191227010638912.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE1MjU1OTI=,size_16,color_FFFFFF,t_70)

然后用热熔胶连接成下图的样子：
![元件连接后](https://img-blog.csdnimg.cn/20191227010746703.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE1MjU1OTI=,size_16,color_FFFFFF,t_70)
这样就可以做成随动机械臂了，但是后来因为用电烙铁把电位器管脚连接起来后接触不良，原因是电烙铁的温度使电位器的连接处发生变形，导致的接触不良。随后就拆掉了，也没心思搞了，最后就变成一开始的样子了。

电路连接图如下：
![电路连接图](https://img-blog.csdnimg.cn/20191227011745783.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE1MjU1OTI=,size_16,color_FFFFFF,t_70)
最终的成品：
![成品图](https://img-blog.csdnimg.cn/20191227011956781.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE1MjU1OTI=,size_16,color_FFFFFF,t_70)
老师布置的任务要求是夹取印章，然后在指定的地方盖章。程序要求能记录步骤，并且复现出来。

为了节约成本，我只用了一个按键
按键功能：（按几次板子上的灯也相应的会闪烁几次，长按会快速闪烁4次）
	按1次：按顺序记录步骤n；
	按2次：复现（把刚刚记录的步骤重复一遍）；
	按3次：暂未定义；
	长按1秒：清除记录的所有步骤。


我自己写的代码请下载：Robot_ Source_Program
