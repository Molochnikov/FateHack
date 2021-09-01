#include "Scene.h"
//Scene *Scene::_exemplar = new Scene(Exemplar());

const char* Scene::toStr() {
  return 0;
}

const int Scene::toInt() {
  return 0;
}

const char Scene::getTypeChar() {
  return Scene::_typeChar;
}

Scene::Scene(Exemplar a): Class(a) {

}

Scene::Scene(const char* s): Class() {
  //if (s[0] == Scene::_typeChar) {
  //s++;
  //int i = ::atoi(s);
  //_scene = scenario[i];
  if (pgm_read_byte_near(s) == Scene::_typeChar) {
    s++;
    _scene = s;
    //_zone = zones[i];
  } else {
    Class::errFlag = 'Y';
  }
}

Class *Scene::make(const char* s) {
  //  if (pgm_read_byte_near(s) == Scene::_typeChar) {
  //    _scene = s;
  //    return this; //singleton
  //  }
  //  return 0;
  Class *retval = new Scene(s);
  if (Class::errFlag) {
    delete retval;
    retval = 0;
  }
  return retval;
}

Class *Scene::addClassToScene(Class *arg, int pos, int newpos, Scene::AddClassAction action, Class *clone) {
  switch (action) {
    case Scene::AddClassAction::MoveDestroy:
      if (_characters[newpos]) {
        if (_characters[newpos] != _block_proto)
          delete _characters[newpos];
        _characters[newpos] = 0;
      }
      _characters[newpos] = arg;
      _characters[pos] = 0;
      return 0;
      break;
    case Scene::AddClassAction::CloneDestroy:
      if (_characters[newpos]) {
        if (_characters[newpos] != _block_proto)
          delete _characters[newpos];
        _characters[newpos] = 0;
      }
      _characters[newpos] = clone->clone();
      _characters[newpos]->atGet(Class::Directive::Up); //for coords movement or restoring hp of clone
      return 0;
      break;
    case Scene::AddClassAction::CloneIfEmpty:
      if (_characters[newpos]) {
        return _characters[newpos];
      } else {
        _characters[newpos] = clone->clone();
        _characters[newpos]->atGet(Class::Directive::Up); //for coords movement or restoring hp of clone
        return 0;
      }
      break;
    case Scene::AddClassAction::CopyIfEmpty:
      if (_characters[newpos]) {
        return _characters[newpos];
      } else {
        _characters[newpos] = clone;
        return 0;
      }
      break;
    case Scene::AddClassAction::MoveIfEmpty:
      if (_characters[newpos] && (_characters[newpos]->atGet(Class::Directive::Block))) {
        return _characters[newpos];
      } else {
        _characters[pos] = _characters[newpos];
        _characters[newpos] = arg;
        return 0;
      }
      break;
    case Scene::AddClassAction::NoAction:
      if (_characters[newpos]) {
        return _characters[newpos];
      } else {
        return 0;
      }
      break;
  }
}

Class *Scene::getUpThingFrom(Class *arg, Scene::AddClassAction action, Class *clone) {
  //  if (arg == 0) {
  //    Class::arduboy.print(pgm_read_byte(error_null_class_instance));
  //    Class::arduboy.print('\n');
  //    Class::arduboy.print(F("getupthingfrom"));
  //    Class::die();
  //  }
  for (int pos = 0; pos < (_xsize * _ysize); pos++) {
    if (_characters[pos] == arg) {
      int newpos = pos - _xsize;
      if (newpos >= 0) {
        return Scene::addClassToScene(arg, pos, newpos, action, clone);
      }
      return arg;
    }
  }
  return arg;
}

Class *Scene::getDownThingFrom(Class *arg, Scene::AddClassAction action, Class *clone) {
  //  if (arg == 0) {
  //    Class::arduboy.print(pgm_read_byte(error_null_class_instance));
  //    Class::arduboy.print('\n');
  //    Class::arduboy.print(F("getdownthingfrom"));
  //    Class::die();
  //  }
  for (int pos = 0; pos < (_xsize * _ysize); pos++) {
    if (_characters[pos] == arg) {
      int newpos = pos + _xsize;
      if (newpos < _xsize * _ysize) {
        return Scene::addClassToScene(arg, pos, newpos, action, clone);
      }
      return arg;
    }
  }
  return arg;
}

Class *Scene::getLeftThingFrom(Class *arg, Scene::AddClassAction action, Class *clone) {
  //  if (arg == 0) {
  //    Class::arduboy.print(pgm_read_byte(error_null_class_instance));
  //    Class::arduboy.print('\n');
  //    Class::arduboy.print(F("getleftthingfrom"));
  //    Class::die();
  //  }
  for (int pos = 0; pos < (_xsize * _ysize); pos++) {
    if (_characters[pos] == arg) {
      int newpos = pos - 1;
      if (((newpos + 1) % _xsize) != 0) {
        return Scene::addClassToScene(arg, pos, newpos, action, clone);
      }
      return arg;
    }
  }
  return arg;
}

Class *Scene::getRightThingFrom(Class *arg, Scene::AddClassAction action, Class *clone) {
  //  if (arg == 0) {
  //    Class::arduboy.print(pgm_read_byte(error_null_class_instance));
  //    Class::arduboy.print('\n');
  //    Class::arduboy.print(F("getrightthingfrom"));
  //    Class::die();
  //  }
  for (int pos = 0; pos < (_xsize * _ysize); pos++) {
    if (_characters[pos] == arg) {
      int newpos = pos + 1;
      if ((newpos % _xsize) != 0) {
        return Scene::addClassToScene(arg, pos, newpos, action, clone);
      }
      return arg;
    }
  }
  return arg;
}

void Scene::clearClasses(Class *arg) {
  for (int pos = 0; pos < (_xsize * _ysize); pos++) {
    if (_characters[pos] && (_characters[pos]->getTypeChar() == arg->getTypeChar())) {
      delete _characters[pos];
      _characters[pos] = 0;
    }
  }
}

Class *Scene::buildScene(Class *player, Class *path_proto, Class *block_proto, int is_scene_already = 0) {
  int plrpos;
  Class * c;
  if (is_scene_already) {
    for (int pos = 0; pos < (_xsize * _ysize); pos++) {
      if (_characters[pos] == player) {
        plrpos = pos;
      }
    }
  } else {
    plrpos = random(_xsize * _ysize);
  }
  _characters[plrpos] = path_proto->clone();
  block_proto->atPut(Class::Directive::Hidden, block_proto);
  c = Scene::buildPath(path_proto, block_proto, is_scene_already);
  if (player) {
    delete _characters[plrpos];
    _characters[plrpos] = player;
  }
  return c;
}

Class *Scene::closest(Class *arg, byte farest = 0, byte is_block = 0, byte is_not_block = 0) {
  int closest_pos = -1;
  int num = 0;
  int max_path_length = 0;
  int path_length;
  int end_path_length;

  for (int pos = 0; pos < (_xsize * _ysize); pos++) { //get max path
    if (_characters[pos] && (_characters[pos]->getTypeChar() == _path_proto->getTypeChar())) {
      if (_characters[pos]->toInt() > max_path_length)
        max_path_length = _characters[pos]->toInt();
    }
  }

  if (farest) {
    path_length = max_path_length;
    end_path_length = 2;
  } else {
    path_length = 0;
    end_path_length = max_path_length;
  }

  while (path_length != end_path_length) {
    for (int pos = 0; pos < (_xsize * _ysize); pos++) {
      if (_characters[pos] && (_characters[pos]->getTypeChar() == _path_proto->getTypeChar())) {
        if (_characters[pos]->toInt() == path_length) {
          num++;
          if (random(num) == 0) {
            closest_pos = pos;
          }
        }
      }
    }

    if ((!is_block) && (num > 0)) {
      delete _characters[closest_pos];
      _characters[closest_pos] = arg;
      return _characters[closest_pos];
    } else if ((is_block) && (num == 1)) {
      delete _characters[closest_pos];
      _characters[closest_pos] = _block_proto->clone();
      return _characters[closest_pos];
    } else if ((is_not_block) && (num > 1)) {
      delete _characters[closest_pos];
      _characters[closest_pos] = arg;
      return _characters[closest_pos];
    }
    num = 0;
    if (farest) {
      path_length--;
    } else {
      path_length++;
    }
  }
  return 0;
}

Class *Scene::buildPath(Class *path_proto, Class *block_proto, int is_scene_already = 0) {
  int has_free = 1;
  int max_instances = 11;
  int min_instances = 10;
  //int chance = 2;
  int last_pos;
  int count_visited;
  int count_created;

  while (has_free) {
    count_created = 0;
    count_visited = 0;
    last_pos = -1;
    for (int pos = 0; pos < (_xsize * _ysize); pos++) {
      //if ((count_created + count_visited) > max_instances)
      //break;
      if (_characters[pos] && (_characters[pos]->getTypeChar() == path_proto->getTypeChar())) {
        count_visited++;
        last_pos = pos;
        if ((((count_created + count_visited) < max_instances) && random(2)) || is_scene_already) {
          if (Scene::getUpThingFrom(_characters[pos], Scene::AddClassAction::CloneIfEmpty, _characters[pos]) == 0)
            count_created++;
        } else {
          Scene::getUpThingFrom(_characters[pos], Scene::AddClassAction::CopyIfEmpty, block_proto);
        }
        if ((((count_created + count_visited) < max_instances) && random(2)) || is_scene_already) {
          if (Scene::getDownThingFrom(_characters[pos], Scene::AddClassAction::CloneIfEmpty, _characters[pos]) == 0)
            count_created++;
        } else {
          Scene::getDownThingFrom(_characters[pos], Scene::AddClassAction::CopyIfEmpty, block_proto);
        }
        if ((((count_created + count_visited) < max_instances) && random(2)) || is_scene_already) {
          if (Scene::getLeftThingFrom(_characters[pos], Scene::AddClassAction::CloneIfEmpty, _characters[pos]) == 0)
            count_created++;
        } else {
          Scene::getLeftThingFrom(_characters[pos], Scene::AddClassAction::CopyIfEmpty, block_proto);
        }
        if ((((count_created + count_visited) < max_instances) && random(2)) || is_scene_already) {
          if (Scene::getRightThingFrom(_characters[pos], Scene::AddClassAction::CloneIfEmpty, _characters[pos]) == 0)
            count_created++;
        } else {
          Scene::getRightThingFrom(_characters[pos], Scene::AddClassAction::CopyIfEmpty, block_proto);
        }
      }
    }
    if ((count_created == 0) && (count_visited < min_instances) && (is_scene_already == 0)) { //fail
      //      for (int pos = 0; pos < (_xsize * _ysize); pos++) {
      //        if (_characters[pos] && (_characters[pos]->getTypeChar() == path_proto->getTypeChar())) {
      //          Scene::getUpThingFrom(_characters[pos], Scene::AddClassAction::CloneDestroy, _characters[pos]);
      //          count_created++;
      //          Scene::getLeftThingFrom(_characters[pos], Scene::AddClassAction::CloneDestroy, _characters[pos]);
      //          count_created++;
      //          Scene::getRightThingFrom(_characters[pos], Scene::AddClassAction::CloneDestroy, _characters[pos]);
      //          count_created++;
      //          Scene::getDownThingFrom(_characters[pos], Scene::AddClassAction::CloneDestroy, _characters[pos]);
      //          count_created++;
      //          break;
      //        }
      //      }
      return 0;
    }
    if (count_created == 0) {
      has_free = 0;
    }
    if ((count_created + count_visited) > max_instances) { //low memory, closing all paths
      for (int pos = 0; pos < (_xsize * _ysize); pos++) {
        if (_characters[pos] && (_characters[pos]->getTypeChar() == path_proto->getTypeChar())) {
          Scene::getUpThingFrom(_characters[pos], Scene::AddClassAction::CopyIfEmpty, block_proto);
          Scene::getLeftThingFrom(_characters[pos], Scene::AddClassAction::CopyIfEmpty, block_proto);
          Scene::getRightThingFrom(_characters[pos], Scene::AddClassAction::CopyIfEmpty, block_proto);
          Scene::getDownThingFrom(_characters[pos], Scene::AddClassAction::CopyIfEmpty, block_proto);
        }
      }
      has_free = 0;
    }
  }
  return this;
}

Class *Scene::atPut(Directive key, Class *arg) {
  switch (key) {
    case Class::Directive::Cursor: //set cursor on owner of arg
      {
        Class * owner = (this->atPut(Class::Directive::Owner, arg));
        if (owner) {
          while (this->atGet(Class::Directive::Left)) {};
          while (this->atGet(Class::Directive::Up)) {};
          do {
            do {
              if ((this->atGet(Class::Directive::Character)) == owner)
                return arg;
            } while (this->atGet(Class::Directive::Right));
            while (this->atGet(Class::Directive::Left)) {};
          } while (this->atGet(Class::Directive::Down));
        }
        return 0;
      }
      break;
    case Class::Directive::Near: //check class is near the cursor
      {
        Class * cls = 0;
        cls = Scene::getUpThingFrom(this->atGet(Class::Directive::Character), Scene::AddClassAction::NoAction, this->atGet(Class::Directive::Character));
        if (cls == arg)
          return arg;
        cls = Scene::getLeftThingFrom(this->atGet(Class::Directive::Character), Scene::AddClassAction::NoAction, this->atGet(Class::Directive::Character));
        if (cls == arg)
          return arg;
        cls = Scene::getRightThingFrom(this->atGet(Class::Directive::Character), Scene::AddClassAction::NoAction, this->atGet(Class::Directive::Character));
        if (cls == arg)
          return arg;
        cls = Scene::getDownThingFrom(this->atGet(Class::Directive::Character), Scene::AddClassAction::NoAction, this->atGet(Class::Directive::Character));
        if (cls == arg)
          return arg;
        return this->atGet(Class::Directive::Character);
      }
      break;
    case Class::Directive::Owner: //get owner of player in the scene
      {
        Class * cl = 0;
        Class * owner = 0;
        for (int pos = 0; pos < (_xsize * _ysize); pos++) {
          if (_characters[pos]) {
            if (_characters[pos] == arg)
              return arg;
            cl = _characters[pos]->atGet(Class::Directive::Next);
            owner = _characters[pos];
            while (cl) {
              if (arg == cl)
                return owner;
              cl = (cl->atGet(Class::Directive::Next));
            }
          }
        }
        return 0;
      }
      break;
    case Class::Directive::Delete: //delete class from scene recursively
      {

        //Class * owner = 0;
        for (int pos = 0; pos < (_xsize * _ysize); pos++) {
          if ((_characters[pos]) && (_characters[pos] == arg)) {
            _characters[pos] = 0;
          }
        }
        //        while (owner = (this->atPut(Class::Directive::Owner, arg))) {
        //          owner->atPut(Class::Directive::Delete, arg);
        //        }
        if (arg) {
          this->atPut(Class::Directive::Delete, (arg->atGet(Class::Directive::Next)));
          delete arg;
        }
      }
      break;
    case Class::Directive::Move: //move arg by shortest path
      {
        int min_path = INT8_MAX;
        Class * cls = 0;
        byte where = 0;
        cls = Scene::getUpThingFrom(arg, Scene::AddClassAction::NoAction, arg);
        if (cls && ((cls->getTypeChar()) == (_path_proto->getTypeChar())) && ((cls->toInt()) < min_path)) {
          min_path = cls->toInt();
          where = 1;
        }
        cls = Scene::getDownThingFrom(arg, Scene::AddClassAction::NoAction, arg);
        if (cls && ((cls->getTypeChar()) == (_path_proto->getTypeChar())) && ((cls->toInt()) < min_path)) {
          min_path = cls->toInt();
          where = 2;
        }
        cls = Scene::getLeftThingFrom(arg, Scene::AddClassAction::NoAction, arg);
        if (cls && ((cls->getTypeChar()) == (_path_proto->getTypeChar())) && ((cls->toInt()) < min_path)) {
          min_path = cls->toInt();
          where = 3;
        }
        cls = Scene::getRightThingFrom(arg, Scene::AddClassAction::NoAction, arg);
        if (cls && ((cls->getTypeChar()) == (_path_proto->getTypeChar())) && ((cls->toInt()) < min_path)) {
          min_path = cls->toInt();
          where = 4;
        }
        switch (where) {
          case 1:
            Scene::getUpThingFrom(arg, Scene::AddClassAction::MoveIfEmpty, arg);
            //Serial.println(F("up"));
            //Class::arduboy.clear();
            break;
          case 2:
            Scene::getDownThingFrom(arg, Scene::AddClassAction::MoveIfEmpty, arg);
            //Serial.println(F("down"));
            //Class::arduboy.clear();
            break;
          case 3:
            Scene::getLeftThingFrom(arg, Scene::AddClassAction::MoveIfEmpty, arg);
            //Serial.println(F("left"));
            //Class::arduboy.clear();
            break;
          case 4:
            Scene::getRightThingFrom(arg, Scene::AddClassAction::MoveIfEmpty, arg);
            //Serial.println(F("right"));
            //Class::arduboy.clear();
            break;
        }
      }
      break;
    case Class::Directive::Turn: //set next turn to all
      {
        Class * clas = 0;
        for (int pos = 0; pos < (_xsize * _ysize); pos++) {
          if (_characters[pos]) {
            _characters[pos]->atPut(Class::Directive::Turn, _characters[pos]);
            clas = _characters[pos]->atGet(Class::Directive::Next);
            while (clas) {
              clas->atPut(Class::Directive::Turn, clas);
              clas = clas->atGet(Class::Directive::Next);
            }
          }
        }
        return 0;
      }
      break;
    case Class::Directive::Next: //clear the scene except arg and his players
      {
        if (_path_proto) {
          delete _path_proto;
        }
        _path_proto = 0;
        Class * cl = arg;
        byte to_delete = 1;
        for (int pos = 0; pos < (_xsize * _ysize); pos++) {
          if (_characters[pos] && (_characters[pos] != arg) && (_characters[pos] != _block_proto)) {
            //            int j = 0;
            //            while (pgm_read_byte((_characters[pos]->_init) + j) != NULL) {
            //              Serial.print((char)pgm_read_byte((_characters[pos]->_init) + j));
            //              j++;
            //            }
            //            Serial.println();
            to_delete = 1;
            cl = arg;
            while (cl) {
              if (cl == _characters[pos])
                to_delete = 0;
              cl = (cl->atGet(Class::Directive::Next));
            }
            if (to_delete)
              delete _characters[pos];
          }
          _characters[pos] = 0;
        }
        if (_block_proto) {
          delete _block_proto;
        }
        _block_proto = 0;
      }
      break;
    case Class::Directive::Far: //set class to be farest by path
      return Scene::closest(arg, 1);
      //      {
      //        int farest_pos = -1;
      //        int path_length = 1;
      //        int num = 0;
      //        int max_path_length = 0;
      //        int done = 0;
      //
      //        for (int pos = 0; pos < (_xsize * _ysize); pos++) { //get max path
      //          if (_characters[pos] && (_characters[pos]->getTypeChar() == _path_proto->getTypeChar())) {
      //            if (_characters[pos]->toInt() > max_path_length)
      //              max_path_length = _characters[pos]->toInt();
      //          }
      //        }
      //        Serial.print(F("max"));
      //        Serial.println(max_path_length);
      //        for (int path_length = max_path_length; path_length > 2; path_length--)  {
      //          for (int pos = 0; pos < (_xsize * _ysize); pos++) {
      //            if (_characters[pos] && (_characters[pos]->getTypeChar() == _path_proto->getTypeChar())) {
      //              if (_characters[pos]->toInt() == path_length) {
      //                done = 1;
      //                num++;
      //                if (random(0, num) == 0) {
      //                  farest_pos = pos;
      //                  Serial.print(F("farest"));
      //                  Serial.println(farest_pos);
      //                }
      //              }
      //            }
      //          }
      //          //if (num > 1) {
      //          if (done) {
      //            Serial.print(F("num"));
      //            Serial.println(farest_pos);
      //            delete _characters[farest_pos];
      //            _characters[farest_pos] = arg;
      //            return _characters[farest_pos];
      //          }
      //          num = 0;
      //        }
      //        return 0;
      //      }
      break;
    case Class::Directive::Close: //set class to be closest by path but may block other paths
      return Scene::closest(arg);
      //      {
      //        int closest_pos;
      //        int path_length = 1;
      //        int num = 0;
      //        int max_path_length = 0;
      //        int done = 0;
      //
      //        for (int pos = 0; pos < (_xsize * _ysize); pos++) { //get max path
      //          if (_characters[pos] && (_characters[pos]->getTypeChar() == _path_proto->getTypeChar())) {
      //            if (_characters[pos]->toInt() > max_path_length)
      //              max_path_length = _characters[pos]->toInt();
      //          }
      //        }
      //        for (int path_length = 0; path_length < max_path_length; path_length++)  {
      //          for (int pos = 0; pos < (_xsize * _ysize); pos++) {
      //            if (_characters[pos] && (_characters[pos]->getTypeChar() == _path_proto->getTypeChar())) {
      //              if (_characters[pos]->toInt() == path_length) {
      //                done = 1;
      //                num++;
      //                if (random(0, num) == 0)
      //                  closest_pos = pos;
      //              }
      //            }
      //          }
      //          if (done) {
      //            delete _characters[closest_pos];
      //            _characters[closest_pos] = arg;
      //            return _characters[closest_pos];
      //          }
      //          num = 0;
      //        }
      //        return 0;
      //      }
      break;
    case Class::Directive::Clear: //build the scene and bounds
      Scene::clearClasses(arg);
      return this;
      break;
    case Class::Directive::Path: //set path
      _path_proto = arg->clone();
      return this;
      break;
    case Class::Directive::Block: //set block
      _block_proto = arg->clone();
      return this;
      break;
    case Class::Directive::Map: //set pathfinding map center
      return Scene::buildScene(arg, _path_proto, _block_proto, 1);
      break;
    case Class::Directive::Build: //build the scene and bounds
      {
        Class * c = 0;
        c = Scene::buildScene(arg, _path_proto, _block_proto);
        for (int pos = 0; pos < (_xsize * _ysize); pos++) {
          if (_characters[pos] == 0) {
            _characters[pos] = _block_proto;
          }
        }
        return c;
      }
      break;
    case Class::Directive::Character: //set character under a cursor or return existing
      {
        int i = (_ycoord->toInt() * _xsize) + _xcoord->toInt();//(::atoi(_ycoord->toStr()) * _xsize) + ::atoi(_xcoord->toStr());
        //Serial.println(i);
        //        if (i < 0 && i >= (_xsize * _ysize)) {
        //          return 0;
        //        }
        //        if (_characters[i]) {
        //          return _characters[i];
        //        }
        _characters[i] = arg;
        //Serial.println(F("player setted"));
        return arg;
      }
      break;
    case Class::Directive::X: //set x coordinate of a cursor within bounds or if new
      //if ((_xsize && (::atoi(arg->toStr()) < _xsize)) || (!_xsize)) {
      _xcoord = arg;
      //} else {
      //  return 0;
      //}
      return 0;
      break;
    case Class::Directive::Y: //set y coordinate of a cursor within bounds or if new
      //if ((_ysize && (::atoi(arg->toStr()) < _ysize)) || (!_ysize)) {
      _ycoord = arg;
      //} else {
      //  return 0;
      //}
      return 0;
      break;
    case Class::Directive::Up: //move pointed character up
      {
        return Scene::getUpThingFrom(arg, Scene::AddClassAction::MoveIfEmpty, arg);
      }
      break;
    case Class::Directive::Down: //move pointed character down
      {
        return Scene::getDownThingFrom(arg, Scene::AddClassAction::MoveIfEmpty, arg);
      }
      break;
    case Class::Directive::Left: //move pointed character left
      {
        return Scene::getLeftThingFrom(arg, Scene::AddClassAction::MoveIfEmpty, arg);
      }
      break;
    case Class::Directive::Right: //move pointed character right
      {
        return Scene::getRightThingFrom(arg, Scene::AddClassAction::MoveIfEmpty, arg);
      }
      break;
    case Class::Directive::Draw: //draw scene on the screen for player
      {
        int xcur = _xcoord->toInt();//::atoi(_xcoord->toStr());
        int ycur = _ycoord->toInt();//::atoi(_ycoord->toStr());
        for (int y = 0; y < _ysize; y++) {
          for (int x = 0; x < _xsize; x++) {
            if ((x == xcur && y == ycur) ) {
              Class::arduboy.setTextColor(BLACK);
              Class::arduboy.setTextBackground(WHITE);
            } else {
              Class::arduboy.setTextColor(WHITE);
              Class::arduboy.setTextBackground(BLACK);
            }
            if (_characters[y * _xsize + x] && (_characters[y * _xsize + x] == arg ||
                                                Scene::getUpThingFrom(_characters[y * _xsize + x], Scene::AddClassAction::NoAction, _characters[y * _xsize + x]) == arg ||
                                                Scene::getDownThingFrom(_characters[y * _xsize + x], Scene::AddClassAction::NoAction, _characters[y * _xsize + x]) == arg  ||
                                                Scene::getRightThingFrom(_characters[y * _xsize + x], Scene::AddClassAction::NoAction, _characters[y * _xsize + x]) == arg  ||
                                                Scene::getLeftThingFrom(_characters[y * _xsize + x], Scene::AddClassAction::NoAction, _characters[y * _xsize + x]) == arg
                                               )) {
              if (_characters[y * _xsize + x]) {
                //if (_characters[y * _xsize + x]->atGet(Class::Directive::Hidden) == 0)
                _characters[y * _xsize + x]->atGet(Class::Directive::Draw);
              } else {
                char c = (char) (pgm_read_byte_near(_scene + (y * _xsize + x)));
                //char z = (char) (pgm_read_byte_near(_zone + (y * _xsize + x) + 1));
                Class::arduboy.print(c);
              }
              _characters[y * _xsize + x]->atPut(Class::Directive::Hidden, _characters[y * _xsize + x]);
            } else {
              if (_characters[y * _xsize + x] && (_characters[y * _xsize + x]->atGet(Class::Directive::Hidden) == 0)) {
                _characters[y * _xsize + x]->atGet(Class::Directive::Draw);
              } else {
                Class::arduboy.print(' ');
              }
            }
          }
          Class::arduboy.println();
        }
      }
      return 0;
      break;
    default:
      return Class::atPut(key, arg);
      break;
  }
  return this;
}

Class *Scene::atGet(Directive key) {
  switch (key) {
    case Class::Directive::Delete: //delete player from scene on cursor
      {
        Class * arg = this->atGet(Class::Directive::Character);
        int count = 0;
        int cur = (_ycoord->toInt() * _xsize) + _xcoord->toInt();
        for (int pos = 0; pos < (_xsize * _ysize); pos++) {
          if (_characters[pos] && (_characters[pos] == arg)) {
            count++;
          }
        }
        _characters[cur] = 0;
        if (count < 2) // last instance
          delete arg;
      }
      break;
    //    case Class::Directive::Turn: //every player makes turn
    //      for (int pos = 0; pos < (_xsize * _ysize); pos++) {
    //        if (_characters[pos] && _characters[pos]->atGet(Class::Directive::Script)) {
    //          _characters[pos]->atGet(Class::Directive::Turn);
    //        }
    //      }
    //      return 0;
    //      break;
    case Class::Directive::Place: //set class to be closest by path but without blocking other paths
      return Scene::closest(0, 0, 0, 1);
      break;
    case Class::Directive::Block: //get block
      return _block_proto;
      break;
    case Class::Directive::Close: //set class to be closest by path but with blocking other paths
      return Scene::closest(0, 0, 1);
      //      {
      //        int closest_pos;
      //        int path_length = 1;
      //        int num = 0;
      //        int max_path_length = 0;
      //
      //        for (int pos = 0; pos < (_xsize * _ysize); pos++) { //get max path
      //          if (_characters[pos] && (_characters[pos]->getTypeChar() == _path_proto->getTypeChar())) {
      //            if (_characters[pos]->toInt() > max_path_length)
      //              max_path_length = _characters[pos]->toInt();
      //          }
      //        }
      //        for (int path_length = 0; path_length < max_path_length; path_length++)  {
      //          for (int pos = 0; pos < (_xsize * _ysize); pos++) {
      //            if (_characters[pos] && (_characters[pos]->getTypeChar() == _path_proto->getTypeChar())) {
      //              if (_characters[pos]->toInt() == path_length) {
      //                num++;
      //                closest_pos = pos;
      //              }
      //            }
      //          }
      //          //          if (num == 0) { //avoid nearest blocks
      //          //            path_length--;
      //          //          } else
      //          if (num == 1) { //found
      //            delete _characters[closest_pos];
      //            _characters[closest_pos] = _block_proto->clone();
      //            return _characters[closest_pos];
      //          }
      //          num = 0;
      //        }
      //        return 0;
      //      }
      break;
    //    case Class::Directive::Begin: //begin next round
    //      {
    //        for (int pos = 0; pos < (_xsize * _ysize); pos++) {
    //          if (_characters[pos]) {
    //            _characters[pos]->atGet(Class::Directive::Next);
    //          }
    //        }
    //        return 0;
    //      }
    //      break;
    case Class::Directive::Turn: //get next character who has turn
      {
        for (int pos = 0; pos < (_xsize * _ysize); pos++) {
          if (_characters[pos] && _characters[pos]->atGet(Class::Directive::Turn)) {
            return _characters[pos];
          }
        }
        return 0;
      }
      break;
    case Class::Directive::Character: //get the character under the cursor
      {
        int x = _xcoord->toInt();//::atoi(_xcoord->toStr());
        //Serial.println(x);
        int y = _ycoord->toInt();//::atoi(_ycoord->toStr());
        //Serial.println(y);
        return _characters[y * _xsize + x];
        break;
      }
    case Class::Directive::Up: //move the cursor up within bounds
      _ycoord->atGet(Class::Directive::Down);
      if (_ycoord->toInt() < 0) {
        _ycoord->atGet(Class::Directive::Up);
        return 0;
      } else {
        return _ycoord;
      }
      break;
    case Class::Directive::Down:
      _ycoord->atGet(Class::Directive::Up);
      if (_ycoord->toInt() > (_ysize - 1)) {
        _ycoord->atGet(Class::Directive::Down);
        return 0;
      } else {
        return _ycoord;
      }
      break;
    case Class::Directive::Left:
      _xcoord->atGet(Class::Directive::Down);
      if (_xcoord->toInt() < 0) {
        _xcoord->atGet(Class::Directive::Up);
        return 0;
      } else {
        return _xcoord;
      }
      break;
    case Class::Directive::Right:
      _xcoord->atGet(Class::Directive::Up);
      if (_xcoord->toInt() > (_xsize - 1)) {
        _xcoord->atGet(Class::Directive::Down);
        return 0;
      } else {
        return _xcoord;
      }
      break;
    case Class::Directive::X: //get x coordinate of a cursor
      return _xcoord;
      break;
    case Class::Directive::Y: //get y coordinate of a cursor
      return _ycoord;
      break;
    //    case Class::Directive::Clear: //clear the scene and bounds
    //      _xsize = 0;
    //      _ysize = 0;
    //      _path_proto = 0;
    //      _block_proto = 0;_xcoord
    //      delete[] _characters;
    //      break;
    case Class::Directive::Build: //build the scene and bounds
      _xsize = _xcoord->toInt();//::atoi(_xcoord->toStr());
      _ysize = _ycoord->toInt();//::atoi(_ycoord->toStr());
      _characters = new Class*[_xsize * _ysize] {};
      return this;
      break;
    case Class::Directive::Draw: //draw scene on the screen
      {
        int xcur = _xcoord->toInt();//::atoi(_xcoord->toStr());
        int ycur = _ycoord->toInt();//::atoi(_ycoord->toStr());
        for (int y = 0; y < _ysize; y++) {
          for (int x = 0; x < _xsize; x++) {
            if ((x == xcur && y == ycur) ) { //|| (_characters[y * _xsize + x] && (_characters[y * _xsize + x]->atGet(Class::Directive::End))))  {
              Class::arduboy.setTextColor(BLACK);
              Class::arduboy.setTextBackground(WHITE);
            }
            if (_characters[y * _xsize + x]) {
              _characters[y * _xsize + x]->atGet(Class::Directive::Draw);
            } else {
              char c = (char) (pgm_read_byte_near(_scene + (y * _xsize + x)));
              //char z = (char) (pgm_read_byte_near(_zone + (y * _xsize + x) + 1));
              Class::arduboy.print(c);
            }
            Class::arduboy.setTextColor(WHITE);
            Class::arduboy.setTextBackground(BLACK);
          }
          Class::arduboy.println();
        }
      }
      return 0;
      break;
    default:
      return Class::atGet(key);
      break;
  }
}

Scene::~Scene() {
  //if (_xcoord)
  //delete _xcoord;
  //if (_ycoord)
  //delete _ycoord;
  delete[] _characters;
}
