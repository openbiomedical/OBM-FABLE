
//LIBRARIES//

#include<Servo.h>
#include"Globals.h"
#include<stdlib.h>
#include<math.h>
#include <EEPROM.h>

#include <avr/io.h>
#include <PlainProtocol.h>

// CAP adafruit
#include<Wire.h>
#include <SPI.h>
#include <Adafruit_CAP1188.h>

// Reset Pin is used for I2C or SPI
#define CAP1188_RESET  9
#define CAP1188_SENSITIVITY 0x1F
// CS pin is used for software or hardware SPI
#define CAP1188_CS  10

// These are defined for software SPI, for hardware SPI, check your 
// board's SPI pins in the Arduino documentation
#define CAP1188_MOSI  11
#define CAP1188_MISO  12
#define CAP1188_CLK  13


int j;
int TouchSeq[3];
bool FastSend_enab=false;

Adafruit_CAP1188 cap = Adafruit_CAP1188();
PlainProtocol FableCom(115200);

void setup() {
  
FableCom.init();
if (!cap.begin()){
  FableCom.write("#1188<Init>0;");
// while(1);
  }
  FableCom.write("#1188<Init>1;");

for (int i=0;i<NumServo;i++){
  FABLE_Servo[i].attach(servo_pin[i]);
  };
InitializeParameter();
pinMode(13, OUTPUT); //LED indicating GUI connected
}

void loop() {
  // put your main code here, to run repeatedly:
if (isGUIpresent)
{
  digitalWrite(13, HIGH);
  ProcessCommand();
  delay(5);
}
else
{
  digitalWrite(13, LOW);
  checkCapSequence();
  
  delay(500);
  GuiConnect();
  }
}


  
