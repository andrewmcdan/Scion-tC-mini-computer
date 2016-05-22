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
        temperature = tempstring.toInt();
        break;
        case 53: // "5"  dest
        tempstring = "";
        while(0 < Wire1.available())
        {
            tempstring += char(Wire1.read());
        }
        airDestination = tempstring.toInt();
        break;
        case 51: // "3"  fanSpeed
        tempstring = "";
        while(0 < Wire1.available())
        {
            tempstring += char(Wire1.read());
        }
        fanSpeed = tempstring.toInt();
        break;
        case 54: // "6"  reCirc
        tempstring = "";
        while(0 < Wire1.available())
        {
            tempstring += char(Wire1.read());
        }
        reCirc = tempstring=="true"?true:false;
        break;
        case 55: // "7"  AC
        tempstring = "";
        while(0 < Wire1.available())
        {
            tempstring += char(Wire1.read());
        }
        AC = tempstring=="true"?true:false;
        break;
        case 56: // "8"  rearDef
        tempstring = "";
        while(0 < Wire1.available())
        {
            tempstring += char(Wire1.read());
        }
        rearDef = tempstring=="true"?true:false;
        break;
        case 99: // "c"  colorRGBr
        tempstring = "";
        while(0 < Wire1.available())
        {
            tempstring += char(Wire1.read());
        }
        colorRGBr = tempstring.toInt();
        break;
        case 100: // "d"  colorRGBg
        tempstring = "";
        while(0 < Wire1.available())
        {
            tempstring += char(Wire1.read());
        }
        colorRGBg = tempstring.toInt();
        break;
        case 101: // "e"  colorRGBb
        tempstring = "";
        while(0 < Wire1.available())
        {
            tempstring += char(Wire1.read());
        }
        colorRGBb = tempstring.toInt();
        break;
        case 103: // "g"  fogLights
        tempstring = "";
        while(0 < Wire1.available())
        {
            tempstring += char(Wire1.read());
        }
        fogLights = tempstring=="true"?true:false;
        break;
        case 104: // "h"  groundLights
        tempstring = "";
        while(0 < Wire1.available())
        {
            tempstring += char(Wire1.read());
        }
        groundLights = tempstring=="true"?true:false;
        break;
        case 106: // "j"  groundBlinkers
        tempstring = "";
        while(0 < Wire1.available())
        {
            tempstring += char(Wire1.read());
        }
        groundBlinkers = tempstring=="true"?true:false;
        break;
        case 105: // "i"  grill
        tempstring = "";
        while(0 < Wire1.available())
        {
            tempstring += char(Wire1.read());
        }
        grill = tempstring=="true"?true:false;
        break;
        case 107: // "k"  halos
        tempstring = "";
        while(0 < Wire1.available())
        {
            tempstring += char(Wire1.read());
        }
        halos = tempstring=="true"?true:false;
        break;
        case 109: // "m"  VSC
        tempstring = "";
        while(0 < Wire1.available())
        {
            tempstring += char(Wire1.read());
        }
        VSC = tempstring=="true"?true:false;
        break;
        case 108: // "l"  trac
        tempstring = "";
        while(0 < Wire1.available())
        {
            tempstring += char(Wire1.read());
        }
        trac = tempstring=="true"?true:false;
        break;
        case 110: // "n"  masterVolume
        tempstring = "";
        while(0 < Wire1.available())
        {
            tempstring += char(Wire1.read());
        }
        masterVolume = tempstring.toInt();
        break;
        case 111: // "o"  subVolume
        tempstring = "";
        while(0 < Wire1.available())
        {
            tempstring += char(Wire1.read());
        }
        subVolume = tempstring.toInt();
        break;
        case 112: // "p"  AudioSource
        tempstring = "";
        while(0 < Wire1.available())
        {
            tempstring += char(Wire1.read());
        }
        AudioSource = tempstring.toInt();
        break;
        default:
        break;
    }
}
