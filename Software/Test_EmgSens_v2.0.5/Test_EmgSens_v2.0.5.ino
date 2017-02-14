// Fable_V2.0.0
//This version include the basic acquisition for the emg sensor V2.0

//LIBRARIES
#include <Servo.h>
#include "Test_EmgSens.h"
#include <EEPROM.h>
#include <EEPROMAnything.h>
#include <avr/io.h>
#include <avr/wdt.h>



//VARIABLES and data structs
int servo_pin[MAX_SERVOS]={3,5,6,9,10,11};   //servo control lines
int csens_pin[MAX_SERVOS]={A1,A2,A3,A4,A5};  //current sensor control line
int Button_Pin[MAX_BUTTONS]={4}; //Buttons control line
int simbtn =4;
boolean hand_open = true;
boolean blinkState;
boolean DiagMode=false;
String inString = "";  
String msgBuf;
boolean MacroKey=false;
int TrigEvents=0;
boolean EMG_Calib=false;

//void (*ResetFunct)(void)=0;

void setup() {
  // Initialization// 
 
  Serial.begin(serial_baude_rate);
  //Reset stato calibrazione//
  
    EMG_Calib=false;
   
   //Read parameter value in EE
   
   EEPROM_readAnything(0, FABLE_InfoParameter);
  //Scrittura in EE dei parametri di default
  //SetDefaultParameter();
  //EEPROM_writeAnything(0,FABLE_DefaultValue);
  //EEPROM_readAnything(0, FABLE_InfoParameter);
  // Inserire menu per modifica dei parametri di default//
  
  
  FABLE_EMGCH0={A0,0,0,0};
  
  
   time0=millis();
   
  // Initialize Servos//
  for (int i=0;i<MAX_SERVOS;i++){
    ioCmd[0]=0;ioCmd[1]=i;ioCmd[2]=servo_pin[i];
    Servo_Attach(ioCmd);
    ioCmd[2]=95; // Imposta il valore di velocit dei servo 
    Set_Servo_Speed(ioCmd);
    } 
 
 // Initialize Buttons//
 for (int i=0;i<MAX_BUTTONS;i++){
    pinMode(Button_Pin[i], INPUT);
    digitalWrite(Button_Pin[i],LOW);}
   // pinMode (simbtn,OUTPUT); 


  DiagMode=EnterDiag();
   while(DiagMode){
     MFL_process();
     }
//Calibrazioni  
if (EMG_Calib==false){
  EMG_calibrate_proc();
}

// digitalWrite(13,HIGH);
 
  
}

void loop() {
  // acquisition & filtering EMG

  long StartAcquisition=millis();
  while ((millis()-StartAcquisition)<MAX_TRIG_TIME){
        EMG_oversampled=oversampling(FABLE_EMGCH0.Channel,FABLE_InfoParameter.OverRatio);
        looptime=millis()-elapsedtime-StartAcquisition;
        EMG_Cmp=ComplementaryFilt(EMG_oversampled,EMG_oversampled,looptime);
        TrigEvents +=(EMG_Cmp>FABLE_EMGCH0.emg_thr)?1:0;
        elapsedtime=(millis()-StartAcquisition);
        SendGUIEMGData(ADCtoVolt(EMG_Cmp,FABLE_InfoParameter.OverRatio));
  }
  // Verifica attivazione Servo 
  MacroManager(1);
  
  
  /*Serial.print(FABLE_EMGCH0.emg_thr);
  Serial.print(';');
  Serial.print(ADCtoVolt(EMG_oversampled,FABLE_InfoParameter.OverRatio),DEC);
  Serial.print(';');
  Serial.print(hand_open);
  Serial.print(';');
  Serial.print(TrigEvents);
  Serial.print(';');
  Serial.print(StartAcquisition);
  Serial.print(';');
  Serial.println(ADCtoVolt(EMG_Cmp,FABLE_InfoParameter.OverRatio),DEC);*/
   TrigEvents=0;

  elapsedtime=(millis()-time0);
}





