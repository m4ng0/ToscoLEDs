#include <FastLED.h>

#define NUM_LEDS 196
#define CHIPSET WS2811
#define DATA_PIN 6

#define STD_DELAY 5
#define DROP_DELAY 5
#define CHASING_DELAY 20
#define FADING_DELAY 8

#define TOSCO_HUE1 16
#define TOSCO_HUE2 160

#define qsubd(x, b)  ((x>b)?sin_brightness:(x>(b/2)?sin_brightness/2:(x>(b/3)?sin_brightness/3:(x>(b/4)?sin_brightness/4:0))))    // digital unsigned subtraction macro

// the array of leds
CRGB leds[NUM_LEDS];
CHSV hsvvalues[NUM_LEDS];
CHSV tosco_color1 = CHSV(TOSCO_HUE1, 255, 255);
CHSV tosco_color2 = CHSV(TOSCO_HUE2, 255, 255);
CHSV tosco_white = CHSV(85, 0, 255);
CHSV tosco_colors[3];
CHSV italian_green = CHSV(106, 255, 200);
CHSV italian_white = CHSV(85, 0, 200);
CHSV italian_red = CHSV(252, 200, 200);
uint8_t sin_speed = 4;
uint8_t sin_freq = 16;         // wave frequency, i.e. width of bars
uint8_t sin_cutoff = 140;      // wave cutoff: lower value means longer wave
uint8_t sin_brightness = 200;  // brightness added/removed by the wave
uint8_t fading_max_delta = 220;

void setup() {
  // sanity check delay
  delay(2000);

  tosco_colors[0] = tosco_color1;
  tosco_colors[1] = tosco_color2;
  tosco_colors[2] = tosco_white;
  FastLED.addLeds<CHIPSET, DATA_PIN, GRB>(leds, NUM_LEDS);
  
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = tosco_white; // full white
  }
}

void loop() {
 waterDrop(DROP_DELAY);
 chaseLeft(3, CHASING_DELAY);
 chaseRight(3, CHASING_DELAY);
 setItalianFlag(DROP_DELAY);
 chaseRight(3, CHASING_DELAY*4);
 tosco_random(DROP_DELAY);
 fadings(5, FADING_DELAY);
 sinusoide(4*NUM_LEDS, CHASING_DELAY);
 rainbow(12, DROP_DELAY);
}

void waterDrop(int dropDelay) {
  for (int a = NUM_LEDS/2; a >= 0; a--) {
    for (int b = 0; b <= a; b++) {
      if (b == 0) {
        leds[0] = tosco_color2;
        leds[NUM_LEDS] = tosco_color1;
      } else {
        leds[b - 1] = tosco_white;
        leds[NUM_LEDS - b ] = tosco_white;
        leds[b] = tosco_color2;
        leds[NUM_LEDS -1 - b] = tosco_color1;
      }
      FastLED.show();
      FastLED.delay(a < (NUM_LEDS/4) ? dropDelay : dropDelay/2);
    }
  }
}

void chaseLeft(uint8_t cycles, int chase_delay) {
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
      FastLED.delay(chase_delay);
    }
  }
}

void chaseRight(uint8_t cycles, int chase_delay) {
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
      FastLED.delay(chase_delay);
    }
  }
}

void setItalianFlag(int dropDelay) {
  for (int l = 0; l < NUM_LEDS; l++) {
    int modulo = (l % 22);
    if (modulo < 7) {
      leds[l] = italian_green;
    } else if (modulo < 14) {
      leds[l] = italian_white;
    } else if (modulo < 21) {
      leds[l] = italian_red;
    } else {
      leds[l] = tosco_color2;
    }
    FastLED.show();
    FastLED.delay(dropDelay);
  }
}

void tosco_random(int dropDelay) {
  for (int l = 0; l < NUM_LEDS; l++) {
    hsvvalues[l] = tosco_colors[random8()%3];
    leds[l] = hsvvalues[l];
    FastLED.show();
    FastLED.delay(dropDelay);
  }
}

void sinusoide(int ncycles, int sdelay) {
  int thisphase = 0;
  for (int c = 0; c < ncycles; c++) {
    for (int l = 0; l < NUM_LEDS; l++) {
      int thisbright = qsubd(cubicwave8(l*sin_freq + thisphase), sin_cutoff);
      leds[l].maximizeBrightness();
      leds[l].fadeLightBy(thisbright);
    }
    FastLED.show();
    FastLED.delay(sdelay);
    thisphase+=sin_speed;
  }
}

void fadings(int ncycles, int fdelay) {
  for (int c = 0; c < ncycles; c++) {
    for (int f = 0; f < 2*fading_max_delta; f++) {
      for (int l = 0; l < NUM_LEDS; l++) {
        hsvvalues[l].value -= (f < fading_max_delta? 1 : -1);
      }
      // bulk conversion
      hsv2rgb_rainbow(hsvvalues, leds, NUM_LEDS);
      FastLED.show();
      FastLED.delay(fdelay);
    }
  }
}

void rainbow(int ncycles, int rdelay) {
  uint8_t rainbowhue = 0;
  uint8_t deltahue = 4;
  for (int c = 0; c < ncycles; c++) {
    for (int l = 0; l < NUM_LEDS; l++) {
      fill_rainbow(leds, NUM_LEDS, rainbowhue++, deltahue);
      FastLED.show();
      FastLED.delay(rdelay);
    }
  }
}
