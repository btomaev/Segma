#pragma once

#include "Arduino.h"

class Display {
  private:
    int symbolLatch;
    int symbolData;
    int symbolClock;
    int greedLatch;
    int greedData;
    int greedClock;
    int symbolsCount;
    int specPosition;
    int registersCount;
    char symbols[32];
    byte specs;
    bool symbolInversion;
    bool greedInversion;
    bool symbolsShiftOrder = LSBFIRST;
    bool greedShiftOrder = MSBFIRST;
    int stepDelay;
    
    void (*doScreen)();
    void (*doSpec)();

    int fontLength = 0;
    int* specials;
    char* symbolsFont;

  public:
    Display(int sl, int sd, int sc, int gl, int gd, int gc, int count, int spec, int regs, bool si=false, bool gi=false);
    Display(int sl, int sd, int sc, int gl, int gd, int gc, int count, int spec, int regs, char _symbolsFont[], int _fontSize, bool si=false, bool gi=false);
    Display(int sl, int sd, int sc, int gl, int gd, int gc, int count, int spec, int regs, int _specials[], char _symbolsFont[], int _fontSize, bool si=false, bool gi=false);

    void begin();

    void update();

    void setShiftOrder(bool _symbolsShiftOrder, bool _greedShiftOrder);
    
    void setStepDelay(int delay); // microseconds

    void setFont(int _specials[], char _symbolsFont[], int _fontLength);

    void setText(String text);

    void setSpec(int specid, bool value);

    void setSymbol(int number, byte value);

    void setScreen(void (*screen)());

    void setSpecScreen(void (*screen)());

    byte getSymbolCode(char symbol);
};