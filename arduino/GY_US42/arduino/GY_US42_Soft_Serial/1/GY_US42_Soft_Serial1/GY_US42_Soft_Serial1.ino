//GY-US42  ARDUINO  测试代码,IO模拟软串口通信 
//硬件串口显示距离数据
//   GY-US42                 arduino pro mini
//   VCC----------------------VCC
//   GND----------------------GND
//   CR-----------------------2
//   DT-----------------------3
//   INT----------------------NC
//   PS-----------------------VCC
/*
使用步骤：
1.先下载程序至arduino
2.再接上GY-US42模块,模块PS引脚先接触VCC.
3.按复位按键
4.打开串口，波特率9600
*/
#include <Wire.h> 
#include <SoftwareSerial.h>

SoftwareSerial mySerial(3,2); // 3=ARDUINO_RX, 2=ARDUINO_TX
unsigned char Re_buf[8],counter=0;
unsigned char sign=0;
int led = 13;

//-----------------------------------------------------------
void setup()
{
  Serial.begin(9600); 
  mySerial.begin(9600);
  mySerial.listen(); 
  delay(200);    
  mySerial.write(0XA5);    //初始化GY-US42,连续输出模式
  mySerial.write(0X56);    //初始化GY-US42,连续输出模式
  mySerial.write(0X02);    //初始化GY-US42,连续输出模式
  mySerial.write(0XFD);    //初始化GY-US42,连续输出模式
  }
//-------------------------------------------------------------
void loop() {
   while (mySerial.available()) {   
    Re_buf[counter++]=(unsigned char)mySerial.read();
     
    if(counter==1&&Re_buf[0]!=0x5A) {counter=0;return;}      // 检查帧头         
    if(counter==2&&Re_buf[1]!=0x5A){counter=0;return; } 
    
    if(counter==7)                //接收到数据
    {    
       counter=0;                 //重新赋值，准备下一帧数据的接收 
       sign=1;
    }      
  }
  if(sign)
  {  
     sign=0;
     if(Re_buf[0]==0x5A&&Re_buf[1]==0x5A )        //检查帧头，帧尾
     {  	       
           Serial.print("R:");
           Serial.print((Re_buf[4]<<8|Re_buf[5]), DEC); 
           Serial.println();  
                  
   }
  } 
} 
//---------------------------------------------------------------



