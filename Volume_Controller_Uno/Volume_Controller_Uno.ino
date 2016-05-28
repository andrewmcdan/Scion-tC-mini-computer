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
addr_SUBW_R = B0111111,
MSTR_VOL_L = 0x00,
MSTR_VOL_R = 0x00,
SUBW_VOL_L = 0x00,
SUBW_VOL_R = 0x00; // volume range of 0x00 to 0x7f (0 to 127)
bool newData = true;

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
    MSTR_VOL_L = rxBuf[0];
    MSTR_VOL_R = rxBuf[1];
    SUBW_VOL_L = rxBuf[2];
    SUBW_VOL_R = rxBuf[3];
    newData = true;
  }

  //  check if new flag is set
  //  if new==true, send vols to i2c pots
  if (newData) {
    Wire.beginTransmission(addr_MSTR_L);
    Wire.write(0x00);
    Wire.write(MSTR_VOL_L);
    Wire.endTransmission();
    Wire.beginTransmission(addr_MSTR_R);
    Wire.write(0x00);
    Wire.write(MSTR_VOL_R);
    Wire.endTransmission();
    Wire.beginTransmission(addr_SUBW_L);
    Wire.write(0x00);
    Wire.write(SUBW_VOL_L);
    Wire.endTransmission();
    Wire.beginTransmission(addr_SUBW_R);
    Wire.write(0x00);
    Wire.write(SUBW_VOL_R);
    Wire.endTransmission();
    newData = false;
  }
}

