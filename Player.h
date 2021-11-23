#pragma once

#include "Class.h"
#include <Arduino.h>

class Player : public Class {
  public:
    Player(Exemplar a);
    Player(const char* s);
    ~Player();
    Class *atPut(Directive key, Class *arg);
    Class *atGet(Directive key);
    Class *make(const char* s);
    char* toStr();
    int toInt();
    char getTypeChar();
    virtual Class *clone() const;
  private:
    Class * _next = 0;
    //static Player *_exemplar;
    byte _bit_mask = 0;
    /*7 6 5 4 3 2 1 0
      +-+-+-+ | | | +- is hidden bit
            | | | +--- has turn bit
            | | +----- has to be placed bit
            | +------- is block/bind bit
            +--------- effect duration
    */
    static const char _typeChar = 'P';
    byte replaceDrawChar(char ch);
};
