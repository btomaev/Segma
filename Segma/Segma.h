#pragma once

#include "Arduino.h"

class Display {
  private:
    uint symbolLatch = 0;
    uint symbolData = 0;
    uint symbolClock = 0;
    uint greedLatch = 0;
    uint greedData = 0;
    uint greedClock = 0;
    uint symbolsCount = 0;
    uint specPosition = 0;
    uint registersCount = 0;
    char symbols[32] = {};
    byte specs;
    bool hasSpec = false;
    bool symbolInversion = false;
    bool greedInversion = false;
    bool symbolsShiftOrder = LSBFIRST;
    bool greedShiftOrder = MSBFIRST;
    uint32_t stepDelay = 0;
    uint32_t frameInterval = 0;
    uint32_t lastFrame = 0;
    
    void (*doScreen)();
    void (*doSpec)();

    uint fontLength = 0;
    uint* specials;
    char* symbolsFont;

  public:
    Display(uint sl, uint sd, uint sc, uint gl, uint gd, uint gc, uint count, uint regs);
    Display(uint sl, uint sd, uint sc, uint gl, uint gd, uint gc, uint count, uint spec, uint regs);

    void begin();

    void update();

    void setShiftOrder(bool _symbolsShiftOrder, bool _greedShiftOrder);
    
    void setStepDelay(uint delay); // in microseconds

    void setFont(uint _specials[], char _symbolsFont[], uint _fontLength);

    void setText(String text);

    void setSpec(uint specid, bool value);

    void setSymbol(uint number, char value);

    void setDot(uint number, bool value);

    void setScreen(void (*screen)());

    void setSpecScreen(void (*screen)());

    void setFrameInterval(uint32_t interval); // in milliseconds

    void setSymbolInversion(bool value);

    void setGreedInversion(bool value);

    byte getSymbolCode(char symbol);

    byte getSymbolFromScreenBuffer(uint position);
};