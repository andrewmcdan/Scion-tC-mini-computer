//#include <mcp_can.h>
//#include <SPI.h>
//int led = 23;
//unsigned char stmp[8] = {0, 1, 2, 3, 4, 5, 6, 7};
//MCP_CAN CAN0(10);
void setup() {
  //pinMode(led, OUTPUT);
  Serial.begin(250000);
  Serial1.begin(2000000);
  //if(CAN0.begin(CAN_500KBPS) == CAN_OK) Serial.print("can init ok!!\r\n");
  //else Serial.print("Can init fail!!\r\n");
}

void loop() {
    if(Serial1.available()>0){
        Serial.write(Serial1.read());
    }
    if(Serial.available()>0){
        Serial1.write(Serial.read());
    }
  //digitalWrite(led, HIGH);
  //digitalWrite(led, LOW);
  //CAN0.sendMsgBuf(0x00, 0, 8, stmp);
}
