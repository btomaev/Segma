#pragma once

#include "Arduino.h"

class Display {
  private:
    uint symbolLatch;
    uint symbolData;
    uint symbolClock;
    uint greedLatch;
    uint greedData;
    uint greedClock;
    uint symbolsCount;
    uint specPosition;
    uint registersCount;
    char symbols[32];
    byte specs;
    bool symbolInversion;
    bool greedInversion;
    bool symbolsShiftOrder = LSBFIRST;
    bool greedShiftOrder = MSBFIRST;
    uint stepDelay;
    
    void (*doScreen)();
    void (*doSpec)();

    uint fontLength = 0;
    uint* specials;
    char* symbolsFont;

  public:
    Display(uint sl, uint sd, uint sc, uint gl, uint gd, uint gc, uint count, uint spec, uint regs, bool si=false, bool gi=false);
    Display(uint sl, uint sd, uint sc, uint gl, uint gd, uint gc, uint count, uint spec, uint regs, char _symbolsFont[], uint _fontSize, bool si=false, bool gi=false);
    Display(uint sl, uint sd, uint sc, uint gl, uint gd, uint gc, uint count, uint spec, uint regs, uint _specials[], char _symbolsFont[], uint _fontSize, bool si=false, bool gi=false);

    void begin();

    void update();

    void setShiftOrder(bool _symbolsShiftOrder, bool _greedShiftOrder);
    
    void setStepDelay(uint delay); // in microseconds

    void setFont(uint _specials[], char _symbolsFont[], uint _fontLength);

    void setText(String text);

    void setSpec(uint specid, bool value);

    void setSymbol(uint number, char value);

    void setScreen(void (*screen)());

    void setSpecScreen(void (*screen)());

    byte getSymbolCode(char symbol);
};