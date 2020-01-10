#include <Servo.h>			//舵机头文件
#include <stdio.h>		
#include <string.h>
#include <MsTimer2.h>		//定时器库的 头文件，需添加对应的头文件才可使用
  
Servo myservo1;				// 创建一个伺服电机对象1
Servo myservo2;				// 创建一个伺服电机对象2
Servo myservo3;				// 创建一个伺服电机对象3
Servo myservo4;				// 创建一个伺服电机对象4

#define Servo_Num 4         //自定义舵机个数
#define STEP 30             //自定义最大记录步数
#define KEY 2               //定义多功能按键管脚
#define Step_time 15        //定义步骤复现时舵机改变1度的时间，单位：ms

int dianweiqi1 = 0;
int dianweiqi2 = 0;
int dianweiqi3 = 0;
int dianweiqi4 = 0;

unsigned int step_num = 1;   //定义已走步数变量
unsigned int flag = 0;       //定义按键次数标志位
unsigned int key_num = 0;    //按键次数

unsigned char Step_Receive [ STEP + 1 ][ Servo_Num ] = {0}; //定义储存步数，初始化为0，最多存储STEP步

//函数声明
void Servo_Read();
void Blink(int i);
void slow_Step(int i);


void setup() 
{
  pinMode(KEY,INPUT_PULLUP);   	//初始化按键，上拉输入
  pinMode(LED_BUILTIN, OUTPUT);	//初始化LED灯
  
  myservo1.attach(6); 			// 爪
  myservo2.attach(9); 			// 小臂舵机
  myservo3.attach(10);			// 大臂舵机
  myservo4.attach(11);			// 底座旋转

  MsTimer2::set(700, RT);      	// 中断设置函数，每 700ms 进入一次中断
  
}

 

void loop()
{
  
  dianweiqi1 = analogRead(0);                     	//读取A0口模拟量
  dianweiqi1 = map(dianweiqi1, 0, 1023, 180, 0);  	//将模拟量转换为0~180之间的数值
  myservo1.write( dianweiqi1 );
  
  dianweiqi2 = analogRead(1);
  dianweiqi2 = map(dianweiqi2, 0, 1023, 0, 180);	//这样更改值会使舵机反着转
  myservo2.write( dianweiqi2 );

  dianweiqi3 = analogRead(2);
  dianweiqi3 = map(dianweiqi3, 0, 1023, 180, 0);
  myservo3.write( dianweiqi3 );

  dianweiqi4 = analogRead(3);
  dianweiqi4 = map(dianweiqi4, 0, 1023, 0, 180);
  myservo4.write( dianweiqi4 );

  Scan_KEY();					   	  //按键扫描
  if(key_num > 0)
  {
    if( flag == 0 && key_num == 1 )   //按一次记录步骤
    {
      Blink(key_num, 400);
      Servo_Read();
      key_num = 0;
    }
  
    else if(flag == 0 && key_num == 2)//按两次记录复现
    {
      Blink(key_num, 400);
      Servo_Step_recur();
      key_num = 0;
    }
  
    else if(flag == 0 && key_num == 3)//按三次循环复现
    {
      Blink(key_num, 400);
      key_num = 0;
    }
  
    else if(flag == 0 && key_num >= 4)//长按清除记录步骤
    {
      memset( Step_Receive, 0, sizeof(Step_Receive) / 4 );
      step_num = 1;                   //步数记录回到第1步
      Blink(key_num, 200);            //LED灯闪烁4次
      key_num = 0;
    }
  }
  
}


void RT()   //中断函数
{
  flag = 0;
  MsTimer2::stop();        			//停止计时
}

void Scan_KEY()            			//按键扫描
{ 
  if( digitalRead(KEY) == 0 )   	//查看按键是否按下
  {
    delay(80);                 		//延时80ms，去抖动  
    if( digitalRead(KEY) == 0 ) 	//查看按键是否按下
    {
      key_num++;
      flag = 1;
      MsTimer2::start();        	//启动定时器
      
      while(digitalRead(KEY) == 0)  //松手检测,若超过700ms未松手，则判断为长按
      {
        delay(10);
        if( key_num == 1 && flag == 0 )
        {
          key_num = 4;
        }
      }
    }
    
  }

}
  
void slow_Step(int i)
{
  int j = 0;
  int arr[Servo_Num + 1] = {0};
  int arr_temp[Servo_Num + 1] = {0};
  
  //如果是第一步，则记录现在的位置，作为对比
  if(i == 1)
  {
    Step_Receive[i - 1][0] = dianweiqi1;
    Step_Receive[i - 1][1] = dianweiqi2;
    Step_Receive[i - 1][2] = dianweiqi3;
    Step_Receive[i - 1][3] = dianweiqi4;
  }

  
  //与上一步对比，缓慢前进
  for(j = 0; j < Servo_Num; j++)
  {
    if( Step_Receive[i][j] - Step_Receive[i - 1][j] > 0 )
    {
      arr[j] = 1;
      arr_temp[j] = 1;
    }
    
    else if( Step_Receive[i][j] - Step_Receive[i - 1][j] < 0 )
    {
      arr[j] = -1;
      arr_temp[j] = -1;
    }
    else
    {
      arr[j] = 0; 
      arr_temp[j] = 0; 
    }
  }

  while( ( (Step_Receive[i - 1][0] + arr[0]) != (Step_Receive[i][0]) ) ||
         ( (Step_Receive[i - 1][1] + arr[1]) != (Step_Receive[i][1]) ) ||
         ( (Step_Receive[i - 1][2] + arr[2]) != (Step_Receive[i][2]) ) ||
         ( (Step_Receive[i - 1][3] + arr[3]) != (Step_Receive[i][3]) ) )     
  {

    myservo1.write( Step_Receive[i - 1][0] + arr[0] );
    if( (Step_Receive[i - 1][0] + arr[0]) != (Step_Receive[i][0]) ){
      arr[0] += arr_temp[0];
      }
    
    myservo2.write( Step_Receive[i - 1][1] + arr[1] );
    if( (Step_Receive[i - 1][1] + arr[1]) != (Step_Receive[i][1]) ){
      arr[1] += arr_temp[1];
      }
      
    myservo3.write( Step_Receive[i - 1][2] + arr[2] );
    if( (Step_Receive[i - 1][2] + arr[2]) != (Step_Receive[i][2]) ){
      arr[2] += arr_temp[2];
      }
      
    myservo4.write( Step_Receive[i - 1][3] + arr[3] );
    if( (Step_Receive[i - 1][3] + arr[3]) != (Step_Receive[i][3]) ){
      arr[3] += arr_temp[3];
      }
      
    delay(Step_time);
  }

}


void Servo_Step_recur()//舵机步骤复现
{
  int i = 0, j = 0;
  for(i = 1; i < step_num; i++)
  {
    slow_Step(i);
//    for(j = 0; j < 1; j++)//循环多次，让舵机接收PWM波持续一段时间
//    {
//      myservo1.write( Step_Receive[i][0] );
//      myservo2.write( Step_Receive[i][1] );
//      myservo3.write( Step_Receive[i][2] );
//      myservo4.write( Step_Receive[i][3] );
//    }
//    delay(1000);//等待舵机到位
  }
  
}



void Servo_Read() //读取脉冲值
{
  Step_Receive[step_num][0] = dianweiqi1;
  Step_Receive[step_num][1] = dianweiqi2;
  Step_Receive[step_num][2] = dianweiqi3;
  Step_Receive[step_num][3] = dianweiqi4;
  if (step_num < STEP)
  {
    step_num++;
  }
  else{
    Blink(15, 100);//超过最大记录步数 LED灯闪烁1.5s
    }
}

void Blink(int i, int j ){    //闪烁灯程序,i为闪烁次数，j为闪烁间隔 单位ms
    for(; i > 0; i--)
    {
      digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(j);                       // wait for a second
      digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
      delay(j);      
    }// wait for a second
  }
