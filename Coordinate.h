#pragma once

#include "Class.h"
#include "Coordinates.h"
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
    int _value = 0;
    static const char _symbol = '.';
    union _is_hidden_or_next {
      int is_hidden = 1; //see/hide coordinates on map
      Class* next;
    };
    _is_hidden_or_next _union; //saving memory for coordinates in class chains
    char numChar(int number);
};
