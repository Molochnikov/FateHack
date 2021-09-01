#include "Coordinate.h"

//Coordinate *Coordinate::_exemplar = new Coordinate(Exemplar());

const char* Coordinate::toStr() {
  return 0;
}

const int Coordinate::toInt() {
  return value;
}

char Coordinate::numChar(int number)
{
  char numberArray = '?';
  int n = log10(number) + 1;
  numberArray = (number % (10 * n)) + '0';
  return numberArray;
}

const char Coordinate::getTypeChar() {
  return Coordinate::_typeChar;
}

Coordinate::Coordinate(Exemplar a): Class(a) {

}

Class *Coordinate::clone() const {
  Class *retval = new Coordinate("C1");
  for (int i = 1; i < value; i++) {
    retval->atGet(Class::Directive::Up);
  }
  return retval;
}

Coordinate::Coordinate(const char* s): Class() {
  if (s[0] == Coordinate::_typeChar) {
    ++s;
    int i = ::atoi(s);
    if (!i) {
      Class::errFlag = 'Y';
    } else {
      value = i;
    }
  } else {
    Class::errFlag = 'Y';
  }
}

Class *Coordinate::make(const char* s) {
  Class *retval = new Coordinate(s);
  if (Class::errFlag) {
    delete retval;
    retval = 0;
  } else {
    //Serial.println(F("coord"));
  }
  return retval;
}

Class *Coordinate::atPut(Directive key, Class *arg) {
  switch (key) {
    case Class::Directive::Greater: // return greater comparing this and arg
      if (arg->toInt() > this->toInt()) {
        return arg;
      } else if (arg->toInt() < this->toInt()) {
        return this;
      } else {
        return 0;
      }
      break;
    case Class::Directive::Less: // return less comparing this and arg
      if (arg->toInt() > this->toInt()) {
        return this;
      } else if (arg->toInt() < this->toInt()) {
        return arg;
      } else {
        return 0;
      }
      break;
    //        case Class::Directive::Hidden: //hide or reveal
    //          if (arg) {
    //            _is_hidden = 0;
    //          } else {
    //            _is_hidden = 1;
    //          }
    //          break;
    default:
      return Class::atPut(key, arg);
      break;
  }
}

Class *Coordinate::atGet(Directive key) {
  switch (key) {
    case Class::Directive::Block:
      return 0;
      break;
    case Class::Directive::Hidden: //hide or reveal
      if (_is_hidden) {
        return this;
      } else {
        return 0;
      }
      break;
    case Class::Directive::Draw: //draw coord in one char
      //Class::arduboy.print(Coordinate::numChar(value));
      Class::arduboy.print(_symbol);
      return this;
      break;
    case Class::Directive::Up: //move coordinate up
      value++;
      break;
    case Class::Directive::Down: //move coordinate down
      value--;
      break;
    default:
      return Class::atGet(key);
      break;
  }
  return this;

}

Coordinate::~Coordinate() {

}
