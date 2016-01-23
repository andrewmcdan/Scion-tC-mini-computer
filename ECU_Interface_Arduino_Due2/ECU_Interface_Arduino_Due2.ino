#include "defines.h"
#include <Wire.h>
extern TwoWire Wire1;

unsigned long tierOneCounter = 0, tierTwoCounter = 0, tierThreeCounter = 0;
unsigned long tierFourCounter = 0, tierFiveCounter = 0, timer = micros();
unsigned short timeDiff;
short maxMPH = 0;
char c[50];
char otherCharArray[9] = "xxxxxxxx";
char* dataToSendi2c[] = {otherCharArray,otherCharArray,otherCharArray,otherCharArray,otherCharArray};
char dataToSendi2cBuff[9] = "xxxxxxxx";
byte fanSpeed = 0, temperature = 0, airDestination = 0, colorRGBr = 0, colorRGBg = 0, colorRGBb = 0, MilesPerHour = 0, MilesPerHourTemp = 0, EngineTemp1 = 0, TransTemp1 = 0, VehVolts = 0, v12volts = 0, v5volts = 0, v33volts = 0, vBattvolts = 0, masterVolume = 0, subVolume = 0, AudioSource=0;
byte dataToSendi2cFlag = 0;
bool RasPiDataReq=false,reCirc=false,AC=false,rearDef=false,fogLights=false,groundLights=false,groundBlinkers=false,grill=false,halos=false,VSC=false,trac=false,AirBagOnIndc8or=false,AirBagOffIndc8or=false,securityIndc8tor=false;
short engineRPM = 0, engineRPMtemp = 0;
char* OBDMessagesToSend[] = {"    ","    ","    ","    ","    "};
byte OBDMessagesToSendCount = 0;
bool waitingForOBDResponse = false;
int OBDResponseCallbackIndex[] = {0,0,0,0,0};


void setup() {
    Wire1.begin(2);
    Wire1.onRequest(requestEvent);
    Wire1.onReceive(receiveEvent);
    serialDebug.begin(115200);
    pinMode(DueInt, INPUT);
    pinMode(DueInt, OUTPUT);
    digitalWrite(DueInt, LOW);
    pinMode(DueRTS, INPUT);
    pinMode(DueRTS, OUTPUT);
    digitalWrite(DueRTS, HIGH);
    pinMode(44, INPUT);
    pinMode(44, OUTPUT);
    digitalWrite(44, HIGH);
    pinMode(RasPitoDueDataReq, INPUT);
    analogWriteResolution(12);
    analogReadResolution(12);
    analogWrite(DAC0, 4095);
    analogWrite(DAC1, 0);
    digitalWrite(DueInt, HIGH); // sends the default "xxxxxxxx" to the raspi to signal that a reset has occurred
    if(!STN1110Setup()){
        // do error thing
    }

    dataToSendi2cFlag = 0;
}

void loop() {
    digitalWrite(42, !digitalRead(42));
    //  timing tiers:    tierOneCounter - 10ms
    //                   tierTwoCounter - 40ms
    //                   tierThreeCounter - 75ms
    //                   tierFourCounter - 1s
    //                   tierFiveCounter - 2.5s
    timeDiff = micros()<timer?4294967295-timer+micros():micros()-timer+5;
    timer=micros();
    tierOneCounter += timeDiff;
    tierTwoCounter += timeDiff;
    tierThreeCounter += timeDiff;
    tierFourCounter += timeDiff;
    tierFiveCounter += timeDiff;

    if(tierOneCounter>5000){
        tierOneCounter=0;
        // do teir one tasks
        /*if(digitalRead(RasPitoDueDataReq)){
        RasPiDataReq = true;
        //}*/

        if(dataToSendi2cFlag!=0){
            addDataToRasPi();
            bool a = true;
            while(a){
                for(int b=0;b<5;b++){
                    if(bitRead(dataToSendi2cFlag,b)==1){
                        for(int i=0;i<9;i++){
                            dataToSendi2cBuff[i] = dataToSendi2c[b][i];
                        }
                        bitClear(dataToSendi2cFlag,b);
                        digitalWrite(DueInt, HIGH);
                        a = false;
                        break;
                    }
                }
            }
        }

        if((OBDMessagesToSendCount!=0)&&(!waitingForOBDResponse)){
            AsyncOBDCANmessageSendMessage();
        }
        if(waitingForOBDResponse && serialOBDCAN.available()){
            AsyncOBDCANmessageRecieved();
        }
    }
    if(tierTwoCounter>25000){
        tierTwoCounter=0;
        // do teir two tasks

        // update hardware from variables
    }
    if(tierThreeCounter>100000){
        tierThreeCounter=0;
        // do teir three tasks
        if(OBDMessagesToSendCount<3){
            AsyncOBDCANmessageAddMessageToSend("010d",0); // Speed   -  123489
            AsyncOBDCANmessageAddMessageToSend("010c",1); // Tach    -  12342700
        }
    }
    if(tierFourCounter>1000000){
        tierFourCounter=0;
        // do teir four tasks
        serialDebug.println(MilesPerHour);
        serialDebug.println(engineRPM);
        serialDebug.println(" ");
    }
    if(tierFiveCounter>2500000){
        tierFiveCounter=0;
        // do teir five tasks
    }
}
