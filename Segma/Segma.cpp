#include "Segma.h"
#include "fonts/defaultFont.h"

void static blankScreen(){}

void maxCharactersError( void ) __attribute__((error("32 characters maximum!")));

Display::Display(uint sl, uint sd, uint sc, uint gl, uint gd, uint gc, uint count, uint regs):
  symbolLatch(sl), symbolData(sd), symbolClock(sc), greedLatch(gl), greedData(gd), greedClock(gc),
  symbolsCount(count), registersCount(regs), hasSpec(false),
  specs(symbolInversion ? ~0b00000000 : 0b00000000)
{
  setFont(default_specs, default_symbols_font, sizeof(default_symbols_font));
  setScreen(blankScreen);
  setSpecScreen(blankScreen);
}

Display::Display(uint sl, uint sd, uint sc, uint gl, uint gd, uint gc, uint count, uint spec, uint regs):
  symbolLatch(sl), symbolData(sd), symbolClock(sc), greedLatch(gl), greedData(gd), greedClock(gc),
  symbolsCount(count), specPosition(spec), registersCount(regs), hasSpec(true),
  specs(symbolInversion ? ~0b00000000 : 0b00000000)
{
  setFont(default_specs, default_symbols_font, sizeof(default_symbols_font));
  setScreen(blankScreen);
  setSpecScreen(blankScreen);
}

void Display::begin() {
  // if(symbolsCount > 32) maxCharactersError();
  pinMode(symbolLatch, OUTPUT);
  pinMode(symbolData, OUTPUT);
  pinMode(symbolClock, OUTPUT);
  pinMode(greedLatch, OUTPUT);
  pinMode(greedData, OUTPUT);
  pinMode(greedClock, OUTPUT);

  digitalWrite(greedLatch, HIGH);
  digitalWrite(symbolLatch, HIGH);
}

void Display::update() {
  if(millis() < lastFrame) return;
  lastFrame = millis() + frameInterval;

  if(lastFrame < frameInterval) return;

  if(hasSpec)
  doSpec();
  doScreen();
  
  for(int i=0; i<symbolsCount; i++) {
    int64_t target = 0;
    bitSet(target, i);
    if(greedInversion) target = ~target;
    
    digitalWrite(greedLatch, LOW);
    digitalWrite(symbolLatch, LOW);

    delayMicroseconds(stepDelay);
    
    for(int j=0; j<registersCount; j++){
      shiftOut(greedData, greedClock, greedShiftOrder, greedShiftOrder == MSBFIRST ? highByte(target << 8 * j) : lowByte(target >> 8 * j));
    }

    char symbol = symbolInversion ? ~symbols[i] : symbols[i];

    shiftOut(symbolData, symbolClock, specPosition == i ? MSBFIRST : symbolsShiftOrder, specPosition == i ? symbol : getSymbolCode(symbol));

    digitalWrite(greedLatch, HIGH);
    digitalWrite(symbolLatch, HIGH);
  }
}

void Display::setShiftOrder(bool _symbolsShiftOrder, bool _greedShiftOrder) {
  symbolsShiftOrder = _symbolsShiftOrder;
  greedShiftOrder = _greedShiftOrder;
}

void Display::setStepDelay(uint delay) {
  stepDelay = delay;
}

void Display::setFont(uint _specials[], char _symbolsFont[], uint _fontLength) {
  specials = _specials;
  symbolsFont = _symbolsFont;
  fontLength = _fontLength;
}

void Display::setText(String text) {
  memset(symbols, 0, sizeof(symbols));
  text.toLowerCase();
  if(hasSpec)
  text = text.substring(0,specPosition)+" "+text.substring(specPosition,32);
  text.toCharArray(symbols, 32);
  symbols[specPosition] = specs;
}

void Display::setSpec(uint specid, bool value) {
  if(!hasSpec) return;
  bitWrite(specs, specials[specid], value);
  symbols[specPosition] = specs;
}

void Display::setSymbol(uint number, char value) {
  symbols[number] = value;
}

void Display::setDot(uint number, bool value) {
  if(number == specPosition) return;
  bitWrite(symbols[number], 7, value);
}

void Display::setScreen(void (*screen)()) {
  doScreen = *screen;
}

void Display::setSpecScreen(void (*screen)()) {
  if(!hasSpec) return;
  doSpec = *screen;
}

void Display::setFrameInterval(uint32_t interval) {
  frameInterval = interval;
}

void Display::setSymbolInversion(bool value) {
  symbolInversion = value;
}

void Display::setGreedInversion(bool value) {
  greedInversion = value;
}

byte Display::getSymbolCode(char symbol) {
  for(int i=0; i<fontLength; i+=2) {
    if(symbolsFont[i] == symbol) return symbolsFont[i+1];
  }
  return symbolInversion ? ~0b00000000 : 0b00000000;
}

byte Display::getSymbolFromScreenBuffer(uint position) {
  return lowByte(symbols[position]);
}
