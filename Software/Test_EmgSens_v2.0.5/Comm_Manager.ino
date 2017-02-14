

boolean MFLgetCommand()
{ int ByteValue;
  
	// handle serial data begin
    {	
    if (Serial.available() > 0)
	{
		//blink LED to indicate activity
		blinkState = !blinkState;

		// read the incoming byte:
		int newByte = Serial.read();
		if(isDigit(newByte)){
                  msgBuf+=(char) newByte;
                  }
                   if((newByte==',')||(newByte=='\n')){
                     ++byteCount;
                    ByteValue=msgBuf.toInt();
                    msgBuf="";
                  
                
		// checking first byte - beginning of message?
		if (byteCount == 1 && ByteValue != MAGIC_NUMBER)
		{
			++errorCount;
			sendError(ERROR_SERIAL);
			// reset - try again
			byteCount = 0;
			return false;
		}

		if (byteCount == 2)
		{
		   // get the size of message
		   // todo check msg < 64 (MAX_MSG_SIZE)
		   msgSize = ByteValue;
		}

		if (byteCount > 2) {
		  // fill in msg data - (2) headbytes -1 (offset)
		  ioCmd[byteCount - 3] = ByteValue;
                  //Serial.print(ioCmd[byteCount-3]);
		}

		// if received header + msg
		if (byteCount == 2 + msgSize)
		{
                  byteCount=0;
                  sendAck();
                  return true;           
		}
	} // if Serial.available
        }

	//Serial.print("false");
      return false;

}
}


void MFL_process(){
  ++loopCount;
  
if (MFLgetCommand()){
    
    switch (ioCmd[0]){
    
      case SERVO_ATTACH:{
        Servo_Attach(ioCmd);
        break;
       }
    case SERVO_SWEEP_START:{
        Servo_Sweep_Start(ioCmd);
        Serial.print("start");
        break;
       }
    case SERVO_SWEEP_STOP:{
        Servo_Sweep_Stop(ioCmd);
        break;
       }
    case SERVO_EVENTS_ENABLE:{
        Servo_Events_Enable(ioCmd);
        break;
       }
    case SERVO_WRITE:{
        Servo_write(ioCmd);
        //Serial.println("cmdok");
        break;
       }
     case SERVO_WRITE_MICROSECONDS :{
        Servo_Write_Microseconds(ioCmd);
        break;
       }
     case SET_SERVO_SPEED:{
	// setting the speed of a servo
	Set_Servo_Speed(ioCmd);
	break;
	}
    case SERVO_DETACH:{
	Servo_Detach(ioCmd);
      break;   
    } 
    case EMG_CALIB:{
        EMG_calibrate_proc();
        EMG_Calib=true;
        break;   
    }
    case SET_CSENS_THR:{
        FABLE_InfoParameter.csens_thr=ioCmd[1];
        break;
    }
    case SET_OPEN_ANG_TARGET:{
        for (int i=0;i<MAX_SERVOS;i++)
          FABLE_InfoParameter.OpenAngleTarget[i]=ioCmd[i+1];
       break;
    }
    case SET_CLOSE_ANG_TARGET:{
        for (int i=0;i<MAX_SERVOS;i++)
          FABLE_InfoParameter.CloseAngleTarget[i]=ioCmd[i+1];
       break;
    }
    case WRITE_DEFAULT_PARAM:{
      SetDefaultParameter();
      EEPROM_writeAnything(0,FABLE_DefaultValue);
    break;
    }
    case READ_EE_PARAM:{
      EEPROM_readAnything(0, FABLE_InfoParameter);
      break;
    }
    case UPDATE_EE_PARAM:{
      EEPROM_writeAnything(0, FABLE_InfoParameter);
      break;
    }
    case DIAGMODE_OFF:{
	   DiagMode=false;
        Serial.println("Diagnostic mode Stop");
      break;   
    }
    case GET_SW_VER:{
      paramCnt=7;
      customParams[0]=GET_SW_VER;
      for(int i=1;i<paramCnt;i++){
        customParams[i]=SW_VER[i-1];
      }
      
      MFL_SendMessage();
      break;
    }
    case GET_HW_VER:{
      paramCnt=7;
      customParams[0]=GET_HW_VER;
      for(int i=1;i<paramCnt;i++){
        customParams[i]=HW_VER[i-1];
      }
      
      MFL_SendMessage();
      break;
    }
    case SERVO_CALIB:{
      Servo_calibration();
      break;
    }
    case AVR_RST:{
        Reset_AVR();
        break;
    }
    case MACRO_TEST:{
        SimButton[ioCmd[1]].BtnIndex=ioCmd[1];
        SimButton[ioCmd[1]].buttonState=ioCmd[2];
        MacroManager(0);
        break;
    }
    default:{
      sendError(ERROR_UNKOWN_CMD);
    }


  }
}

//update servo status
ServoTestManager(loopCount);

}

void sendError(int type){
  // Serial.print ("WRONG Command:");
  Serial.print(MAGIC_NUMBER+1);
  Serial.print(","); // size = 1 FN + 1 TYPE
  Serial.print(2);
  Serial.print(",");
  Serial.print(COMM_ERROR);
  Serial.print(",");
  Serial.println(type);
}

void sendAck(){
  //Serial.print ("ACKmess:");
  Serial.print(MAGIC_NUMBER+1);
  Serial.print(","); // size = 1 FN + 1 TYPE
  Serial.print(2);
  Serial.print(",");
  Serial.print(ioCmd[0]);
  Serial.print(",");
  Serial.println(COMM_OK);
  
}

void MFL_SendMessage(){
   
  Serial.print(MAGIC_NUMBER+1);
  Serial.print(",");
  Serial.print(paramCnt); 
  
  for (int i = 0; i < paramCnt; ++i){
    Serial.print(",");
    Serial.print(customParams[i],DEC);
  }
     Serial.println();
     paramCnt = 0;
  
}

boolean EnterDiag (){
  int Diagtimer=10000;
  
  int StartTime=millis();
  Serial.println("Press d to enter in diag mode(10 sec)");
  //Serial.println("Press d to enter in diag mode (10 sec)");

  while((millis()-StartTime)<Diagtimer){
     if (Serial.available()>0){
       if (Serial.read()=='d'){
       Serial.println("Diagnostic mode enabled");
         return true;
       }
     } 
  
  }return false;

}
void SendGUIEMGData(float LastEmgValue){
   
  int timestamp;
  //aggiorna l'array dei dati
  if (EmgCounterData<=MaxEMGBufferSize){
      customParams[EmgCounterData]=LastEmgValue;
      //strcat(customParams,itoa(LastEmgValue,customParams,10));
      EmgCounterData++;
  }else{
    timestamp=millis();
    //strcat(customParams,itoa(timestamp,customParams,10));
    customParams[0]=EMG_DATA;
    customParams[EmgCounterData]=millis();
    paramCnt=EmgCounterData+1;
    MFL_SendMessage();
    EmgCounterData=0;

}
//Serial.println(EmgCounterData);
//MFL_SendMessage();
}
void SendGUIUserMessage(char* UserMessage){

  Serial.print(MAGIC_NUMBER+1);
  Serial.print (",");
  Serial.print(USER_MESSAGE);
  Serial.print (",");
  Serial.print(UserMessage);
  Serial.println();

}
