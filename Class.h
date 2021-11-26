#pragma once

#include "Exemplar.h"

#define CUSTOM_TILES

#include <Arduboy2.h>

#ifdef CUSTOM_TILES
#include <SpritesB.h>
#include <Print.h>
#endif

class Class : public Print  {
  protected:
    static Class *list;
    Class *next;
    static char errFlag;
    static byte getBits(byte x, byte p, byte n);
    static byte setBits(byte x, byte pos_new_bits, byte new_bits, byte num_new_bits);
#ifdef CUSTOM_TILES
    static int8_t _cursorX;    // Default is 0.
    static int8_t _baseX;      // needed for linebreak.
    static int8_t _cursorY;    // Default is 0.
    static int8_t _textColor;  // BLACK == 0, everything else is WHITE. Default is WHITE.
    static int8_t _textBackground;  // BLACK == 0, everything else is WHITE. Default is WHITE.
    static uint8_t _letterSpacing;  // letterSpacing controls the distance between letters. Default is 1.
    static uint8_t _lineHeight;     // lineHeight controls the height between lines breakend by \n. Default is 8.
    static int getFreeMemory();
#endif
  public:
    const char* _init = 0;
    enum Directive {
      Delete,
      Up,
      Down,
      Left,
      Right,
      Path,
      Block,
      Hidden,
      Far,
      Close,
      Clear,
      Build,
      X,
      Y,
      Greater,
      Less,
      Next,
      Place,
      Map,
      Turn,
      Search,
      Scene,
      Move,
      Draw,
      Character,
      Add,
      Owner,
      Count,
      Near,
      Cursor
    };

#ifdef CUSTOM_TILES
    static Arduboy2Base arduboy;
#endif
#ifndef CUSTOM_TILES
    static Arduboy2 arduboy;
#endif
    static SpritesB sprites;
    static Class exemplar;

    Class(Exemplar);
    Class();
    virtual ~Class();
    virtual Class *atPut(Directive key, Class *arg);
    virtual Class *atGet(Directive key);
    virtual Class *clone() const;
    virtual Class *make(const char* s);
    virtual char* toStr();
    virtual int toInt();
    virtual char getTypeChar();
    static byte dropDice(byte d = 2, byte n = 1);
    static void printDebug(char* c);

    virtual size_t write(uint8_t c);
    using Print::write;
    static void setCursor(int8_t x, int8_t y);
    static void setTextColor(const uint8_t color);
    static void setTextBackground(const uint8_t color);
#ifdef CUSTOM_TILES
    static void printChar(const unsigned char c, const int8_t x, int8_t y);
#endif
    static int hasMoreMemory(); //returns free memory size or 0 if the size is under 300
};
