void SetCapSens(int sensValue){
switch (sensValue){
  case 1:
    cap.writeRegister(CAP1188_SENSITIVITY, 0x7F);
  break;
  case 2:
    cap.writeRegister(CAP1188_SENSITIVITY, 0x6F);  // 2x  sensitivity THIS SEEMS TO WORK THE BEST FOR 3.5" plate sensors    
  break;
  case 4:
    cap.writeRegister(CAP1188_SENSITIVITY, 0x5F);  // 4x  sensitivity
  break;
  case 8:
  cap.writeRegister(CAP1188_SENSITIVITY, 0x4F);   //8X
  break;
  case 16:
    cap.writeRegister(CAP1188_SENSITIVITY, 0x3F); //16X
  break;
  case 32:
    cap.writeRegister(CAP1188_SENSITIVITY, 0x2F); //32X Default
  break;
  

}
}
int GetCapSens(){
 return cap.readRegister(CAP1188_SENSITIVITY);}

 void checkCapSequence(){
 
  uint8_t touched=cap.touched();
  //Serial.println(touched);
  if (touched ==0){
   // no touch detected
   return; 
    }
  for (uint8_t i=0;i<8;i++){
    if (touched & (1<<i)){
     
      TouchSeq[j]=i+1;
  //    Serial.print(TouchSeq[j]);Serial.print("/t");Serial.println(j);
      j++;
      
      if (j>3){
        j=0;}
      }
    }
    Serial.print(OpenSeq[0]);Serial.print(OpenSeq[1]);Serial.println(OpenSeq[2]);
    Serial.print(TouchSeq[0]);Serial.print(TouchSeq[1]);Serial.println(TouchSeq[2]);

  if (!memcmp(TouchSeq,OpenSeq,3*sizeof(int))){
      FableCom.write("<Open>100;");
      OpenHand();
    }

    if (!memcmp(TouchSeq,CloseSeq,3*sizeof(int))){
      FableCom.write("<Close>100;");
      CloseHand();
    }

  }

