typedef void (*GenFunct) (String);
GenFunct OBDResponseParser [] = {UpdateVehicleSpeed,UpdateTach};

bool STN1110Setup(){
    delay(3000);
    serialOBDCAN.begin(1500000); // 460800
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
        while(((serialOBDCAN.available() == 0 )&&(char(inChar)!='>'))&&((millis()-timeoutStartTime)<100)){
            if(inChar!='>'){
                return false;
            }
        }
    }
    serialOBDCAN.println("ate0");
    delay(1);
    String inString = "";
    timeoutStartTime = millis();
    while(serialOBDCAN.available()>0){
        //delayMicroseconds(25);
        int inChar = serialOBDCAN.read();
        inString += (char)inChar;
        while(((serialOBDCAN.available() == 0 )&&(char(inChar)!='>'))&&((millis()-timeoutStartTime)<100)){
            if(inChar!='>'){
                return false;
            }
        }
    }
    String testString = "OK";
    testString += (char)0x0d;
    testString += (char)0x0d;
    testString += ">";
    if(inString != testString){
        delay(1);
    }
    return true;
}
/*
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
*/

void UpdateVehicleSpeed(String textRes) {
    char s[3];
    textRes.substring(4,6).toCharArray(s,3);
    MilesPerHour = ((short)strtol(s,NULL,16))*0.621371;
    MilesPerHour = random(99);
    if(MilesPerHourTemp!=MilesPerHour){
        bitSet(dataToSendi2cFlag,MPHflagMask);
        MilesPerHourTemp=MilesPerHour;
    }
}

void UpdateTach(String textRes) {
    char s[5];
    textRes.substring(4,8).toCharArray(s,5);
    engineRPM = ((long)strtol(s,NULL,16))/4;
    engineRPM = random(9999);
    if(engineRPMtemp != engineRPM){
        bitSet(dataToSendi2cFlag,RPMflagMask);
        engineRPMtemp=engineRPM;
    }
}

bool AsyncOBDCANmessageAddMessageToSend(char mes[5], byte CBI /*CallBackIndex*/){
    OBDMessagesToSend[OBDMessagesToSendCount] = mes;
    OBDResponseCallbackIndex[OBDMessagesToSendCount] = CBI;
    OBDMessagesToSendCount++;
    return true;
}

void AsyncOBDCANmessageSendMessage(){
    waitingForOBDResponse = true;
    while(serialOBDCAN.available()>0){
        serialOBDCAN.read();
    }
    serialOBDCAN.println(OBDMessagesToSend[0]);
}

void AsyncOBDCANmessageRecieved(){
    String incoming = "";
    //  test  //////////////////////////////////////////////////////////////////////////////////////////
    //while(serialOBDCAN.available()>0){                                    // commented out for testing
    //int inChar = serialOBDCAN.read();                                     // commented out for testing
    //incoming += (char)inChar;                                             // commented out for testing
    //while(((serialOBDCAN.available() == 0 )&&(char(inChar)!='>'))){}      // commented out for testing
    //}                                                                     // commented out for testing
    //  test  //////////////////////////////////////////////////////////////////////////////////////////

    OBDResponseParser [OBDResponseCallbackIndex[0]] (incoming);
    for(int i=0;i<4;i++){
        OBDResponseCallbackIndex[i]=OBDResponseCallbackIndex[i+1];
    }
    OBDMessagesToSendCount--;
    for(int i=0;i<4;i++){
        OBDMessagesToSend[i]=OBDMessagesToSend[i+1];
    }
    waitingForOBDResponse = false;
}
