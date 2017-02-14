
int GetServoPos(int ServoIndex){
  return ArmoMov.angolo[ServoIndex];
  }
void SetServoPos(int ServoIndex, int Pos){
  ArmoMov.Pos[ServoIndex]=Pos;  
  }
  
void SetServoAngleMin (int ServoIndex,int AngleMin){
  FABLE_InfoParameter.OpenAngleTarget[ServoIndex]=AngleMin;
  }

void SetServoAngleMax (int ServoIndex,int AngleMax){
  FABLE_InfoParameter.CloseAngleTarget[ServoIndex]=AngleMax;
  }
void SetEnableFinger(int FingerCode){
  }
int GetEnablefinger(){
   int EnableCount=0;
   for (int i =0;i<NumServo;i++)
    EnableCount+=FABLE_InfoParameter.FingerEnabled[i];
  return EnableCount;
  }  
int GetServoAngleMin (int ServoIndex){
  return FABLE_InfoParameter.OpenAngleTarget[ServoIndex];
  }

 int GetServoAngleMax (int ServoIndex){
  return FABLE_InfoParameter.CloseAngleTarget[ServoIndex];
  }

int GetServoSpeed(){
  return FABLE_InfoParameter.Speed;
   }

void SetServoSpeed(int Speed){
  FABLE_InfoParameter.Speed=Speed;
  }



void MoveServo(int ServoIndex){
  
  int Speed=GetServoSpeed();
  int anglemin=GetServoAngleMin(ServoIndex);
  int anglemax=GetServoAngleMax(ServoIndex);
  float Stepmin=(anglemax-anglemin);
  float AngleStep=(Stepmin*Speed)/1048 ;
  int CurrPos=GetServoPos(ServoIndex);
  
 // Serial.println(Stepmin);
 // Serial.println(anglemin);
 // Serial.println(anglemax);
 // Serial.println(AngleStep);
  if (ArmoMov.Pos[ServoIndex]<CurrPos){
      AngleStep=-AngleStep;
      while(ArmoMov.grado[ServoIndex]>ArmoMov.Pos[ServoIndex])
      {ServoMovement(ServoIndex,AngleStep);
      //Serial.println(ArmoMov.grado[ServoIndex]);
      }
      
      }
   else{
    while(ArmoMov.grado[ServoIndex]<ArmoMov.Pos[ServoIndex])
    {ServoMovement(ServoIndex,AngleStep);
   // Serial.println(ArmoMov.grado[ServoIndex]);
   }
    
    }
  
  
  }
  
void MoveFinger(int index,int Pos){
  SetServoPos(index,Pos);
  MoveServo(index);
  }

void OpenHand(){
  for (int i=0;i<NumServo;i++){
    MoveFinger(i,FABLE_InfoParameter.OpenAngleTarget[i]);
   delay(FABLE_InfoParameter.FingerDelay);
     }
  ArmoMov.Direction=0;

  }

void CloseHand(){
  for (int i=0;i<NumServo;i++){
    MoveFinger(i,FABLE_InfoParameter.CloseAngleTarget[i]);
    //FableCom.write("MoveFinger",i);
   delay(FABLE_InfoParameter.FingerDelay);
 }
 ArmoMov.Direction=1; 
  }

void ServoMovement(int ServoIndex,float AngleStep){
    //float stepnum=(Servoanglemax-Servoanglemin)/pos ;
  
  int anglemin=5;//GetServoAngleMin(ServoIndex);
  int anglemax=160;//GetServoAngleMax(ServoIndex);
  
 float sen = sin(PI*ArmoMov.angolo[ServoIndex]/180);
 // coseno=cos( (angolo[Servoindex]/rad2grad) );
 ArmoMov.grado[ServoIndex] =  (sen*((anglemax-anglemin)/2))+((anglemax+anglemin)/2) ;
 
  FABLE_Servo[ServoIndex].write(ArmoMov.grado[ServoIndex]);
  
 ArmoMov.angolo[ServoIndex]=ArmoMov.angolo[ServoIndex]+AngleStep;
 
 }
 
void SetDefaultParameter(){

  FABLE_DefaultParameter.LevelShift =512;
  FABLE_DefaultParameter.OverRatio=2;
  FABLE_DefaultParameter.CalibrationTime=4000;
  FABLE_DefaultParameter.tau=0.095;
  FABLE_DefaultParameter.csens_thr=0.3;
  FABLE_DefaultParameter.OvercurrentAngle=5;
  FABLE_DefaultParameter.Speed=50;
  for (int i=0;i<5;i++)
    FABLE_DefaultParameter.FingerEnabled[i]=1;
  
  FABLE_DefaultParameter.CloseAngleTarget[0]=150;
  FABLE_DefaultParameter.CloseAngleTarget[1]=150;
  FABLE_DefaultParameter.CloseAngleTarget[2]=150;
  FABLE_DefaultParameter.CloseAngleTarget[3]=150;
  FABLE_DefaultParameter.CloseAngleTarget[4]=70;
  FABLE_DefaultParameter.OpenAngleTarget[0]=20;
  FABLE_DefaultParameter.OpenAngleTarget[1]=20;
  FABLE_DefaultParameter.OpenAngleTarget[2]=20;
  FABLE_DefaultParameter.OpenAngleTarget[3]=20;
  FABLE_DefaultParameter.OpenAngleTarget[4]=20;
  FABLE_DefaultParameter.isUpdated=0;
  
}
