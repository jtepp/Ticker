#include <FastLED.h>
#include "Letters.h"

#define NUM_LEDS 5

#define BRIGHTNESS  64
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB

int* codes[NUM_STRIPS] = {
    0, 0, 0, 0, 0
};

int* temp[NUM_STRIPS] = {
    0, 0, 0, 0, 0
  };
int counter[NUM_STRIPS] = {
    0, 0, 0, 0, 0
  };

//int temp[NUM_LEDS] = {0,0,0,1,0};
//int test[NUM_LEDS] = {0,0,0,1,0};

CRGB leds[NUM_STRIPS][NUM_LEDS];

CRGB green = CRGB::Green;
CRGB black = CRGB::Black;

#define UPDATES_PER_SECOND 3


void setup() {
  Serial.begin(115200);
    // delay( 3000 ); // power-up safety delay
     FastLED.addLeds<LED_TYPE, 3, COLOR_ORDER>(leds[0], NUM_LEDS).setCorrection( TypicalLEDStrip );
//     FastLED.addLeds<LED_TYPE, 4, COLOR_ORDER>(leds[1], NUM_LEDS).setCorrection( TypicalLEDStrip );
//     FastLED.addLeds<LED_TYPE, 5, COLOR_ORDER>(leds[2], NUM_LEDS).setCorrection( TypicalLEDStrip );
//     FastLED.addLeds<LED_TYPE, 7, COLOR_ORDER>(leds[3], NUM_LEDS).setCorrection( TypicalLEDStrip );
//     FastLED.addLeds<LED_TYPE, 8, COLOR_ORDER>(leds[4], NUM_LEDS).setCorrection( TypicalLEDStrip );
//     FastLED.setBrightness(  BRIGHTNESS );
    fillCodesFromChar('G');
    printCodes();
    
    for (int i = 0; i<10; i++) {
      Serial.println('\n');
      scrubArray(codes[0], temp[0] + (i % NUM_LEDS));
      printCodes();
    }
//    fillStrips();
//    FastLED.delay(300);
}


void loop() {
  
  // fillStrips();
  //  FastLED.delay(300);
}

void fillStrips() {
      for(int i = 0; i < NUM_STRIPS; i++) {
        for(int j = 0; j < NUM_LEDS; j++) {
        // leds[i][j] = codes[i][j] == 1 ? green : black;
        }
         scrubArray(codes[i], temp[i] + (counter[i]++ % NUM_LEDS));
      }
      
 }


void fillCodesFromChar(char x) {
    for(int i = 0; i < NUM_STRIPS; i++) {
        codes[i] = &letterMap(x)[i * LETTER_LENGTH];
        temp[i] = &letterMap(x)[i * LETTER_LENGTH];
//        Serial.println(letterMap(x)[i]);
    }
  }

void scrubArray(int* arr, int* first) {
  for (int i=1; i<NUM_LEDS; i++) {
    arr[i-1] = arr[i];
  }
  arr[NUM_LEDS - 1] = *first;
}

void printCodes() {
  for(int i = 0; i < NUM_STRIPS; i++) {
        for(int j = 0; j < NUM_LEDS; j++) {
        Serial.print(codes[i][j]);
      }
      Serial.println();
    }

//for(int j = 0; j < NUM_LEDS; j++) {
//        Serial.print(test[j]);
//      }
}
