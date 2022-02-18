#include "Scene.h"
//Scene *Scene::_exemplar = new Scene(Exemplar());

char* Scene::toStr() {
  return 0;
}

int Scene::toInt() {
  return 0;
}

char Scene::getTypeChar() {
  return Scene::_typeChar;
}

Scene::Scene(Exemplar a): Class(a) {

}

Scene::Scene(const char* s): Class() {
  if (pgm_read_byte_near(s) == Scene::_typeChar) {
    s++;
    _scene = s;
  } else {
    Class::errFlag = 'Y';
  }
}

Class *Scene::make(const char* s) {
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
      _characters[newpos]->atGet(Class::Directive::Up); //for coords movement
      return 0;
      break;
    case Scene::AddClassAction::CloneIfCoordinate:
      {
        Class* c = 0;
        if (clone->getTypeChar() == _path_proto->getTypeChar()) {
          c = clone->clone();
          c->atGet(Class::Directive::Up); //for coords movement
        } else if ((clone->atGet(Class::Directive::Next))->getTypeChar() == _path_proto->getTypeChar()) {
          c = (clone->atGet(Class::Directive::Next))->clone();
          c->atGet(Class::Directive::Up); //for coords movement
        }
        if (_characters[newpos] && (_characters[newpos]->atGet(Class::Directive::Block))) {
          if (c)
            delete c;
          return _characters[newpos];
        } else if (_characters[newpos] && (_characters[newpos]->atPut(Class::Directive::Character, _path_proto) == 0)) { //not contains coord
          _characters[newpos]->atPut(Class::Directive::Add, c);
          return 0;
        } else if (_characters[newpos]) {
          if (c)
            delete c;
          return _characters[newpos];
        } else {
          _characters[newpos] = c;
          return 0;
        }
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
  return 0;
}

Class *Scene::getUpThingFrom(Class *arg, Scene::AddClassAction action, Class *clone) {
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
  Class* c = 0;
  for (int pos = 0; pos < (_xsize * _ysize); pos++) {
    if (_characters[pos] && (_characters[pos]->getTypeChar() == arg->getTypeChar())) {
      c = _characters[pos];
      while (c) {
        if ((c->_init) == (arg->_init))
        {
          this->atPut(Class::Directive::Delete, c);
          pos = 0; //if more clones in this cell
          break;
        }
        c = c->atGet(Class::Directive::Next);
      }
    }
  }
  /*Class * t = 0;
    this->atGet(Class::Directive::Cursor);
    while ((t = (this->atPut(Class::Directive::Search, arg)))) {
    //char buff[10];   Class::printDebug(itoa(random(5), buff, 10));
    this->atPut(Class::Directive::Delete, t);
    }*/
}

Class *Scene::buildScene(Class * player, Class * path_proto, Class * block_proto, int is_scene_already) {
  //  if (is_scene_already == 0) {
  //    char buff[10];
  //    Class::printDebug(itoa(random(5), buff, 10));
  //  }
  int plrpos = 0;
  Class * c = 0;
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

Class *Scene::closest(Class * arg, byte farest, byte is_block) {
  int closest_pos = -1;
  int num = 0;
  int max_path_length = 0;
  int path_length = 0;
  int end_path_length = 0;

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
  //char buff1[10]; Class::printDebug(itoa(path_length, buff1, 10));
  //char buff2[10]; Class::printDebug(itoa(end_path_length, buff2, 10));

  while (path_length != end_path_length) {
    //char buff2[10]; Class::printDebug(itoa(path_length + 10, buff2, 10));
    for (int pos = 0; pos < (_xsize * _ysize); pos++) {
      if (_characters[pos] && (_characters[pos]->getTypeChar() == _path_proto->getTypeChar())) {
        if (_characters[pos]->toInt() == path_length) {
          num++;
          if (random(num) == 0) {
            closest_pos = pos;
          }
          //char buff4[10]; Class::printDebug(itoa(num, buff4, 10));
        }
      }
    }
    //char buff4[10]; Class::printDebug(itoa(num, buff4, 10));
    if ((!is_block) && (num > 0)) {
      delete _characters[closest_pos];
      _characters[closest_pos] = arg;
      return _characters[closest_pos];
    } else if ((is_block) && (num == 1)) {
      delete _characters[closest_pos];
      _characters[closest_pos] = _block_proto->clone();
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

byte Scene::isPath(byte count_created, byte count_visited, byte max_instances, byte chance, byte is_scene_already) {
  if ((((count_created + count_visited) < max_instances) && random(chance)) || is_scene_already)
    return 1;
  else
    return 0;
}

Class *Scene::buildPath(Class * path_proto, Class * block_proto, int is_scene_already) {
  int has_free = 1;
  int min_instances;
  int max_instances;
  if (is_scene_already)
    max_instances = 30000;
  else
    max_instances = (_max_path_proto->toInt());
  if (is_scene_already)
    min_instances = 0;
  else
    min_instances = (_min_path_proto->toInt()); //TODO if <7 then bug with max_path_length = 0
  int chance = 4;
  //int last_pos;
  int count_visited = 0;
  int count_created = 0;
  //int iter = random(5);
  //char buff2[10]; Class::printDebug(itoa((random(10)+10), buff2, 10));
  while (has_free) {
    count_created = 0;
    count_visited = 0;
    //last_pos = -1;
    for (int pos = 0; pos < (_xsize * _ysize); pos++) {
      //if ((count_created + count_visited) > max_instances)
      //break;
      if (_characters[pos] && (_characters[pos]->atPut(Class::Directive::Character, path_proto))) {
        count_visited++;
        //last_pos = pos;
        if (isPath(count_created, count_visited, max_instances, chance, is_scene_already)) {
          if (Scene::getUpThingFrom(_characters[pos], Scene::AddClassAction::CloneIfCoordinate, _characters[pos]) == 0)
            count_created++;
        } else {
          Scene::getUpThingFrom(_characters[pos], Scene::AddClassAction::CopyIfEmpty, block_proto);
        }
        if (isPath(count_created, count_visited, max_instances, chance, is_scene_already)) {
          if (Scene::getDownThingFrom(_characters[pos], Scene::AddClassAction::CloneIfCoordinate, _characters[pos]) == 0)
            count_created++;
        } else {
          Scene::getDownThingFrom(_characters[pos], Scene::AddClassAction::CopyIfEmpty, block_proto);
        }
        if (isPath(count_created, count_visited, max_instances, chance, is_scene_already)) {
          if (Scene::getLeftThingFrom(_characters[pos], Scene::AddClassAction::CloneIfCoordinate, _characters[pos]) == 0)
            count_created++;
        } else {
          Scene::getLeftThingFrom(_characters[pos], Scene::AddClassAction::CopyIfEmpty, block_proto);
        }
        if (isPath(count_created, count_visited, max_instances, chance, is_scene_already)) {
          if (Scene::getRightThingFrom(_characters[pos], Scene::AddClassAction::CloneIfCoordinate, _characters[pos]) == 0)
            count_created++;
        } else {
          Scene::getRightThingFrom(_characters[pos], Scene::AddClassAction::CopyIfEmpty, block_proto);
        }
        //char buff1[10]; Class::printDebug(itoa(pos, buff1, 10));
        //char buff2[10]; Class::printDebug(itoa(count_created, buff2, 10));
      }

    }
    if ((count_created == 0) && (count_visited < min_instances) && (is_scene_already == 0)) { //fail
      return 0;
    }
    if (count_created == 0) {
      has_free = 0;
    }
    if ((count_created + count_visited) > max_instances) { //low memory, closing all paths
      if (is_scene_already == 0) {
        for (int pos = 0; pos < (_xsize * _ysize); pos++) {
          if (_characters[pos] && (_characters[pos]->getTypeChar() == path_proto->getTypeChar())) {
            Scene::getUpThingFrom(_characters[pos], Scene::AddClassAction::CopyIfEmpty, block_proto);
            Scene::getLeftThingFrom(_characters[pos], Scene::AddClassAction::CopyIfEmpty, block_proto);
            Scene::getRightThingFrom(_characters[pos], Scene::AddClassAction::CopyIfEmpty, block_proto);
            Scene::getDownThingFrom(_characters[pos], Scene::AddClassAction::CopyIfEmpty, block_proto);
          }
        }
      }
      has_free = 0;
    }
    //if (is_scene_already == 0) {
    //  char buff[10];
    //  iter++;
    //  Class::printDebug(itoa(iter, buff, 10));
    //}
  }

  return this;
}

void Scene::recursiveDeleteClass(Class * arg) {
  if (arg) {
    byte is_delete = 1;
    for (int pos = 0; pos < (_xsize * _ysize); pos++) { //don't delete shared classes (for example _block_proto)
      Class* cl = _characters[pos];//->atGet(Class::Directive::Next);
      while (cl) {
        if (arg == cl)
          is_delete = 0;
        cl = (cl->atGet(Class::Directive::Next));
      }
    }
    Scene::recursiveDeleteClass(arg->atGet(Class::Directive::Next));
    if (is_delete)
      delete arg;
  }
}

byte Scene::checkPath(Class * cls, byte min_path) {
  Class * c = cls;
  if (c && (c->getTypeChar() == _path_proto->getTypeChar())) {
    return c->toInt();
  } else if (c) {
    c = c->atGet(Class::Directive::Next);
    if (c && (c->getTypeChar() == _path_proto->getTypeChar())) {
      return c->toInt();
    }
  }
  //char buff2[10]; Class::printDebug(itoa((random(10)+10), buff2, 10));
  return INT8_MAX;
}

void Scene::updatePath(Class* cls, byte* min_path, byte *new_min_path, byte *where, byte *is_rand, byte *dir) {
  *dir = *dir + 1;
  *new_min_path = checkPath(cls, *min_path);
  if (*new_min_path < *min_path) {
    *min_path = *new_min_path;
    *where = *dir;
    *is_rand = 0;
  } else if (*new_min_path == *min_path) {
    if (random(2))
      *is_rand = *dir;
  }
}

Class *Scene::atPut(Directive key, Class * arg) {
  switch (key) {
    case Class::Directive::Greater: //atPut clone arg and set clone to be min path in autogenerated scene
      _min_path_proto = arg->clone();
      return this;
      break;
    case Class::Directive::Less: //atPut clone arg and set clone to be max path in autogenerated scene
      _max_path_proto = arg->clone();
      return this;
      break;
    case Class::Directive::Cursor: //atPut set cursor on owner of arg or return NULL
      {
        Class * owner = (this->atPut(Class::Directive::Owner, arg));
        if (owner) {
          this->atGet(Class::Directive::Cursor);
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
    case Class::Directive::Near: //atPut check arg is near the cursor and return arg if true. if false then return class under cursor
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
    case Class::Directive::Free: //atPut check coordindate proto is near the arg and return arg if true. if false then return 0
      {
        Class * cls = 0;
        cls = Scene::getUpThingFrom(arg, Scene::AddClassAction::NoAction, arg);
        if (cls && (cls->getTypeChar() == _path_proto->getTypeChar()))
          return arg;
        cls = Scene::getLeftThingFrom(arg, Scene::AddClassAction::NoAction, arg);
        if (cls && (cls->getTypeChar() == _path_proto->getTypeChar()))
          return arg;
        cls = Scene::getRightThingFrom(arg, Scene::AddClassAction::NoAction, arg);
        if (cls && (cls->getTypeChar() == _path_proto->getTypeChar()))
          return arg;
        cls = Scene::getDownThingFrom(arg, Scene::AddClassAction::NoAction, arg);
        if (cls && (cls->getTypeChar() == _path_proto->getTypeChar()))
          return arg;
        //char buff2[10]; Class::printDebug(itoa((random(10)+10), buff2, 10));
        return 0;
      }
      break;
    case Class::Directive::Owner: //atPut get next nearest owner of arg in the scene if it exists or return NULL
      {
        Class * cl = 0;
        Class * owner = 0;
        for (int pos = 0; pos < (_xsize * _ysize); pos++) {
          if (_characters[pos]) {
            //if (_characters[pos] == arg)
            //  return arg;
            cl = _characters[pos]->atGet(Class::Directive::Next);
            owner = _characters[pos];
            while (cl) {
              if (arg == cl)
                return owner;
              if ((cl->atGet(Class::Directive::Place)) != 0) // stop on first owner
                break;
              cl = (cl->atGet(Class::Directive::Next));
            }
          }
        }
        return 0;
      }
      break;
    case Class::Directive::Delete: //atPut delete arg from every other class on the scene if arg is not scene block
      {
        if (arg) {
          for (int pos = 0; pos < (_xsize * _ysize); pos++) { //we need to remove arg from class chains of every other class
            if (_characters[pos]) {
              _characters[pos]->atPut(Class::Directive::Delete, arg);
              if ((_characters[pos] == arg) && (_characters[pos] != _block_proto)) {//&& ((_characters[pos] != _block_proto) || ((_characters[pos] == _block_proto) && (_characters[pos] == this->atGet(Class::Directive::Character))))) { //now check for the arg itself
                _characters[pos] = 0;
              }
            }
          }
          //after all chains has freed from arg we can finally free memory
          Scene::recursiveDeleteClass(arg);
          //char buff[1] = "d";
          //Class::printDebug(buff);
        }
        return 0;
      }
      break;
    case Class::Directive::Move: //atPut move arg by shortest path
      {
        byte min_path = INT8_MAX;
        byte new_min_path = 0;
        Class * cls = 0;
        byte where = 0;
        byte is_rand = 0;
        byte dir = 0;
        cls = Scene::getUpThingFrom(arg, Scene::AddClassAction::NoAction, arg);
        updatePath(cls, &min_path, &new_min_path, &where, &is_rand, &dir);
        cls = Scene::getDownThingFrom(arg, Scene::AddClassAction::NoAction, arg);
        updatePath(cls, &min_path, &new_min_path, &where, &is_rand, &dir);
        cls = Scene::getLeftThingFrom(arg, Scene::AddClassAction::NoAction, arg);
        updatePath(cls, &min_path, &new_min_path, &where, &is_rand, &dir);
        cls = Scene::getRightThingFrom(arg, Scene::AddClassAction::NoAction, arg);
        updatePath(cls, &min_path, &new_min_path, &where, &is_rand, &dir);
        if (is_rand) {

          where = is_rand;
        }
        switch (where) {
          case 1:
            Scene::getUpThingFrom(arg, Scene::AddClassAction::MoveIfEmpty, arg);
            break;
          case 2:
            Scene::getDownThingFrom(arg, Scene::AddClassAction::MoveIfEmpty, arg);
            break;
          case 3:
            Scene::getLeftThingFrom(arg, Scene::AddClassAction::MoveIfEmpty, arg);
            break;
          case 4:
            Scene::getRightThingFrom(arg, Scene::AddClassAction::MoveIfEmpty, arg);
            break;
        }
        //char buff2[10]; Class::printDebug(itoa(where, buff2, 10));
        return 0;
      }
      break;
    case Class::Directive::Turn: //atPut set next turn to all classes in the scene
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
    case Class::Directive::Search: //atPut find next position of clone of arg in the scene and moves cursor. Use atGet ::Cursor to start search again
      {
        if ((_ycoord->toInt()) || (_xcoord->toInt())) { //if not first search
          if ((this->atGet(Class::Directive::Right)) == 0) { //move cursor next
            if (this->atGet(Class::Directive::Down)) {
              while (this->atGet(Class::Directive::Left)) {};
            }
          }
        }
        Class * cls = 0;
        do {
          do {
            cls = this->atGet(Class::Directive::Character);
            while (cls) {
              if ((cls->_init) == (arg->_init)) {
                return cls;
              }
              cls = (cls->atGet(Class::Directive::Next));
            }
          } while (this->atGet(Class::Directive::Right));
          while (this->atGet(Class::Directive::Left)) {};
        } while (this->atGet(Class::Directive::Down));
        return 0;
      }
      break;
    case Class::Directive::Next: //atPut clear the scene except arg and his classes
      {
        if (_path_proto) {
          delete _path_proto;
        }
        if (_max_path_proto) {
          delete _max_path_proto;
        }
        if (_min_path_proto) {
          delete _min_path_proto;
        }
        _max_path_proto = 0;
        _min_path_proto = 0;
        _path_proto = 0;
        Class * cl = arg;
        byte to_delete = 1;
        for (int pos = 0; pos < (_xsize * _ysize); pos++) {
          if (_characters[pos] && (_characters[pos] != arg) && (_characters[pos] != _block_proto)) {
            to_delete = 1;
            cl = arg;
            while (cl) {
              if (cl == _characters[pos])
                to_delete = 0;
              cl = (cl->atGet(Class::Directive::Next));
            }
            if (to_delete)
              this->atPut(Class::Directive::Delete, _characters[pos]);
          }
          _characters[pos] = 0;
        }
        if (_block_proto) {
          delete _block_proto;
        }
        if (_max_path_proto) {
          delete _max_path_proto;
        }
        if (_min_path_proto) {
          delete _min_path_proto;
        }
        _block_proto = 0;
        _max_path_proto = 0;
        _min_path_proto = 0;
      }
      break;
    case Class::Directive::Far: //atPut place arg on scene to be farest by path
      return Scene::closest(arg, 1);
      break;
    case Class::Directive::Close: //atPut place arg to be closest by path but may block the only path through
      return Scene::closest(arg);
      break;
    case Class::Directive::Clear: //atPut clear all clones of arg in the scene
      Scene::clearClasses(arg);
      return this;
      break;
    case Class::Directive::Path: //atPut clone arg and set clone to be path prototype for the scene
      _path_proto = arg->clone();
      return this;
      break;
    case Class::Directive::Block: //atPut clone arg and set clone to be block prototype for the scene
      if (_block_proto)
        delete _block_proto;
      _block_proto = arg->clone();
      return this;
      break;
    case Class::Directive::Map: //atPut set path coordinates leading to arg
      //char buff2[10]; Class::printDebug(itoa((random(10)+10), buff2, 10));
      return Scene::buildScene(arg, _path_proto, _block_proto, 1);
      break;
    case Class::Directive::Build: //atPut try to build the scene and bounds with x and y sizes already setted then place arg there. If fail then return NULL
      {
        Class * c = 0;
        //char buff2[3]= "bug"; Class::printDebug(buff2);
        c = Scene::buildScene(arg, _path_proto, _block_proto);
        for (int pos = 0; pos < (_xsize * _ysize); pos++) {
          if (_characters[pos] == 0) {
            _characters[pos] = _block_proto;
          }
        }
        return c;
      }
      break;
    case Class::Directive::Character: //atPut place arg under a cursor
      {
        int i = (_ycoord->toInt() * _xsize) + _xcoord->toInt();
        _characters[i] = arg;
        return arg;
      }
      break;
    case Class::Directive::X: //atPut set x coordinate of a cursor within bounds if they exists. Set this to build initial scene
      _xcoord = arg;
      return 0;
      break;
    case Class::Directive::Y: //atPut set y coordinate of a cursor within bounds if they exists. Set this to build initial scene
      _ycoord = arg;
      return 0;
      break;
    case Class::Directive::Up: //atPut move arg. Return NULL if success, class that blocks way, arg if bounds
      {
        return Scene::getUpThingFrom(arg, Scene::AddClassAction::MoveIfEmpty, arg);
      }
      break;
    case Class::Directive::Down: //atPut move arg. Return NULL if success, class that blocks way, arg if bounds
      {
        return Scene::getDownThingFrom(arg, Scene::AddClassAction::MoveIfEmpty, arg);
      }
      break;
    case Class::Directive::Left: //atPut move arg. Return NULL if success, class that blocks way, arg if bounds
      {
        return Scene::getLeftThingFrom(arg, Scene::AddClassAction::MoveIfEmpty, arg);
      }
      break;
    case Class::Directive::Right: //atPut move arg. Return NULL if success, class that blocks way, arg if bounds
      {
        return Scene::getRightThingFrom(arg, Scene::AddClassAction::MoveIfEmpty, arg);
      }
      break;
    case Class::Directive::Draw: //atPut draw scene on the screen for arg
      {
        int xcur = _xcoord->toInt();
        int ycur = _ycoord->toInt();
        for (int y = 0; y < _ysize; y++) {
          for (int x = 0; x < _xsize; x++) {
            if ((x == xcur && y == ycur) ) {
              Class::exemplar.setTextColor(BLACK);
              Class::exemplar.setTextBackground(WHITE);
            }
            if (_characters[y * _xsize + x] && (_characters[y * _xsize + x] == arg ||
                                                Scene::getUpThingFrom(_characters[y * _xsize + x], Scene::AddClassAction::NoAction, _characters[y * _xsize + x]) == arg ||
                                                Scene::getDownThingFrom(_characters[y * _xsize + x], Scene::AddClassAction::NoAction, _characters[y * _xsize + x]) == arg  ||
                                                Scene::getRightThingFrom(_characters[y * _xsize + x], Scene::AddClassAction::NoAction, _characters[y * _xsize + x]) == arg  ||
                                                Scene::getLeftThingFrom(_characters[y * _xsize + x], Scene::AddClassAction::NoAction, _characters[y * _xsize + x]) == arg
                                               )) {
              if (_characters[y * _xsize + x]) {
                _characters[y * _xsize + x]->atGet(Class::Directive::Draw);
              } else {
                char c = (char) (pgm_read_byte_near(_scene + (y * _xsize + x)));
                Class::exemplar.print(c);
              }
              _characters[y * _xsize + x]->atPut(Class::Directive::Hidden, _characters[y * _xsize + x]);
            } else {
              if (_characters[y * _xsize + x] && (_characters[y * _xsize + x]->atGet(Class::Directive::Hidden) == 0)) {
                _characters[y * _xsize + x]->atGet(Class::Directive::Draw);
              } else {
                Class::exemplar.print(' ');
              }
            }
            Class::exemplar.setTextColor(WHITE);
            Class::exemplar.setTextBackground(BLACK);
          }
          Class::exemplar.println();
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
    case Class::Directive::Cursor: //atGet set cursor at start of scene
      while (this->atGet(Class::Directive::Left)) {};
      while (this->atGet(Class::Directive::Up)) {};
      break;
    case Class::Directive::Delete: //atGet delete player under cursor from scene
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
    case Class::Directive::Place: //atGet place scene block to be closest by path but without blocking other paths
      return Scene::closest(this->atGet(Class::Directive::Block), 0, 1);
      break;
    case Class::Directive::Block: //atGet return block prototype of the scene
      return _block_proto;
      break;
    case Class::Directive::Close: //atGet place scene block to be closest by path but with blocking other paths
      return Scene::closest(0, 0, 1);
      break;
    case Class::Directive::Turn: //atGet get next class who has turn
      {
        for (int pos = 0; pos < (_xsize * _ysize); pos++) {
          if (_characters[pos] && _characters[pos]->atGet(Class::Directive::Turn)) {
            return _characters[pos];
          }
        }
        return 0;
      }
      break;
    case Class::Directive::Character: //atGet get the class under the cursor
      {
        int x = _xcoord->toInt();
        int y = _ycoord->toInt();
        return _characters[y * _xsize + x];
        break;
      }
    case Class::Directive::Up: //atGet move the cursor within bounds. Return NULL if bounds, or y path prototype if moved
      _ycoord->atGet(Class::Directive::Down);
      if (_ycoord->toInt() < 0) {
        _ycoord->atGet(Class::Directive::Up);
        return 0;
      } else {
        return _ycoord;
      }
      break;
    case Class::Directive::Down: //atGet move the cursor within bounds. Return NULL if bounds, or y path prototype if moved
      _ycoord->atGet(Class::Directive::Up);
      if (_ycoord->toInt() > (_ysize - 1)) {
        _ycoord->atGet(Class::Directive::Down);
        return 0;
      } else {
        return _ycoord;
      }
      break;
    case Class::Directive::Left: //atGet move the cursor within bounds. Return NULL if bounds, or y path prototype if moved
      _xcoord->atGet(Class::Directive::Down);
      if (_xcoord->toInt() < 0) {
        _xcoord->atGet(Class::Directive::Up);
        return 0;
      } else {
        return _xcoord;
      }
      break;
    case Class::Directive::Right: //atGet move the cursor within bounds. Return NULL if bounds, or y path prototype if moved
      _xcoord->atGet(Class::Directive::Up);
      if (_xcoord->toInt() > (_xsize - 1)) {
        _xcoord->atGet(Class::Directive::Down);
        return 0;
      } else {
        return _xcoord;
      }
      break;
    case Class::Directive::X: //atGet get x coordinate of a cursor
      return _xcoord;
      break;
    case Class::Directive::Y: //atGet get y coordinate of a cursor
      return _ycoord;
      break;
    case Class::Directive::Build: //atGet initialize empty scene and its bounds. Set X and Y coordinates first before calling this method
      _xsize = _xcoord->toInt();
      _ysize = _ycoord->toInt();
      _characters = new Class*[_xsize * _ysize] {};
      return this;
      break;
    default:
      return Class::atGet(key);
      break;
  }
  return 0;
}

Scene::~Scene() {
  delete[] _characters;
}
