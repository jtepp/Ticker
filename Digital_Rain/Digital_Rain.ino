#include <FastLED.h>

#define LED_TYPE    WS2812
#define COLOR_ORDER 

#define NUM_LEDS 50
#define NUM_STRIPS 5

#define FPS 15
#define BRIGHTNESS 60

CRGB white = CRGB::White;

// this method is really dumb but I wanna save memory instead of 
// storing the state of each LED in two seperate arrays
CRGB greens[] = {
   CRGB(0,255,0), // if = greens[0], random from greens[1-3]
   CRGB(0,255,5), // if past greens[1], increase index by 1
   CRGB(5,255,0),
   CRGB(0,255,5),
   CRGB(5,255,0),
   CRGB(0,200,0),
   CRGB(0,130,0),
   CRGB(0,80,0),
   CRGB(0,30,0)   
  };
  
CRGB off = CRGB(0,10,0);

CRGB leds[NUM_STRIPS][NUM_LEDS];


void setup() {
  FastLED.addLeds<LED_TYPE, 13, COLOR_ORDER>(leds[0], NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<LED_TYPE, 12, COLOR_ORDER>(leds[1], NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<LED_TYPE, 11, COLOR_ORDER>(leds[2], NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<LED_TYPE, 10, COLOR_ORDER>(leds[3], NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<LED_TYPE, 6, COLOR_ORDER>(leds[4], NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
  clearLEDs();
  FastLED.show();
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

}

void clearLEDs() {
  for (int i=0; i<NUM_STRIPS; i++) {
      for (int j=0; j<NUM_LEDS; j++) {
          leds[i][j] = 0;
      }
    }
  }
  
  
