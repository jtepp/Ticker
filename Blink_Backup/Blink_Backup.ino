#include <FastLED.h>
#include "Letters.h"

#define BRIGHTNESS  64
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB

int codes[NUM_STRIPS][NUM_LEDS] = {
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0}
};
CRGB leds[NUM_STRIPS][NUM_LEDS];

CRGB green = CRGB::Green;
CRGB black = CRGB::Black;

#define UPDATES_PER_SECOND 3


void setup() {
  Serial.begin(115200);
    delay( 3000 ); // power-up safety delay
    FastLED.addLeds<LED_TYPE, 3, COLOR_ORDER>(leds[0], NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.addLeds<LED_TYPE, 4, COLOR_ORDER>(leds[1], NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.addLeds<LED_TYPE, 5, COLOR_ORDER>(leds[2], NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.addLeds<LED_TYPE, 7, COLOR_ORDER>(leds[3], NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.addLeds<LED_TYPE, 8, COLOR_ORDER>(leds[4], NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );
    fillCodesFromArray(TWO);
}


void loop() {
    fillStrips();
    FastLED.delay(300);
//    FastLED.delay(1000 / UPDATES_PER_SECOND);
}

void fillCodesFromArray(int arr[NUM_STRIPS][NUM_LEDS]) {  
  for(int i = 0; i < NUM_STRIPS; i++) {
        for(int j = 0; j < NUM_LEDS; j++) {
        codes[i][j] = arr[i][j];
        }
      }
}

void fillStrips() {
      for(int i = 0; i < NUM_STRIPS; i++) {
        for(int j = 0; j < NUM_LEDS; j++) {
        leds[i][j] = codes[i][j] == 1 ? green : black;
        }
        scrubArray(codes[i]);
      }
      
 }


void scrubArray(int arr[NUM_LEDS]) {
  int temp = arr[NUM_LEDS - 1];
  Serial.print(temp);
  Serial.print(' ');
  for (int i=0; i<NUM_LEDS; i++) {
    arr[i-1] = arr[i];
  }
  arr[0] = temp;
  Serial.println(arr[0]);
}
