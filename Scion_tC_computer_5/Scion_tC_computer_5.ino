// Coded to compile in Arduino 1.5.4 for Arduino Due
//
//
// Based on version 4
// Removed support for SD, LCD, RTC
// Started support for sending data over ethernet UDP frames
//

#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUDP.h>

#define de_lay 100
#define serialOBDCAN Serial2
#define serialDebug Serial

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xE2 };
byte ip[] = {192,168,5,78};
byte dns1[] = {8,8,8,8};
byte gateway[] = {192,168,1,77};
byte subnet[] = {255,255,248,0};
unsigned int localPort = 41230;      // local port to listen for UDP packets
IPAddress infoServer(192,168,1,61);
const int MAX_PACKET_SIZE = 24; // max size of packet. If all elements are shorts, half this number for num of elements
byte packetBuffer[MAX_PACKET_SIZE];
short MPH = 1, RPM = 1200, TEMP = 190, OilTEMP = 50;

EthernetUDP Udp;  // A UDP instance to let us send and receive packets over UDP

void setup() {
  //serialDebug.begin(9600);
  //STN1110Setup();
  Ethernet.begin(mac,ip,dns1,gateway,subnet);
  Udp.begin(localPort);
}

void loop() {
  
  //MPH = GetVehicleSpeed();
  //RPM = GetEngineRPM();
  //TEMP = GetEngineTemp();
  //OilTEMP = GetEngineOilTemp();
  MPH++;
  if(MPH>99){MPH=0;}
  RPM++;
  if(RPM>8000){RPM=0;}
  TEMP++;
  if(TEMP>240){TEMP=30;}
  OilTEMP++;
  if(OilTEMP>240){OilTEMP=30;}
  
  short rand = random(65535);
  
  memset(packetBuffer, 0, MAX_PACKET_SIZE); // initialize the array to 0
  
  // Convert the MPH and RPM shorts into seperated bytes and add to new array.
  // These values will have to put back together by the server.
  packetBuffer={(byte)TEMP,(byte)MPH,((byte)(RPM>>8)),(byte)RPM,(byte)OilTEMP,(byte)TEMP,((byte)(rand>>8)),(byte)rand};
  
  // Send the packet to the server.
  Udp.beginPacket(infoServer, 41234); 
  Udp.write(packetBuffer,MAX_PACKET_SIZE);
  Udp.endPacket();
  
  // need to rewrite this so that data is sent every 100ms. This delay should include the time 
  // it takes to communicate with the STN1110 and get data off the CAN bus. 
  delay(de_lay); 
}

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

short GetEngineTemp(){
  String textSpeed = OBDCANmessage("0105");
  char s[16];
  textSpeed.toCharArray(s,16);
  short yy;
  sscanf(s, "%*4x%2x", &yy);
  return ((yy-40)*(9/5))+32;  //  °C  x  9/5 + 32 = °F
}

short GetEngineOilTemp(){
  String textSpeed = OBDCANmessage("015c");
  char s[16];
  textSpeed.toCharArray(s,16);
  short yy;
  sscanf(s, "%*4x%2x", &yy);
  return ((yy-40)*(9/5))+32;  //  °C  x  9/5 + 32 = °F
}

short GetVehicleSpeed(){
  String textSpeed = OBDCANmessage("010d");
  char s[16];
  textSpeed.toCharArray(s,16);
  short yy;
  sscanf(s, "%*4x%2x", &yy);
  return yy*0.621371;
}

short GetEngineRPM(){
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
  //delay(40); // make sure we wait long enough for the device to respond
  // it usually takes about 30 milliseconds for the device to be done sending
  // we wait 40 just to make sure
  while((serialOBDCAN.available()==0)/*&&((millis()-messageStartTime)<500)*/){}
  while(serialOBDCAN.available()>0){
    int inChar = serialOBDCAN.read();
    incoming += (char)inChar;
    while(((serialOBDCAN.available() == 0 )&&(char(inChar)!='>'))/*&&((millis()-messageStartTime)<100)*/){}
  }
  //delay(5);
  return incoming;
}

