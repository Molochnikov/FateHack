#pragma once

#include "Class.h"

class Scene : public Class {
    friend Class *Class::make(const char* s);
  public:
    Class *atPut(Directive key, Class *arg);
    Class *atGet(Directive key);
    char* toStr();
    int toInt();
    char getTypeChar();
    Scene(Exemplar a);
    Scene(const char *s);
    ~Scene();
  private:
    enum AddClassAction {
      NoAction, CloneDestroy, CloneIfCoordinate, MoveIfEmpty, MoveDestroy, CopyIfEmpty, CloneIfEmpty
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
    Class *closest(Class *arg, byte farest = 0, byte is_block = 0);
    void recursiveDeleteClass(Class* arg);
    byte checkPath(Class *cls, byte min_path);
    void updatePath(Class* cls, byte* min_path, byte *new_min_path, byte *where, byte *is_rand,byte *dir);
    byte isPath(byte count_created, byte count_visited, byte max_instances, byte chance, byte is_scene_already);
    Class* _max_path_proto = 0;
    Class* _min_path_proto = 0;
    int _saved_x = 0; //saved cursor x
    int _saved_y = 0; //saved cursor y
};
