#include <FastLED.h>

#define LED_TYPE    WS2812
#define COLOR_ORDER GRB

#define NUM_LEDS 50
#define NUM_STRIPS 5

#define FPS 10
#define BRIGHTNESS 50

#define RAIN_F 5
#define REAL_GREENS 5
CRGB white = CRGB(100,100,100);


CRGB greens[] = {
    white,
   CRGB(0,200,0),
   CRGB(0,180,0),
   CRGB(0,170,0),
   CRGB(0,160,0),
   CRGB(0,150,0),
   CRGB(0,50,0),
   CRGB(0,30,0),
   CRGB(0,20,0),
   CRGB(0,10,0),
  };
  
CRGB off = CRGB(0,0,0);

CRGB leds[NUM_STRIPS][NUM_LEDS];
byte states[NUM_LEDS];


void setup() {
  clearLEDs();
  FastLED.addLeds<LED_TYPE, 13, COLOR_ORDER>(leds[0], NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<LED_TYPE, 12, COLOR_ORDER>(leds[1], NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<LED_TYPE, 11, COLOR_ORDER>(leds[2], NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<LED_TYPE, 10, COLOR_ORDER>(leds[3], NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<LED_TYPE, 6, COLOR_ORDER>(leds[4], NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
  FastLED.show();

}

void loop() {
  scroll();
  FastLED.show();
  delay(1000/FPS);
}

void clearLEDs() {
  for (int i=0; i<NUM_STRIPS; i++) {
      for (int j=0; j<NUM_LEDS; j++) {
          leds[i][j] = 0;
          if (i==0) {
            states[j] = 0;
          }
      }
    }
  }


void scroll() {
 for (int i=NUM_STRIPS-1; i>0; i--) { // pull down from bottom
        for (int j=0; j<NUM_LEDS; j++) {
          leds[i][j] = leds[i-1][j];
        }
  }
  for (int i=0; i<NUM_LEDS; i++) {
          switch(states[i]) {
            case 0: // off
              if (rand() % RAIN_F == 0) { // allow it to stay off
                  states[i]++;
                }
            break;
            case 1: // white
              states[i] += 1 + round(randomFloat()*(REAL_GREENS));
            break;
            case 11:
              states[i] = 0;
              break;
            default: // all others (white, first fade, greens) are just state changes of 2-9
              states[i]++;
          } 
          leds[0][i] = colorFromState(states[i]);
  }
}

CRGB colorFromState(byte state) {
    switch(state) {
      case 0:
        return off;
      case 1:
        return white;
      case 6:
      case 7:
      case 8:
      case 9:
      case 10:
        return greens[state]; // fade tail
      default:
        return greens[(int) (randomFloat()*REAL_GREENS) + 1];     
      }
  }

float randomFloat() {
    return random(0,100)/100.0;
  }
