//Calibrate procedure//

#define CalHandOpen  170
#define CalHandClose  180 

void EMG_calibrate_proc()
{
  int emgraw;
  int OptValue=-1;
  byte SerialValue;
  float filtemg; 
  int sensMax=0;
  int Starttime=0;
  int sensMin=1023*pow(2,FABLE_InfoParameter.OverRatio);
  
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  //Serial.setTimeout(10000);

  /********************CALIBRAZIONE***************************/
   SendGUIUserMessage("Start Calibration .....");
   SendGUIUserMessage("RILASSARE IL MUSCOLO E  PREMERE START");
   //Serial.println("Start Calibration .....");
   //Serial.println("RILASSARE IL MUSCOLO E  INVIARE 170");
   //Fake reading of analog
   emgraw=oversampling(FABLE_EMGCH0.Channel,FABLE_InfoParameter.OverRatio);

    while (1){
       if (Serial.available()>0){
           OptValue=Serial.parseInt();
             if (OptValue==CalHandOpen){
                //if (Serial.read()=='c'){
              Starttime=millis();
              while((millis()-Starttime)<FABLE_InfoParameter.CalibrationTime){
                  emgraw=oversampling(FABLE_EMGCH0.Channel,FABLE_InfoParameter.OverRatio);
                  looptime=millis()-elapsedtime-Starttime;
                  filtemg=ComplementaryFilt(emgraw,emgraw,looptime);
                  //Serial.println(filtemg);
                  SendGUIEMGData(ADCtoVolt(filtemg,FABLE_InfoParameter.OverRatio));
                   // record the maximum sensor value
                  if (filtemg > sensMax) {
                      sensMax = filtemg;
                      }

                    // record the minimum sensor value
                    if (filtemg < sensMin) {
                      sensMin = filtemg;
                     }
                     elapsedtime=millis()-Starttime;
                    
                }
                 Serial.print(sensMax);Serial.print(';');Serial.println(sensMin);
                 FABLE_EMGCH0.minimum=(sensMax-sensMin)/2;
                Serial.print("Valore minimo: ");
                Serial.println(FABLE_EMGCH0.minimum, DEC);
                break;           
        } 
      }
  }
    
    OptValue=-1;
    sensMax=0;
    sensMin=1023*pow(2,FABLE_InfoParameter.OverRatio);
    Starttime=0;
   SendGUIUserMessage("ESEGUIRE CONTRAZIONE E PREMERE START");
    //Serial.println("ESEGUIRE CONTRAZIONE E INVIARE 180");
    while (1) {
      if (Serial.available()>0){
          OptValue=Serial.parseInt();
           //Serial.print(OptValue);
            if ((OptValue==CalHandClose)){
//              if (Serial.read()=='c'){
            Starttime=millis();
            while(millis()-Starttime<FABLE_InfoParameter.CalibrationTime){
                  emgraw=oversampling(FABLE_EMGCH0.Channel,FABLE_InfoParameter.OverRatio);
                  looptime=millis()-elapsedtime-Starttime;
                  filtemg=ComplementaryFilt(emgraw,emgraw,looptime);
                  SendGUIEMGData(ADCtoVolt(filtemg,FABLE_InfoParameter.OverRatio));
                   // record the maximum sensor value
                  if (filtemg > sensMax) {
                      sensMax = filtemg;
                      }

                    // record the minimum sensor value
                    if (filtemg < sensMin) {
                      sensMin = filtemg;
                     }
                     elapsedtime=millis()-Starttime;
                    
                }
                 Serial.print(sensMax);Serial.print(';');Serial.println(sensMin);
                 FABLE_EMGCH0.maximum=(sensMax-sensMin)/2;
                Serial.print("Valore massimo: ");
                Serial.println(FABLE_EMGCH0.maximum, DEC);
              break;
            }
        
      }
  }
    FABLE_EMGCH0.emg_thr=(FABLE_EMGCH0.maximum+FABLE_EMGCH0.minimum)/2;
    Serial.print("Soglia intermedia: ");
    Serial.println(FABLE_EMGCH0.emg_thr);
    digitalWrite(13, LOW);
    
/**********************************************************/
}




void Servo_calibration(){
  // selezione del servo motore
  unsigned char data[64];
  data[0]=0;
  data[1]=ioCmd[3];//servo number 
  data[2]=ioCmd[4];//servo speed
  Set_Servo_Speed(data);
  data[2]=ioCmd[5];//servo start pos
  data[3]=ioCmd[6];//servo target pos
  
  //FABLE_InfoParameter.ser
  // Set angolo di start e angolo di stop
  // Test impostazione
  // salvataggio dati 


}

void SetDefaultParameter(){

  FABLE_DefaultValue.LevelShift =512;
  FABLE_DefaultValue.OverRatio=2;
  FABLE_DefaultValue.CalibrationTime=4000;
  FABLE_DefaultValue.tau=0.095;
  FABLE_DefaultValue.csens_thr=1;
  FABLE_DefaultValue.OvercurrentAngle=5;
  for (int i = 0; i < MAX_SERVOS; ++i){
    FABLE_DefaultValue.OpenAngleTarget[i]=0;
  }
  for (int i = 0; i < MAX_SERVOS; ++i){
    if ((i==0) || (i==MAX_SERVOS))
      FABLE_DefaultValue.CloseAngleTarget[i]=90;
    else   
      FABLE_DefaultValue.CloseAngleTarget[i]=180;

  }

}

void SendEmgCalibrationResult(){
paramCnt=2;
customParams[0]=EMG_CAL_THR;
customParams[1]=FABLE_EMGCH0.emg_thr;
MFL_SendMessage();
}
