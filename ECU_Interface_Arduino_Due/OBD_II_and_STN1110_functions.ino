void STN1110Setup(){
  delay(3000);
  serialOBDCAN.begin(460800);
  delay(10);
  serialOBDCAN.println(' ');
  delay(1);
  serialOBDCAN.println("atz");
  delay(1100);
  serialOBDCAN.println("ate0");
  delay(1);
  serialOBDCAN.println("ath0");
  delay(1);
  serialOBDCAN.println("atl0");
  delay(1);
  serialOBDCAN.println("ats0");
  delay(1);
  serialOBDCAN.println("atl0");
  delay(1);
  unsigned long timeoutStartTime = millis();
  while(serialOBDCAN.available()>0){
    char inChar = serialOBDCAN.read();
    //serialDebug.write(inChar);
    while(((serialOBDCAN.available() == 0 )&&(char(inChar)!='>'))/*&&((millis()-timeoutStartTime)<100)*/){}
  }
  serialOBDCAN.println("ate0");
  delay(1);
  String inString = "";
  timeoutStartTime = millis();
  while(serialOBDCAN.available()>0){
    //delayMicroseconds(25);
    int inChar = serialOBDCAN.read();
    inString += (char)inChar;
    while(((serialOBDCAN.available() == 0 )&&(char(inChar)!='>'))/*&&((millis()-timeoutStartTime)<100)*/){}
  }
  String testString = "OK";
  testString += (char)0x0d;
  testString += (char)0x0d;
  testString += ">";
  if(inString != testString){
    delay(1);
  }
}

short EngineTemp(){
  String textSpeed = OBDCANmessage("0105");
  char s[16];
  textSpeed.toCharArray(s,16);
  short yy;
  sscanf(s, "%*4x%2x", &yy);
  return ((yy-40)*(9/5))+32;  //  °C  x  9/5 + 32 = °F
}

short TransTemp(){
  String textSpeed = OBDCANmessage("015c");
  char s[16];
  textSpeed.toCharArray(s,16);
  short yy;
  sscanf(s, "%*4x%2x", &yy);
  return ((yy-40)*(9/5))+32;  //  °C  x  9/5 + 32 = °F
}

short VehicleSpeed(){
  String textSpeed = OBDCANmessage("010d");
  char s[16];
  textSpeed.toCharArray(s,16);
  short yy;
  sscanf(s, "%*4x%2x", &yy);
  return yy*0.621371;
}

short EngineRPM(){
  String textRPM = OBDCANmessage("010c");
  char s[18];
  textRPM.toCharArray(s,20);
  short xx;
  short yy;
  sscanf(s, "%*4x%2x%2x", &yy, &xx);
  return ((yy*256)+xx)/4;
}

String OBDCANmessage(String CANmessage){ // sends a text string to CAN controller and returns the response as a string
  String incoming = "";
  unsigned long messageStartTime = millis();
  while(serialOBDCAN.available()>0){
    serialOBDCAN.read();
  }
  serialOBDCAN.println(CANmessage);
  while((serialOBDCAN.available()==0)/*&&((millis()-messageStartTime)<500)*/){}
  while(serialOBDCAN.available()>0){
    int inChar = serialOBDCAN.read();
    incoming += (char)inChar;
    while(((serialOBDCAN.available() == 0 )&&(char(inChar)!='>'))/*&&((millis()-messageStartTime)<100)*/){}
  }
  return incoming;
}

void fakeECUinfo(byte *MPH, byte *TEMPERATURE, byte *TEMP, byte *TransTEMP, short *RPM, short *AFR, short *fRATE){
  *MPH=random(99);
  *RPM=random(8000);
  *TEMPERATURE=random(255);
  *TransTEMP=random(255);
}
