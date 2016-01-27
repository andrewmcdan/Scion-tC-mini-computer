#define serialOBDCAN Serial1
#define serialDebug Serial
#define serialLeonardo Serial3
#define serialTeensy Serial2
#define DueInt 41
#define DueRTS 42
#define RasPitoDueDataReq 43
#define AirTempServoPin 2
#define AirTempPotPin A0
#define AirDestServoPin 5
#define AirDestPotPin A2
#define SubVolServoPin 3
#define SubVolPotPin A1
#define MCP23017IntPin 40
#define MPHflagMask 0
#define RPMflagMask 1
#define AirDestFlagMask 2
#define AirTempFlagMask 3
#define CustomCAN0CSPin 9
#define CustomCAN0IntPin 8

unsigned long
tierOneCounter = 0,
tierTwoCounter = 0,
tierThreeCounter = 0,
tierFourCounter = 0,
tierFiveCounter = 0,
timer = micros(),
tempTimer = 0;

unsigned short timeDiff;

short
maxMPH = 0,
digitalInputs = 0;

char
c[50],
otherCharArray[9] = "xxxxxxxx",
dataToSendi2cBuff[9] = "xxxxxxxx";

long unsigned int rxID = 0x500;

unsigned char
len = 8,
rxBuf[8] = {0, 1, 2, 3, 4, 5, 6, 7};

char* dataToSendi2c[] = {otherCharArray,otherCharArray,otherCharArray,otherCharArray,otherCharArray};
char* OBDMessagesToSend[] = {"    ","    ","    ","    ","    "};

byte
fanSpeed = 0,
temperature = 0,
airDestination = 0,
colorRGBr = 0,
colorRGBg = 0,
colorRGBb = 0,
MilesPerHour = 0,
MilesPerHourTemp = 0,
EngineTemp1 = 0,
TransTemp1 = 0,
VehVolts = 0,
v12volts = 0,
v5volts = 0,
v33volts = 0,
vBattvolts = 0,
masterVolume = 0,
subVolume = 0,
AudioSource=0,
dataToSendi2cFlag = 0,
OBDMessagesToSendCount = 0,
pattern = 0;

bool
RasPiDataReq=false,
reCirc=false,
AC=false,
rearDef=false,
fogLights=false,
groundLights=false,
groundBlinkers=false,
grill=false,
halos=false,
VSC=false,
trac=false,
AirBagOnIndc8or=false,
AirBagOffIndc8or=false,
securityIndc8tor=false,
waitingForOBDResponse = false,
newPattern = false;

long engineRPM = 0, engineRPMtemp = 0;

int
OBDResponseCallbackIndex[] = {0,0,0,0,0};
int
i2cresetCounter = 0;
