#include <Segma.h>

#define SYMBOLS_COUNT           14              // the number of characters on the screen (together with the special character). 32 max!
#define SPECIAL_SYMBOL_NUMBER   0               // special character number
#define GRID_REGISTERS_COUNT    2               // number of shift registers for grids

#define SYMBOL_LATCH_PIN        14
#define SYMBOL_DATA_PIN         16
#define SYMBOL_CLOCK_PIN        12
#define GRID_LATCH_PIN          2
#define GRID_DATA_PIN           10
#define GRID_CLOCK_PIN          13

bool blinkFlag;
uint32_t specScreenTimer;

Display display(SYMBOL_LATCH_PIN,
                SYMBOL_DATA_PIN,
                SYMBOL_CLOCK_PIN,
                GRID_LATCH_PIN,
                GRID_DATA_PIN,
                GRID_CLOCK_PIN,
                SYMBOLS_COUNT,
                SPECIAL_SYMBOL_NUMBER,
                GRID_REGISTERS_COUNT);

void helloScreen() {
  display.setText("hello");
}

void specScreen() {
  if(millis() >= specScreenTimer){
    specScreenTimer = millis()+100;
    blinkFlag = !blinkFlag;
    display.setSpec(0,blinkFlag);
  }
}

void setup() {
  display.setShiftOrder(LSBFIRST, MSBFIRST);    // shift direction for symbols and grids (default is LSBFIRST, MSBFIRST)
  display.begin();                              // initializing the display
  display.setScreen(helloScreen);               // setting up the screen function
  display.setSpecScreen(specScreen);            // setting up the screen function for special characters
  display.setFrameInterval(10);                 // target frame interval (0 is unlimited)
  display.setSymbolInversion(false);            // invert symbol bits (default false)
  display.setGreedInversion(false);             // invert greed bits (default false)
  //display.setStepDelay(1000);                 // delay between drawing characters in microseconds (for gas-discharge and fluorescent indicators)
  
  /*
  Total frame rate is approximately: 1000 / FrameInterval + SYMBOLS_COUNT * StepDelay / 1000
  */
}

void loop() {
  display.update();                             // perform dynamic indication
}
