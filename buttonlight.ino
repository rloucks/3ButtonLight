#include <FastLED.h>
#include "colorutils.h"
#include "colorpalettes.h"

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

//This is where we adjust things to match our unique project:
#define NUM_LEDS    12      // adjust this to the number of LEDs you have: 16 or more
#define LED_TYPE    WS2812B // adjust this to the type of LEDS. This is for Neopixels
#define DATA_PIN    5       // adjust this to the pin you've connected your LEDs to   
//#define BRIGHTNESS  245      // 255 is full brightness, 128 is half, 32 is an eighth.s
#define SATURATION  255     // 0-255, 0 is pure white, 255 is fully saturated color
#define BUTTON_PIN_L  13       // Connect the button to GND and one of the pins. 
#define BUTTON_PIN_M  12       // Connect the button to GND and one of the pins. 
#define BUTTON_PIN_R  11       // Connect the button to GND and one of the pins. 
#define UPDATES_PER_SECOND 100
#define COLOR_ORDER BRG     // Try mixing up the letters (RGB, GBR, BRG, etc) for a whole new world of color combinations
#define NUM_MODES 153         //Update this number to the highest number of "cases"
#define FORWARD 0
#define BACKWARD 1
#define SLOW 250
#define MEDIUM 50
#define FAST 5

uint8_t gHue = 0;           // rotating "base color" used by many of the patterns
uint16_t STEPS = 30;        // STEPS set dynamically once we've started up
uint16_t SPEED = 30;        // SPEED set dynamically once we've started up
uint16_t BRIGHTNESS = 245;        // Default Brightness


CRGB leds[NUM_LEDS];
CRGBPalette16 currentPalette;
TBlendType    currentBlending;

int ledMode = 0;

const TProgmemPalette16 MyColors_p PROGMEM =
{
  CRGB:: DarkBlue,
  CRGB:: HotPink,
  CRGB:: Teal,
  CRGB:: BlueViolet,
  CRGB:: DodgerBlue,    
  CRGB:: DeepPink,
  CRGB:: Turquoise,
  CRGB:: Indigo,
  CRGB:: DarkBlue,
  CRGB:: HotPink,
  CRGB:: Teal,
  CRGB:: BlueViolet,
  CRGB:: DodgerBlue,    
  CRGB:: DeepPink,
  CRGB:: Turquoise,
  CRGB:: Indigo,
};
byte prevKeyState_L = HIGH;         // Left button is active low
byte prevKeyState_M = HIGH;         // Middle button is active low
byte prevKeyState_R = HIGH;         // Right button is active low
//------------------SETUP------------------
void setup() {
  delay( 2000 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip); // ahh duhh
  FastLED.setBrightness(BRIGHTNESS); // Set the inital Brightness 
  currentBlending;
  pinMode(BUTTON_PIN_L, INPUT_PULLUP); // init Left Button
  pinMode(BUTTON_PIN_M, INPUT_PULLUP); // init Middle Button
  pinMode(BUTTON_PIN_R, INPUT_PULLUP); // init Right Button
}


//------------------MAIN LOOP------------------
void loop() {

  byte currKeyState_L = digitalRead(BUTTON_PIN_L); // is left pressed?
  if ((prevKeyState_L == LOW) && (currKeyState_L == HIGH)) {
    shortKeyPress_L();
  }
  prevKeyState_L = currKeyState_L; // back to high state

  byte currKeyState_M = digitalRead(BUTTON_PIN_M); // is middle pressed?

  if ((prevKeyState_M == LOW) && (currKeyState_M == HIGH)) {
    shortKeyPress_M();
  }
  prevKeyState_M = currKeyState_M; // back to high state
  byte currKeyState_R = digitalRead(BUTTON_PIN_R); // is right pressed?

  if ((prevKeyState_R == LOW) && (currKeyState_R == HIGH)) {
    shortKeyPress_R();
  }
  prevKeyState_R = currKeyState_R; // back to high state

  static uint8_t startIndex = 0;
  startIndex = startIndex + 20; /* motion speed */

  switch (ledMode) {
       case 0: rainbow(); break;
       case 1: juggle();  break;
       case 2: sinelon(); break;
       case 3: confetti(); break;
       case 4: fadeColors();  break;
       case 5: rainbow(SLOW,1); break;
       case 6: theaterChaseRainbow(1,MEDIUM); break;
       case 7: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: AliceBlue); break;
       case 8: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: Amethyst); break;
       case 9: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: AntiqueWhite); break;
       case 10: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: Aqua); break;
       case 11: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: Aquamarine); break;
       case 12: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: Azure); break;
       case 13: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: Beige); break;
       case 14: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: Bisque); break;
       case 15: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: Black); break;
       case 16: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: BlanchedAlmond); break;
       case 17: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: Blue); break;
       case 18: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: BlueViolet); break;
       case 19: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: Brown); break;
       case 20: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: BurlyWood); break;
       case 21: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: CadetBlue); break;
       case 22: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: Chartreuse); break;
       case 23: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: Chocolate); break;
       case 24: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: Coral); break;
       case 25: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: CornflowerBlue); break;
       case 26: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: Cornsilk); break;
       case 27: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: Crimson); break;
       case 28: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: Cyan); break;
       case 29: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: DarkBlue); break;
       case 30: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: DarkCyan); break;
       case 31: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: DarkGoldenrod); break;
       case 32: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: DarkGray); break;
       case 33: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: DarkGreen); break;
       case 34: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: DarkKhaki); break;
       case 35: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: DarkMagenta); break;
       case 36: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: DarkOliveGreen); break;
       case 37: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: DarkOrange); break;
       case 38: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: DarkOrchid); break;
       case 39: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: DarkRed); break;
       case 40: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: DarkSalmon); break;
       case 41: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: DarkSeaGreen); break;
       case 42: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: DarkSlateBlue); break;
       case 43: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: DarkSlateGray); break;
       case 44: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: DarkTurquoise); break;
       case 45: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: DarkViolet); break;
       case 46: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: DeepPink); break;
       case 47: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: DeepSkyBlue); break;
       case 48: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: DimGray); break;
       case 49: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: DodgerBlue); break;
       case 50: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: FireBrick); break;
       case 51: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: FloralWhite); break;
       case 52: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: ForestGreen); break;
       case 53: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: Fuchsia); break;
       case 54: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: Gainsboro); break;
       case 55: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: GhostWhite); break;
       case 56: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: Gold); break;
       case 57: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: Goldenrod); break;
       case 58: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: Gray); break;
       case 59: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: Green); break;
       case 60: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: GreenYellow); break;
       case 61: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: Honeydew); break;
       case 62: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: HotPink); break;
       case 63: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: IndianRed); break;
       case 64: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: Indigo); break;
       case 65: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: Ivory); break;
       case 66: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: Khaki); break;
       case 67: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: Lavender); break;
       case 68: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: LavenderBlush); break;
       case 69: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: LawnGreen); break;
       case 70: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: LemonChiffon); break;
       case 71: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: LightBlue); break;
       case 72: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: LightCoral); break;
       case 73: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: LightCyan); break;
       case 74: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: LightGoldenrodYellow); break;
       case 75: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: LightGreen); break;
       case 76: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: LightGrey); break;
       case 77: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: LightPink); break;
       case 78: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: LightSalmon); break;
       case 79: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: LightSeaGreen); break;
       case 80: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: LightSkyBlue); break;
       case 81: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: LightSlateGray); break;
       case 82: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: LightSteelBlue); break;
       case 83: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: LightYellow); break;
       case 84: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: Lime); break;
       case 85: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: LimeGreen); break;
       case 86: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: Linen); break;
       case 87: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: Magenta); break;
       case 88: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: Maroon); break;
       case 89: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: MediumAquamarine); break;
       case 90: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: MediumBlue); break;
       case 91: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: MediumOrchid); break;
       case 92: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: MediumPurple); break;
       case 93: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: MediumSeaGreen); break;
       case 94: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: MediumSlateBlue); break;
       case 95: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: MediumSpringGreen); break;
       case 96: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: MediumTurquoise); break;
       case 97: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: MediumVioletRed); break;
       case 98: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: MidnightBlue); break;
       case 99: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: MintCream); break;
       case 100: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: MistyRose); break;
       case 101: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: Moccasin); break;
       case 102: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: NavajoWhite); break;
       case 103: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: Navy); break;
       case 104: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: OldLace); break;
       case 105: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: Olive); break;
       case 106: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: OliveDrab); break;
       case 107: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: Orange); break;
       case 108: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: OrangeRed); break;
       case 109: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: Orchid); break;
       case 110: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: PaleGoldenrod); break;
       case 111: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: PaleGreen); break;
       case 112: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: PaleTurquoise); break;
       case 113: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: PaleVioletRed); break;
       case 114: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: PapayaWhip); break;
       case 115: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: PeachPuff); break;
       case 116: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: Peru); break;
       case 117: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: Pink); break;
       case 118: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: Plaid); break;
       case 119: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: Plum); break;
       case 120: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: PowderBlue); break;
       case 121: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: Purple); break;
       case 122: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: Red); break;
       case 123: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: RosyBrown); break;
       case 124: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: RoyalBlue); break;
       case 125: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: SaddleBrown); break;
       case 126: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: Salmon); break;
       case 127: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: SandyBrown); break;
       case 128: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: SeaGreen); break;
       case 129: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: Seashell); break;
       case 130: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: Sienna); break;
       case 131: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: Silver); break;
       case 132: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: SkyBlue); break;
       case 133: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: SlateBlue); break;
       case 134: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: SlateGray); break;
       case 135: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: Snow); break;
       case 136: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: SpringGreen); break;
       case 137: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: SteelBlue); break;
       case 138: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: Tan); break;
       case 139: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: Teal); break;
       case 140: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: Thistle); break;
       case 141: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: Tomato); break;
       case 142: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: Turquoise); break;
       case 143: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: Violet); break;
       case 144: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: Wheat); break;
       case 145: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: White); break;
       case 146: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: WhiteSmoke); break;
       case 147: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: Yellow); break;
       case 148: FastLED.setBrightness(BRIGHTNESS); fill_solid(leds, NUM_LEDS, CRGB:: YellowGreen); break;
       case 149:  colorWipe(CRGB::Black,FORWARD,MEDIUM);
       case 150:  disolve(15,100,MEDIUM);
       case 151:  cylon(randomColor(), 10,SLOW);
       case 152:  theaterChase(randomColor(),10,SLOW);
  }
  
  FastLED.show();
  FastLED.delay(1000 / SPEED);  
 
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow

}

//------------------Button Press Setup------------------
void shortKeyPress_L() {
  ledMode++;
    if (ledMode > NUM_MODES){
    ledMode = 0; 
  }  
}
void shortKeyPress_M() {
  BRIGHTNESS = BRIGHTNESS + 10;
    if (BRIGHTNESS > 255){
    BRIGHTNESS = 25; 
  } 
  ledMode = ledMode; 
}
void shortKeyPress_R() {
  ledMode = ledMode - 1;
    if (ledMode = 0){
    ledMode = NUM_MODES; 
  }  
}


//GLITTER
void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::Gray;
  }
}

void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 32);
  fadeToBlackBy( leds, NUM_LEDS, 255-BRIGHTNESS); // fade the whole strip down.
}

void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 255, BRIGHTNESS*2); //Adjusted Brightness with variable
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 16);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16(i+7,0,NUM_LEDS)] |= CHSV(dothue, 255, BRIGHTNESS*2);//Adjusted Brightness with variable
    dothue += 32;
  }
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fill_rainbow( leds, NUM_LEDS, gHue, 32);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = RainbowColors_p; //can adjust the palette here
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255); 
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}


void fadeColors() {
    for( int colorStep=0; colorStep<256; colorStep++ ) {

      int r = colorStep;  // Redness starts at zero and goes up to full
      int b = 255-colorStep;  // Blue starts at full and goes down to zero
      int g = 0;              // No green needed to go from blue to red

      // Now loop though each of the LEDs and set each one to the current color

      for(int x = 0; x < NUM_LEDS; x++){
          leds[x] = CRGB(r,g,b);
      }
    }
}

void allColor(CRGB c){
  for(int i=0; i<NUM_LEDS; i++){
    leds[i] = c;
  }
  FastLED.show();
}

void allRandom(){
  for(int i=0; i<NUM_LEDS; i++){
    leds[i] = randomColor();
  }
  FastLED.show(); 
}

void disolve(int simultaneous, int cycles, int speed){
  for(int i=0; i<cycles; i++){
    for(int j=0; j<simultaneous; j++){
      int idx = random(NUM_LEDS);
      leds[idx] = CRGB::Black;
    }
    FastLED.show();
    delay(speed);
  }
  allColor(CRGB::Black);
}

// Wipes color from end to end
void colorWipe(CRGB c, int speed, int direction){
  for(int i=0; i<NUM_LEDS; i++){
    if(direction == FORWARD){
      leds[i] = c;
    }
    else{
      leds[NUM_LEDS-1-i] = c;
    }
    FastLED.show();
    delay(speed);
  }
}

// Rainbow colors that slowly cycle across LEDs
void rainbow(int cycles, int speed){ // TODO direction
  if(cycles == 0){
    for(int i=0; i< NUM_LEDS; i++) {
      leds[i] = Wheel(((i * 256 / NUM_LEDS)) & 255);
    }
    FastLED.show();
  }
  else{
    for(int j=0; j<256*cycles; j++) {
      for(int i=0; i< NUM_LEDS; i++) {
        leds[i] = Wheel(((i * 256 / NUM_LEDS) + j) & 255);
      }
      FastLED.show();
      delay(speed);
    }
  }
}

// Theater-style crawling lights
void theaterChase(CRGB c, int cycles, int speed){ // TODO direction
 for (int j=0; j<cycles; j++) {  
    for (int q=0; q < 3; q++) {
      for (int i=0; i < NUM_LEDS; i=i+3) {
        int pos = i+q;
        leds[pos] = c;    //turn every third pixel on
      }
      FastLED.show();
      delay(speed);
      for (int i=0; i < NUM_LEDS; i=i+3) {
        leds[i+q] = CRGB::Black;        //turn every third pixel off
      }
    }
  }
}

// Theater-style crawling lights with rainbow effect
void theaterChaseRainbow(int cycles, int speed){ // TODO direction, duration
  for (int j=0; j < 256 * cycles; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (int i=0; i < NUM_LEDS; i=i+3) {
        int pos = i+q;
        leds[pos] = Wheel( (i+j) % 255);    //turn every third pixel on
      }
      FastLED.show();
      delay(speed);
      for (int i=0; i < NUM_LEDS; i=i+3) {
        leds[i+q] = CRGB::Black;  //turn every third pixel off
      }
    }
  }
}

// Sliding bar across LEDs
void cylon(CRGB c, int width, int speed){
  // First slide the leds in one direction
  for(int i = 0; i <= NUM_LEDS-width; i++) {
    for(int j=0; j<width; j++){
      leds[i+j] = c;
    }
    FastLED.show();
    // now that we've shown the leds, reset to black for next loop
    for(int j=0; j<5; j++){
      leds[i+j] = CRGB::Black;
    }
    delay(speed);
  }

  // Now go in the other direction.  
  for(int i = NUM_LEDS-width; i >= 0; i--) {
    for(int j=0; j<width; j++){
      leds[i+j] = c;
    }
    FastLED.show();
    for(int j=0; j<width; j++){
      leds[i+j] = CRGB::Black;
    }
    delay(speed);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.

CRGB Wheel(byte WheelPos) {
  if(WheelPos < 85) {
    return CRGB(WheelPos * 3, 255 - WheelPos * 3, 0);
  } 
  else if(WheelPos < 170) {
    WheelPos -= 85;
    return CRGB(255 - WheelPos * 3, 0, WheelPos * 3);
  } 
  else {
    WheelPos -= 170;
    return CRGB(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

CRGB randomColor(){
  return Wheel(random(256)); 
}
