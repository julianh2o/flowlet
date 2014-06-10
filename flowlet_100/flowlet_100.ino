#include <Adafruit_NeoPixel.h>

#define CHARGE_PIN 12
#define LED_PIN 4
#define bracelet_length 26

Adafruit_NeoPixel strip = Adafruit_NeoPixel(bracelet_length, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.setBrightness(20);
  strip.show();
  pinMode(CHARGE_PIN,INPUT_PULLUP);
}


byte hueLookup[765] = {0,255,0,6,255,0,11,255,0,17,255,0,24,255,0,30,255,0,35,255,0,41,255,0,48,255,0,54,255,0,60,255,0,65,255,0,72,255,0,78,255,0,83,255,0,89,255,0,96,255,0,102,255,0,108,255,0,113,255,0,120,255,0,126,255,0,132,255,0,138,255,0,144,255,0,150,255,0,155,255,0,162,255,0,168,255,0,173,255,0,179,255,0,186,255,0,191,255,0,198,255,0,204,255,0,210,255,0,216,255,0,222,255,0,228,255,0,234,255,0,240,255,0,246,255,0,252,255,0,255,251,0,255,245,0,255,240,0,255,234,0,255,227,0,255,222,0,255,215,0,255,210,0,255,204,0,255,198,0,255,192,0,255,186,0,255,179,0,255,174,0,255,168,0,255,162,0,255,156,0,255,150,0,255,144,0,255,138,0,255,132,0,255,126,0,255,120,0,255,113,0,255,107,0,255,101,0,255,96,0,255,89,0,255,83,0,255,78,0,255,72,0,255,65,0,255,59,0,255,53,0,255,48,0,255,41,0,255,35,0,255,30,0,255,24,0,255,17,0,255,11,0,255,5,0,255,0,0,255,0,6,255,0,12,255,0,18,255,0,24,255,0,30,255,0,35,255,0,41,255,0,47,255,0,54,255,0,60,255,0,65,255,0,71,255,0,78,255,0,84,255,0,90,255,0,96,255,0,101,255,0,107,255,0,113,255,0,119,255,0,126,255,0,131,255,0,137,255,0,143,255,0,150,255,0,156,255,0,162,255,0,167,255,0,173,255,0,179,255,0,185,255,0,191,255,0,197,255,0,203,255,0,209,255,0,215,255,0,222,255,0,227,255,0,233,255,0,239,255,0,245,255,0,252,252,0,255,245,0,255,240,0,255,233,0,255,227,0,255,221,0,255,215,0,255,209,0,255,203,0,255,197,0,255,192,0,255,186,0,255,179,0,255,173,0,255,167,0,255,162,0,255,156,0,255,150,0,255,144,0,255,138,0,255,131,0,255,125,0,255,119,0,255,113,0,255,107,0,255,101,0,255,96,0,255,90,0,255,83,0,255,77,0,255,71,0,255,65,0,255,60,0,255,54,0,255,48,0,255,42,0,255,35,0,255,29,0,255,23,0,255,17,0,255,11,0,255,5,0,255,0,0,255,0,5,255,0,12,255,0,18,255,0,24,255,0,30,255,0,36,255,0,42,255,0,48,255,0,54,255,0,60,255,0,65,255,0,71,255,0,78,255,0,83,255,0,90,255,0,95,255,0,102,255,0,108,255,0,114,255,0,120,255,0,126,255,0,131,255,0,137,255,0,143,255,0,149,255,0,156,255,0,162,255,0,168,255,0,174,255,0,180,255,0,186,255,0,192,255,0,197,255,0,203,255,0,210,255,0,215,255,0,222,255,0,227,255,0,234,255,0,239,255,0,246,255,0,252,255,0,255,252,0,255,246,0,255,240,0,255,234,0,255,228,0,255,222,0,255,216,0,255,209,0,255,203,0,255,197,0,255,192,0,255,186,0,255,180,0,255,174,0,255,168,0,255,162,0,255,155,0,255,150,0,255,143,0,255,138,0,255,131,0,255,126,0,255,120,0,255,114,0,255,107,0,255,102,0,255,95,0,255,90,0,255,83,0,255,78,0,255,71,0,255,65,0,255,60,0,255,54,0,255,48,0,255,42,0,255,36,0,255,30,0,255,24,0,255,18,0,255,11,0,255,5};
void hue(byte hue, byte * r, byte * g, byte * b) {
  *r = hueLookup[hue*3];
  *g = hueLookup[hue*3+1];
  *b = hueLookup[hue*3+2];
}

void clearStrip(Adafruit_NeoPixel * leds, byte r, byte g, byte b) {
  int i;
  for (i = 0; i<leds->numPixels(); i++) {
    leds->setPixelColor(i, leds->Color(r,g,b));
  }
}

void loop() {
  int i = 0;
  while(1) {
    i = chargingIndicator(&strip,i);
//    i = entireColorCycle(&strip,i);
    strip.show();
    delay(10);
  }
}

int entireColorCycle(Adafruit_NeoPixel * leds, int n) {
    int i;
    byte r, g, b;
    hue(n,&r,&g,&b);
    clearStrip(leds, r, g, b);
    return n >= 255 ? 0 : n+1;
}

int chargingIndicator(Adafruit_NeoPixel * leds, int n) {
    clearStrip(leds, 0, 0, 0);
    byte c = n > 50 ? 0 : (10*(n < 25 ? n : (25*2) - n));
    if (digitalRead(CHARGE_PIN)) {
      leds->setPixelColor(0, leds->Color(0,c,0));
    } else {
      leds->setPixelColor(0, leds->Color(c,c,0));      
    }
    return n >= (25*2+150) ? 0 : n+1;
}

//  colorWipe(strip.Color(255, 0, 0), 50); // Red
//  colorWipe(strip.Color(0, 255, 0), 50); // Green
//  colorWipe(strip.Color(0, 0, 255), 50); // Blue
//  
//  theaterChase(strip.Color(127, 127, 127), 50); // White
//  theaterChase(strip.Color(127,   0,   0), 50); // Red
//  theaterChase(strip.Color(  0,   0, 127), 50); // Blue
//
//  rainbow(20);
//  rainbowCycle(20);
//  theaterChaseRainbow(50);

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();
     
      delay(wait);
     
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
        }
        strip.show();
       
        delay(wait);
       
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, 0);        //turn every third pixel off
        }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

