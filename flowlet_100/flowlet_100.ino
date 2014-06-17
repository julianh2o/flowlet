#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

#define CHARGE_PIN 12
#define LED_PIN 4
#define bracelet_length 30
#define BUTTON_1 8
#define BUTTON_2 9

Adafruit_NeoPixel strip = Adafruit_NeoPixel(bracelet_length, LED_PIN, NEO_GRB + NEO_KHZ800);

int lpDelay(int quarterSeconds) {
  int oldClkPr = CLKPR;  // save old system clock prescale
  CLKPR = 0x80;    // Tell the AtMega we want to change the system clock
  CLKPR = 0x08;    // 1/256 prescaler = 60KHz for a 16MHz crystal
  delay(quarterSeconds);  // since the clock is slowed way down, delay(n) now acts like delay(n*256)
  CLKPR = 0x80;    // Tell the AtMega we want to change the system clock
  CLKPR = oldClkPr;    // Restore old system clock prescale
}

void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.setBrightness(10);
  strip.show();
  pinMode(CHARGE_PIN,INPUT_PULLUP);
  pinMode(BUTTON_1,INPUT_PULLUP);
  pinMode(BUTTON_2,INPUT_PULLUP);
  power_adc_disable();
}

uint8_t isinTable8[] = { 
  0, 4, 9, 13, 18, 22, 27, 31, 35, 40, 44, 
  49, 53, 57, 62, 66, 70, 75, 79, 83, 87, 
  91, 96, 100, 104, 108, 112, 116, 120, 124, 128, 

  131, 135, 139, 143, 146, 150, 153, 157, 160, 164, 
  167, 171, 174, 177, 180, 183, 186, 190, 192, 195, 
  198, 201, 204, 206, 209, 211, 214, 216, 219, 221, 

  223, 225, 227, 229, 231, 233, 235, 236, 238, 240, 
  241, 243, 244, 245, 246, 247, 248, 249, 250, 251, 
  252, 253, 253, 254, 254, 254, 255, 255, 255, 255, 
}; 

int isin(long x) {
  boolean pos = true;  // positive - keeps an eye on the sign.
  if (x < 0) {
    x = -x;
    pos = !pos;  
  }  
  if (x >= 360) x %= 360;   
  if (x > 180) {
    x -= 180;
    pos = !pos;
  }
  if (x > 90) x = 180 - x;
  if (pos) return isinTable8[x]/2 ;
  return -isinTable8[x]/2 ;
}

boolean isButtonPressed(int button) {
  return !digitalRead(button);
}

boolean debounceButton(int button) {
  if (!isButtonPressed(button)) return false;
  while(isButtonPressed(button));
  return true;
}

boolean anyButtonPressed() {
  return isButtonPressed(BUTTON_1) || isButtonPressed(BUTTON_2);
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

unsigned long button_start = 0;
boolean button_down = false;
unsigned long manageButtonHold(boolean reset) {
  if (reset) {
    button_down = false;
    button_start = 0;
  }
  
  if (button_down == false) {
    if (isButtonPressed(BUTTON_1)) {
      button_down = true;
      button_start = millis();
    }
  } else {
    if (!isButtonPressed(BUTTON_1)) {
      button_down = false;
      button_start = 0;
    }
  }
  return button_down == false ? 0 : millis() - button_start;
}

static inline bool usbConnected(void) {
  return ((USBSTA & (1 << VBUS)) ? true : false);
}

boolean isDoneCharging() {
  return digitalRead(CHARGE_PIN);
}

boolean testMode = false;
void loop() {
  int i = 0;
  boolean on = false;
  int mode = 2;
  
  while(1) {
    if ((testMode && isButtonPressed(BUTTON_1)) || manageButtonHold(false) > 2000) {
      on = !on;
      while (testMode && isButtonPressed(BUTTON_1));
      manageButtonHold(true);
      delay(200);
    }
    
    if (debounceButton(BUTTON_2)) {
      mode++;
    }
    
    if (!on) { //if we're in off mode, handle special things
      if (usbConnected()) {
        clearStrip(&strip, 0,0,0);
        if (isDoneCharging()) {
          strip.setPixelColor(0, strip.Color(0,30,0));
        } else {
          strip.setPixelColor(0, strip.Color(30,30,0));          
        }
        strip.show();
        delay(200);
      } else {
        clearStrip(&strip, 0,0,0);
        strip.show();
        if (!testMode && !anyButtonPressed()) lpDelay(4); //sleep the chip
        delay(100);
      }
    } else {

      switch(mode) {
        case 0: i = entireColorCycle(&strip,i); break;
        case 1: i = rotatingColorCycle(&strip,i,1,.5); break;
        case 2: i = colorWave(&strip,i,2,1,0,0,255,255,255,255); break;
        case 3: i = colorWave(&strip,i,2,2,0,0,255,255,255,255); break;
        case 4: i = colorWave(&strip,i,2,3,0,0,255,255,255,255); break;
        default: mode = 0; break;
      }
      strip.show();
      delay(10);
    }
  }
}

int entireColorCycle(Adafruit_NeoPixel * leds, int n) {
    int i;
    byte r, g, b;
    hue(n,&r,&g,&b);
    clearStrip(leds, r, g, b);
    return n >= 255 ? 0 : n+1;
}

int rotatingColorCycle(Adafruit_NeoPixel * leds, int n, float scale, float spd) {
    int i,l;
    int v;
    byte r, g, b;
    int animationLength = scale*leds->numPixels();
    float huePerLed = ((float)255/(float)animationLength);
    for (i=0; i<leds->numPixels(); i++) {
      l = ((int)(i+(n*spd))) % animationLength;
      v = (int)(l*huePerLed);
      hue(v,&r,&g,&b);
      leds->setPixelColor(i,leds->Color(r,g,b));
    }
    return n >= (int)((float)animationLength/spd) ? 0 : n+1;
}

int colorWave(Adafruit_NeoPixel * leds, int n, byte cyclesPerStrip, float spd, byte r1, byte g1, byte b1, byte r2, byte g2, byte b2) {
    int i;
    byte r, g, b;
    int a;
    float ratio;
    float multiplier = ((float)360*(int)cyclesPerStrip)/leds->numPixels();
    spd = spd * 3;
    for (i=0; i<leds->numPixels(); i++) {
      a = isin((int)(multiplier*i+n*spd));
      ratio = ((float)(a + 128)) / (float)255;
      
//      r = (int)((float)255 * ratio);
//      g = (int)((float)255 * ratio);
//      b = (int)((float)255 * ratio);
      
//      Serial.print((float)r1*ratio);
//      Serial.print("  ");
//      Serial.print((float)r2*(1.0/ratio));
//      Serial.print("  ");
//      Serial.print((1.0-ratio));
//      Serial.println();
      r = (int)((float)r1*ratio + (float)r2*(1.0-ratio));
      g = (int)((float)g1*ratio + (float)g2*(1.0-ratio));
      b = (int)((float)b1*ratio + (float)b2*(1.0-ratio));
      leds->setPixelColor(i,leds->Color(r,g,b));
    }
    return n >= 360*spd ? 0 : n+1;
}

int coolmistake1(Adafruit_NeoPixel * leds, int n, byte cyclesPerStrip, float spd, byte r1, byte g1, byte b1, byte r2, byte g2, byte b2) {
    int i;
    byte r, g, b;
    int a;
    float ratio;
    float multiplier = ((float)360*(int)cyclesPerStrip)/leds->numPixels();
    spd = spd * 3;
    for (i=0; i<leds->numPixels(); i++) {
      a = isin((int)(multiplier*i+n*spd));
      ratio = ((float)(a + 128)) / (float)255;
      
//      r = (int)((float)255 * ratio);
//      g = (int)((float)255 * ratio);
//      b = (int)((float)255 * ratio);
      Serial.println(ratio);
      r = (int)((float)r1*ratio + (float)r2*(1.0/ratio));
      g = (int)((float)g1*ratio + (float)g2*(1.0/ratio));
      b = (int)((float)b1*ratio + (float)b2*(1.0/ratio));
      leds->setPixelColor(i,leds->Color(r,g,b));
    }
    return n >= 360*spd ? 0 : n+1;
}

//int chargingIndicator(Adafruit_NeoPixel * leds, int n) {
//    clearStrip(leds, 0, 0, 0);
//    byte c = n > 50 ? 0 : (10*(n < 25 ? n : (25*2) - n));
//    if (digitalRead(CHARGE_PIN)) {
//      leds->setPixelColor(0, leds->Color(0,c,0));
//    } else {
//      leds->setPixelColor(0, leds->Color(c,c,0));      
//    }
//    return n >= (25*2+150) ? 0 : n+1;
//}
