void EMG_calibrate_proc(){
  int emgraw;
  int sensMax=0;
  int sensMin=1023*pow(2,FABLE_InfoParameter.OverRatio);
  int Starttime=0;
  
 Serial.println("Start Calibration");
 Serial.println("Rilassare il muscolo");
 emgraw=oversampling(FABLE_EMGCH0.Channel,FABLE_InfoParameter.OverRatio);
 while(1){
  Starttime=millis();
  while((millis()-Starttime)<FABLE_InfoParameter.CalibrationTime){
    emgraw=oversampling(FABLE_EMGCH0.Channel,FABLE_InfoParameter.OverRatio);
    if (FastSend_enab)
        scope.send();
    if (emgraw>sensMax)
        sensMax=emgraw;}
      FABLE_EMGCH0.minimum=(sensMax-sensMin)/2;
      break;
  }
  sensMax=0;
    sensMin=1023*pow(2,FABLE_InfoParameter.OverRatio);
    Starttime=0;
   Serial.println("ESEGUIRE CONTRAZIONE E PREMERE START");
    //Serial.println("ESEGUIRE CONTRAZIONE E INVIARE 180");
    while (1) {
      Starttime=millis();
      while(millis()-Starttime<FABLE_InfoParameter.CalibrationTime){
      emgraw=oversampling(FABLE_EMGCH0.Channel,FABLE_InfoParameter.OverRatio);
      if (FastSend_enab)
      scope.send();
       if (emgraw < sensMin) 
           sensMin = emgraw;
           }
        FABLE_EMGCH0.maximum=(sensMax-sensMin)/2;
        break;
    }
    FABLE_EMGCH0.emg_thr=(FABLE_EMGCH0.maximum+FABLE_EMGCH0.minimum)/2;
    EMG_Calib=true;
 }


