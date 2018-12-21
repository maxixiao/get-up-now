#include "OurSpark.h"//引用头文件，将OurSpark.h头文件放到项目同一文件夹内

ColorLEDController LEDController1(2, 6);  //实例化一个ColorLEDController对象
//参数中第一个2表示最大级联ColorLED个数，第二个6表示使用D6口控制LED灯。




#define val_max 255
#define val_min 0
#include <ESP8266.h>
#ifdef ESP32
#error "This code is not recommended to run on the ESP32 platform! Please check your Tools->Board setting."
#endif
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1284P__) || defined (__AVR_ATmega644P__) || defined(__AVR_ATmega128RFA1__)
#define EspSerial Serial1
#define UARTSPEED  115200
#endif
#if defined (__AVR_ATmega168__) || defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__)
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); 
#define EspSerial mySerial
#define UARTSPEED  9600
#endif
#define SSID        F("iPhone")//WIFI名
#define PASSWORD    F("mxx20000313")//WIFI密码
#define HOST_NAME   F("api.heclouds.com")//域名
#define HOST_PORT   (80)
static const byte  GETDATA[]  PROGMEM = {
  "GET https://api.heclouds.com/devices/502967882/datapoints?datastream_id=ah,am,status,lx&limit=1 HTTP/1.1\r\nHost:api.heclouds.com\r\napi-key:OKB8cC3wlJWl6YBVEH18zsnkfzU=\r\nConnection: close\r\n\r\n"
};//关键头文件
/* "GET https://api.heclouds.com/devices/23334240/datapoints?datastream_id=id,status,precent&limit=1 //url+  【 datapoints？datastream id=】 你的几个数据名称【&limit=】（几个数据1个表示最近）
 *  HTTP/1.1\r\n
 *  Host:api.heclouds.com//域名
 *  \r\n
 *  api-key:sCCE3BRbnvyyKsX1iWY7=dSFlME=///API
 *  \r\nConnection: close\r\n\r\n"
*/
ESP8266 wifi(&EspSerial);
//wifi

#include <U8glib.h>
//OLED
#include <Microduino_RTC.h>
//RTC时钟模块
int AL=1;
RTC rtc;
//short RECV_PIN = 10;//遥控//_________________________________________________________________________________________________int RECV_PIN = 10;//遥控
short pin3 = 2;//触摸      //_________________________________________________________________________________________________int pin3 = 2;//触摸
short pin2 = A0;//光敏     //_________________________________________________________________________________________________int pin2 = A0;//光敏
#define POUT 5   //led灯控制引脚   //led灯控制引脚
int sensorValue;
int up=23,down=21,rh=0,rm=0;
//#include <IRremote.h>
//IRrecv irrecv(RECV_PIN);
//decode_results results;
//遥控 内存不够
#include "audio.h"
//#include <SoftwareSerial.h>
int music_vol = 1; //初始音量0~30


DateTime dateTime = {2018, 12, 5, 21,19,35, 0};//时间初值
short lx=0;//______________________________________________________________short temp1,Rstatus=0,lx=0;
short ah=19,am=40,ch=0; 
bool Rstatus=0,ws=0,tcp=0,ala=0,touch=1;
String rstatus,wstatus,astatus;

uint16_t tYear;
uint8_t tMonth, tWeekday, tDay, tHour, tMinute, tSecond; 
#define INTERVAL_LCD             1000             //定义OLED刷新时间间隔  
 long lcd_time = millis();                 //OLED刷新时间计时器
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);     //设置OLED型号
//-------字体设置，大、中、小
#define setFont_H u8g.setFont(u8g_font_timB18)
#define setFont_M u8g.setFont(u8g_font_timB10)
//#define setFont_S u8g.setFont(u8g_font_chikitar)


unsigned long currentMillis = 0;


void setup() {
  Serial.begin(9600);
  lx=255;
 audio_init(DEVICE_Flash,4,music_vol); 
  pinMode(pin2, INPUT);//光敏
  pinMode(pin3, INPUT);//触摸
  pinMode(POUT, OUTPUT);
  //                 pinMode(9,OUTPUT);
  //               pinMode(11,OUTPUT);
 // irrecv.enableIRIn(); // 启动红外解码
  rtc.begin();
  rtc.clearAll();//设置启动时间
  rtc.setDateTime(dateTime);
  Serial.begin(115200);
 while (!Serial);
 delay(100);
  WifiInit(EspSerial, UARTSPEED);
  if (wifi.setOprToStationSoftAP()) {} else {}
if (wifi.joinAP(SSID, PASSWORD)) {ws=1;} else {ws=0;}
  if (wifi.disableMUX()) {} else {}
}
//---------------------------------------------------------------------------------------------------------------------------------------遥控
/*
void remotecontrol( long a)
{
  if(a==33441975)
  {if(Rstatus)
  Rstatus=0;
  else
  Rstatus=1;
  delay(500);
  }//第一输入激活遥控，第二次撤销
          if(Rstatus)//如果激活则控制
          {
                  switch(a){
                        //case 33441975://电源
                        case 33456255://A
                        case 33439935://B
                        case 33472575://C
                        case 33431775:break;//D
                        case 33448095:lx+=5;
                        break;//E
                        case 33464415:(lx+50)>val_max?255:lx+=15;
                        break;//上
                        case 33478695:((lx-50)<0)||((lx-50)>2000)?0:lx-=15;
                        break;//下
                        case 33480735://左
                        case 33460335:
                        break;//右
                        case 33427695:lx=lx>0?0:255;
                        break;//OK
                        case 33486855://音大
                        case 33452175://音效
                        case 33444015://重置
                        case 33468495://禁言
                        case 33435855://暂停？
                        default:
                        break;}
  
  }
}    */
//_______________________________________________________________________________________________________________________   /alarm
void alarm()
{
      lx=255;
     // ala=0;
      ala=1;
      audio_play();
       audio_choose(1);
}
//_______________________________________________________________________________________________________________________   /online
void online()
{

 //if(!tcp)
wifi.createTCP(HOST_NAME, HOST_PORT);

 
 wifi.sendFromFlash(GETDATA, sizeof(GETDATA));
//从Flash读取发送内容，节约内存
  uint8_t buffer[512] = {0};
  uint32_t len = wifi.recv(buffer, sizeof(buffer), 20000);
  if (len > 0)
  {
     short k=0,temp=0,tax[3];
 tax[0]=ah;
 tax[1]=am;
 tax[2]=lx;
    for (uint32_t i = 0; i < len-12; i++) {
      if((char)buffer[i]=='v'&&(char)buffer[i+1]=='a'&&(char)buffer[i+2]=='l'&&(char)buffer[i+3]=='u'&&(char)buffer[i+4]=='e')
      {
          for (uint32_t j = 0; j<4;j++)
              if((char)buffer[i+j+7]>='0'&&(char)buffer[i+j+7]<='9')
              {temp=((short)buffer[i+j+7]-48)+temp*10;
              //Serial.print(temp);
              }
              // Serial.print('\n');
              //Serial.println(temp);

       
           if(k==0)
           {//Serial.println(temp);
           ah=temp;}
           else if(k==1) 
           {//Serial.println(temp);
           am=temp;}
           else if(k==2)
           {//Serial.println(temp);
           lx=temp;}
            else if(k==3)
           {//Serial.println(temp);
          AL=temp;
          }
           k++;
           temp=0;     
    } }
  if(ah!=tax[0]||am!=tax[1]||lx!=tax[2]){
  Rstatus=1;

  }

  //Serial.println(currentMillis - previousMillis);
  }
  else
 {if(!tSecond%2) ws=0;}
 //wifi.releaseTCP();
}

//*******************************************************************************************************************************************

void loop() {


    rtc.getTime(&tHour, &tMinute, &tSecond);
  rtc.getDate(&tYear, &tMonth, &tWeekday, &tDay);
    
  sensorValue = analogRead(pin2);
 //_______________________________________________________________________________________________________________________   
  if(digitalRead(pin3))
  {
  if(tMinute==ch)
   touch=1;
   }
   else
   {
    tMinute+2<60?ch=tMinute+2:ch=tMinute-58;
   touch=0;
    }
  
    //_______________________________________________________________________________________________________________________   
    Serial.println(digitalRead(pin3));
  currentMillis = millis();

//_______________________________________________________________________________________________________________________   //红外控制 
//  if (irrecv.decode(&results)) {
 //   remotecontrol(results.value);
 //   irrecv.resume(); // 接收下一个值
 //   previousMillis = currentMillis; 
//  } 
//_______________________________________________________________________________________________________________________   //光强控制 
if((tHour==ah&&((tMinute==am)||(tMinute==am+1)))&&touch&&AL==1)
alarm( );
else{
  audio_pause();
  ala=0;
 
}

  if(Rstatus&&ala==0)
  {
    if(rh==0&&rm==0)
  {rh=tHour;
  rm=tMinute;}
  else
  {
    if(tHour==rh+1&&tMinute==((rm+15)<60?rm+15:rm-45)&&touch)
    {if(tSecond%5)
    {
      lx=125;
     }
     
    }
    if(tHour==rh+1&&tMinute==((rm+30)<60?(rm+30):(rm-30)))
    {
     alarm( );
     }
     else{
  audio_pause();
  ala=0;
  }
  }
  }
  else
  {
    if(tHour==21&&tMinute==0)//熄灯
    lx=255;
    else if(tHour>=down&&tHour<=up)
    {
          if(tSecond==0)lx-=2;
     }
    else if(tHour>=18&&tHour<=20);//反休眠
    else
    lx=0;//休眠
  }
   
   if(tSecond==0&&ws==0&&ala==0)
     {
      if (wifi.joinAP(SSID, PASSWORD)) {ws=1;} else {ws=0;}
     }
  Rstatus?rstatus="ON":rstatus="OFF";
  ws?wstatus="ON":wstatus="OFF";
  AL==1?astatus="ON":astatus="OFF";
 //_______________________________________________________________________________________________________________________   //OLED  
       u8g.firstPage();
      do {
               setFont_H;
        u8g.setPrintPos(0, 20);
        u8g.print(tHour);                
        u8g.setPrintPos(30, 20);
        u8g.print(":");
        u8g.setPrintPos(45, 20);
        u8g.print(tMinute);
                setFont_M;
        u8g.setPrintPos(75, 12);
        u8g.print(tMonth);
        u8g.setPrintPos(88, 12);
        u8g.print("/");
        u8g.setPrintPos(93, 12);
        u8g.print(tDay);
        u8g.setPrintPos(105, 12);
        u8g.print("/");
        u8g.setPrintPos(111, 12);
        u8g.print(tYear%100);
        u8g.setPrintPos(75, 24);
        u8g.print(tSecond);
        u8g.setPrintPos(95, 24);
        u8g.print(rstatus);
        u8g.setPrintPos(95, 36);
        u8g.print(wstatus);
        u8g.setPrintPos(95, 48);
        u8g.print(astatus);
        u8g.setPrintPos(0, 48);
        u8g.print((float)lx/2.55);
        u8g.setPrintPos(30, 48);
        u8g.print('%');
        u8g.setPrintPos(0, 36);
        u8g.print(ah);        
        u8g.setPrintPos(25, 36);
        u8g.print(':');        
        u8g.setPrintPos(50, 36);
        u8g.print(am);
                u8g.setPrintPos(0, 64);
        u8g.print(sensorValue);
        u8g.setPrintPos(50, 64);
   
                u8g.setPrintPos(80, 64);
        u8g.print(touch);

      } while ( u8g.nextPage() );
 //_______________________________________________________________________________________________________________________   //抹除      

          if(sensorValue>200)
     LEDController1.ShowColor(0,255,0,0); //第一个参数表示点亮0号灯，让其显示红光
  delay(1000);  //延迟1秒输出
  
  LEDController1.ShowColor(0,0,255,0); //第一个参数表示点亮0号灯，让其显示绿光
  delay(1000);  //延迟1秒输出
  
  LEDController1.ShowColor(0,0,0,255); //第一个参数表示点亮0号灯，让其显示蓝光
  delay(1000);  //延迟1秒输出
 
  LEDController1.ShowColor(0,0,0,0);  //熄灭
  delay(1000);  //延迟1秒输出


     //Serial.println(Rstatus);
     //Serial.println(tcp);
     //Serial.println(ala);


 // audio_play();
   //  audio_choose(1);


}

