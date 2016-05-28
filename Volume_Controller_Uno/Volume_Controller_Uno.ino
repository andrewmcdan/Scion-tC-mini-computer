#include <Wire.h>
#include <mcp_can.h>
#include <SPI.h>

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];

MCP_CAN CAN0(9);

byte
addr_MSTR_L = B0111100,
addr_MSTR_R = B0111110,
addr_SUBW_L = B0111101,
addr_SUBW_R = B0111111;

void setup() {
  Wire.begin();
  pinMode(8, INPUT);
  CAN0.begin(CAN_500KBPS);
  CAN0.init_Mask(0, 0, 0x7ff);
  CAN0.init_Mask(1, 0, 0x7ff);
  CAN0.init_Filt(0, 0, 0x00f);
  CAN0.init_Filt(1, 0, 0x00f);
  CAN0.init_Filt(2, 0, 0x00f);
  CAN0.init_Filt(3, 0, 0x00f);
  CAN0.init_Filt(4, 0, 0x00f);
  CAN0.init_Filt(5, 0, 0x00f);
}

void loop() {
  //  check for CAN bus interrupt
  //  if interrupt, read data and store, set new flag

  if (!digitalRead(8)) {
    CAN0.readMsgBuf(&len, rxBuf);
    Wire.beginTransmission(addr_MSTR_L);
    Wire.write(0x00);
    Wire.write(rxBuf[0]);
    Wire.endTransmission();
    Wire.beginTransmission(addr_MSTR_R);
    Wire.write(0x00);
    Wire.write(rxBuf[1]);
    Wire.endTransmission();
    Wire.beginTransmission(addr_SUBW_L);
    Wire.write(0x00);
    Wire.write(rxBuf[2]);
    Wire.endTransmission();
    Wire.beginTransmission(addr_SUBW_R);
    Wire.write(0x00);
    Wire.write(rxBuf[3]);
    Wire.endTransmission();
  }
}

