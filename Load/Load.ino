#include <FastLED.h>
#include <WiFiNINA.h>
#include <SPI.h>

#define NUM_LEDS 30
#define NUM_STRIPS 5
#define MSG_LENGTH 150

#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
#define BRIGHTNESS 100

#define FPS 10

CRGB leds[NUM_STRIPS][NUM_LEDS];
CRGB message[NUM_STRIPS][MSG_LENGTH];

CRGB on = CRGB(255,0,0);

int useWifi = 1;
int innerIndex = -1;
int outerIndex = 0;

char ssid[] = "TeppermanFamily";
char pass[] = "0110202165";

int status = WL_IDLE_STATUS;
int run = 1;
int counter = NUM_LEDS;

char server[] = "www.jacobtepperman.com";

WiFiClient client;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
    fillLEDs(CRGB(0,0,0));
     FastLED.addLeds<LED_TYPE, 13, COLOR_ORDER>(leds[0], NUM_LEDS).setCorrection( TypicalLEDStrip );
     FastLED.addLeds<LED_TYPE, 12, COLOR_ORDER>(leds[1], NUM_LEDS).setCorrection( TypicalLEDStrip );
     FastLED.addLeds<LED_TYPE, 11, COLOR_ORDER>(leds[2], NUM_LEDS).setCorrection( TypicalLEDStrip );
     FastLED.addLeds<LED_TYPE, 10, COLOR_ORDER>(leds[3], NUM_LEDS).setCorrection( TypicalLEDStrip );
     FastLED.addLeds<LED_TYPE, 6, COLOR_ORDER>(leds[4], NUM_LEDS).setCorrection( TypicalLEDStrip );
     FastLED.setBrightness(  BRIGHTNESS );
  FastLED.show();
  Serial.begin(9600);
  if (useWifi == 1) {
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(6000);
  }
  
  Serial.println("Connected to wifi");
 
if (client.connect(server, 80)) {
    // Make a HTTP request:
//    client.println("GET /.netlify/functions/ticker?mode=text&q=TOR:4-PHI:3%20OAK:6-TEX:6%20SD:3-COL:5%20 HTTP/1.1");
    client.println("GET /.netlify/functions/ticker?mode=stocks HTTP/1.1");
    client.println("Host: www.jacobtepperman.com");
    client.println("Connection: close");
    client.println();
    digitalWrite(LED_BUILTIN, LOW);
  }
  }
}

void loop() {
  if (run == 1 && useWifi == 1) {
  // if there are incoming bytes available
  // from the server, read them and print them:
  while (client.available()) {

    char c = client.read();

    switch(c) {
        case '0': 
        if (innerIndex<NUM_LEDS) {
          leds[outerIndex][innerIndex] = CRGB(0,0,0);
        }
          message[outerIndex][innerIndex] = CRGB(0,0,0);
        break;

        case '1':
        if (innerIndex<NUM_LEDS) {
          leds[outerIndex][innerIndex] = on;
        }
          message[outerIndex][innerIndex] = on;
        break;

        case ',':
          innerIndex++;
//          innerIndex %= NUM_LEDS;
        break;

        case ']':
          outerIndex++;
          innerIndex = 0;
        break;
      }

  }
  }

  scrub();
  FastLED.show();

  
if (run == 1 & useWifi == 1) {
  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting from server.");
    client.stop();
    // do nothing forevermore:
    run = 0;
    
  }
}

  counter++;
}


void fillLEDs(CRGB col) {
  for (int i=0; i<NUM_STRIPS; i++) {
      for (int j=0; j<NUM_LEDS; j++) {
          leds[i][j] = col;
      }
      for (int j=0; j<MSG_LENGTH; j++) {
          message[i][j] = col;
        }
    }
  }


  void scrub() {
    for (int i=0; i<NUM_STRIPS; i++) {
      for (int j=1; j<NUM_LEDS; j++) { // the LEDs move left
          leds[i][j-1] = leds[i][j];
        }
        leds[i][NUM_LEDS-1] = message[i][counter % MSG_LENGTH]; // last led is first of past matrix
    }
    delay(1000/FPS);
  }
