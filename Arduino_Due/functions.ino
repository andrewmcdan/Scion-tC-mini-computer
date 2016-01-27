void PinsSetup(){
    pinMode(DueInt, INPUT);
    pinMode(DueInt, OUTPUT);
    digitalWrite(DueInt, LOW);
    pinMode(DueRTS, INPUT);
    pinMode(DueRTS, OUTPUT);
    digitalWrite(DueRTS, HIGH);
    pinMode(44, INPUT);
    pinMode(44, OUTPUT);
    digitalWrite(44, HIGH);
    pinMode(45, INPUT);
    pinMode(45, OUTPUT);
    digitalWrite(45, HIGH);
    pinMode(RasPitoDueDataReq, INPUT);
    analogWriteResolution(12);
    analogReadResolution(12);
    analogWrite(DAC0, 4095);
    analogWrite(DAC1, 0);
    digitalWrite(DueInt, HIGH);
    pinMode(CustomCAN0IntPin, INPUT);
    for(int i=22;i<=35;i++){
        pinMode(i,INPUT);
        pinMode(i,OUTPUT);
        digitalWrite(i, LOW);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////   I2C Functions   //////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void I2Csetup(){
    Wire1.begin(2);
    Wire1.onRequest(requestEvent);
    Wire1.onReceive(receiveEvent);
}

void addDataToRasPi(){
    char tempchar[9] = "x";
    String temp = "";
    if(bitRead(dataToSendi2cFlag,RPMflagMask)==1){
        temp = "b";
        temp += engineRPM;
        temp.toCharArray(tempchar,50);
        dataToSendi2c[RPMflagMask] = tempchar;
    }
    if(bitRead(dataToSendi2cFlag,MPHflagMask)==1){
        temp = "a";
        temp += MilesPerHour;
        temp.toCharArray(tempchar,50);
        dataToSendi2c[MPHflagMask] = tempchar;
    }
}

void requestEvent()
{
    //digitalWrite(42, HIGH);
    Wire1.write(dataToSendi2cBuff);
    digitalWrite(DueInt, LOW);
}

void receiveEvent(int howMany)
{
    String tempstring = "";
    char a = Wire1.read();
    switch (a){
        case 52: // "4"  hotCool
        tempstring = "";
        while(0 < Wire1.available())
        {
            tempstring += char(Wire1.read());
        }
        if(tempstring.substring(0,3)=="UUU"){
            temperature = tempstring.substring(3).toInt();
            //digitalWrite(45,!digitalRead(45));
        }
        //temperature = tempstring.toInt();
        break;
        case 53: // "5"  dest
        tempstring = "";
        while(0 < Wire1.available())
        {
            tempstring += char(Wire1.read());
        }
        if(tempstring.substring(0,3)=="UUU"){
            airDestination = tempstring.substring(3).toInt();
            //digitalWrite(45,!digitalRead(45));
        }
        //airDestination = tempstring.toInt();
        break;
        case 51: // "3"  fanSpeed
        tempstring = "";
        while(0 < Wire1.available())
        {
            tempstring += char(Wire1.read());
        }
        if(tempstring.substring(0,3)=="UUU"){
            fanSpeed = tempstring.substring(3).toInt();
            //digitalWrite(45,!digitalRead(45));
        }
        //fanSpeed = tempstring.toInt();
        break;
        case 54: // "6"  reCirc
        tempstring = "";
        while(0 < Wire1.available())
        {
            tempstring += char(Wire1.read());
        }
        if(tempstring.substring(0,3)=="UUU"){
            reCirc = tempstring.substring(3)=="true"?true:false;
            //digitalWrite(45,!digitalRead(45));
        }
        //reCirc = tempstring=="true"?true:false;
        break;
        case 55: // "7"  AC
        tempstring = "";
        while(0 < Wire1.available())
        {
            tempstring += char(Wire1.read());
        }
        if(tempstring.substring(0,3)=="UUU"){
            AC = tempstring.substring(3)=="true"?true:false;
            //digitalWrite(45,!digitalRead(45));
        }
        //AC = tempstring=="true"?true:false;
        break;
        case 56: // "8"  rearDef
        tempstring = "";
        while(0 < Wire1.available())
        {
            tempstring += char(Wire1.read());
        }
        if(tempstring.substring(0,3)=="UUU"){
            rearDef = tempstring.substring(3)=="true"?true:false;
            //digitalWrite(45,!digitalRead(45));
        }
        //rearDef = tempstring=="true"?true:false;
        break;
        case 99: // "c"  colorRGBr
        tempstring = "";
        while(0 < Wire1.available())
        {
            tempstring += char(Wire1.read());
        }
        if(tempstring.substring(0,3)=="UUU"){
            colorRGBr = tempstring.substring(3).toInt();
            //digitalWrite(45,!digitalRead(45));
        }
        //colorRGBr = tempstring.toInt();
        break;
        case 100: // "d"  colorRGBg
        tempstring = "";
        while(0 < Wire1.available())
        {
            tempstring += char(Wire1.read());
        }
        if(tempstring.substring(0,3)=="UUU"){
            colorRGBg = tempstring.substring(3).toInt();
            //digitalWrite(45,!digitalRead(45));
        }
        //colorRGBg = tempstring.toInt();
        break;
        case 101: // "e"  colorRGBb
        tempstring = "";
        while(0 < Wire1.available())
        {
            tempstring += char(Wire1.read());
        }
        if(tempstring.substring(0,3)=="UUU"){
            colorRGBb = tempstring.substring(3).toInt();
            //digitalWrite(45,!digitalRead(45));
        }
        //colorRGBb = tempstring.toInt();
        break;
        case 103: // "g"  fogLights
        tempstring = "";
        while(0 < Wire1.available())
        {
            tempstring += char(Wire1.read());
        }
        if(tempstring.substring(0,3)=="UUU"){
            fogLights = tempstring.substring(3)=="true"?true:false;
            //digitalWrite(45,!digitalRead(45));
        }
        //fogLights = tempstring=="true"?true:false;
        break;
        case 104: // "h"  groundLights
        tempstring = "";
        while(0 < Wire1.available())
        {
            tempstring += char(Wire1.read());
        }
        if(tempstring.substring(0,3)=="UUU"){
            groundLights = tempstring.substring(3)=="true"?true:false;
            //digitalWrite(45,!digitalRead(45));
        }
        //groundLights = tempstring=="true"?true:false;
        break;
        case 106: // "j"  groundBlinkers
        tempstring = "";
        while(0 < Wire1.available())
        {
            tempstring += char(Wire1.read());
        }
        if(tempstring.substring(0,3)=="UUU"){
            groundBlinkers = tempstring.substring(3)=="true"?true:false;
            //digitalWrite(45,!digitalRead(45));
        }
        //groundBlinkers = tempstring=="true"?true:false;
        break;
        case 105: // "i"  grill
        tempstring = "";
        while(0 < Wire1.available())
        {
            tempstring += char(Wire1.read());
        }
        if(tempstring.substring(0,3)=="UUU"){
            grill = tempstring.substring(3)=="true"?true:false;
            //digitalWrite(45,!digitalRead(45));
        }
        //grill = tempstring=="true"?true:false;
        break;
        case 107: // "k"  halos
        tempstring = "";
        while(0 < Wire1.available())
        {
            tempstring += char(Wire1.read());
        }
        if(tempstring.substring(0,3)=="UUU"){
            halos = tempstring.substring(3)=="true"?true:false;
            //digitalWrite(45,!digitalRead(45));
        }
        //halos = tempstring=="true"?true:false;
        break;
        case 109: // "m"  VSC
        tempstring = "";
        while(0 < Wire1.available())
        {
            tempstring += char(Wire1.read());
        }
        if(tempstring.substring(0,3)=="UUU"){
            VSC = tempstring.substring(3)=="true"?true:false;
            //digitalWrite(45,!digitalRead(45));
        }
        //VSC = tempstring=="true"?true:false;
        break;
        case 108: // "l"  trac
        tempstring = "";
        while(0 < Wire1.available())
        {
            tempstring += char(Wire1.read());
        }
        if(tempstring.substring(0,3)=="UUU"){
            trac = tempstring.substring(3)=="true"?true:false;
            //digitalWrite(45,!digitalRead(45));
        }
        //trac = tempstring=="true"?true:false;
        break;
        case 110: // "n"  masterVolume
        tempstring = "";
        while(0 < Wire1.available())
        {
            tempstring += char(Wire1.read());
        }
        if(tempstring.substring(0,3)=="UUU"){
            masterVolume = tempstring.substring(3).toInt();
            //digitalWrite(45,!digitalRead(45));
        }
        //masterVolume = tempstring.toInt();
        break;
        case 111: // "o"  subVolume
        tempstring = "";
        while(0 < Wire1.available())
        {
            tempstring += char(Wire1.read());
        }
        if(tempstring.substring(0,3)=="UUU"){
            subVolume = tempstring.substring(3).toInt();
            //digitalWrite(45,!digitalRead(45));
        }
        //subVolume = tempstring.toInt();
        break;
        case 112: // "p"  AudioSource
        tempstring = "";
        while(0 < Wire1.available())
        {
            tempstring += char(Wire1.read());
        }
        if(tempstring.substring(0,3)=="UUU"){
            AudioSource = tempstring.substring(3).toInt();
            //digitalWrite(45,!digitalRead(45));
        }
        //AudioSource = tempstring.toInt();
        break;
        case 113: // "q"  Pattern
        tempstring = "";
        while(0 < Wire1.available())
        {
            tempstring += char(Wire1.read());
        }
        if(tempstring.substring(0,3)=="UUU"){
            newPattern = pattern==tempstring.substring(3).toInt()?false:true;
            pattern = tempstring.substring(3).toInt();
        }
        break;
        default:
        break;
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////      OBDII and STN1110 Functions     ////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef void (*GenFunct) (String);
GenFunct OBDResponseParser [] = {UpdateVehicleSpeed,UpdateTach};

bool STN1110Setup(){
    delay(3000);
    serialOBDCAN.begin(2000000); // 460800
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


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////// Color Patterns ////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////// testing ////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void sendManyVariablesSerialDebug(){
    /////  testing //////////////////
    //serialDebug.println(fanSpeed);
    //serialDebug.println(temperature);
    //serialDebug.println(airDestination);
    //serialDebug.println(colorRGBr);
    //serialDebug.println(colorRGBg);
    //serialDebug.println(colorRGBb);
    //serialDebug.println(MilesPerHour);
    //serialDebug.println(MilesPerHourTemp);
    //serialDebug.println(EngineTemp1);
    //serialDebug.println(TransTemp1);
    //serialDebug.println(masterVolume);
    //serialDebug.println(subVolume);
    //serialDebug.println(AudioSource);
    //serialDebug.println(pattern);
    //serialDebug.println(reCirc);
    //serialDebug.println(AC);
    //serialDebug.println(rearDef);
    //serialDebug.println(fogLights);
    //serialDebug.println(groundLights);
    //serialDebug.println(groundBlinkers);
    //serialDebug.println(grill);
    //serialDebug.println(halos);
    //serialDebug.println(VSC);
    //serialDebug.println(trac);
    //serialDebug.println(AirBagOnIndc8or);
    //serialDebug.println(AirBagOffIndc8or);
    //serialDebug.println(securityIndc8tor);
    //serialDebug.println(engineRPM);
    serialDebug.println(digitalInputs,BIN);
    int ab = random(22,36);
    digitalWrite(ab, !digitalRead(ab));
    /////  testing //////////////////
}
