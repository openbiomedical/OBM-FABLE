int oversampling (int emgchannel,int OverRatio){
  int sensor_value=0;
  int oversampled_window=pow(4,OverRatio);
  for (int i=0;i<oversampled_window;i++){
  sensor_value+=analogRead(emgchannel);
}
  int oversampled_value=sensor_value/pow(2,OverRatio);
  return oversampled_value;
}

//Rectify function//
int Rectifier (int value){
  int rect_value=abs(value);
  return rect_value;
}

// Shift function//
int ShiftValue (int value,int levelshift,int OverRatio){
  int shifted_value=value-levelshift*pow(2,OverRatio);
  return shifted_value;
}

//Complementary filter//
float ComplementaryFilt (int S1,int S2, int looptime) {
  float tau=0.1; 
  float dtC = float(looptime)/1000.0;
  float a=tau/(tau+dtC);
  EMG_C= a* (EMG_C + S2 * dtC) + (1-a) * (S1);
  return EMG_C;
}
float ADCtoVolt(float value,int OverRatio){
int  QuantRatio=1024*pow(2,OverRatio);
return value*(5000/QuantRatio);}

