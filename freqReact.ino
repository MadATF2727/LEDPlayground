// Fast Forier Transform Test Program for Circuit Playground
// Mike Barela for Adafruit Industries    September, 2016

#include <Adafruit_CircuitPlayground.h>
#include <FastLED.h>
#define BINS   32          // The number of FFT frequency bins
#define FRAMES 7           // This many FFT cycles are averaged 

// parameters for LEDs 
#define LED_PIN     6    //led strand is soldered to pin 6
#define CP_PIN      17   //circuit playground's neopixels live on pin 17
#define NUM_LEDS    60   // number of LEDs in my strand
#define NUM_CP      10   // number of neopixels on the circuit playground
#define COLOR_ORDER GRB

  
CRGB leds[NUM_LEDS];  //LED strip
//Adafruit_NeoPixel leds = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
CRGB cp[NUM_CP];      //Playground LEDs
CRGBPalette16 currentPalette;
TBlendType    currentBlending;
    
uint8_t brightness = 255;  //led strand brightness control
uint8_t cpbrightness = 40;  //circuit playground brightness control

void setup() {
  CircuitPlayground.begin();  // Set up the board library and serial
  Serial.begin(9600);
  FastLED.addLeds<WS2812B, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.addLeds<WS2812B, CP_PIN, COLOR_ORDER>(cp, NUM_CP);
}

void loop() {
  uint8_t i,j, h1,h2, numFill;
  uint16_t spectrum[BINS];     // FFT spectrum output buffer
  uint16_t avg[BINS];          // The average of FRAME "listens"

  for(j=1; j <= FRAMES; j++) {             // We gather data FRAMES times and average it
     CircuitPlayground.mic.fft(spectrum);  // Here is the CP listen and FFT the data routine
     for(i=0; i < BINS; i++) {             // Add for an average
       if(spectrum[i] > 255) spectrum[i] = 255; // limit outlier data
       if(i == 0)
         avg[i] = spectrum[i];
       else
         avg[i] = avg[i] + spectrum[i];
     }
  } 
  for(i=0; i < BINS; i++) {               // For each output bin average
     avg[i] = avg[i] / FRAMES;            //  divide about the number of values aaveraged
  }
  int maxVal = 0, maxIndex = 0;
  for(i=0; i < BINS; i++) {               // For each output bin average
     if(avg[i] >= maxVal) {               //  find the peak value
       maxVal = avg[i];
       maxIndex = i;                      //  and the bin that max value is in
     }
  }
  for(j=0; j < 32; j++) {           // print spectrum 32 bins
     Serial.print(avg[j]);
     Serial.print(" ");
  }
  Serial.println("");              // and print the highest value and the bin it is in
  Serial.print("Max Value = "); Serial.print(maxVal);
  Serial.print(", Index of Max Value = "); Serial.println(maxIndex);
  //propPixels(numFill, h1, h2);
  //after determining the max index and max value, use the max index to feed an if/else block which will cause the LED strip to light a different color based on index
  if (maxIndex < 10){
      h1 = 192;
      h2 = 128;
      if (maxIndex == 0){
           h1 = 192;
           h2 = 128;
           numFill = 3;
      }
      else if (maxIndex == 1){
           h1 = 181;
           h2 = 143;
           numFill = 5;
      }
      else if (maxIndex == 2){
           h1 = 173;
           h2 = 127;
           numFill = 7;
      }
      else if (maxIndex == 3){
           h1 = 162;
           h2 = 217;
           numFill = 1;
      }
      else if (maxIndex == 4){
           h1 = 200;
           h2 = 37;
           numFill = 5;
      }
      else if (maxIndex == 5){
           h1 = 111;
           h2 = 96;
           numFill = 7;
      }
      else if (maxIndex == 6){
           h1 = 42;
           h2 = 179;
           numFill = 7;
      }
      else if (maxIndex == 7){
           h1 = 214;
           h2 = 76;
           numFill = 7;
      }
      else if (maxIndex == 8){
           h1 = 31;
           h2 = 139;
           numFill = 7;      }
      else{
           h1 = 153;
           h2 = 65;
           numFill = 7;
      }
  propPixels(numFill, h1, h2);
  }
  else if (maxIndex >= 10 and maxIndex < 20){
      h1 = 160;
      h2 = 32;
      if (maxIndex == 10){
           numFill = 0;
      }
      else if (maxIndex == 11){
        numFill = 1;
      }
      else if (maxIndex == 12){
        numFill = 3;
      }
      else if (maxIndex == 13){
        numFill = 4;
      }
      else if (maxIndex == 14){
        numFill = 5;
      }
      else if (maxIndex == 15){
        numFill = 6;
      }
      else if (maxIndex == 16){
        numFill = 7;
      }
      else if (maxIndex == 17){
        numFill = 8;
      }
      else if (maxIndex == 18){
        numFill = 9;
      }
      else{
        numFill = 10;
      }
  propPixels(numFill, h1, h2);
  }

  else{
    h1 = 64;
    h2 = 64;
    numFill = 5;
    propPixels(numFill, h1, h2);
  }
}


void propPixels(uint8_t numFill, uint8_t h1, uint8_t h2){
  //for (int i = 0; i < NUM_LEDS; i++)leds[i] = CHSV(0,0,255);
  currentPalette = RainbowColors_p;
  for (int k = 0; k < numFill; k++) cp[k]  = ColorFromPalette( currentPalette, h1, cpbrightness, LINEARBLEND);
  FastLED.show();
  for (int m = numFill; m < NUM_CP; m++) cp[m] = ColorFromPalette( currentPalette, h2, cpbrightness, LINEARBLEND);
  FastLED.show();
}


