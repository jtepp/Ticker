#include <FastLED.h>

#define LED_TYPE    WS2812
#define COLOR_ORDER GRB

#define FPS 10
#define BRIGHTNESS 50

#define NUM_LEDS 30
#define NUM_STRIPS 5

struct Point {
  int x;
  int y;  
};
typedef struct Point point;

class Ball {
  public:
  int x_vel = 1;
  int y_vel = 1;
  point p = {0,0};
    Ball() {}

  void move() {
    if ((p.x == NUM_LEDS-1 && x_vel > 0) || (p.x == 0 && x_vel < 0)) {
        x_vel *= -1;
      }

    if ((p.y == NUM_STRIPS-1 && y_vel > 0) || (p.y == 0 && y_vel < 0)) {
        y_vel *= -1;
      }

    p.x += x_vel;
    p.y += y_vel;
  }  
  
};

CRGB on = CRGB(255,0,0);
CRGB off = CRGB(0,0,0);

Ball b = Ball();

CRGB leds[NUM_STRIPS][NUM_LEDS];

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
  leds[b.p.y][b.p.x] = on;
  FastLED.show();
  clearLEDs();
  b.move();
  delay(1000/FPS);
}

void clearLEDs() {
  for (int i=0; i<NUM_STRIPS; i++) {
      for (int j=0; j<NUM_LEDS; j++) {
          leds[i][j] = 0;
      }
    }
  }
