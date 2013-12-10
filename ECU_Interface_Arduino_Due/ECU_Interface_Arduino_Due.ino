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
const IPAddress testTest(192,168,1,200);

File daFile;
EthernetUDP updatesUDP;  // A UDP instance for regular updates to other controllers
EthernetUDP starterUDP;  // A UDP instance to let us listen for incoming connections from the ctarter controller
EthernetServer webServer(80);

void setup() {
  //serialDebug.begin(115200);
  //STN1110Setup();
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
  byte MilesPerHour = 0, EngineTemp = 0, TransTemp = 0, ECUvoltage = 0;
  short engineRPM = 0, AirFuelRatio = 0, FuelRate = 0;
  const byte MAX_PACKET_SIZE = 12;
  byte packetBuffer[MAX_PACKET_SIZE];  // max size of packet. 
  
  //MilesPerHour = VehicleSpeed();
  //engineRPM = EngineRPM();
  //EngineTemp = EngineTemp();
  //TransTemp = TransTemp();
  //fakeECUinfo(&MilesPerHour,&EngineTemp,&TransTemp,&ECUvoltage,&engineRPM,&AirFuelRatio,&FuelRate);  
  
  short rand = random(65535);
  //short rand = time2 - time1;
  packetBuffer={EngineTemp,MilesPerHour,(engineRPM>>8),engineRPM,ECUvoltage,TransTemp,((byte)(rand>>8)),(byte)rand,0,0,0,0};

  sendPacketUDP(packetBuffer,MAX_PACKET_SIZE,infoServer);
  if(starterUDP.parsePacket()){fobStatus();}
  CheckAndDoWebServer();
  delay(de_lay);
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
  if(pBuffer[0]==' '){
    
    // get the fob status
    
    starterUDP.beginPacket(vehicleStarter,41234);
    starterUDP.write(pBuffer,1);
    starterUDP.endPacket();
    starterUDP.stop();
  }
}
