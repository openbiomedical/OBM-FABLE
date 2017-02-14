void ProcessCommand(){
  if (FableCom.receiveFrame()){
    if (FableCom.receivedCommand=="startACQ"){//Start Fast Acq
      FastSend_enab=true;
      FableCom.write("startACQ",0);}
      
    else if (FableCom.receivedCommand=="setPos"){ // Set Pos Servo
        FableCom.write("setPosCommand",0);
        SerialCmd.FingerNum=FableCom.receivedContent[0];
        SerialCmd.StopPos=FableCom.receivedContent[1];
        SerialCmd.MSpeed=FableCom.receivedContent[2];
                
        SetServoSpeed(SerialCmd.MSpeed);
        MoveFinger(SerialCmd.FingerNum,SerialCmd.StopPos);
        MoveServo(SerialCmd.FingerNum);
      }  
    else if (FableCom.receivedCommand=="setPWMFreq"){

      SerialCmd.FingerNum=FableCom.receivedContent[0];
      int divisor=FableCom.receivedContent[1];
      setPwmFrequency(SerialCmd.FingerNum,divisor);
      }
    else if (FableCom.receivedCommand=="SetCapGain"){
    SetCapSens(FableCom.receivedContent[0]);
    FableCom.write("SetCapGain",0);
    
    }
    else if (FableCom.receivedCommand=="setAngleOp"){
      FableCom.write("setAngleOp",0);
      SerialCmd.FingerNum=FableCom.receivedContent[0];
      SerialCmd.StopPos=FableCom.receivedContent[1];
      SetServoAngleMin(SerialCmd.FingerNum,SerialCmd.StopPos);
            
    }
    else if (FableCom.receivedCommand=="setAngleCl"){
      FableCom.write("setAngleCl",0);
      SerialCmd.FingerNum=FableCom.receivedContent[0];
      SerialCmd.StopPos=FableCom.receivedContent[1];
      SetServoAngleMax(SerialCmd.FingerNum,SerialCmd.StopPos);
      
    }
    else if (FableCom.receivedCommand=="getAngleOp"){
      
      SerialCmd.FingerNum=FableCom.receivedContent[0];
      SerialCmd.StopPos=GetServoAngleMin(SerialCmd.FingerNum);
      FableCom.write("getAngleOp",0,SerialCmd.FingerNum,SerialCmd.StopPos);
    }
    else if (FableCom.receivedCommand=="getAngleCl"){
      SerialCmd.FingerNum=FableCom.receivedContent[0];
      SerialCmd.StopPos=GetServoAngleMin(SerialCmd.FingerNum);
      FableCom.write("getAngleCl",0,SerialCmd.FingerNum,SerialCmd.StopPos);
    }
    else if (FableCom.receivedCommand=="setEnFinger"){
      SerialCmd.FingerNum=FableCom.receivedContent[0];
      FableCom.write("setEnFinger",0);
      SetEnableFinger(SerialCmd.FingerNum);
    }

    else if (FableCom.receivedCommand=="connGUI"){
      isGUIpresent=FableCom.receivedContent[0];
      FableCom.write("connGUI",0);
    }
    
      else if (FableCom.receivedCommand=="stopACQ"){
       FastSend_enab=false;
       FableCom.write("StopACQ",0); 
        }
  }
}

bool GuiConnect (){
  if (FableCom.receiveFrame()){
    if (FableCom.receivedCommand=="connGUI"){
      isGUIpresent = FableCom.receivedContent[0];
      FableCom.write("connGUI",0);
       }
    }  
}
