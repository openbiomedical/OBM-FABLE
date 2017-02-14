// Define constant
#define MAX_BUF 64
#define Baude_Rate 115200
#define NumServo  5
//#define MYSERIAL Serial
#define BLANK (-1)


// COMMAND DEFINITION
/* MOVEMENT CONTROL*/ 
#define SET_OPEN_ANGLE_TARGET   110
#define SET_CLOSE_ANGLE_TARGET  120
#define SPEED                   130

/*EEPROM FUNCTION*/
#define WRITE_DEFAULT_PARAM     200
#define UPDATE_EE_PARAM         201
#define READ_EE_PARAM           202
#define FABLE_SETTING_LOC       0
/*MACRO FUNCTION*/
#define MACRO_TEST              210  
#define START_DEMO              216
#define STOP_DEMO               217
#define SERVOANGLE_MIN          10
#define SERVOANGLE_MAX          180

const int  OpenSeq[3]={1,2,3};
const int  CloseSeq[3]={3,2,1};
boolean isGUIpresent=false;
float EMG_C;
//ADAFruit CAP sensor


// For I2C, connect SDA to your Arduino's SDA pin, SCL to SCL pin
// On UNO/Duemilanove/etc, SDA == Analog 4, SCL == Analog 5
// On Leonardo/Micro, SDA == Digital 2, SCL == Digital 3
// On Mega/ADK/Due, SDA == Digital 20, SCL == Digital 

// Use I2C, no reset pin!
//Adafruit_CAP1188 cap = Adafruit_CAP1188();

// Or...Use I2C, with reset pin


//SERIAL CONTROL
struct serialCmdType
{
  char cmdBuff[MAX_BUF];
  char *strPtr;
  int cmdToGet=0;
  int instructionFlag;
  int demoFlag;
  int FingerNum;
  int AdaSensitivityAdjust;
  int MSpeed;
  int StopPos;
  int Mdirection;
 };
 struct FableEEInfoType {

  int tau;
  int CalibrationTime;
  int OverRatio;
  int LevelShift;
  int csens_thr;
  int CAPSens;
  int Speed=50;
  int OvercurrentAngle;
  int OpenAngleTarget[NumServo];
  int CloseAngleTarget[NumServo];
  int FingerEnabled[5];
  int FingerDelay=500;
  int isUpdated;
};
struct ArmonicMov{  
float sen=0;
int Direction;
int Pos[NumServo]={0};
float angolo[NumServo]={0};
float grado[NumServo]={0};  
 };
 
 struct EMG_info {
int Channel;
int minimum;
int maximum;
int value;
int emg_thr;
};

 int servo_pin[NumServo]={3,6,9,10,11};
 int csens_Pin[NumServo]={A1,A2,A3,A4,A5};
  
Servo FABLE_Servo[NumServo];

 struct ArmonicMov ArmoMov;
 struct serialCmdType SerialCmd;
 struct FableEEInfoType FABLE_InfoParameter;
 struct FableEEInfoType FABLE_DefaultParameter;
 struct EMG_info FABLE_EMGCH0;



 
