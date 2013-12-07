const unsigned long RED = 0xff0000;
const unsigned long BLUE = 0x0000ff;
const unsigned long GREEN = 0x00ff00;
const unsigned long YELLOW = 0xffff00;
const unsigned long ORANGE = 0xff5500;
const unsigned long PURPLE = 0xD900FF;
const unsigned long PINK = 0xff00BB;
const unsigned long AQUA = 0x00ffff;
const unsigned long WHITE = 0xffffff;
const unsigned long OFF = 0x000000;

int a = 0; //this sets the delay
int b = 1000;   //sets the trans time
int red = 10; //this sets the red led pin
int green = 9; //this sets the green led pin
int blue = 11; //this sets the blue led pin
boolean gotoKeyFob = false;
int pressCount=0;
int buttonPress[10];
unsigned long buttonTimer = 0;
int mode = 3;
boolean transBrake = false;

void setup() { //this sets the output pins

  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(2,INPUT);
  pinMode(3,INPUT);
  pinMode(4,INPUT);
  pinMode(5,INPUT);
  pinMode(6,INPUT);
  pinMode(7,OUTPUT);
  attachInterrupt(0,keyFobInterrupt,RISING);
  //Serial.begin(115200);
}

void loop() {
  a = 3000;

  /*
  Serial.println(buttonPress[0]);
   Serial.println(buttonPress[1]);
   Serial.println(buttonPress[2]);
   Serial.println(buttonPress[3]);
   Serial.println(" ");
   */
  if(pressCount==0)buttonTimer=millis();
  if((millis()-buttonTimer>2000)&&(pressCount!=3)&&(transBrake!=true))pressCount=0;
  transBrake = false;

  if(pressCount>=2){
    if((buttonPress[0]==1)&&(buttonPress[1]==4)){
      delay(1000);

      if(pressCount>=3){
        switch(buttonPress[2]){
        case 1:mode = 0;break; 
        case 2:
          mode = 1;
          break;
        case 3:
          mode = 4;
          break;
        case 4:
          mode = 2;
          break;
        case 5:
          mode = 5;
          break;
        case 8:
          mode = 3;
          break;
        case 10:
          mode = 6;
          break;
        case 12:
          mode = 7;
          break;
        default:
          break;
        }
      }
    }
    pressCount=0;
  }

  switch(mode){
  case 0: //  A
    fadeColors(WHITE,WHITE,1);
    delay(1);
    break;
  case 1: //  B
    fadeColors(RED,RED,1);
    delay(1);
    break;
  case 2: //  C
    fadeColors(GREEN,GREEN,1);
    delay(1);
    break;
  case 3: //  D
    fadeColors(YELLOW,YELLOW,1);
    delay(1);
    break;
  case 4: //  A + B
    fadeColors(OFF,OFF,1);
    delay(1);
    break;
  case 5: //  A + C
    fadeColors(RED,BLUE,1);
    delay(250);
    fadeColors(BLUE,RED,1);
    delay(250);    
    break;
  case 6: //  B + D
    fadeColors(RED,ORANGE,b);
    delay(a);
    fadeColors(ORANGE,YELLOW,b);
    delay(a);
    fadeColors(YELLOW,GREEN,b);
    delay(a);
    fadeColors(GREEN,AQUA,b);
    delay(a);
    fadeColors(AQUA,BLUE,b);
    delay(a);
    fadeColors(BLUE,PURPLE,b);
    delay(a);
    fadeColors(PURPLE,PINK,b);
    delay(a);
    fadeColors(PINK,RED,b);
    delay(a);
    break;
  case 7: //  C + D
    fadeColors(WHITE,OFF,1500);
    delay(1);
    fadeColors(OFF,WHITE,1500);
    delay(1);
    break;
  }
} 

unsigned long fadeColors(unsigned long oldColor, unsigned long newColor, int fadeTime){
  unsigned long fadeStartTime = millis();
  float blueTimeSteps = 0;
  float redTimeSteps = 0;
  float greenTimeSteps = 0;
  int newBlue = newColor % 256;
  int newGreen = (newColor >> 8 ) % 256;
  int newRed = (newColor >> 16) % 256;
  int oldBlue = oldColor % 256;
  int oldGreen = (oldColor >> 8 ) % 256;
  int oldRed = (oldColor >> 16) % 256;

  /*newRed = 0xff;
   newGreen = 0x00;
   newBlue = 0xff;
   oldRed = 0xff;
   oldGreen = 0xff;
   oldBlue = 0x00;*/


  if(oldBlue < newBlue) blueTimeSteps = fadeTime / ( newBlue - oldBlue );
  else if(oldBlue > newBlue) blueTimeSteps = fadeTime / ( oldBlue - newBlue );
  else blueTimeSteps = 0;

  if(oldRed < newRed) redTimeSteps = fadeTime / ( newRed - oldRed );
  else if(oldRed > newRed) redTimeSteps = fadeTime / ( oldRed - newRed );
  else redTimeSteps = 0;

  if(oldGreen < newGreen) greenTimeSteps = fadeTime / ( newGreen - oldGreen );
  else if(oldGreen > newGreen) greenTimeSteps = fadeTime / ( oldGreen - newGreen );
  else greenTimeSteps = 0;

  float colorStep = 0;

  boolean notDone = true;

  while(notDone){
    if(transBrake){
      //mode = 3;
      return 0;
    }
    if(millis()-fadeStartTime>fadeTime){
      notDone=false;
      oldBlue=newBlue;
      oldRed=newRed;
      oldGreen=newGreen;
      analogWrite(blue,oldBlue);
      analogWrite(red,oldRed);
      analogWrite(green,oldGreen);
    }
    else{
      colorStep = (millis() - fadeStartTime) / blueTimeSteps;
      if(oldBlue < newBlue){
        analogWrite(blue,constrain(oldBlue+colorStep,0,255));
      }
      else if(oldBlue > newBlue){
        analogWrite(blue,constrain(oldBlue-colorStep,0,255));
      }

      colorStep = (millis() - fadeStartTime) / redTimeSteps;
      if(oldRed < newRed){
        analogWrite(red,constrain(oldRed+colorStep,0,255));
      }
      else if(oldRed > newRed){
        analogWrite(red,constrain(oldRed-colorStep,0,255));
      }

      colorStep = (millis() - fadeStartTime) / greenTimeSteps;
      if(oldGreen < newGreen){
        analogWrite(green,constrain(oldGreen+colorStep,0,255));
      }
      else if(oldGreen > newGreen){
        analogWrite(green,constrain(oldGreen-colorStep,0,255));
      }
    }
  }
  return 0;
}

void keyFobInterrupt(){
  detachInterrupt(0);
  digitalWrite(7,HIGH);

  buttonPress[pressCount] = ((digitalRead(3)*B0001)+(digitalRead(4)*B0010)+(digitalRead(5)*B0100)+(digitalRead(6)*B1000));
  if ( buttonPress[pressCount] == 15){
    transBrake=true;
    a=0;
  }
  if ( buttonPress[pressCount] != 0 )pressCount++;

  while(digitalRead(2)==HIGH){
  }
  digitalWrite(7,LOW);

  while((digitalRead(2)==HIGH)||(digitalRead(3)==HIGH)||(digitalRead(4)==HIGH)||(digitalRead(5)==HIGH)){
  }
  attachInterrupt(0,keyFobInterrupt,RISING);
}



