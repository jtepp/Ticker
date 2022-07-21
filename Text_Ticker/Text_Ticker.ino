#include <FastLED.h>
#include <WiFiNINA.h>
#include "arduino_secrets.h"
/* TEXT TICKER
 * by Jacob Tepperman - July 2022
 * 
 * DESCRIPTION:
 * This is the first program I made for my ticker.
 * It shows text that scrolls across the LEDs and 
 * can be set to show static text or use a preset
 * mode on the text conversion server
 * 
 * 
 * OPERATION:
 * By this point you should already have the Arduino
 * plugged into your computer and know how to upload
 * a program. If you need help with that, just google
 * it. For configuring your preferences, read the
 * below sections SERVER MODES, CONSTANTS, and 
 * IMPORTANT VARIABLES, and change any of those values
 * to make the ticker display whatever you desire.
 * 
 * 
 * SERVER MODES:
 * - text:
 *   pass a q value of your custom text
 *   
 * - stocks:
 *   pass a q value of a comma separated list of
 *   desired symbols
 * 
 * CONSTANTS:
 * - NUM_LEDS: # of LEDs on your light strip to work with
 * 
 * - NUM_STRIPS: # of LED strips you're using. The server only has letter
 *               conversion compatible with 5 strips so unless you're making your
 *               own conversion server, use 5 strips
 * 
 * - PT_REFRESH: # of complete passthroughs (text getting to the end of the strip)
 *               between each refresh of the message to display on the ticker
 *
 * - FPS: # of times the strip updates (scrolls) per second. It's implemented below in a delay(1000/FPS)
 *        so feel free to replace that with just a delay if you want
 *
 * - BRIGHTNESS: brightness of the LEDs 0-255
 * 
 * - MODE: The desired server mode. Options are listed above the CONSTANTS section
 * 
 * - Q: The query for the server. Has different effects depending on the server mode.
 *      Make sure all spaces are replaced with %20. For other URL sensitive characters/
 *      symbols, just google [symbol name] URL-encoded
 * 
 * IMPORTANT VARIABLES:
 * - run: a true/false value that when false makes the ticker display a preset "HELLO" text,
 *        and when true makes the ticker display the desired web-fetched text
 * 
 * - on: the color you want the message to be shown in
 * 
 * - refresh: whether or not you want the text to update periodically
 */

#define MSG_LENGTH 600
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
 
#define NUM_LEDS 50
#define NUM_STRIPS 5

#define PT_REFRESH 4
#define FPS 10
#define BRIGHTNESS 255
#define MODE "text"
#define Q "This%20was%20a%20great%20idea"



CRGB on = CRGB::Green;

bool refresh = false;

/**********stuff not to change below**********/
byte wipeOrder[NUM_STRIPS] = {0,4,1,3,2}; // order to load strips in. used to stop it from looking like it wipes down every refresh

CRGB leds[NUM_LEDS];
byte message[NUM_STRIPS][MSG_LENGTH] = {
  {0,0,0,0,0,1,0,1,0,1,1,1,0,1,0,0,0,1,0,0,0,0,1,1,0},
  {0,0,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,1},
  {0,0,0,0,0,1,1,1,0,1,1,1,0,1,0,0,0,1,0,0,0,1,0,0,1},
  {0,0,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,1},
  {0,0,0,0,0,1,0,1,0,1,1,1,0,1,1,1,0,1,1,1,0,0,1,1,0}
  };


int innerIndex = -1;
int outerIndex = 0;
int realLength = 25;

bool run = true;
bool canMakeRequest = true;

int status = WL_IDLE_STATUS;
int counter = 0;
byte passThroughs = 0;

char server[] = "www.jacobtepperman.com";

WiFiClient client;

void setup() {
  clearLEDs();
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
     FastLED.addLeds<LED_TYPE, 13, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
     FastLED.addLeds<LED_TYPE, 12, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
     FastLED.addLeds<LED_TYPE, 11, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
     FastLED.addLeds<LED_TYPE, 10, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
     FastLED.addLeds<LED_TYPE, 6, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
     FastLED.setBrightness(  BRIGHTNESS );
  FastLED.show();
  Serial.begin(9600);

  while (run && status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(6000);
  }

  if (run)
  Serial.println("Connected to wifi");
}

void loop() {
  if (run) {

  if (canMakeRequest) {
    action();
    canMakeRequest = false;
  }
    
  // if there are incoming bytes available
  // from the server, read them and print them:
  while (client.available()) {

    char c = client.read();

    if (c == '0' || c == '1') { //count length of actual line
              if (innerIndex>realLength && c == '1') {
                realLength = innerIndex; // find the deepest '1'
                }
    }
    switch(c) {
        case '0': 

          message[outerIndex][innerIndex] = 0;
        break;
        case '1':
          message[outerIndex][innerIndex] = 1;
        break;
        case ',':
          innerIndex++;
        break;
        case ']':
          outerIndex++;
          innerIndex = 0;
        break;
      }

  

  }
  
  }
  scroll();

  
if (run) {
  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting from server.");
    client.stop();
    // do nothing forevermore:
    run = false;
    
  }
}

  counter++;


  if (counter % min(realLength, MSG_LENGTH) == 0 && refresh) { // at each start
      if (passThroughs == PT_REFRESH) {
        action();
        passThroughs = 0;
      }
      passThroughs++;
    }
}
  void action() {
    //      makeHTTPRequest("stocks","GME,TSLA,TLRY");
    makeHTTPRequest(MODE,Q);
//      makeHTTPRequest("waves","");
//      makeHTTPRequest("text","AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
}


  void scroll() {
    for (int i=0; i<NUM_STRIPS; i++) {
        shift(leds, message[wipeOrder[i]]);
        FastLED[wipeOrder[i]].showLeds();
    }
    delay(1000/FPS);
  }

  void shift(CRGB* arr, byte* msg) {
    for (int j=0; j<NUM_LEDS; j++) { // fill LEDs based on message, using counter as an offset
          arr[j] = msg[(counter + j) % min(realLength + 5, MSG_LENGTH)] == 0 ? CRGB(0,0,0) : on;
        }
    }

  void makeHTTPRequest(String mode, String q) {
    if (client.connect(server, 80)) {
      digitalWrite(LED_BUILTIN, HIGH);
      innerIndex = -1;
      outerIndex = 0;
      realLength = 0;
      run = true;

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

    
void clearLEDs() {
  for (int j=0; j<NUM_LEDS; j++) {
          leds[j] = 0;
      }
  for (int i=0; i<NUM_STRIPS; i++) {
      if (run) {
        for (int j=0; j<MSG_LENGTH; j++) {
            message[i][j] = 0;
          }
      }
    }
  }
