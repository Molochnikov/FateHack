#include "Class.h"

//#define CUSTOM_TILES

#ifdef CUSTOM_TILES
Arduboy2Base Class::arduboy;
#endif
#ifndef CUSTOM_TILES
Arduboy2 Class::arduboy;
#endif

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

//void Class::exemplar.printDebug(char* c) {
//  for (int i = 0; i < 500; i++) {
//    Class::arduboy.clear();
//    Class::setCursor(0, 0);
//    Class::exemplar.println(c);
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
void Class::setTextColor(const uint8_t color) {
  return Class::arduboy.setTextColor(color);
}
void Class::setTextBackground(const uint8_t color) {
  return Class::arduboy.setTextBackground(color);
}
void Class::setCursor(int8_t x, int8_t y) {
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
  CHAR_PERIOD = 46,
  CHAR_LETTER_A = 65,
  CHAR_LETTER_Z = 90,
  CHAR_NUMBER_0 = 48,
  CHAR_NUMBER_9 = 57,
  CHAR_OPENING_BRACKET = 91,
  CHAR_CLOSING_BRACKET = 93,
  CHAR_SINGLE_QUOTE = 39,
  CHAR_SEMICOLON = 59,
  CHAR_COMMA = 44,
  CHAR_SLASH = 47,
  CHAR_AT = 64
};

enum {
  FONT_LETTER_INDEX = 0,
  FONT_NUMBER_INDEX = 26,
  FONT_PERIOD_INDEX = FONT_NUMBER_INDEX + 10,
  FONT_COMMA_INDEX,
  FONT_SLASH_INDEX,
  FONT_SEMICOLON_INDEX,
  FONT_SINGLE_QUOTE_INDEX,
  FONT_OPENING_BRACKET_INDEX,
  FONT_CLOSING_BRACKET_INDEX,
  FONT_AT_INDEX
};

enum {
  FATE_BACKGROUND = 10
};

const uint8_t PROGMEM font_images[] = {
  4, 8,
  0x3E,  0x09, 0x09, 0x3E, 0x3F, 0x25, 0x25, 0x1A, 0x1E, 0x21, 0x21, 0x12, 0x3F, 0x21, 0x21, 0x1E, 0x3F, 0x25, 0x25, 0x21, 0x3F, 0x05, 0x05, 0x01, 0x1E, 0x21, 0x29, 0x3A, 0x3F, 0x04, 0x04, 0x3F, 0x21, 0x3F, 0x21, 0x00, 0x10, 0x21, 0x21, 0x1F, 0x3F, 0x04, 0x0A, 0x31, 0x3F, 0x20, 0x20, 0x20, 0x3F, 0x02, 0x02, 0x3F, 0x3F, 0x02, 0x04, 0x3F, 0x1E, 0x21, 0x21, 0x1E, 0x3F, 0x09, 0x09, 0x06, 0x1E, 0x21, 0x11, 0x2E, 0x3F, 0x09, 0x09, 0x36, 0x22, 0x25, 0x25, 0x19, 0x01, 0x3F, 0x01, 0x01, 0x1F, 0x20, 0x20, 0x1F, 0x0F, 0x10, 0x20, 0x1F, 0x3F, 0x10, 0x10, 0x3F, 0x3B, 0x04, 0x04, 0x3B, 0x03, 0x04, 0x38, 0x07, 0x31, 0x2D, 0x23, 0x21, 0x1E, 0x29, 0x25, 0x1E, 0x22, 0x3F, 0x20, 0x00, 0x32, 0x29, 0x29, 0x26, 0x12, 0x21, 0x25, 0x1A, 0x0C, 0x0A, 0x3F, 0x08, 0x17, 0x25, 0x25, 0x19, 0x1E, 0x25, 0x25, 0x18, 0x01, 0x39, 0x05, 0x03, 0x1A, 0x25, 0x25, 0x1A, 0x06, 0x29, 0x29, 0x1E, 0x00, 0x20, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x10, 0x08, 0x04, 0x02, 0x00, 0x64, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x3F, 0x21, 0x00, 0x00, 0x21, 0x3F, 0x00,
  0xAC,  0x72, 0xFE, 0xA0 //@
};

const uint8_t PROGMEM fill[] = {
  4, 8,
  0xFF
};

size_t Class::write(uint8_t c) {
  if (c == '\n')      {
    _cursorX = _baseX;
    _cursorY += _lineHeight;
  } else {
    if (_textBackground == WHITE) {
      SpritesB::drawSelfMasked(_cursorX, _cursorY, font_images, FATE_BACKGROUND);
    }
    printChar(c, _cursorX, _cursorY);
    _cursorX += FONT4x6_WIDTH + _letterSpacing;
    if (_textBackground == WHITE) {
      SpritesB::drawSelfMasked(_cursorX - 1, _cursorY, fill, 0);
    }
  }
  return 1;
}

void Class::printChar(const char c, const int8_t x, int8_t y) {
  int8_t idx = -1;
  ++y;

  switch (c) {
    case CHAR_LETTER_A ... CHAR_LETTER_Z:
      idx = c - CHAR_LETTER_A;
      break;
    case CHAR_NUMBER_0 ... CHAR_NUMBER_9:
      idx = c - CHAR_NUMBER_0 + FONT_NUMBER_INDEX;
      break;
    case CHAR_PERIOD:
      idx = FONT_PERIOD_INDEX;
      break;
    case CHAR_OPENING_BRACKET:
      idx = FONT_OPENING_BRACKET_INDEX;
      break;
    case CHAR_CLOSING_BRACKET:
      idx = FONT_CLOSING_BRACKET_INDEX;
      break;
    case CHAR_SINGLE_QUOTE:
      idx = FONT_SINGLE_QUOTE_INDEX;
      break;
    case CHAR_SEMICOLON:
      idx = FONT_SEMICOLON_INDEX;
      break;
    case CHAR_COMMA:
      idx = FONT_COMMA_INDEX;
      break;
    case CHAR_SLASH:
      idx = FONT_SLASH_INDEX;
      break;
    case CHAR_AT:
      idx = FONT_AT_INDEX;
      break;
  }

  if (idx > -1) {
    if (_textColor == WHITE) {
      SpritesB::drawSelfMasked(x, y, font_images, idx);
    }
    else {
      SpritesB::drawErase(x, y, font_images, idx);
    }
  }
}

void Class::setCursor(const int8_t x, const int8_t y) {
  _cursorX = _baseX = x;
  _cursorY = y;
}

void Class::setTextColor(const uint8_t color) {
  _textColor = color;
}

void Class::setTextBackground(const uint8_t color) {
  _textBackground = color;
}

#endif
