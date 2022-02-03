#include "Class.h"

#define CUSTOM_TILES

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

int Class::hasMoreMemory() {
  int m = Class::getFreeMemory();
  if (m < 280)
    return 0;
  else
    return m;
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

unsigned int Class::getDigit(const char* c) {
  unsigned int num = 0;
  while (::isdigit(pgm_read_byte_near(c))) {
    num = (num * 10) + (pgm_read_byte_near(c) - '0');
    c++;
  }
  return num;
}

int Class::dropDice(const char* c) {
  unsigned int num = 0;
  unsigned int dice = 0;
  unsigned int constant = 0;
  int res = 0;
  char oper = 0;

  num = Class::getDigit(c);
  if (num == 0)
    num = 1;
  c++; // d
  if (pgm_read_byte_near(c) == 'F') {
    dice = 3;
    constant = 2 * num;
    oper = '-';
  } else {
    dice = Class::getDigit(c);
    if (c) {
      oper = pgm_read_byte_near(c);
    }
    if (c) {
      constant = Class::getDigit(c);
    }
  }
  while (num) {
    res += random(dice) + 1;
    num--;
  }
  switch (oper) {
    case '+':
      res = res + constant;
      break;
    case '-':
      res = res - constant;
      break;
    case '*':
      res = res * constant;
      break;
  }
  return res;
}

void Class::printDebug(char* c) {
  for (int i = 0; i < 500; i++) {
    Class::arduboy.clear();
    Class::exemplar.setCursor(0, 0);
    Class::exemplar.println(c);
    Class::arduboy.display();
  }
}

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
  CHAR_AT = 64,
  CHAR_COLON = 58,
  CHAR_GREATER = 62,
  CHAR_LESS = 60,
  CHAR_EPSILON = 238,
  CHAR_A = 97,
  CHAR_TILDE = 126,
  CHAR_OPEN_BRACKET = 40,
  CHAR_CLOSE_BRACKET = 41,
  CHAR_BACKGROUND = 219,
  CHAR_QUESTION = 63,
  CHAR_ARROW = 26,
  CHAR_DOWN_ARROW = 25,
  CHAR_F = 102,
  CHAR_D = 100,
  CHAR_O = 111,
  CHAR_B = 98,
  CHAR_H = 104,
  CHAR_R = 114,
  CHAR_S = 115
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
  FONT_AT_INDEX,
  FONT_COLON_INDEX,
  FONT_GREATER_INDEX,
  FONT_LESS_INDEX,
  FONT_BACKGROUND_INDEX,
  FONT_EPSILON_INDEX,
  FONT_A_INDEX,
  FONT_TILDE_INDEX,
  FONT_OPEN_BRACKET_INDEX,
  FONT_CLOSE_BRACKET_INDEX,
  FONT_QUESTION_INDEX,
  FONT_ARROW_INDEX,
  FONT_DOWN_ARROW_INDEX,
  FONT_F_INDEX,
  FONT_D_INDEX,
  FONT_O_INDEX,
  FONT_B_INDEX,
  FONT_H_INDEX = FONT_B_INDEX,
  FONT_R_INDEX,
  FONT_S_INDEX
};

const uint8_t PROGMEM font_images[] = {
  4, 8,
  0x3E,  0x09, 0x09, 0x3E, 0x3F, 0x25, 0x25, 0x1A, 0x1E, 0x21, 0x21, 0x12, 0x3F, 0x21, 0x21, 0x1E, 0x3F, 0x25, 0x25, 0x21, 0x3F, 0x05, 0x05, 0x01, 0x1E, 0x21, 0x29, 0x3A, 0x3F, 0x04, 0x04, 0x3F, 0x21, 0x3F, 0x21, 0x00, 0x10, 0x21, 0x21, 0x1F, 0x3F, 0x04, 0x0A, 0x31, 0x3F, 0x20, 0x20, 0x20, 0x3F, 0x02, 0x02, 0x3F, 0x3F, 0x02, 0x04, 0x3F, 0x1E, 0x21, 0x21, 0x1E, 0x3F, 0x09, 0x09, 0x06, 0x1E, 0x21, 0x11, 0x2E, 0x3F, 0x09, 0x09, 0x36, 0x22, 0x25, 0x25, 0x19, 0x01, 0x3F, 0x01, 0x01, 0x1F, 0x20, 0x20, 0x1F, 0x0F, 0x10, 0x20, 0x1F, 0x3F, 0x10, 0x10, 0x3F, 0x3B, 0x04, 0x04, 0x3B, 0x03, 0x04, 0x38, 0x07, 0x31, 0x2D, 0x23, 0x21, 0x1E, 0x29, 0x25, 0x1E, 0x22, 0x3F, 0x20, 0x00, 0x32, 0x29, 0x29, 0x26, 0x12, 0x21, 0x25, 0x1A, 0x0C, 0x0A, 0x3F, 0x08, 0x17, 0x25, 0x25, 0x19, 0x1E, 0x25, 0x25, 0x18, 0x01, 0x39, 0x05, 0x03, 0x1A, 0x25, 0x25, 0x1A, 0x06, 0x29, 0x29, 0x1E, 0x00, 0x20, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x10, 0x08, 0x04, 0x02, 0x00, 0x64, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x3F, 0x21, 0x00, 0x00, 0x21, 0x3F, 0x00,
  0x56,  0x39, 0x39, 0x56, // @
  0x00,  0x24, 0x00, 0x00, // :
  0x7F,  0x6A, 0x6A, 0x7F, //>
  0x7F,  0x2B, 0x2B, 0x7F, //<
  0xFF, 0xFF, 0xFF, 0xFF, //background
  0x38,  0x54, 0x54, 0x44, //\xEE epsilon
  0x56,  0x3F, 0x3F, 0x56, //a
  0x10,  0x08, 0x10, 0x08, //~
  0x00,  0x0C, 0x12, 0x21, //(
  0x21,  0x12, 0x0C, 0x00, //)
  0x02,  0x29, 0x05, 0x02,  //?
  0x08,  0x2A, 0x1C, 0x08, //\x26 ->
  0x30,  0x7E, 0x30, 0x10, //\x25 down arrow
  0x0E,  0x1E, 0x3C, 0x1E, //f
  0x02,  0x6C, 0x3C, 0x62, //d
  0x20,  0x66, 0x7F, 0x66, //o
  0x74, 0x16, 0x77, 0x76, //b
  0x7F,  0x49, 0x41, 0x7F, //r
  0x40,  0x65, 0x6B, 0x40 //s
};

const uint8_t PROGMEM fill[] = {
  1, 8,
  0xFF
};

size_t Class::write(uint8_t c) {
  if (c == '\n')      {
    _cursorX = _baseX;
    _cursorY += _lineHeight;
  } else {
    if (_textBackground == WHITE) {
      sprites.drawSelfMasked(_cursorX, _cursorY, font_images, FONT_BACKGROUND_INDEX);
    }
    printChar(c, _cursorX, _cursorY);
    _cursorX += FONT4x6_WIDTH + _letterSpacing;
    if (_textBackground == WHITE) {
      sprites.drawSelfMasked(_cursorX - 1, _cursorY, fill, 0);
    }
  }
  return 1;
}

void Class::printChar(const unsigned char c, const int8_t x, int8_t y) {
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
    case CHAR_COLON:
      idx = FONT_COLON_INDEX;
      break;
    case CHAR_GREATER:
      idx = FONT_GREATER_INDEX;
      break;
    case CHAR_LESS:
      idx = FONT_LESS_INDEX;
      break;
    case CHAR_EPSILON:
      idx = FONT_EPSILON_INDEX;
      break;
    case CHAR_A:
      idx = FONT_A_INDEX;
      break;
    case CHAR_TILDE:
      idx = FONT_TILDE_INDEX;
      break;
    case CHAR_OPEN_BRACKET:
      idx = FONT_OPEN_BRACKET_INDEX;
      break;
    case CHAR_CLOSE_BRACKET:
      idx = FONT_CLOSE_BRACKET_INDEX;
      break;
    case CHAR_BACKGROUND:
      idx = FONT_BACKGROUND_INDEX;
      break;
    case CHAR_QUESTION:
      idx = FONT_QUESTION_INDEX;
      break;
    case CHAR_ARROW:
      idx = FONT_ARROW_INDEX;
      break;
    case CHAR_DOWN_ARROW:
      idx = FONT_DOWN_ARROW_INDEX;
      break;
    case CHAR_F:
      idx = FONT_F_INDEX;
      break;
    case CHAR_D:
      idx = FONT_D_INDEX;
      break;
    case CHAR_O:
      idx = FONT_O_INDEX;
      break;
    case CHAR_B:
      idx = FONT_B_INDEX;
      break;
    case CHAR_H:
      idx = FONT_H_INDEX;
      break;
    case CHAR_R:
      idx = FONT_R_INDEX;
      break;
       case CHAR_S:
      idx = FONT_S_INDEX;
      break;
  }

  if (idx > -1) {
    if (_textColor == WHITE) {
      sprites.drawSelfMasked(x, y, font_images, idx);
    }
    else {
      sprites.drawErase(x, y, font_images, idx);
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
