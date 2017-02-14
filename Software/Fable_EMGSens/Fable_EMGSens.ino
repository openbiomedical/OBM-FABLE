
//LIBRARIES//

#include<Servo.h>
#include"Globals.h"
#include<stdlib.h>
#include<math.h>
#include <EEPROM.h>

#include <avr/io.h>
#include <PlainProtocol.h>
#include <FastSend.h>

FastSend scope;

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

Adafruit_CAP1188 cap = Adafruit_CAP1188();
int j;
bool FastSend_enab=false;
boolean EMG_Calib=false;
float EMG_Sig;
int EMG_TrigEvents;
int TouchSeq[3];


PlainProtocol FableCom(57600);

void setup() {
  
FableCom.init();

for (int i=0;i<NumServo;i++){
  FABLE_Servo[i].attach(servo_pin[i]);
  };
InitializeParameter();
pinMode(13, OUTPUT); //LED indicating GUI connected

}

void loop() {
  // put your main code here, to run repeatedly:
if (isGUIpresent){
  digitalWrite(13, HIGH);
  ProcessCommand();
  delay(5);
  if (EMG_Calib==false){
    EMG_calibrate_proc();
  }
}
else{
  digitalWrite(13, LOW);
  delay(500);
  GuiConnect();
  }
  
 EMG_Sig=oversampling(FABLE_EMGCH0.Channel,FABLE_InfoParameter.OverRatio);
 EMG_TrigEvents +=(EMG_Sig>FABLE_EMGCH0.emg_thr)?1:0;
 if (FastSend_enab){ 
    scope.send();};
 if (EMG_TrigEvents==3){
  if (ArmoMov.Direction==0)
      OpenHand();
  else
      CloseHand();
 
 }
 
delay(500);
}


  
