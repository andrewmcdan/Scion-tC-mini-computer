#define TIMING_ID 0x001
#define DueSerial Serial1
#define BlueToothSerial Serial2
#define ProMicroSerial Serial3

#include <FlexCAN.h>

unsigned long
tierOneCounter = 0,
tierTwoCounter = 0,
tierThreeCounter = 0,
tierFourCounter = 0,
tierFiveCounter = 0,
timer = micros(),
timeDiff = 0,
tempTimer = 0;
FlexCAN CANbus(500000);
static CAN_message_t msg,rxmsg;
int
led = 13,
timingFrameNum = 0;
byte
blinkerState = 0,
USBswitchState = 0,
AudioSwitchState = 0;
char mesForProMicro[6];

void setup() {
    Serial.begin(115200); // debug over USB
    DueSerial.begin(2000000); // Due
    BlueToothSerial.begin(115200); // BlueTooth
    ProMicroSerial.begin(115200); // ProMicro
    CANbus.begin();
    mesForProMicro[0] = '0';
    pinMode(2, INPUT);
    pinMode(6, INPUT);
    pinMode(23,OUTPUT);
    digitalWrite(23, LOW);
}

void loop() {

    tempTimer = micros();
    timeDiff = tempTimer<timer?4294967295-timer+tempTimer:tempTimer-timer+5;
    timer=micros();
    tierOneCounter += timeDiff;
    tierTwoCounter += timeDiff;
    tierThreeCounter += timeDiff;


    ///////////////////////  tier Zero  ////////////
    if(DueSerial.available()>8){
        String incoming = "";
        char inChar = ' ';
        while(DueSerial.available()>0){
            inChar = DueSerial.read();
            incoming += inChar;
            if(inChar=='>') break;
        }
        char s[10];
        incoming.substring(0).toCharArray(s,10);
        if((s[0]^s[1]^s[2]^s[3]^s[4]^s[5]^s[6])==s[7]){
            switch(s[0]){
                case 97: // 'a' - blinkerState update
                blinkerState = s[1];
                break;
                case 98: // 'b' - USBswitchState update
                USBswitchState = s[1];
                break;
                case 99: // 'c' - AudioSwitchState update
                AudioSwitchState = s[1];
                break;
                case 100: // 'd' - mesForProMicro update
                for(int i=0;i<6;i++){
                    mesForProMicro[i] = s[i+1];
                }
                break;
                default:
                break;
            }
        }
    }

    if(digitalRead(2)){
        digitalWrite(23, HIGH);
        while(digitalRead(2)){}
        while(ProMicroSerial.available()){
            DueSerial.write(ProMicroSerial.read());
        }
        digitalWrite(23, LOW);
    }
    
    if(digitalRead(6)){
        digitalWrite(23, HIGH);
        while(digitalRead(6)){}
        while(ProMicroSerial.available()){
            BlueToothSerial.write(ProMicroSerial.read());
        }
        digitalWrite(23, LOW);
        while(digitalRead(6)){}
    }


    ////////////////////////////////////////////////

    if(tierOneCounter>21780){ // 10ms
        tierOneCounter=0;
        //////////////////////////////////////

        msg.len = 2;
        msg.id = TIMING_ID;
        msg.buf[0] = timingFrameNum==9?timingFrameNum=0,9:timingFrameNum++;
        msg.buf[1] = blinkerState;
        CANbus.write(msg);
    }
    if(tierTwoCounter>45000){ //
        tierTwoCounter=0;
        //////////////////////////////////////
        if(mesForProMicro[0]!='0'){
            mesForProMicro[0] = '0';
        }
    }
    if(tierThreeCounter>200000/*2178000*/){ // 1000ms
        tierThreeCounter=0;
        //////////////////////////////////////
    }
}
