#pragma once

#include "Class.h"
#include <Arduino.h>

class Coordinate : public Class {
    friend Class *Class::make(const char* s);
  public:
    Class *atPut(Directive key, Class *arg);
    Class *atGet(Directive key);
    char* toStr();
    int toInt();
    char getTypeChar();
    ~Coordinate();
    Coordinate(Exemplar a);
    virtual Class *clone() const;
  private:
    Coordinate(const char* s);
    //static Coordinate *_exemplar;
    Class *make(const char* s);
    static const char _typeChar = 'C';
    int value = 0;
    static const char _symbol = '.';
    int _is_hidden = 1;
    char numChar(int number);
};
