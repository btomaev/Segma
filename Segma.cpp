#include "Segma.h"
#include "fonts/defaultFont.h"

void static blankScreen(){}

void maxCharactersError( void ) __attribute__((error("32 characters maximum!")));

Display::Display(uint sl, uint sd, uint sc, uint gl, uint gd, uint gc, uint count, uint regs) {

  this->symbolLatch = sl;
  this->symbolData = sd;
  this->symbolClock = sc;
  this->greedLatch = gl;
  this->greedData = gd;
  this->greedClock = gc;
  this->symbolsCount = count;
  this->registersCount = regs;
  this->specs = this->symbolInversion ? ~0b00000000 : 0b00000000;
  
  this->setFont(default_specs, default_symbols_font, sizeof(default_symbols_font));
  this->setScreen(blankScreen);
  this->setSpecScreen(blankScreen);
}

Display::Display(uint sl, uint sd, uint sc, uint gl, uint gd, uint gc, uint count, uint spec, uint regs) {

  this->symbolLatch = sl;
  this->symbolData = sd;
  this->symbolClock = sc;
  this->greedLatch = gl;
  this->greedData = gd;
  this->greedClock = gc;
  this->symbolsCount = count;
  this->specPosition = spec;
  this->registersCount = regs;
  this->specs = this->symbolInversion ? ~0b00000000 : 0b00000000;
  this->hasSpec = true;
  
  this->setFont(default_specs, default_symbols_font, sizeof(default_symbols_font));
  this->setScreen(blankScreen);
  this->setSpecScreen(blankScreen);
}

void Display::begin() {
  pinMode(this->symbolLatch, OUTPUT);
  pinMode(this->symbolData, OUTPUT);
  pinMode(this->symbolClock, OUTPUT);
  pinMode(this->greedLatch, OUTPUT);
  pinMode(this->greedData, OUTPUT);
  pinMode(this->greedClock, OUTPUT);

  digitalWrite(this->greedLatch, HIGH);
  digitalWrite(this->symbolLatch, HIGH);
}

void Display::update() {
  if(millis()<this->lastFrame)return;

  this->lastFrame = millis()+this->frameInterval;

  if(this->hasSpec)
  this->doSpec();
  this->doScreen();
  
  for(int i=0; i<this->symbolsCount; i++) {
    int64_t target = 0;
    bitSet(target, i);
    target = this->greedInversion ? ~target : target;
    
    digitalWrite(this->greedLatch, LOW);
    digitalWrite(this->symbolLatch, LOW);

    delayMicroseconds(this->stepDelay);
    
    for(int j=0; j<this->registersCount; j++){
      shiftOut(this->greedData, this->greedClock, this->greedShiftOrder, this->greedShiftOrder == MSBFIRST ? highByte(target << 8*j) : lowByte(target >> 8*j));
    }

    char symbol = this->symbolInversion ? ~this->symbols[i] : this->symbols[i];

    shiftOut(this->symbolData, this->symbolClock, this->specPosition == i ? MSBFIRST : this->symbolsShiftOrder, this->specPosition == i? symbol : this->getSymbolCode(symbol));

    digitalWrite(greedLatch, HIGH);
    digitalWrite(symbolLatch, HIGH);
  }
}

void Display::setShiftOrder(bool _symbolsShiftOrder, bool _greedShiftOrder) {
  this->symbolsShiftOrder = _symbolsShiftOrder;
  this->greedShiftOrder = _greedShiftOrder;
}

void Display::setStepDelay(uint delay) {
  this->stepDelay = delay;
}

void Display::setFont(uint _specials[], char _symbolsFont[], uint _fontLength) {
  this->specials = _specials;
  this->symbolsFont = _symbolsFont;
  this->fontLength = _fontLength;
}

void Display::setText(String text) {
  memset(this->symbols, 0, sizeof(this->symbols));
  text.toLowerCase();
  if(this->hasSpec)
  text = text.substring(0,this->specPosition)+" "+text.substring(this->specPosition,32);
  text.toCharArray(symbols, 32);
  this->symbols[this->specPosition] = this->specs;
}

void Display::setSpec(uint specid, bool value) {
  if(!this->hasSpec) return;
  bitWrite(this->specs, specials[specid], value);
  this->symbols[this->specPosition] = this->specs;
}

void Display::setSymbol(uint number, char value) {
  this->symbols[number] = value;
}

void Display::setScreen(void (*screen)()) {
  this->doScreen = *screen;
}

void Display::setSpecScreen(void (*screen)()) {
  if(!this->hasSpec) return;
  this->doSpec = *screen;
}

void Display::setFrameInterval(uint32_t interval) {
  this->frameInterval = interval;
}

void Display::setSymbolInversion(bool value) {
  this->symbolInversion = value;
}

void Display::setGreedInversion(bool value) {
  this->greedInversion = value;
}

byte Display::getSymbolCode(char symbol) {
  for(int i=0; i<this->fontLength; i+=2) {
    if(this->symbolsFont[i]==symbol) return this->symbolsFont[i+1];
  }
  return this->symbolInversion ? ~0b00000000 : 0b00000000;
}
