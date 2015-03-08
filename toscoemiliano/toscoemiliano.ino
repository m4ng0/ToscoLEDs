#include <FastLED.h>

#define NUM_LEDS 98
#define CHIPSET WS2811
#define DATA_PIN 6

#define STD_DELAY 20

#define TOSCO_HUE1 32
#define TOSCO_HUE2 160

// the array of leds
CRGB leds[NUM_LEDS];

void setup() {
  // sanity check delay
  delay(2000);

  FastLED.addLeds<CHIPSET, DATA_PIN, RGB>(leds, NUM_LEDS);
  
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(TOSCO_HUE1, 0, 255); // full white
  }
}

void loop() {
 for (int a = NUM_LEDS; a >0; a--) {
   for (int b = 0; b < a; b++) {
     if (b == 0) {
       leds[0] = CHSV(TOSCO_HUE1, 255, 255);
     } else {
       leds[b - 1] = CHSV(TOSCO_HUE1, 0, 255);
       leds[b] = CHSV(TOSCO_HUE1, 255, 255);
     }
     FastLED.show();
     FastLED.delay(STD_DELAY);
   }
 }
 for (int a = NUM_LEDS; a >0; a--) {
   for (int b = 0; b < a; b++) {
     if (b == 0) {
       leds[0] = CHSV(TOSCO_HUE2, 255, 255);
     } else {
       leds[b - 1] = CHSV(TOSCO_HUE2, 0, 255);
       leds[b] = CHSV(TOSCO_HUE2, 255, 255);
     }
     FastLED.show();
     FastLED.delay(STD_DELAY);
   }
 }
}
