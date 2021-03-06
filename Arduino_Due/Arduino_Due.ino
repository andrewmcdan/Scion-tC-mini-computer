#include "header.h"
#include <mcp_can.h>
#include <SPI.h>
#include <Servo.h>
#include <Wire.h>
extern TwoWire Wire1;

Servo AirTempServo,AirDestServo,SubVolServo;

MCP_CAN CustomCAN0(9);  //  see end of file for message id info

void setup() {
    ///  just a bit to overclock the Due  ///////////////////
    #define SYS_BOARD_PLLAR (CKGR_PLLAR_ONE | CKGR_PLLAR_MULA(15UL) | CKGR_PLLAR_PLLACOUNT(0x3fUL) | CKGR_PLLAR_DIVA(1UL))
    #define SYS_BOARD_MCKR ( PMC_MCKR_PRES_CLK_2 | PMC_MCKR_CSS_PLLA_CLK)
    EFC0->EEFC_FMR = EEFC_FMR_FWS(4);
    EFC1->EEFC_FMR = EEFC_FMR_FWS(4);
    PMC->CKGR_PLLAR = SYS_BOARD_PLLAR;
    while (!(PMC->PMC_SR & PMC_SR_LOCKA)) {}
    PMC->PMC_MCKR = SYS_BOARD_MCKR;
    while (!(PMC->PMC_SR & PMC_SR_MCKRDY)) {}
    SystemCoreClockUpdate();
    ////////////////////////////////////////////////////////

    I2Csetup();
    PinsSetup();
    CustomCANbusSetup();
    ServoSetup();
    SerialSetup();

    for(int i=22;i<=35;i++){
        digitalRead(i)?bitSet(digitalInputs,i-22):bitClear(digitalInputs,i-22);
    }
}

void loop() {
    /////  testing //////////////////
    digitalWrite(42, !digitalRead(42));
    /////  testing //////////////////

    if((OBDMessagesToSendCount!=0)&&(!waitingForOBDResponse)){
        AsyncOBDCANmessageSendMessage();
    }
    if(waitingForOBDResponse && serialOBDCAN.available()){
        AsyncOBDCANmessageRecieved();
    }

    if(serialTeensy.available()){
        ReceiveTeensySerial();
    }


    //  timing tiers:    tierOneCounter - 10ms
    //                   tierTwoCounter - 40ms
    //                   tierThreeCounter - 75ms
    //                   tierFourCounter - 1s
    //                   tierFiveCounter - 2.5s
    tempTimer = micros();
    timeDiff = tempTimer<timer?4294967295-timer+tempTimer:tempTimer-timer+5;
    timer=micros();
    tierOneCounter += timeDiff;
    tierTwoCounter += timeDiff;
    tierThreeCounter += timeDiff;
    tierFourCounter += timeDiff;
    tierFiveCounter += timeDiff;

    if(tierOneCounter>4000){
        tierOneCounter=0;
        //////  do tier one tasks  //////
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

        // Read digital inputs and compare for changes.
        for(int i=22;i<=35;i++){
            digitalRead(i)?bitSet(digitalInputs,i-22):bitClear(digitalInputs,i-22);
        }


        ///////////////////////
        //CustomCAN0.sendMsgBuf(rxID, 0, 1, rxBuf);
        /////////////////////////
    }
    if(tierTwoCounter>20000){
        tierTwoCounter=0;
        //////  do teir two tasks  //////
        /////////////////// update hardware from variables ////////////////////////////////////////////////////////////
        // Update servo positions for airTemp and airDest
        AirTempServo.write(constrain((((map(analogRead(AirTempPotPin),0,4095,0,255)-temperature)*1.8)+90),0,180));
        //AirTempServo.write(90);
        AirDestServo.write(constrain((((map(analogRead(AirDestPotPin),0,4095,0,255)-airDestination)*4)+90),0,180));
        SubVolServo.write(180);

        // send update to Teensy for all outputs
        SendMessageToTeensy(TeensyMessageID_BlinkerState,blinkerState++);

        // send volujme to can bus

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }
    if(tierThreeCounter>100000){
        tierThreeCounter=0;
        //////  do teir three tasks  //////
        if(OBDMessagesToSendCount<3){
            AsyncOBDCANmessageAddMessageToSend("010d",0); // Speed   -  123489
            AsyncOBDCANmessageAddMessageToSend("010c",1); // Tach    -  12342700
        }



        /////  testing //////////////////
        rxBuf[0] = pattern;
        rxID = rxID==0x5ff?0x500:rxID+1;
        if(newPattern){
            CustomCAN0.sendMsgBuf(rxID, 0, 1, rxBuf);
            newPattern = false;
        }


    }
    if(tierFourCounter>500000){
        tierFourCounter=0;
        //////  do teir four tasks  //////

        /////  testing //////////////////
        CustomCAN0.sendMsgBuf(rxID, 0, 1, rxBuf);
        /////  testing //////////////////

    }
    if(tierFiveCounter>2500000){
        tierFiveCounter=0;
        //////  do teir five tasks  //////
        i2cresetCounter += digitalRead(DueInt)?1:0;
        if(i2cresetCounter>5){digitalWrite(DueInt, LOW);}
        /////  testing //////////////////
        sendManyVariablesSerialDebug();
        /////  testing //////////////////
    }
}


/*   ////     Info on ID's and messages for CustomCANbus      ////

/////////////////  Color updates and timing

Mask for filtering message ID's - 0x7F0

Filter ID   range   Purpose
0x000   ->  0x00F   Master timing broadcast including turn signal status and volume updates
                    0x001 -  high resolution master timing
                    0x002 -> 0x00e - turn signals, fog states, brake lights, etc 
                    byte structure
                    byte 0:
                    0:1 - turn signal state
                    2:3 - fog light state
                    4:7 - brakes
                    byte 1:
                    byte 2:
                    byte 3:
                    
                    0x00f - volume
                    structure
                    byte 0: Master Volume left
                    byte 1: Master Volume right
                    byte 2: Sub Vol left
                    byte 3: Sub Vol right

0x010   ->  0x01F   Color group Master
0x020   ->  0x02F   Color group 1
0x030   ->  0x03F   Color group 2
0x040   ->  0x04F   Color group 3
|           |         |
V           V         V
0x2F0   ->  0x2FF   Color group 46

0x300   ->  0x6FF   Not currently Used

0x700   ->  0x7ff   Status updates
                    0x700 - Left fog light
                    0x701 - right fog light
                    0x702 - volume controller
Status updates will be perioducally transmitted by modules.
The ID that they send it to will identify which module the
message is being sent from. Status updates are transmitted
the lowest priority ID so that they don't interfere with the
timing of other messages.


*/
