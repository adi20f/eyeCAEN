/////////////////////
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
//////////////////
#include <SoftwareSerial.h>

SoftwareSerial mySerial(3,2); // 3=ARDUINO_RX, 2=ARDUINO_TX
unsigned char Re_buf[11],counter=0;
unsigned char sign=0;
uint16_t distance=0;
void setup() {
   Serial.begin(9600);
   mySerial.begin(9600);
   mySerial.listen();  
} 

void loop() {
  unsigned char i=0,sum=0;
  send_command(0x01);//查询输出
  delay(100);
  while (mySerial.available()) {   
    Re_buf[counter]=(unsigned char)mySerial.read();
    //Serial.println(Re_buf[counter++],HEX);
    if(counter==0&&Re_buf[0]!=0x5A) return;      // 检查帧头         
    counter++;       
    if(counter==7)                //接收到数据
    {    
       counter=0;                 //重新赋值，准备下一帧数据的接收 
       sign=1;
    }      
  }
  if(sign)
  {   
     sign=0;
     for(i=0;i<6;i++)
      sum+=Re_buf[i]; 
     if(sum==Re_buf[i] )        //检查帧头，帧尾
     {  	       
         distance=(Re_buf[4]<<8)|Re_buf[5];
         Serial.print("distance:");
         Serial.println( distance);
           
   }
  } 

}
void send_command(uint8_t command)
{
  uint8_t send_datas[4]={0xa5,0x56,0,0};
  send_datas[2]=command;
  send_datas[3]=send_datas[0]+send_datas[1]+send_datas[2];
  for(uint8_t i=0;i<4;i++)
  mySerial.write(send_datas[i]);
}
