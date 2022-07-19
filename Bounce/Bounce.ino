#include <FastLED.h>

#define LED_TYPE    WS2812
#define COLOR_ORDER GRB

#define FPS 10
#define BRIGHTNESS 50

#define NUM_LEDS 30
#define NUM_STRIPS 5

#define NUM_BALLS 5

CRGB leds[NUM_STRIPS][NUM_LEDS];

struct Point {
  int x;
  int y;  
};
typedef struct Point point;

class Ball {
  private:
  bool overflowX = false;
  public:
  int x_vel = 1;
  int y_vel = 1;
  point p = {0,0};
  CRGB on = CRGB(255,255,255);
    Ball(CRGB sOn) {
        on = sOn;
      }
    Ball(CRGB sOn, int sX, int sY) {
      on = sOn;
      p.x = sX;
      p.y = sY;
    }
    Ball(CRGB sOn, int sX, int sY, int xV, int yV) {
      on = sOn;
      p.x = sX;
      p.y = sY;
      x_vel = xV;
      y_vel = yV;
    }

  void move() {
    if ((p.x == NUM_LEDS-1 && x_vel > 0) || (p.x == 0 && x_vel < 0)) {
        if (!overflowX)
        x_vel *= -1;
      }

    if ((p.y == NUM_STRIPS-1 && y_vel > 0) || (p.y == 0 && y_vel < 0)) {
        y_vel *= -1;
      }

    p.x += x_vel;
    p.y += y_vel;
    
    if (overflowX) {
        p.x %= NUM_LEDS;
        if (p.x < 0) {
          p.x = NUM_LEDS-1;  
        }
    }

  }

  void show() {
    leds[p.y][p.x] = on;
  }

  void allowOverflowX() {
      overflowX = true;
    }
  
};

CRGB off = CRGB(0,0,0);
Ball balls[NUM_BALLS] = {
    Ball(CRGB::Red),
    Ball(CRGB::Blue, 3, 1),
    Ball(CRGB::Green, 10, 3, -1, 1),
    Ball(CRGB::Orange, NUM_LEDS-1, 4, 1, 1),
    Ball(CRGB::Purple, 20, 2, 1, -1)
  };
  
void setup() {
   clearLEDs();
  FastLED.addLeds<LED_TYPE, 13, COLOR_ORDER>(leds[0], NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<LED_TYPE, 12, COLOR_ORDER>(leds[1], NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<LED_TYPE, 11, COLOR_ORDER>(leds[2], NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<LED_TYPE, 10, COLOR_ORDER>(leds[3], NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<LED_TYPE, 6, COLOR_ORDER>(leds[4], NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
  FastLED.show();

  balls[0].allowOverflowX();
}

void loop() {
  for (Ball b : balls){
   b.show();
  }
  FastLED.show();
  clearLEDs();
  for (int i=0; i<NUM_BALLS; i++){
   balls[i].move();
  }
  delay(1000/FPS);
}

void clearLEDs() {
  for (int i=0; i<NUM_STRIPS; i++) {
      for (int j=0; j<NUM_LEDS; j++) {
          leds[i][j] = 0;
      }
    }
  }
