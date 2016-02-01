/*
// Volume
void increase(void);
void decrease(void);
void mute(void);

// Playback
void play(void);
void pause(void);
void stop(void);

// Track Controls
void next(void);
void previous(void);
void forward(void);
void rewind(void);

// Send an empty report to prevent repeated actions
void clear(void);
*/

int strnWhlCntrlVal1 = 0,strnWhlCntrlVal2 = 0;

void setup() {
    Serial.begin(115200); // Debug over USB
    Serial1.begin(115200); // Teensy
    pinMode(2, OUTPUT); // trigger to write to Due
    digitalWrite(2, LOW);
    pinMode(3, OUTPUT); // trigger to write to BT module
    digitalWrite(3, LOW);
    pinMode(4, INPUT);
}

void loop() {
    //Remote.next();
    //delay(1);
    //Remote.clear();
    strnWhlCntrlVal1=analogRead(A0);
    if(strnWhlCntrlVal1!=strnWhlCntrlVal2){
        strnWhlCntrlVal2=strnWhlCntrlVal1;
    }
    String res = SendMessageToTeensy("D",3); // send "D" to Bt module and set response to "res"
    res = SendMessageToTeensy("abcd",2); // send "abcd" to Due and set response to "res"
}

String SendMessageToTeensy(String message,int DueOrBT){
    //////  send message to teensy
    String incoming = "";
    char inChar = ' ';
    Serial1.print(message); // put data into teensy's buffer
    digitalWrite(DueOrBT,HIGH); // interrupt the teensy
    while(digitalRead(4)){} // wait for the teensy to put data into promicro's buffer
    digitalWrite(DueOrBT,LOW); // prevent furthur interrupt of teensy
    while(Serial1.available()){
        inChar = Serial1.read(); // read data from teensy
        incoming += inChar;
    }

}
