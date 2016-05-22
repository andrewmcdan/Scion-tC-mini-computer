#include <SPI.h>         
#include <Ethernet.h>
#include <EthernetUdp.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xE1 };
byte ip[] = {192,168,5,79};
byte dns1[] = {8,8,8,8};
byte gateway[] = {192,168,1,77};
byte subnet[] = {255,255,248,0};
unsigned int localPort = 41230;
IPAddress infoServer(192,168,1,61);
const int UDP_PACKET_SIZE= 8;
byte packetBuffer[ UDP_PACKET_SIZE];

EthernetUDP Udp;

void setup() 
{
  Ethernet.begin(mac,ip,dns1,gateway,subnet);
  Udp.begin(localPort);
}

void loop()
{
  sendUDPpacket(infoServer);
  delay(100); 
}

unsigned long sendUDPpacket(IPAddress& address){
  memset(packetBuffer, 0, UDP_PACKET_SIZE);
  packetBuffer[0] = random(255);
  packetBuffer[1] = random(255);
  packetBuffer[2] = random(255);
  packetBuffer[3] = 0xEC;
  packetBuffer[4] = 49;
  packetBuffer[5] = 0x4E;
  packetBuffer[6] = 49;
  packetBuffer[7] = 52;

  Udp.beginPacket(address, 41235);
  Udp.write(packetBuffer,UDP_PACKET_SIZE);
  Udp.endPacket(); 
}
