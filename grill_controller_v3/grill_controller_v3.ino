#include <SPI.h>

struct animation {
  byte FR;
  byte numFrames;
  unsigned int F[100];
};

animation TurnSigScroll = {
 /* FrameRate         */ 5,
 /* Number of frames  */ 9,
 /* Frame 1           */{((B00000000)+(long(B00000000)<<8)),
 /* Frame 2           */ ((B00000001)+(long(B10000000)<<8)),
 /* Frame 3           */ ((B00000010)+(long(B01000000)<<8)),
 /* Frame 4           */ ((B00000100)+(long(B00100000)<<8)),
 /* Frame 5           */ ((B00001000)+(long(B00010000)<<8)),
 /* Frame 6           */ ((B00010000)+(long(B00001000)<<8)),
 /* Frame 7           */ ((B00100000)+(long(B00000100)<<8)),
 /* Frame 8           */ ((B01000000)+(long(B00000010)<<8)),
 /* Frame 9           */ ((B10000000)+(long(B00000001)<<8))  }};
 
animation TurnSigRight = {
 /* FrameRate         */ 5,
 /* Number of frames  */ 9,
 /* Frame 1           */{((B00000000)+(long(B00000000)<<8)),
 /* Frame 2           */ ((B00000000)+(long(B10000000)<<8)),
 /* Frame 3           */ ((B00000000)+(long(B01000000)<<8)),
 /* Frame 4           */ ((B00000000)+(long(B00100000)<<8)),
 /* Frame 5           */ ((B00000000)+(long(B00010000)<<8)),
 /* Frame 6           */ ((B00000000)+(long(B00001000)<<8)),
 /* Frame 7           */ ((B00000000)+(long(B00000100)<<8)),
 /* Frame 8           */ ((B00000000)+(long(B00000010)<<8)),
 /* Frame 9           */ ((B00000000)+(long(B00000001)<<8))  }};
 
animation TurnSigLeft = {
 /* FrameRate         */ 5,
 /* Number of frames  */ 9,
 /* Frame 1           */{((B00000000)+(long(B00000000)<<8)),
 /* Frame 2           */ ((B00000001)+(long(B00000000)<<8)),
 /* Frame 3           */ ((B00000010)+(long(B00000000)<<8)),
 /* Frame 4           */ ((B00000100)+(long(B00000000)<<8)),
 /* Frame 5           */ ((B00001000)+(long(B00000000)<<8)),
 /* Frame 6           */ ((B00010000)+(long(B00000000)<<8)),
 /* Frame 7           */ ((B00100000)+(long(B00000000)<<8)),
 /* Frame 8           */ ((B01000000)+(long(B00000000)<<8)),
 /* Frame 9           */ ((B10000000)+(long(B00000000)<<8))  }}; 
 
 animation demoAnim = {
   5,29,{
 /* Frame 1           */ ((B00000000)+(long(B00000000)<<8)),
 /* Frame 2           */ ((B00000001)+(long(B00000000)<<8)),
 /* Frame 3           */ ((B00000010)+(long(B00000000)<<8)),
 /* Frame 4           */ ((B00000100)+(long(B00000000)<<8)),
 /* Frame 5           */ ((B00001000)+(long(B00000000)<<8)),
 /* Frame 6           */ ((B00010000)+(long(B00000000)<<8)),
 /* Frame 7           */ ((B00100000)+(long(B00000000)<<8)),
 /* Frame 8           */ ((B01000000)+(long(B00000000)<<8)),
 /* Frame 9           */ ((B10000000)+(long(B00000000)<<8)),
 /* Frame 10          */ ((B00000000)+(long(B10000000)<<8)),
 /* Frame 11          */ ((B00000000)+(long(B01000000)<<8)),
 /* Frame 12          */ ((B00000000)+(long(B00100000)<<8)),
 /* Frame 13          */ ((B00000000)+(long(B00010000)<<8)),
 /* Frame 14          */ ((B00000000)+(long(B00001000)<<8)),
 /* Frame 15          */ ((B00000000)+(long(B00000100)<<8)),
 /* Frame 16          */ ((B00000000)+(long(B00000010)<<8)),
 /* Frame 17          */ ((B00000000)+(long(B00000001)<<8)),
 /* Frame 18          */ ((B10000000)+(long(B00000001)<<8)),
 /* Frame 19          */ ((B01000000)+(long(B00000010)<<8)),
 /* Frame 20          */ ((B00100000)+(long(B00000100)<<8)),
 /* Frame 21          */ ((B10010000)+(long(B00001001)<<8)),
 /* Frame 22          */ ((B01001000)+(long(B00010010)<<8)),
 /* Frame 23          */ ((B00100100)+(long(B00100100)<<8)),
 /* Frame 24          */ ((B00010010)+(long(B01001000)<<8)),
 /* Frame 25          */ ((B00001001)+(long(B10010000)<<8)),
 /* Frame 26          */ ((B00000100)+(long(B00100000)<<8)),
 /* Frame 27          */ ((B00000010)+(long(B01000000)<<8)),
 /* Frame 28          */ ((B00000001)+(long(B10000000)<<8)),
 /* Frame 29          */ ((B00000000)+(long(B00000000)<<8)), }};
 

#define LATCHPIN 8
#define leftTurnInput 2
#define rightTurnInput 3
#define StripEnable 7
long THECOLOR = 0xffff00;
int de_lay = 100;
int blinkTimeOut = 1500;
byte blinkerMode = 1;
byte blinkerState = 0;
unsigned long startTimeOne = 0;
unsigned long startTimeTwo = 0;
byte leftTurn = 0;
byte rightTurn = 0;

long leftBottomColorsLongs[9] = {
  THECOLOR, THECOLOR, THECOLOR, THECOLOR, THECOLOR, THECOLOR, THECOLOR, THECOLOR, 0};
long leftTopColorsLongs[9] = {
  THECOLOR, THECOLOR, THECOLOR, THECOLOR, THECOLOR, THECOLOR, THECOLOR, THECOLOR, 0};
long rightBottomColorsLongs[9] = {
  THECOLOR, THECOLOR, THECOLOR, THECOLOR, THECOLOR, THECOLOR, THECOLOR, THECOLOR, 0};
long rightTopColorsLongs[9] = {
  THECOLOR, THECOLOR, THECOLOR, THECOLOR, THECOLOR, THECOLOR, THECOLOR, THECOLOR, 0};

byte tempOne = 8;

unsigned long startTime = 0;
boolean doinIt = true;
boolean doinItFirstTime = true;

void setup(){  
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV8);
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);

  pinMode(LATCHPIN,OUTPUT);
  pinMode(StripEnable,OUTPUT);
  digitalWrite(StripEnable,LOW);
  pinMode(rightTurnInput,INPUT);
  pinMode(leftTurnInput,INPUT);
  digitalWrite(LATCHPIN,HIGH);

  digitalWrite(LATCHPIN,LOW);
  SPI.transfer(0);
  SPI.transfer(0);
  digitalWrite(LATCHPIN,HIGH);
}

void loop(){
 startTimeOne=millis();

  if((digitalRead(leftTurnInput)==HIGH)&&(digitalRead(rightTurnInput)==HIGH)){
    blinkerState=1;
  }
  else if((digitalRead(leftTurnInput)==HIGH)&&(digitalRead(rightTurnInput)==LOW)){
    blinkerState=2;
  }
  else if((digitalRead(leftTurnInput)==LOW)&&(digitalRead(rightTurnInput)==HIGH)){
    blinkerState=3;
  }
  else if((digitalRead(leftTurnInput)==LOW)&&(digitalRead(rightTurnInput)==LOW)){
    blinkerState = 4;
  }



  if(blinkerState!=blinkerMode){
    if(blinkerState == 4){
      if(startTimeOne-startTimeTwo>blinkTimeOut){
      startTimeTwo=startTimeOne;
      }
    }
    if(doinItFirstTime==true){
      doinItFirstTime = false;
      startTime=startTimeOne;
    }
    if(blinkerState!= 4){
      blinkerMode=blinkerState;
    }
    doinIt=true;
  }
  
  int temp = startTimeOne-startTimeTwo;

  if(  (doinIt==true)  &&  (temp>=blinkTimeOut)  &&  (blinkerState==4)  ){
    blinkerMode = blinkerState;
    doinIt = false;
    doinItFirstTime = true;
  }

  tempOne = int(floor(((startTimeOne-startTime)%(de_lay*8))/de_lay)); //  

  switch(blinkerMode){
  case 1: // scroll from center out
    rightTurn = bit(tempOne);
    leftTurn = rightTurn;
    break;
  case 2: // turn left
    leftTurn = bit(tempOne);
    rightTurn = 0;
    break;
  case 3: // turn right
    rightTurn = bit(tempOne);
    leftTurn = 0;
    break;
  case 4: // off
    rightTurn = 0;
    leftTurn=0;
    break;
  case 5: // knight rider
    break;
  case 6:
    break;
  }
  OutputColors();
}

void OutputColors(){   
  digitalWrite(StripEnable,HIGH);
  for(int i = 7; i >= 0; i--){
    if((tempOne==i)&&(rightTurn!=0)){
      SPI.transfer(0);
      SPI.transfer(0);
      SPI.transfer(0);
    }
    else{
      SPI.transfer((rightBottomColorsLongs[i]&0xff0000)>>16);
      SPI.transfer((rightBottomColorsLongs[i]&0xff00)>>8);
      SPI.transfer(rightBottomColorsLongs[i]&0xff);
    }
  }
  for(int i = 7; i >= 0; i--){
    if((tempOne==i)&&(rightTurn!=0)){
      SPI.transfer(0);
      SPI.transfer(0);
      SPI.transfer(0);
    }
    else{
      SPI.transfer((rightTopColorsLongs[i]&0xff0000)>>16);
      SPI.transfer((rightTopColorsLongs[i]&0xff00)>>8);
      SPI.transfer(rightTopColorsLongs[i]&0xff);
    }
  }
  for(int i = 0; i < 8; i++){
    if((tempOne==i)&&(leftTurn!=0)){
      SPI.transfer(0);
      SPI.transfer(0);
      SPI.transfer(0);
    }
    else{
      SPI.transfer((leftTopColorsLongs[i]&0xff0000)>>16);
      SPI.transfer((leftTopColorsLongs[i]&0xff00)>>8);
      SPI.transfer(leftTopColorsLongs[i]&0xff);
    }
  }
  for(int i = 0; i < 8; i++){
    if((tempOne==i)&&(leftTurn!=0)){
      SPI.transfer(0);
      SPI.transfer(0);
      SPI.transfer(0);
    }
    else{
      SPI.transfer((leftBottomColorsLongs[i]&0xff0000)>>16);
      SPI.transfer((leftBottomColorsLongs[i]&0xff00)>>8);
      SPI.transfer(leftBottomColorsLongs[i]&0xff);
    }
  }
  digitalWrite(StripEnable,LOW);
  delay(1);
  digitalWrite(LATCHPIN,LOW);
  SPI.transfer(0x00);
  SPI.transfer(rightTurn);
  SPI.transfer(leftTurn);
  digitalWrite(LATCHPIN,HIGH);
}

byte bitswap (byte x)
{
  byte result;
  asm("mov __tmp_reg__, %[in] \n\t"
   "lsl __tmp_reg__  \n\t"   /* shift out high bit to carry */
   "ror %[out] \n\t"  /* rotate carry __tmp_reg__to low bit (eventually) */
   "lsl __tmp_reg__  \n\t"   /* 2 */
  "ror %[out] \n\t"
   "lsl __tmp_reg__  \n\t"   /* 3 */
  "ror %[out] \n\t"
   "lsl __tmp_reg__  \n\t"   /* 4 */
  "ror %[out] \n\t"
   "lsl __tmp_reg__  \n\t"   /* 5 */
  "ror %[out] \n\t"
   "lsl __tmp_reg__  \n\t"   /* 6 */
  "ror %[out] \n\t"
   "lsl __tmp_reg__  \n\t"   /* 7 */
  "ror %[out] \n\t"
   "lsl __tmp_reg__  \n\t"   /* 8 */
  "ror %[out] \n\t"
: 
    [out] "=r" (result) : 
  [in] "r" (x));
  return(result);
}
