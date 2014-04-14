//  This sketch make use of a modified version of the ethernet library. The modified version
//  must reside in the Arduino core libraries folders. This sketch has a copy with it for
//  safe keeping.

#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUDP.h>
#include <SD.h>

#define de_lay 100
#define serialOBDCAN Serial2
#define serialDebug Serial

const unsigned int localPort = 41231;      // local port to listen for UDP packets
const IPAddress infoServer(192,168,1,61);
const IPAddress vehicleStarter(192,168,4,4);
const IPAddress HP12VGcontroller(192,168,4,5);
const IPAddress LP5VGcontroller(192,168,4,6);
const IPAddress testTest(192,168,1,200);
unsigned long tierOneCounter = 0;
unsigned long tierTwoCounter = 0;
unsigned long tierThreeCounter = 0;
unsigned long tierFourCounter = 0;
unsigned long tierFiveCounter = 0;
unsigned short timeDiff;
unsigned long timer = micros();
short maxMPH = 0;

File daFile;
EthernetUDP updatesUDP;  // A UDP instance for regular updates to other controllers
EthernetUDP starterUDP;  // A UDP instance to let us listen for incoming connections from the ctarter controller
EthernetServer webServer(80);

void setup() {
  //serialDebug.begin(115200);
  if(!STN1110Setup()){
    // do error thing
  }
  byte mac[] = { 0xB8, 0x27, 0xEB, 0x22, 0x33, 0x00 };
  byte ip[] = {192,168,4,3};
  byte dns1[] = {8,8,8,8};
  byte gateway[] = {192,168,4,1};
  byte subnet[] = {255,255,248,0};
  Ethernet.begin(mac,ip,dns1,gateway,subnet);
  webServer.begin();
  starterUDP.begin(41230);
  SD.begin(4);  
}

void loop() {
  //  timing teirs:    tierOneCounter - 10ms
  //                   tierTwoCounter - 40ms
  //                   tierThreeCounter - 75ms
  //                   tierFourCounter - 250ms
  //                   tierFiveCounter - 700ms
  if(micros()<timer){
    timer=micros();
  }
  timeDiff = micros()-timer;
  timer=micros();
  //serialDebug.println(timeDiff);
  tierOneCounter += timeDiff;
  tierTwoCounter += timeDiff;
  tierThreeCounter += timeDiff;
  tierFourCounter += timeDiff;
  tierFiveCounter += timeDiff;
  
  if(tierOneCounter>10000){
    tierOneCounter=0;
    // do teir one tasks
  }
  if(tierTwoCounter>40000){
    tierTwoCounter=0;
    // do teir two tasks
  }
  if(tierThreeCounter>75000){
    tierThreeCounter=0;
    // do teir three tasks
    byte MilesPerHour = 0, EngineTemp = 0, TransTemp = 0, ECUvoltage = 0;
    short engineRPM = 0, AirFuelRatio = 0, FuelRate = 0;
    const byte MAX_PACKET_SIZE = 12;
    byte packetBuffer[MAX_PACKET_SIZE];  // max size of packet. 
    //MilesPerHour = VehicleSpeed();
    if(MilesPerHour>maxMPH){maxMPH=MilesPerHour;}
    //engineRPM = EngineRPM();
    //EngineTemp = EngineTemp();
    //TransTemp = TransTemp();
    //fakeECUinfo(&MilesPerHour,&EngineTemp,&TransTemp,&ECUvoltage,&engineRPM,&AirFuelRatio,&FuelRate);  
    short rand = random(65535);
    //short rand = time2 - time1;
    packetBuffer={EngineTemp,MilesPerHour,(engineRPM>>8),engineRPM,ECUvoltage,TransTemp,((byte)(rand>>8)),(byte)rand,0,0,0,0};
    sendPacketUDP(packetBuffer,MAX_PACKET_SIZE,infoServer);
  }
  if(tierFourCounter>250000){
    tierFourCounter=0;
    // do teir four tasks
    CheckAndDoWebServer();
  }
  if(tierFiveCounter>700000){
    tierFiveCounter=0;
    // do teir five tasks
    if(starterUDP.parsePacket()){fobStatus();}
  }
  //delay(de_lay);
}

void sendPacketUDP(byte *pBuffer,byte pSize,const IPAddress addy){
  updatesUDP.begin(41231);
  updatesUDP.beginPacket(addy, 41234); 
  updatesUDP.write(pBuffer,pSize);
  updatesUDP.endPacket();
  updatesUDP.stop();
}

void fobStatus(){
  byte pBuffer[1];
  starterUDP.read(pBuffer,1);
  if(pBuffer[0]==0xAA){
    
    // get the fob status
    pBuffer[0]=0x55;
    
    starterUDP.beginPacket(vehicleStarter,41234);
    starterUDP.write(pBuffer,1);
    starterUDP.endPacket();
    starterUDP.stop();
  }
}
