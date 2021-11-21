#include "Class.h"

//#define CUSTOM_TILES

Arduboy2 Class::arduboy;
Class *Class::list = 0;
Class Class::exemplar = Class(Exemplar());
char Class::errFlag = 0;

#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__

int Class::getFreeMemory() {
  char top;
#ifdef __arm__
  return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  return &top - __brkval;
#else  // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
  return 0;
}

Class::Class(Exemplar) {
  next = list;
  list = this;
}

Class *Class::make(const char* s) {
  Class *retval = 0;
  for (Class *a = list; a; a = a->next) {
    if (a != (&Class::exemplar)) {
      if ((retval = a->make(s))) {
        retval->_init = s;
        return retval;
      }
    }
  }
  return 0;
}

Class::Class() {
  next = 0;
  Class::errFlag = 0;
}

Class::~Class() {

}

Class *Class::atPut(Directive, Class *) {
  return 0;
}

Class *Class::atGet(Directive) {
  return 0;
}

Class *Class::clone() const {
  return Class::exemplar.make(_init);
}

char* Class::toStr() {
  return 0;
}

byte Class::dropDice(byte d = 2, byte n = 1) {
  int sum = 0;
  for (int i = 0; i < n; i++) {
    sum += random(d);
  }
  return sum;
}

//void Class::printDebug(char* c) {
//  for (int i = 0; i < 500; i++) {
//    Class::arduboy.clear();
//    Class::setCursor(0, 0);
//    Class::println(c);
//    Class::arduboy.display();
//  }
//}

byte Class::getBits(byte x, byte p, byte n) {
  return (x >> (p + 1 - n)) & (~((~0U) << n));
}

byte Class::setBits(byte x, byte pos_new_bits, byte new_bits, byte num_new_bits) {
  return (x & (~(((byte)(1 << num_new_bits) - 1) << (pos_new_bits + 1 - num_new_bits)))) | (new_bits << (pos_new_bits + 1 - num_new_bits));
}

int Class::toInt() {
  return 0;
}

char Class::getTypeChar() {
  return 0;
}

#ifndef CUSTOM_TILES
size_t Class::write(uint8_t c) {
  return Class::arduboy.write(c);
}
size_t Class::print(const __FlashStringHelper * c) {
  return Class::arduboy.print(c);
}
size_t Class::println(const __FlashStringHelper * c) {
  return Class::arduboy.println(c);
}
size_t Class::println() {
  return Class::arduboy.println();
}
void Class::setTextColor(const uint8_t color) {
  return Class::arduboy.setTextColor(color);
}
void Class::setTextBackground(const uint8_t color) {
  return Class::arduboy.setTextBackground(color);
}
void Class::setCursor(int16_t x, int16_t y) {
  return Class::arduboy.setCursor(x, y);
}
#endif
#ifdef CUSTOM_TILES

uint8_t Class::_lineHeight = 8;
uint8_t Class::_letterSpacing = 1;

int8_t Class::_cursorX = 0;
int8_t Class::_textBackground = 0;
int8_t Class::_cursorY = 0;
int8_t Class::_baseX = 0;
int8_t Class::_textColor = 1;

enum {
  FONT4x6_WIDTH = 4,
  FONT4x6_HEIGHT = 7
};

enum {
  CHAR_NUMBER_0 = 48
};

enum {
  FONT_NUMBER_0_INDEX = 26
};

enum {
  FONT_BACKGROUND = 10
};

const uint8_t PROGMEM font_images[] = {
  4, 8,

  // Just and example. You can add your tiles.
  0x1E,  // ░░░▓▓▓▓░
  0x29,  // ░░▓░▓░░▓
  0x25,  // ░░▓░░▓░▓
  0x1E,  // ░░░▓▓▓▓░
};

const uint8_t PROGMEM fill[] = {
  1, 8,
  0xFF
};

size_t Class::write(uint8_t c) {
  if (c == '\n')      {
    Class::_cursorX = Class::_baseX;
    Class::_cursorY += Class::_lineHeight;
  } else {
    if (Class::_textBackground == WHITE) {
      SpritesB::drawSelfMasked(Class::_cursorX, Class::_cursorY, font_images, FONT_BACKGROUND);
    }
    Class::printChar(c, Class::_cursorX, Class::_cursorY);
    Class::_cursorX += FONT4x6_WIDTH + Class::_letterSpacing;
    if (Class::_textBackground == WHITE) {
      SpritesB::drawSelfMasked(Class::_cursorX - 1, Class::_cursorY, fill, 0);
    }
  }
  return 1;
}

size_t Class::print(const __FlashStringHelper * c) {
  return Class::exemplar.print(c);
}
size_t Class::println(const __FlashStringHelper * c) {
  return Class::exemplar.println(c);
}

void Class::printChar(const char c, const int8_t x, int8_t y) {

  int8_t idx = -1;

  ++y;

  switch (c) {
    case CHAR_NUMBER_0:
      idx = FONT_NUMBER_0_INDEX;
      break;
  }

  if (idx > -1) {
    if (_textColor == WHITE) {
      SpritesB::drawSelfMasked(x, y, font_images, idx);
    } else {
      SpritesB::drawErase(x, y, font_images, idx);
    }
  }
}

void Class::setCursor(const int8_t x, const int8_t y) {
  Class::_cursorX = Class::_baseX = x;
  Class::_cursorY = y;
}

void Class::setTextColor(const uint8_t color) {
  Class::_textColor = color;
}

void Class::setTextBackground(const uint8_t color) {
  Class::_textBackground = color;
}


void Class::setHeight(const uint8_t color) {
  Class::_lineHeight = color;
}

#endif
