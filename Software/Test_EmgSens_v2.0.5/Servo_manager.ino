void Servo_Attach(unsigned char data[64]){

  servo_type& s = servos[data[1]];
  s.Servoindex = data[1];
  Serial.print(s.Servoindex);
  Serial.print(data[2]);
  if (s.servo == NULL){
    s.servo = new Servo();
  }
  s.servo->attach(data[2],1000,3500);
  s.step = 1;
  s.eventsEnabled = false;
}

void Servo_Sweep_Start(unsigned char data[64]){

    servo_type& s = servos[data[1]];
    s.min = data[2];
    s.max = data[3];
    s.step = data[4];
    s.isMoving = true;
    s.isSweeping = true;
}

void Servo_Sweep_Stop(unsigned char data[64]){
      servo_type& s = servos[data[1]];
      s.isMoving = false;
      s.isSweeping = false;
}

void Servo_write(unsigned char data[64]){
  servo_type& s = servos[data[1]];
  if (s.speed == 100 && s.servo != 0)// move at regular/full 100% speed
  {
     s.targetPos = data[2];
	   s.currentPos = data[2];
	   s.isMoving = false;
     s.servo->write(data[2]);
     if (s.eventsEnabled) sendServoEvent(s, SERVO_EVENT_STOPPED);
  } else if (s.speed < 100 && s.speed > 0) {
	    s.targetPos = data[2];
	    s.isMoving = true;
  }
//Serial.print("write");
//Serial.print(s.targetPos);
//Serial.println(s.speed);
}


void Servo_Write_Microseconds(unsigned char data[64]){

  servo_type& s = servos[data[1]];
  if (s.servo != 0) {
	// 1500 midpoint
	s.servo->writeMicroseconds(data[2]);
  }

}

void Set_Servo_Speed(unsigned char data[64]){
// setting the speed of a servo
  servo_type& servo = servos[data[1]];
  servo.speed = data[2];
}

void Servo_Events_Enable(unsigned char data[64]){
//enable debug write on serial
servo_type& s=servos[data[1]];
s.eventsEnabled=data[2];
}


void Servo_Detach(unsigned char data[64]){
  servo_type& s = servos[data[1]];
  if (s.servo != 0){
	s.servo->detach();
  }
}


void ServoTestManager(int loopCount){
  
  for (int i = 0; i < MAX_SERVOS; ++i)
	{
		
    servo_type& s = servos[i];
                
	    if (s.isMoving && s.servo != 0){
              //s.eventsEnabled=true;
            if (s.currentPos != s.targetPos)
	    {
				    // caclulate the appropriate modulus to drive
				    // the servo to the next position
				    // TODO - check for speed > 0 && speed < 100 - send ERROR back?
				    int speedModulus = (100 - s.speed) * 10;
				
            if (loopCount % speedModulus == 0)
		{  //if (ServoOvercurrent(i)){
                     //s.targetPos-= FABLE_InfoParameter.OvercurrentAngle;
                    //}
			int increment = s.step * ((s.currentPos < s.targetPos)?1:-1);
			// move the servo an increment
			s.currentPos = s.currentPos + increment;
			s.servo->write(s.currentPos);
			if (s.eventsEnabled) sendServoEvent(s, SERVO_EVENT_POSITION_UPDATE);
		 }
		} else {
			if (s.isSweeping) {
				if (s.targetPos == s.min){
					s.targetPos = s.max;
				} else {
					s.targetPos = s.min;
					}
				} else {
					if (s.eventsEnabled) sendServoEvent(s, SERVO_EVENT_STOPPED);
					s.isMoving = false;
				}
			}
		}s.eventsEnabled=false;
	}

}

void ServoMovement(int servoindex){
  
    
    servo_type& s = servos[servoindex];
                
    while (s.isMoving && s.servo != 0){
    loopCount++;
            s.eventsEnabled=false;
            if (s.currentPos != s.targetPos)
           {
            // caclulate the appropriate modulus to drive
            // the servo to the next position
            // TODO - check for speed > 0 && speed < 100 - send ERROR back?
            int speedModulus = (100 - s.speed) * 5;     
          if (loopCount % speedModulus == 0)
         {
            if (ServoOvercurrent(servoindex)){
                //  s.targetPos-= FABLE_InfoParameter.OvercurrentAngle;
            }
            int increment = s.step * ((s.currentPos < s.targetPos)?1:-1);
            // move the servo an increment
            s.currentPos = s.currentPos + increment;
            s.servo->write(s.currentPos);
            if (s.eventsEnabled) sendServoEvent(s, SERVO_EVENT_POSITION_UPDATE);
         }
        }
         else {
          if (s.eventsEnabled) sendServoEvent(s, SERVO_EVENT_STOPPED);
          s.isMoving = false;
        }
      }
    s.eventsEnabled=false;
} 




void OpenCloseActivation(){
    
    if (TrigEvents>MAX_TRIG_EVENTS){   
    if (hand_open){
    //close Hand
    Serial.println("CLOSE HAND");
    for (int i = 0; i < MAX_SERVOS; ++i){
      servo_type& s = servos[i];
      ioCmd[0]=0;ioCmd[1]=i;ioCmd[2]=FABLE_InfoParameter.CloseAngleTarget[i];
     // Serial.println(FABLE_InfoParameter.CloseAngleTarget[i]);
      Servo_write(ioCmd);
      ServoMovement(i);
    }
    hand_open=false;
    }else{
    Serial.println("OPEN HAND");
    for (int i = MAX_SERVOS; i > 0; --i){
      servo_type& s = servos[i];
      ioCmd[0]=0;ioCmd[1]=i;ioCmd[2]=FABLE_InfoParameter.OpenAngleTarget[i];
      //Serial.println(FABLE_InfoParameter.OpenAngleTarget[i]);
      Servo_write(ioCmd);
      ServoMovement(i);
  }
  hand_open=true;}
 }

}

void MacroKeyActivation(){
  //Close
  
   if (TrigEvents>MAX_TRIG_EVENTS){   
    if (hand_open){
    //close Pollice
    Serial.println("Close Poll");
      servo_type& s = servos[MAX_SERVOS-1];
      ioCmd[0]=0;ioCmd[1]=MAX_SERVOS-1;ioCmd[2]=FABLE_InfoParameter.CloseAngleTarget[MAX_SERVOS-1];
     // Serial.println(FABLE_InfoParameter.CloseAngleTarget[i]);
      Servo_write(ioCmd);
      ServoMovement(MAX_SERVOS-1);
   hand_open=false;
    }
    
    else{
    Serial.println("Open Poll");
      servo_type& s = servos[MAX_SERVOS-1];
      ioCmd[0]=0;ioCmd[1]=MAX_SERVOS-1;ioCmd[2]=FABLE_InfoParameter.OpenAngleTarget[MAX_SERVOS-1];
      //Serial.println(FABLE_InfoParameter.OpenAngleTarget[i]);
      Servo_write(ioCmd);
      ServoMovement(MAX_SERVOS-1);
      hand_open=true;
    }
  }
 
 
 

}


void sendServoEvent(servo_type& s, int eventType){
  	// check type of event - STOP vs CURRENT POS

	Serial.print(MAGIC_NUMBER);
	Serial.print(5); // size = 1 FN + 1 INDEX + 1 eventType + 1 curPos
	Serial.print(SERVO_EVENT);
	Serial.print(s.Servoindex); // send my index
	// write the long value out
	Serial.print(eventType);
	Serial.print(s.currentPos);
        Serial.println(s.targetPos);
        
    
}
// Servo Overcurrent manager //
boolean ServoOvercurrent(int servoindex){
  
 csens_val=analogRead(csens_pin[servoindex]);
  if (csens_val>FABLE_InfoParameter.csens_thr)
     return true;
 else
     return false;

}

//MACRO MANAGER//
void MacroManager(int caller){
int BtnReading;

 
  
 for (int i = 0; i < MAX_BUTTONS; ++i){
    switch(caller){
     case 1:{ 
     BtnReading = digitalRead(Button_Pin[i]);
     break;}
     case 0:{
       if (i==SimButton[i].BtnIndex)
           BtnReading=SimButton[i].buttonState;
       break;}
    }
  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH),  and you've waited
  // long enough since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
//  if (BtnReading != FableButton[i].lastButtonState) {
//    // reset the debouncing timer
//      FableButton[i].lastDebounceTime = millis();
//   }
//   if ((millis() - FableButton[i].lastDebounceTime) > FableButton[i].debounceDelay) {
//////    // whatever the reading is at, it's been there for longer
//////    // than the debounce delay, so take it as the actual current state:
//////    
//    
//////    // if the button state has changed:
      if (BtnReading != FableButton[i].buttonState) {
      FableButton[i].buttonState = BtnReading;
      }
// }
  
 }
 
 
 Serial.print("ButtonState");
 Serial.println(FableButton[0].buttonState);
  //insert here the macro manager
  if (MacroKey==false){
  if ((FableButton[0].buttonState)==HIGH){
     
          MacroKey=true;
          Serial.println("MACRO_ON");  
          for (int i = 0; i < MAX_SERVOS-1; ++i){
            servo_type& s = servos[i];
            ioCmd[0]=0;ioCmd[1]=i;
            if (i==0)
                ioCmd[2]=FABLE_InfoParameter.OpenAngleTarget[i];
            else
                ioCmd[2]=FABLE_InfoParameter.CloseAngleTarget[i];
                Servo_write(ioCmd);
                ServoMovement(i);
              }
              }else{
             OpenCloseActivation(); }
  }
     // disattivazione della macro  
              
   else{
         if ((FableButton[0].buttonState)==HIGH){
            MacroKey=false;
            //Reset open position
            for (int i = 0; i < MAX_SERVOS-1; ++i){
            servo_type& s = servos[i];
            ioCmd[0]=0;ioCmd[1]=i;
            if (i==0)
                ioCmd[2]=FABLE_InfoParameter.CloseAngleTarget[i];
            else
                ioCmd[2]=FABLE_InfoParameter.OpenAngleTarget[i];
                Servo_write(ioCmd);
                ServoMovement(i);
              }
              hand_open=true;
             Serial.println("MACRO_OFF");
             
         } else{
     MacroKeyActivation();
   }
  }
  FableButton[0].lastButtonState=FableButton[0].buttonState;    
}

