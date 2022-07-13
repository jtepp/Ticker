#include <FastLED.h>
#include <WiFiNINA.h>
#include <SPI.h>
#include <String.h>

#define NUM_LEDS 30
#define NUM_STRIPS 5
#define MSG_LENGTH 300

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
int realLength = 0;

int canMakeRequest = 1;

char ssid[] = "TeppermanFamily";
char pass[] = "0110202165";

int status = WL_IDLE_STATUS;
int run = 1;
int counter = 0;

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

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(6000);
  }
  
  Serial.println("Connected to wifi");
}

void loop() {
  if (run == 1) {

  if (canMakeRequest == 1) {
    //  makeHTTPRequest("stocks","");
    makeHTTPRequest("text","Surely%20this%20message%20is%20long%20enough");
    canMakeRequest = 0;
  }
    
  translateMessage();
  Serial.println(realLength);
  
  }

  scrub();
  FastLED.show();

  
if (run == 1) {
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

  if (millis() >= 60000 && millis() % 60000 < 100 ) { // past the first minute and within first 0.1s of minute
      makeHTTPRequest("text", "NEWMESSAGE");
    }
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
        leds[i][NUM_LEDS-1] = message[i][counter % realLength]; // last led is first of past matrix
    }
    delay(1000/FPS);
  }

  void translateMessage() {
    // if there are incoming bytes available
  // from the server, read them and print them:
  while (client.available()) {

    char c = client.read();

    if ((c == '0' || c == '1') && outerIndex == 0) { //count length of actual line
              realLength++;
            }

    switch(c) {
        case '0': 
//        if (innerIndex<NUM_LEDS) {
//          leds[outerIndex][innerIndex] = CRGB(0,0,0);
//        }
          message[outerIndex][innerIndex] = CRGB(0,0,0);
        break;

        case '1':
//        if (innerIndex<NUM_LEDS) {
//          leds[outerIndex][innerIndex] = on;
//        }
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

  void makeHTTPRequest(String mode, String q) {
    if (client.connect(server, 80)) {
      digitalWrite(LED_BUILTIN, HIGH);
      fillLEDs(CRGB(0,0,0));
      innerIndex = -1;
      outerIndex = 0;
      realLength = 0;
      run = 1;
    // Make a HTTP request:
//    client.println("GET /.netlify/functions/ticker?mode=text&q=TOR:4-PHI:3%20OAK:6-TEX:6%20SD:3-COL:5%20 HTTP/1.1");
    String intro = "GET /.netlify/functions/ticker?mode=";
    String query = "&q=";
    String outro = " HTTP/1.1";
    String requestString = intro + mode + query + q + outro;
    client.println(requestString);
    client.println("Host: www.jacobtepperman.com");
    client.println("Connection: close");
    client.println();
    digitalWrite(LED_BUILTIN, LOW);
  }
    }
