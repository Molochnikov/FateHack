#pragma once

#include "Class.h"

//#include "Scenes.h"
//#include "Zones.h"

//#include <Arduino.h>

class Scene : public Class {
    friend Class *Class::make(const char* s);
  public:
    Class *atPut(Directive key, Class *arg);
    Class *atGet(Directive key);
    const char* toStr();
    const int toInt();
    const char getTypeChar();
    Scene(Exemplar a);
    Scene(const char *s);
    ~Scene();
  private:
    enum AddClassAction {
      NoAction, CloneDestroy, CloneIfEmpty, MoveIfEmpty, MoveDestroy, CopyIfEmpty
    };
    //static Scene *_exemplar;
    Class *make(const char* s);
    static const char _typeChar = 'S';
    Class* _xcoord = 0; //cursor x
    Class* _ycoord = 0; //cursor y
    int _xsize = 0;
    int _ysize = 0;
    Class** _characters = 0;
    const char* _scene = 0;
    //const char* _zone = 0;
    //int _curpos = 0;
    Class *_path_proto = 0;
    Class *_block_proto = 0;
    Class *addClassToScene(Class *arg, int pos, int newpos, Scene::AddClassAction action, Class *clone);
    Class *getUpThingFrom(Class *arg, Scene::AddClassAction action, Class *clone);
    Class *getDownThingFrom(Class *arg, Scene::AddClassAction action, Class *clone);
    Class *getLeftThingFrom(Class *arg, Scene::AddClassAction action, Class *clone);
    Class *getRightThingFrom(Class *arg, Scene::AddClassAction action, Class *clone);
    Class *buildScene(Class *player, Class *path_proto, Class *wall_proto, int is_scene_already = 0);
    Class *buildPath(Class *path_proto, Class *block_proto, int is_scene_already = 0);
    void clearClasses(Class *arg);
    Class *closest(Class *arg, byte farest = 0, byte is_block = 0, byte is_not_block = 0);
};
