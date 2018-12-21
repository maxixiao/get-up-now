//定义引脚
int Sensor = 8; //接收传感器的信号
int LED =11;
int buzzer =12;
const int TrigPin = 2; //发送超声波
const int EchoPin = 3; //接收超声波
float cm;
int Left_motor_go = 6; //左电机前进 AIN1
int Left_motor_back = 10; //左电机后退 AIN2
int Right_motor_go = 5; //右电机前进 BIN1
int Right_motor_back = 9; //右电机后退 BIN2
//const int FollowSensor = A4; //定义左边跟随的红外传感器引脚为A3
int FollowSensor = A4; //定义左边跟随的红外传感器引脚为A3
//const int FollowSensorRight = A5; //定义右边跟随的红外传感器引脚为A1
int SensorValue ; //定义变量来保存红外传感器采集的数据大小
//int RightSensorValue ;

void setup()

{

  Serial.begin(115200);
  Serial.println("Initialisation complete.");
  pinMode(LED,OUTPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(Sensor, INPUT); // 设置为输入
  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);
//初始化电机驱动IO口为输出方式
pinMode(Left_motor_go, OUTPUT);
pinMode(Left_motor_back, OUTPUT);
pinMode(Right_motor_go, OUTPUT);
pinMode(Right_motor_back, OUTPUT);

//定义左右传感器为输入接口
pinMode(FollowSensor, INPUT);
//pinMode(FollowSensorRight, INPUT);
//左右传感器初始化为高电平
digitalWrite(FollowSensor, 0);
//digitalWrite(FollowSensorRight, HIGH);


}
void run() // 前进函数
{
digitalWrite(Right_motor_go,HIGH); // 右电机前进
digitalWrite(Right_motor_back,HIGH);
//digitalWrite(Right_motor_back,LOW);
analogWrite(Right_motor_go,200);//PWM比例0~255调速，左右轮差异略增减
analogWrite(Right_motor_back,200);
digitalWrite(Left_motor_go,HIGH); // 左电机前进
digitalWrite(Left_motor_back,HIGH);
analogWrite(Left_motor_go,200);//PWM比例0~255调速，左右轮差异略增减
analogWrite(Left_motor_back,200);
//delay(time * 100); //执行时间，可以调整
}

//void brake(int time) //刹车，停车函数
void brake()
//刹车，停车函数
{
digitalWrite(Right_motor_go,LOW);
digitalWrite(Right_motor_back,LOW);
digitalWrite(Left_motor_go,LOW);
digitalWrite(Left_motor_back,LOW);
//delay(time * 100);//执行时间，可以调整
}


void loop()

{
//遇到跟随物,红外跟随模块的指示灯亮,端口电平为LOW
//未遇到跟随物,红外跟随模块的指示灯灭,端口电平为HIGH
  unsigned char i,j;
  int val = digitalRead(Sensor); // 读取引脚电平
SensorValue = digitalRead(FollowSensor);
 if (val==1)  // 若检测到有人，则val值为1
      { 
       run();
        int sum=0;
      
      digitalWrite(LED,HIGH);
      delay(200);
      digitalWrite(LED,LOW);
      delay(200);
      digitalWrite(buzzer,HIGH);
       Serial.println("who's there?");
       for(int i=0;i<3;i++)
       {
       // 产生一个10us的高电平脉冲去触发TrigPin
    digitalWrite(TrigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(TrigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(TrigPin, LOW);

    // 检测脉冲宽度，并计算出距离
cm = pulseIn(EchoPin, HIGH) / 58.0;
    //通过串口监视器输出
    sum=sum+cm;
    delay(100);
       }
    Serial.print("Distance:");
    Serial.println(sum/3.0);
   
       for(i=0;i<150;i++)
  {
    digitalWrite(buzzer,HIGH);
    delay(1);
    digitalWrite(buzzer,LOW);
    delay(1);
  }
  for(j=0;j<200;j++)
  {
    digitalWrite(buzzer,HIGH);
    delay(2);
    digitalWrite(buzzer,LOW);
    delay(2);}
      }
      
  else
      { digitalWrite(LED,LOW);
       digitalWrite(buzzer,LOW);
      Serial.println("emmmm,sleep");
brake();}

  delay(100);

}

