#include "Player.h"

//Player *Player::_exemplar = new Player(Exemplar());

const char* Player::toStr() {
  char* c  = _init;
  c += 3;
  while (::isdigit(pgm_read_byte_near(c))) {
    ++c;
  }
  return c;
  //return _name;
}

const int Player::toInt() {
  char* c  = _init;
  byte _script = 0;
  c += 3;
  while (::isdigit(pgm_read_byte_near(c))) {
    _script = (_script * 10) + (pgm_read_byte_near(c) - '0');
    ++c;
  }
  return _script;
}


const char Player::getTypeChar() {
  return Player::_typeChar;
}

Player::Player(Exemplar a): Class(a) {

}

Class *Player::clone() const {
  //Class *retval = new Player(_init);
  return Class::exemplar.make(_init);
  //return retval;
}

Player::Player(const char * s): Class() {
  if (pgm_read_byte_near(s) == Player::_typeChar) {
    _bit_mask = Class::setBits(_bit_mask, 0, 1, 1);
    _bit_mask = Class::setBits(_bit_mask, 1, 1, 1);
    //_init = s;
    ++s;
    //_symbol = pgm_read_byte_near(s);
    ++s;
//    while (::isdigit(pgm_read_byte_near(s))) {
//      ++s;
//    }
    if (pgm_read_byte_near(s) == 'B')
      //_is_block = 1;
      _bit_mask = Class::setBits(_bit_mask, 3, 1, 1);
    ++s;
    //while (::isdigit(pgm_read_byte_near(s))) {
    //_script = (_script * 10) + (pgm_read_byte_near(s) - '0');
    //++s;
    //}
    //_name = s;
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
  //Class::errFlag = 0;
  return retval;
}

Class *Player::atPut(Directive key, Class *arg) {
  switch (key) {
    case Class::Directive::Delete: //delete aspect from this
      {
        Class * c = (this->atGet(Class::Directive::Next));
        Class * prev = this;
        while (c && ((c->atGet(Class::Directive::Place)) == 0)) {
          if (c == arg)  {
            prev->atPut(Class::Directive::Next, (c->atGet(Class::Directive::Next)));
            return arg;
          } else {
            prev = c;
            c = (c->atGet(Class::Directive::Next));
          }
        }
      }
      return 0;
      break;
    case Class::Directive::Block: //set block path or aspect removal
      //      if (arg) {
      //        _is_block = 1;
      //      } else {
      //        _is_block = 0;
      //      }
      if (arg) {
        _bit_mask = Class::setBits(_bit_mask, 3, 1, 1);
      } else {
        _bit_mask = Class::setBits(_bit_mask, 3, 0, 1);
      }
      return this;
      break;
    case Class::Directive::Character: //find aspect in this
      {
        Class * c = (this->atGet(Class::Directive::Next));
        while (c && ((c->atGet(Class::Directive::Place)) == 0)) {
          if ((c->_init) == (arg->_init))  {
            return arg;
          } else {
            c = (c->atGet(Class::Directive::Next));
          }
        }
      }
      return 0;
      break;
    case Class::Directive::Next: //add class
      {
        //        Class * c = this->_next;
        //        if (c) {
        //          arg->_next = c;
        //          this->_next = arg;
        //        }
        this->_next = arg;
        //        Player * c = this;
        //        if (arg) {
        //          while (c->_next) {
        //            c = (c->_next);
        //          }
        //          c->_next = arg;
        //          return arg;
        //        }
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
      //      if (arg) {
      //        _has_to_be_placed = 1;
      //      } else {
      //        _has_to_be_placed = 0;
      //      }
      if (arg) {
        _bit_mask = Class::setBits(_bit_mask, 2, 1, 1);
      } else {
        _bit_mask = Class::setBits(_bit_mask, 2, 0, 1);
      }
      break;
    case Class::Directive::Turn: //has script
      //      if (arg) {
      //        _has_turn = 1;
      //      } else {
      //        _has_turn = 0;
      //      }
      if (arg) {
        _bit_mask = Class::setBits(_bit_mask, 1, 1, 1);
      } else {
        _bit_mask = Class::setBits(_bit_mask, 1, 0, 1);
      }
      break;
    case Class::Directive::Hidden: //hide or reveal
      //      if (arg) {
      //        _is_hidden = 0;
      //      } else {
      //        _is_hidden = 1;
      //      }
      if (arg) {
        _bit_mask = Class::setBits(_bit_mask, 0, 0, 1);
      } else {
        _bit_mask = Class::setBits(_bit_mask, 0, 1, 1);
      }
      break;
    case Class::Directive::Up: //restore HP
      return this;
      break;
    //    case Class::Directive::Begin: //begin turn
    //      _isProcess = true;
    //      return this;
    //      break;
    //    case Class::Directive::End: //end turn
    //      _isProcess = false;
    //      _isFinished = true;
    //      return this;
    //      break;
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
    //    case Class::Directive::Character: //get next aspect in player ignoring "to be placed players"
    //      {
    //        Class * classs = this->atGet(Class::Directive::Next);
    //        while (classs && (classs->atGet(Class::Directive::Place))) {
    //          classs = (classs->atGet(Class::Directive::Next));
    //        }
    //        return classs;
    //      }
    //      break;
    case Class::Directive::Next: //get next player in player
      return this->_next;
      break;
    case Class::Directive::Turn: //has script
      //      if (_has_turn) {
      //        return this;
      //      } else {
      //        return 0;
      //      }
      if (Class::getBits(_bit_mask, 1, 1)) {
        return this;
      } else {
        return 0;
      }
      break;
    //    case Class::Directive::Script: //has script
    //      if (_script) {
    //        return this;
    //      } else {
    //        return 0;
    //      }
    //      break;
    case Class::Directive::Place: //has to be placed
      //      if (_has_to_be_placed) {
      //        return this;
      //      } else {
      //        return 0;
      //      }
      if (Class::getBits(_bit_mask, 2, 1)) {
        return this;
      } else {
        return 0;
      }
      break;
    case Class::Directive::Block: //is blocking paths or blocking aspect removal
      //      if (_is_block) {
      //        return this;
      //      } else {
      //        return 0;
      //      }
      if (Class::getBits(_bit_mask, 3, 1)) {
        return this;
      } else {
        return 0;
      }
      break;
    case Class::Directive::Hidden: //hide or reveal
      //      if (_is_hidden) {
      //        return this;
      //      } else {
      //        return 0;
      //      }
      if (Class::getBits(_bit_mask, 0, 1)) {
        return this;
      } else {
        return 0;
      }
      break;
    //    case Class::Directive::Process: //making turn?
    //      if (_isProcess)
    //        return this;
    //      else
    //        return 0;
    //      break;
    //    case Class::Directive::End: //end turn?
    //      if (_isFinished)
    //        return this;
    //      else
    //        return 0;
    //      break;
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
