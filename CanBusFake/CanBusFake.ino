void setup() {
    Serial.begin(115200);
    delay(5000);
}

void loop() {

    if(Serial.available()>0){
        String incoming = "";
        while(Serial.available()>0){
            int inChar = Serial.read();
            incoming += (char)inChar;
            while(((Serial.available() == 0 )&&(char(inChar)!='\n'))){}
        }
        if(incoming.substring(0,4)=="010c"){
            Serial.print(random(36000),HEX);
            Serial.print(">");
        }else if(incoming.substring(0,4)=="010d"){
            Serial.print(random(100),HEX);
            Serial.print(">");
        }else{
            Serial.print("12345678>");
        }
    }
}
