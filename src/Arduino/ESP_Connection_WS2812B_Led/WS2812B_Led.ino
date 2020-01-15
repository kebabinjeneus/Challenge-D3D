/*
 * Documentation:
 * https://github.com/FastLED/FastLED/wiki/Basic-usage
 * 
 * Color-picker:
 * https://www.w3schools.com/colors/colors_picker.asp
*/

#include "FastLED.h"

#define NUM_LEDS 8                        // Number of leds: 8
#define DATA_PIN 14                       // Connected to GPIO 14, NodeMCU pin D5
#define BRIGHTNESS 120                     // Sets the brightness of the leds between 0-255 

CRGB leds[NUM_LEDS];                      // Block of memory that will be used for storing and manipulating the led data

void setup() {
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
//leds[ID] = CRGB(red, green, blue) Only use value of 10 to reduce the amp draw of the strip when lit
  leds[0] = CRGB(0,255,0);                // GREEN
  leds[1] = CRGB(0,255,0);                // GREEN
  leds[2] = CRGB(191,255,0);              // LIGHT-GREEN
  leds[3] = CRGB(255,255,0);              // YELLOW
  leds[4] = CRGB(255,191,0);              // ORANGE
  leds[5] = CRGB(255,64,0);               // LIGHT-RED
  leds[6] = CRGB(255,0,0);                // RED
  leds[7] = CRGB(255,0,0);                // RED
  FastLED.show();
  delay(30000);                           // Update every 30 seconds
}
