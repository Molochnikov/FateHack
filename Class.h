#pragma once

#include "Exemplar.h"
#include <Arduboy2.h>

class Class {
  protected:
    static Class *list;
    Class *next;
    static char errFlag;
    static byte getBits(byte x, byte p, byte n);
    static byte setBits(byte x, byte pos_new_bits, byte new_bits, byte num_new_bits);
  public:
    const char* _init = 0;
    static int getFreeMemory();
    static Arduboy2 arduboy;
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
};
