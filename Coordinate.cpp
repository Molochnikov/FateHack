#include "Coordinate.h"

//Coordinate *Coordinate::_exemplar = new Coordinate(Exemplar());

char* Coordinate::toStr() {
  return 0;
}

int Coordinate::toInt() {
  return _value;
}

char Coordinate::numChar(int number)
{
  while (number > 9) {
    number = number % 10;
  }
  return (number + '0');
}

char Coordinate::getTypeChar() {
  return Coordinate::_typeChar;
}

Coordinate::Coordinate(Exemplar a): Class(a) {

}

Class *Coordinate::clone() const {
  Class *retval = Class::exemplar.make(default_coord);
  for (int i = 1; i < _value; i++) {
    retval->atGet(Class::Directive::Up);
  }
  return retval;
}

Coordinate::Coordinate(const char* s): Class() {
  if (pgm_read_byte_near(s) == Coordinate::_typeChar) {
    ++s;
    int i = Class::getDigit(s);
    if (!i) {
      Class::errFlag = 'Y';
    } else {
      _value = i;
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
  }
  return retval;
}

Class *Coordinate::atPut(Directive key, Class *arg) {
  switch (key) {
    case Class::Directive::Character: //atPut find clone of arg in this class chain
      if ((this->_init) == (arg->_init))
        return this;
      else
        return 0;
      break;
    case Class::Directive::Next: //atPut push arg to this class destructive. Don't use this method. Use Add
      this->_union.next = arg;
      return arg;
      break;
    case Class::Directive::Greater: //atPut return greater comparing this and arg
      if (arg->toInt() > this->toInt()) {
        return arg;
      } else if (arg->toInt() < this->toInt()) {
        return this;
      } else {
        return 0;
      }
      break;
    case Class::Directive::Less: //atPut return less comparing this and arg
      if (arg->toInt() > this->toInt()) {
        return this;
      } else if (arg->toInt() < this->toInt()) {
        return arg;
      } else {
        return 0;
      }
      break;
    default:
      return Class::atPut(key, arg);
      break;
  }
}

Class *Coordinate::atGet(Directive key) {
  switch (key) {
    case Class::Directive::Next: //atGet get next class after this in class chain
      if (this->_union.is_hidden == 1)
        return 0;
      else
        return this->_union.next;
      break;
    case Class::Directive::Place:
      return 0;
      break;
    case Class::Directive::Block:
      return 0;
      break;
    case Class::Directive::Hidden: //atGet if return NULL then this revealed class
      if (_union.is_hidden == 0)
        return 0;
      else
        return this;
      break;
    case Class::Directive::Draw: //atGet draw this class symbol on screen
      //Class::exemplar.print(Coordinate::numChar(_value));
      Class::exemplar.print(_symbol);
      return this;
      break;
    case Class::Directive::Up: //atGet move coordinate up
      _value++;
      return this;
      break;
    case Class::Directive::Down: //atGet move coordinate down
      _value--;
      return this;
      break;
    default:
      return Class::atGet(key);
      break;
  }
  return 0;
}

Coordinate::~Coordinate() {

}
