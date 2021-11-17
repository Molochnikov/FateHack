#include "Player.h"

//Player *Player::_exemplar = new Player(Exemplar());

char* Player::toStr() {
  char* c  = _init;
  c += 3;
  while (::isdigit(pgm_read_byte_near(c))) {
    ++c;
  }
  return c;
  //return _name;
}

int Player::toInt() {
  char* c  = _init;
  byte _script = 0;
  c += 3;
  //return ::atoi((pgm_read_byte_near(c)));
  while (::isdigit(pgm_read_byte_near(c))) {
    _script = (_script * 10) + (pgm_read_byte_near(c) - '0');
    ++c;
  }
  return _script;
}


char Player::getTypeChar() {
  return Player::_typeChar;
}

Player::Player(Exemplar a): Class(a) {

}

Class *Player::clone() const {
  return Class::exemplar.make(_init);
}

Player::Player(const char * s): Class() {
  if (pgm_read_byte_near(s) == Player::_typeChar) {
    _bit_mask = Class::setBits(_bit_mask, 0, 1, 1);
    _bit_mask = Class::setBits(_bit_mask, 1, 1, 1);
    ++s;
    ++s;
    if (pgm_read_byte_near(s) == 'B')
      _bit_mask = Class::setBits(_bit_mask, 3, 1, 1);
  } else {
    Class::errFlag = 'Y';
  }
}

Class *Player::make(const char* s) {
  Class *retval = new Player(s);
  if (Class::errFlag) {
    delete retval;
    retval = 0;
  }
  return retval;
}

Class *Player::atPut(Directive key, Class *arg) {
  switch (key) {
    case Class::Directive::Delete: //delete arg from this class chain by pointer ignoring chain owner
      {
        Class * cl = 0;
        Class * before = 0;
        Class * after = 0;
        before = 0;
        after = 0;
        cl = this;
        while (cl) {
          if ((cl->atGet(Class::Directive::Next)) == arg)
            before = cl;
          cl = (cl->atGet(Class::Directive::Next)); //go next even if found
          if (before) {
            cl = (cl->atGet(Class::Directive::Next));
            break;
          }
        }
        if (before) { //if found then join before and after
          while (cl) { //if arg is "need to be placed" class then ignore full chain until next "need to be placed" class
            if (((arg->atGet(Class::Directive::Place)) && (cl->atGet(Class::Directive::Place))) || ((arg->atGet(Class::Directive::Place)) == 0)) {
              after = cl;
              break;
            }
            cl = (cl->atGet(Class::Directive::Next));
          }
          before->atPut(Class::Directive::Next, after); //finally join
        }
        return arg;
      }
      return 0;
      break;
    case Class::Directive::Count: //reset sleep
      if (arg) {
        _bit_mask = Class::setBits(_bit_mask, 7, 15, 4);
      } else {
        _bit_mask = Class::setBits(_bit_mask, 7, 0, 4);
      }
      return this;
      break;
    case Class::Directive::Block: //set block path or bind
      if (arg) {
        _bit_mask = Class::setBits(_bit_mask, 3, 1, 1);
      } else {
        _bit_mask = Class::setBits(_bit_mask, 3, 0, 1);
      }
      return this;
      break;
    case Class::Directive::Character: //find clone of arg in this
      {
        if ((this->_init) == (arg->_init))
          return this;
        Class * c = (this->atGet(Class::Directive::Next));
        while (c && ((c->atGet(Class::Directive::Place)) == 0)) {
          if ((c->_init) == (arg->_init))  {
            return c;
          } else {
            c = (c->atGet(Class::Directive::Next));
          }
        }
      }
      return 0;
      break;
    case Class::Directive::Next: //add class
      {
        this->_next = arg;
        return arg;
      }
      break;
    case Class::Directive::Add: //add class
      {
        Class * c = this;
        if (this->_next) {
          c = (this->_next);
        } else {
          this->_next = arg;
          return arg;
        }
        this->_next = arg;
        arg->atPut(Class::Directive::Next, c);
      }
      break;
    case Class::Directive::Place: //has to be placed
      if (arg) {
        _bit_mask = Class::setBits(_bit_mask, 2, 1, 1);
      } else {
        _bit_mask = Class::setBits(_bit_mask, 2, 0, 1);
      }
      break;
    case Class::Directive::Turn: //has turn
      if (arg) {
        _bit_mask = Class::setBits(_bit_mask, 1, 1, 1);
      } else {
        _bit_mask = Class::setBits(_bit_mask, 1, 0, 1);
      }
      break;
    case Class::Directive::Hidden: //hide or reveal
      if (arg) {
        _bit_mask = Class::setBits(_bit_mask, 0, 0, 1);
      } else {
        _bit_mask = Class::setBits(_bit_mask, 0, 1, 1);
      }
      break;
    case Class::Directive::Up: //restore HP
      return this;
      break;
    case Class::Directive::Scene:
      return this;
      break;
    default:
      return Class::atPut(key, arg);
      break;
  }
  return this;
}

Class *Player::atGet(Directive key) {
  switch (key) {
    case Class::Directive::Next: //get next player in player
      return this->_next;
      break;
    case Class::Directive::Turn: //has turn
      if (Class::getBits(_bit_mask, 1, 1)) {
        return this;
      } else {
        return 0;
      }
      break;
    case Class::Directive::Place: //has to be placed
      if (Class::getBits(_bit_mask, 2, 1)) {
        return this;
      } else {
        return 0;
      }
      break;
    case Class::Directive::Count: //iterate and return sleep
      {
        byte new_countdown = Class::getBits(_bit_mask, 7, 4) - 1;
        _bit_mask = Class::setBits(_bit_mask, 7, new_countdown, 4);
        if (Class::getBits(_bit_mask, 7, 4)) {
          return this;
        } else {
          this->atPut(Class::Directive::Count, this);
          return 0;
        }
      }
      break;
    case Class::Directive::Block: //is blocking paths or blocking aspect removal
      if (Class::getBits(_bit_mask, 3, 1)) {
        return this;
      } else {
        return 0;
      }
      break;
    case Class::Directive::Hidden: //hide or reveal
      if (Class::getBits(_bit_mask, 0, 1)) {
        return this;
      } else {
        return 0;
      }
      break;
    case Class::Directive::Draw:
      Class::arduboy.print((char)(pgm_read_byte_near((this->_init) + 1)));
      return this;
      break;
    default:
      return Class::atGet(key);
      break;
  }
}

Player::~Player() {

}
