/*
-----------GYUS42
VCC ---------- 5V
GND  --------- GND
A5 -------- RC (SCL)
A4 -------- TD (SDA)
*/

#include <SoftwareSerial.h>
#include <Wire.h>

#define NO_PIN -1

//The Arduino Wire library uses the 7-bit version of the address, so the code example uses 0x70 instead of the 8-bit 0xE0
#define SensorAddress byte(0x70)
//The sensors ranging command has a value of 0x51
#define RangeCommand byte(0x51)
//These are the two commands that need to be sent in sequence to change the sensor address
#define ChangeAddressCommand1 byte(0xAA)
#define ChangeAddressCommand2 byte(0xA5)

#define NUM_MAXSONAR 2

typedef struct {
  int analog_pin;
  int pw_pin;
  int rx_pin;
  int tx_pin;
  unsigned long dist_cm;
} sMaxSonar;

sMaxSonar g_maxsonar[NUM_MAXSONAR] = {
  {NO_PIN, 6, 7, NO_PIN, 0},
  {NO_PIN, 8, 9, NO_PIN, 0}
};
SoftwareSerial soft_ser(2, 3, true); // RX, TX

void MaxSonarInitPw(sMaxSonar *sonar) {
  pinMode(sonar->rx_pin, OUTPUT);
  // Pull low to disable reading
  digitalWrite(sonar->rx_pin, 0);
  pinMode(sonar->pw_pin, INPUT);
}

void MaxSonarInitSerial(sMaxSonar *sonar) {
  pinMode(sonar->rx_pin, OUTPUT);
  // Pull low to disable reading
  digitalWrite(sonar->rx_pin, 0);
  soft_ser.begin(9600);
}

void MaxSonarInitiateReading(sMaxSonar *sonar) {
  digitalWrite(sonar->rx_pin, 1);
  delay(2);

  // Pulse width output will be ready after 1 ms
  //delay();
  //digitalWrite(sonar->rx_pin, 0);
}

int MaxSonarGetReadingPw(sMaxSonar *sonar) {
  //digitalWrite(sonar->rx_pin, 1);
  //delay(2);
  // Need to read pw signal within 37.5 ms after the initiation of a reading
  //Pulse Width representation with a scale factor of 147 uS per Inch.
  unsigned long pulse = pulseIn(sonar->pw_pin, HIGH);
  //unsigned long pulse = pulseInLong(sonar->pw_pin, HIGH);

  //147uS per inch
  //inches = pulse / 147;
  //change inches to centimetres
  //cm = inches * 2.54;
  sonar->dist_cm = pulse / 57.874;
  
  //digitalWrite(sonar->rx_pin, 0);

  //Serial.print(inches);
  //Serial.print("in, ");
  //Serial.print(cm);
  //Serial.print("cm");
  //Serial.println();
  return 0;
}

inline char SoftSerialBlockingRead() {
  while (!soft_ser.available());
  return soft_ser.read();
}

int MaxSonarGetReadingSerial(sMaxSonar *sonar) {
  //if (soft_ser.available()) {
//    char c = soft_ser.read();
//    if (c == 13) {
//      Serial.write('\n');
//    } else if (c == 'R') {
//      ;
//    } else {
//      Serial.write(c);
//    }
    //Serial.write();
    //Serial.write('\n');
    //Serial.write
  //}
  char c;
  char num[4];
  c = SoftSerialBlockingRead();
  if (c != 'R') {
    return -1;
  }
  num[0] = SoftSerialBlockingRead();
  num[1] = SoftSerialBlockingRead();
  num[2] = SoftSerialBlockingRead();
  num[3] = '\0';
  c = SoftSerialBlockingRead();
  if (c != 13) {
    return -1;
  }
  sonar->dist_cm = atoi(num) * 2.54;
  return 0;
}


void GYUS42Init(void) {
  Wire.begin();
  // changeAddress(SensorAddress,0x40,0);
}

//Commands the sensor to take a range reading
void GYUS42InitiateReading(){
  Wire.beginTransmission(SensorAddress); //Start addressing
  Wire.write(RangeCommand); //send range command
  Wire.endTransmission(); //Stop and do something else now
}

//Returns the last range that the sensor determined in its last ranging cycle in centimeters. Returns 0 if there is no communication.
word GYUS42GetReading(){
  Wire.requestFrom(SensorAddress, byte(2));
  if(Wire.available() >= 2){ //Sensor responded with the two bytes
    byte HighByte = Wire.read(); //Read the high byte back
    byte LowByte = Wire.read(); //Read the low byte back
    word range = word(HighByte, LowByte); //Make a 16-bit word out of the two bytes for the range
    return range;
  } else {
    return word(0); //Else nothing was received, return 0
  }
}
/* Commands a sensor at oldAddress to change its address to newAddress
oldAddress must be the 7-bit form of the address that is used by Wire
7BitHuh determines whether newAddress is given as the new 7 bit version or the 8 bit version of the address
If true, if is the 7 bit version, if false, it is the 8 bit version
*/
void changeAddress(byte oldAddress, byte newAddress, boolean SevenBitHuh){
  Wire.beginTransmission(oldAddress); //Begin addressing
  Wire.write(ChangeAddressCommand1); //Send first change address command
  Wire.write(ChangeAddressCommand2); //Send second change address command
  byte temp;
  if(SevenBitHuh){ temp = newAddress << 1; } //The new address must be written to the sensor
  else { temp = newAddress; } //in the 8bit form, so this handles automatic shifting
  Wire.write(temp); //Send the new address to change to
  Wire.endTransmission();
}

void setup() {
  // put your setup code here, to run once:
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  //while (!Serial) {
    //; // wait for serial port to connect. Needed for native USB port only
  //}
  // prints title with ending line break
  Serial.println("Serial connected");

  //MaxSonar setup
  MaxSonarInitPw(&g_maxsonar[0]);
  MaxSonarInitPw(&g_maxsonar[1]);

  // GY-US42 setup
  //GYUS42Init();
}

void PrintDist(const char *name, unsigned long dist) {
  Serial.print(name);
  Serial.print(": ");
  Serial.print(dist);
  Serial.println(" cm");
}

void loop(){
  //while (1);
  MaxSonarInitiateReading(&g_maxsonar[0]);
  MaxSonarGetReadingPw(&g_maxsonar[0]);
  MaxSonarInitiateReading(&g_maxsonar[1]);
  MaxSonarGetReadingPw(&g_maxsonar[1]);
  PrintDist("MaxSonar0", g_maxsonar[0].dist_cm);
  PrintDist("MaxSonar1", g_maxsonar[1].dist_cm);
  // Need at least 50 ms before the next read
  //delay(50);

  //GYUS42InitiateReading(); //Tell the sensor to perform a ranging cycle
  //delay(100); //Wait for sensor to finish
  //word range = GYUS42GetReading(); //Get the range from the sensor
  //PrintDist("GYUS42   ", range);
// changeAddress(0x70,0x20,0);
  digitalWrite(g_maxsonar[0].rx_pin, 0);
  digitalWrite(g_maxsonar[1].rx_pin, 0);

  delay(1000);

}
