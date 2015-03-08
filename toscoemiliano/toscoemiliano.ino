#include <FastLED.h>

#define NUM_LEDS 98
#define CHIPSET WS2811
#define DATA_PIN 6

#define STD_DELAY 5
#define SHORT_DELAY 20

#define TOSCO_HUE1 16
#define TOSCO_HUE2 160

// the array of leds
CRGB leds[NUM_LEDS];

void setup() {
  // sanity check delay
  delay(2000);

  FastLED.addLeds<CHIPSET, DATA_PIN, GRB>(leds, NUM_LEDS);
  
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(TOSCO_HUE1, 0, 255); // full white
  }
}

void loop() {
 waterDrop();
 chaseLeft(3);
 chaseRight(3);
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

void waterDrop() {
  for (int a = NUM_LEDS/2; a >= 0; a--) {
    for (int b = 0; b <= a; b++) {
      if (b == 0) {
        leds[0] = CHSV(TOSCO_HUE2, 255, 255);
        leds[NUM_LEDS] = CHSV(TOSCO_HUE1, 255, 255);
      } else {
        leds[b - 1] = CHSV(TOSCO_HUE2, 0, 255);
        leds[NUM_LEDS - b ] = CHSV(TOSCO_HUE1, 0, 255);
        leds[b] = CHSV(TOSCO_HUE2, 255, 255);;
        leds[NUM_LEDS -1 - b] = CHSV(TOSCO_HUE1, 255, 255);
      }
      FastLED.show();
      FastLED.delay(STD_DELAY);
    }
  }
}

void chaseLeft(uint8_t cycles) {
  for (uint8_t c = 0; c < cycles; c++) {
    for (int p = 0; p < NUM_LEDS; p++) {
      CRGB temp = leds[0];
      for (int q = 1; q <= NUM_LEDS; q++) {
        if (q == NUM_LEDS) {
          leds[NUM_LEDS - 1] = temp;
        } else {
          leds[q - 1] = leds[q];
        }
      }
      FastLED.show();
      FastLED.delay(SHORT_DELAY);
    }
  }
}

void chaseRight(uint8_t cycles) {
  for (uint8_t c = 0; c < cycles; c++) {
    for (int p = 0; p < NUM_LEDS; p++) {
      CRGB temp = leds[NUM_LEDS - 1];
      for (int q = NUM_LEDS - 2; q >= -1; q--) {
        if (q == -1) {
          leds[0] = temp;
        } else {
          leds[q + 1] = leds[q];
        }
      }
      FastLED.show();
      FastLED.delay(SHORT_DELAY);
    }
  }
}
