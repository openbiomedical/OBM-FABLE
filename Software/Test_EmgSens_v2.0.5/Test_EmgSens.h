//DEFINE Constant and versions
#define SW_VER  "V2.0.5"
#define HW_VER   "V2.0.0"

#define serial_baude_rate                  115200
#define MaxEMGBufferSize                       20
// Command Definition
#define MAGIC_NUMBER  	                      170 // 10101010
#define USER_MESSAGE                          150
#define EMG_DATA                              160
#define GET_SW_VER                             81
#define GET_HW_VER                             80

#define MAX_SERVOS                              6
#define ANALOG_PIN_COUNT                        5
#define MAX_BUTTONS                             1

// error types
#define ERROR_SERIAL			        1
#define ERROR_UNKOWN_CMD		        2
#define COMM_ERROR			        29
#define COMM_OK                                 25

// servo event types
#define  SERVO_EVENT_STOPPED                    1
#define  SERVO_EVENT_POSITION_UPDATE 	        2
#define  SERVO_EVENTS_ENABLE	                40
#define  SERVO_EVENT			        41

#define SERVO_ATTACH         			6
#define SERVO_WRITE          			7
#define SERVO_WRITE_MICROSECONDS		8
#define SERVO_DETACH         			9
#define SET_SERVO_SPEED           		10
#define SERVO_SWEEP_START		        20
#define SERVO_SWEEP_STOP		        30

#define EMG_CAL_MIN                             50
#define EMG_CAL_MAX                             60
#define EMG_CAL_THR                             70
#define SERVO_CALIB                             90

#define DIAGMODE_OFF                            170
#define EMG_CALIB                               100
#define SET_OPEN_ANG_TARGET                     110
#define SET_CLOSE_ANG_TARGET                    120
#define SET_CSENS_THR                           130
#define SET_FILTER_TAU                          140
//EEPROM FUNCTION
#define WRITE_DEFAULT_PARAM                     200
#define UPDATE_EE_PARAM                         201
#define READ_EE_PARAM                           202
#define AVR_RST                                 250
#define MACRO_TEST                              210



#define MAX_TRIG_TIME                400
#define MAX_TRIG_EVENTS               5 

#define Reset_AVR() wdt_enable(WDTO_30MS); while(1) {} 


// Servos
struct servo_type
{
    Servo* servo;
    int Servoindex; // index of this servo
    int speed;
    int targetPos;
    int currentPos;
    bool isMoving;
    //int targetPos_Open;
    //int targetPos_Close;
    int step; // affects speed usually 1
    
    // sweep related
    int min;
    int max;
    // int delay; - related to speed
    int increment;
    bool isSweeping;
    // event related
    bool eventsEnabled;
};


/*
typedef struct HandServo{
int ServoPin;
int OpenedAngle;
int ClosedAngle;
int ServoTimer;
int DeltaAngle;
}HandServo;
*/
typedef struct Button{
  int BtnIndex;
  int buttonState;
  int lastButtonState;
  long lastDebounceTime=0;
  long debounceDelay=50;
}Button_Info;


typedef struct EMG_info {
int Channel;
int minimum;
int maximum;
int value;
int emg_thr;
}EMG_info;


typedef struct FableEEInfo {
  unsigned long LevelShift;
  unsigned long OverRatio;
  unsigned long CalibrationTime;
  float tau;
  float csens_thr;
  int OvercurrentAngle;
  int OpenAngleTarget[MAX_SERVOS];
  int CloseAngleTarget[MAX_SERVOS];
  
}FableEEInfo;

FableEEInfo FABLE_InfoParameter;
FableEEInfo FABLE_DefaultValue;
Button_Info FableButton[MAX_BUTTONS];
Button_Info SimButton[MAX_BUTTONS];
servo_type servos[MAX_SERVOS];


EMG_info FABLE_EMGCH0;
//HandServo FABLEServoData[numservo];
//Servo FableServo;


int byteCount;
float csens_val=0;
float EMG_Cmp=0;
int EMG_oversampled;
int time0=0,elapsedtime=0;
int looptime;
//byte serialByte;
float EMG_C;
int stab_timer=0;
int pos;
int msgSize; // the NUM_BYTES of current message
int EmgCounterData=1;
//int SimBtnStatus;
unsigned long loopCount;
unsigned long lastMicros;

unsigned char ioCmd[64];  // message buffer for all inbound messages
 unsigned char customParams[64]; // message buffer for all outbound messages
int paramCnt=0;
int readValue;
unsigned int errorCount;


