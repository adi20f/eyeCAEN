#include "SR04.h"
#define TRIG_PIN 2
#define ECHO_PIN 3
//硬件接法
/*
pro mini--------GY-US42
VCC--------------VCC
GND--------------GND
2----------------CR
3----------------DT
GND--------------PS
*/
SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);
long a;

void setup() {
   Serial.begin(9600);
   Serial.println("Example written by Coloz From Arduin.CN");
   delay(1000);
}

void loop() {
   a=sr04.Distance();
   Serial.print(a);
   Serial.println("cm");
   delay(100);
}
