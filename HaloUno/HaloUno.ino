#include <mcp_can.h>
#include <SPI.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define INT_PIN 2
#define LED_PIN 6
#define FOG_ENABLE_PIN 7
#define FOG_OVERRIDE_PIN 8

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(24, LED_PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

MCP_CAN CAN0(10);
bool CANgood = false;
long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[4];
byte lightState = B00000100; // default state is fog enabled, turn signals off, no override
byte turnTiming = 0;

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  pinMode(FOG_ENABLE_PIN, OUTPUT);
  pinMode(FOG_OVERRIDE_PIN, OUTPUT);
  pinMode(INT_PIN, INPUT);
  digitalWrite(FOG_ENABLE_PIN, LOW);
  digitalWrite(FOG_OVERRIDE_PIN, LOW);
  Serial.begin(115200);
  if (CAN0.begin(CAN_1000KBPS) == CAN_OK) {
    CAN0.init_Mask(0, 0, 0x7F0);
    CAN0.init_Mask(1, 0, 0x7f0);
    CAN0.init_Filt(0, 0, 0x000); // timing and status
    CAN0.init_Filt(1, 0, 0x010); // master color group
    CAN0.init_Filt(2, 0, 0x020); // colro group 1 (left side)
    CAN0.init_Filt(3, 0, 0x030); // color group 2 (right side)
    CAN0.init_Filt(4, 0, 0x1f0); // color group 45 (front)
    CAN0.init_Filt(5, 0, 0x2f0); // color gourp 46 (
    Serial.print("can init ok!!\r\n");
    CANgood = true;
  }
  else {
    Serial.println("not ok");
    Serial.println("continuing");
  }
}

void loop() {

  if (!digitalRead(INT_PIN) && CANgood) {
    CAN0.readMsgBuf(&len, rxBuf);
    rxId = CAN0.getCanId();
    if (rxId == 0x001) { // high resolution timing broadcast address
      // increase timing clock

      // for every frame of "animation" in a color sequence, the high res timing clock sendsone message.
      // each message contains the number of milliseconds from the master controller.
      // the millis are used to keep each animation in sync.
      // Each controller should check that the millis are maintaining a consistent offset from its own millis in
      // order to avoid misseed frames. If the controller detects that a frame has been missed, double speed
      // for a single frame in order to catch up. If multiple fames are being missed consecutively, adjust
      // local frame delay time to compensate.
    } else if (rxId >= 0x002 && rxId <= 0x00f) {
      lightState = rxBuf[0];
      turnTiming = rxBuf[1];
    }
  }

  bool leftTurn = lightState & B00000001;
  bool rightTurn = (lightState & B00000010) >> 1;
  bool fogEnable = (lightState & B00000100) >> 2;
  bool fogOverride = (lightState & B00001000) >> 3;

  digitalWrite(FOG_ENABLE_PIN, fogEnable);
  digitalWrite(FOG_OVERRIDE_PIN, fogOverride);

////////////////////////////////////////////////////////////////////////////////////////////
//                    dev code, to be deleted or midified                                 //
////////////////////////////////////////////////////////////////////////////////////////////

  colorWipe(strip.Color(255, 200, 0), 0);
  //colorWipe(strip.Color(255, 200, 0), 15);
  //colorWipe(strip.Color(0, 0, 0), 15);
  Serial.println("loop");
  delay(1000);



////////////////////////////////////////////////////////////////////////////////////////////
//        Example code                                                                    //
////////////////////////////////////////////////////////////////////////////////////////////
  // Some example procedures showing how to display to the pixels:
  // colorWipe(strip.Color(255, 0, 0), 50); // Red
  // colorWipe(strip.Color(0, 255, 0), 50); // Green
  //  colorWipe(strip.Color(0, 0, 255), 50); // Blue
  // Send a theater pixel chase in...
  // theaterChase(strip.Color(255, 255, 255), 350); // White
  // theaterChase(strip.Color(255, 0, 0), 150); // Red
  // theaterChase(strip.Color(0, 0, 255), 150); // Blue
  //theaterChase(strip.Color(0, 255, 0), 150); // Green

  // rainbow(5);
  //rainbowCycle(5);
  //theaterChaseRainbow(150);


}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j = 0; j < 10; j++) { //do 10 cycles of chasing
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, c);  //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j = 0; j < 256; j++) {   // cycle all 256 colors in the wheel
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, Wheel( (i + j) % 255)); //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
