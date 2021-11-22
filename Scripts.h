#pragma once

#define DEMO_MODE

byte (*scripts[]) (Class* cls, Class* owner, Class* scene, Class* target_of) = { //all players scripts
  [](Class * cls, Class * owner, Class *, Class *) -> byte { //0
#ifdef DEMO_MODE
    if ((cls == player) && isBotMode) {
      Class *c = Class::exemplar.make(thirst); //create
      if (player->atPut(Class::Directive::Character, c)) { //find
        delete c;
        c = SetCursor(ascend); //find
      } else {
        delete c;
        c = SetCursor(descend); //find
      }
      if (c) {
        if ((scene->atPut(Class::Directive::Near, owner)) == owner) { //if in cursor near this
          (*scripts[c->toInt()]) (c, c, scene, player);
          owner->atPut(Class::Directive::Turn, 0);
          Class::arduboy.display();
          return 1;
        }
        scene->atPut(Class::Directive::Clear, path);
        scene->atPut(Class::Directive::Map, c);
        scene->atPut(Class::Directive::Move, owner);
      }
      owner->atPut(Class::Directive::Turn, 0);
      Class::arduboy.display();
    }
#endif
    return 0;
  },
  [](Class * cls, Class * owner, Class * scene, Class *) -> byte { //1
    scene->atPut(Class::Directive::Clear, path);
    scene->atPut(Class::Directive::Map, owner);
    scene->atPut(Class::Directive::Move, cls);
    return 0;
  },
  [](Class *, Class * owner, Class * scene, Class *) -> byte { //2
    Class * hgr = Class::exemplar.make(hunger);
    if ((hour == 5) && (minute == 0) && ((owner->atPut(Class::Directive::Character, hgr)) == 0))  {
      owner->atPut(Class::Directive::Add, hgr);
      PrintMessage(owner, 8, hgr);
    } else if ((hour == 5) && (minute == 0)  && ((owner->atPut(Class::Directive::Character, hgr))))  {
      Rip(owner, 1);
      scene->atPut(Class::Directive::Delete, owner); //dissapear
    } else {
      delete hgr;
    }
    Class * thrst = Class::exemplar.make(thirst);
    if ((hour == 3) && (minute == 0) && ((owner->atPut(Class::Directive::Character, thrst)) == 0)) {
      owner->atPut(Class::Directive::Add, thrst);
      PrintMessage(owner, 8, thrst);
    } else if ((hour == 3) && (minute == 0) && ((owner->atPut(Class::Directive::Character, thrst)))) {
      Rip(owner, 3);
      scene->atPut(Class::Directive::Delete, owner); //dissapear
    } else {
      delete thrst;
    }
    Class * drws = Class::exemplar.make(drowsy);
    if ((hour == 0) && (minute == 0) && ((owner->atPut(Class::Directive::Character, drws)) == 0)) { //new day
      owner->atPut(Class::Directive::Add, drws);
      PrintMessage(owner, 8, drws);
    } else if ((hour == 0) && (minute == 0) && (owner->atPut(Class::Directive::Character, drws))) {
      scene->atPut(Class::Directive::Delete, (owner->atPut(Class::Directive::Character, drws))); //if has then delete
      Rip(drws);
      Class * slp = Class::exemplar.make(sleep);
      owner->atPut(Class::Directive::Add, slp);
      PrintMessage(owner, 8, slp);
    } else {
      delete drws;
    }
    Class * tlt = Class::exemplar.make(toilet);
    if ((hour == 4) && (minute == 0) && ((owner->atPut(Class::Directive::Character, tlt)) == 0)) {
      owner->atPut(Class::Directive::Add, tlt);
      PrintMessage(owner, 8, tlt);
    } else if ((hour == 4) && (minute == 0) && ((owner->atPut(Class::Directive::Character, tlt)))) {
      Rip(owner, 2);
      scene->atPut(Class::Directive::Delete, owner); //dissapear
    } else {
      delete tlt;
    }
    return 0;
  },
  [](Class *, Class *, Class * scene, Class * target_of) -> byte { //3
    if (target_of == player) {
      NextScene(1, 1);
      return 1;
    } else if (target_of) { //dissapear to next scene
      Rip(target_of);
      scene->atPut(Class::Directive::Delete, target_of);
    }
    return 0;
  },
  [](Class *, Class *, Class * scene, Class * target_of) -> byte { //4
    if (target_of == player) {
      NextScene(-1, 1);
      return 1;
    } else if (target_of) { //dissapear to next scene
      Rip(target_of);
      scene->atPut(Class::Directive::Delete, target_of);
    }
    return 0;
  },
  [](Class *, Class * owner, Class * scene, Class *) -> byte { //5
    if (scene_num == 255) {
      return 0;
    }
    int r = random(scene_num + 5) + 1;
    scene->atPut(Class::Directive::Clear, path);  //clear pathes from scene
    if ((r == 0) && (SetCursor(adventurer) == 0) && SetCursor(outside) && ((scene->atPut(Class::Directive::Near, 0)) == 0)) {  //if scene without adventurer and near space is not blocked
      Class * c  = Class::exemplar.make(adventurer); //create npc
      PrintMessage(0, 8, c);
      scene->atPut(Class::Directive::Map, owner); //create paths to this
      scene->atPut(Class::Directive::Close, c); //set npc close to this
      c->atPut(Class::Directive::Hidden, c); //reveal npc
      //c = (c->atPut(Class::Directive::Next, Class::exemplar.make(adventurer))); //npc is adventurer
      //c = (c->atPut(Class::Directive::Next, Class::exemplar.make(mnr))); //adventurer has class (miner)
      c = (c->atPut(Class::Directive::Next, Class::exemplar.make(life))); //alive
    }
    return 0;
  },
  [](Class *, Class * owner, Class * scene, Class *) -> byte { //6
    Class * c = SetCursor(descend); //find descend in scene
    if (c) {
      if ((scene->atPut(Class::Directive::Near, owner)) == owner) { //if descend in cursor near this
        (*scripts[c->toInt()]) (c, c, scene, owner);
        //Rip(owner);
        //scene->atPut(Class::Directive::Delete, owner); //dissapear to next scene
        return 2;
      }
      scene->atPut(Class::Directive::Clear, path);
      scene->atPut(Class::Directive::Map, c);
      scene->atPut(Class::Directive::Move, owner);
    }
    return 0;
  },
  [](Class * cls, Class * owner, Class * scene, Class * target_of) -> byte { //7
    //    if (target_of) {
    //      for (unsigned int i = 0; i < size(waters); i++) { //find target in waters
    //        if ((target_of->_init) == waters[i]) { //found
    //          scene->atPut(Class::Directive::Cursor, owner); //set cursor on owner
    //          if ((scene->atPut(Class::Directive::Near, target_of)) == target_of) { //if near target
    //            PrintMessage(owner, 0); //print message
    //            owner->atPut(Class::Directive::Delete, cls); //delete this
    //            Rip(cls); //print message
    //          } else {
    //            PrintMessage(owner, 6, target_of);
    //          }
    //        }
    //      }
    //      for (unsigned int i = 0; i < size(potions); i++) { //find target in potions
    //        if ((target_of->_init) == potions[i]) { //found
    //          if ((scene->atPut(Class::Directive::Owner, target_of)) == owner) { //if owner of target
    //            (*scripts[target_of->toInt()]) (target_of, owner, scene, cls); //run potion script
    //            owner->atPut(Class::Directive::Delete, cls); //delete this
    //            Rip(cls); //print message
    //          } else {
    //            PrintMessage(owner, 7, target_of);
    //          }
    //        }
    //      }
    //    }
    return 0;
  },
  [](Class *, Class *, Class *, Class * target_of) -> byte { //8
    if (target_of) {
      Class * pck = Class::exemplar.make(pick);
      if ((target_of->atPut(Class::Directive::Character, pck)) == 0) {
        PrintMessage(target_of, 8, pck);
        target_of->atPut(Class::Directive::Add, pck);
      } else {
        delete pck;
      }
      if (target_of == player) {
        currentState = State::Bookkeeper;
      }
    }
    return 0;
  },
  [](Class * cls, Class * owner, Class *, Class *) -> byte { //9
    if (owner->atGet(Class::Directive::Count)) { //sleep active
      PrintMessage(owner, 10);
      owner->atPut(Class::Directive::Turn, 0); //no turn for owner
      Class *c = (owner->atGet(Class::Directive::Next)); //next class
      while (c && ((c->atGet(Class::Directive::Place)) == 0)) {
        c->atPut(Class::Directive::Turn, 0); //no turn for class
        c = (c->atGet(Class::Directive::Next));
      }
    } else {
      Rip(cls);
      scene->atPut(Class::Directive::Delete, cls); //delete this
      pcur = 0;
    }
    return 0;
  },
  [](Class * cls, Class *, Class * scene, Class * target_of) -> byte { //10
    if (target_of) {
      Class* hngr = Class::exemplar.make(hunger); //create
      Class* a = (target_of->atPut(Class::Directive::Character, hngr)); //find
      delete hngr; //free object
      if (a) {
        Rip(a);
        scene->atPut(Class::Directive::Delete, a); //if has then delete
        Rip(cls); //print message
        scene->atPut(Class::Directive::Delete, cls); //delete this
      }
    }
    return 0;
  },
  [](Class * cls, Class * owner, Class * scene, Class * target_of) -> byte { //11
    if (target_of) {
      Class* thrst = Class::exemplar.make(thirst); //create
      Class* a = target_of->atPut(Class::Directive::Character, thrst); //find
      delete thrst; //free object
      if (a) {
        Rip(a);
        scene->atPut(Class::Directive::Delete, a); //if has then delete
        Rip(cls); //print message
        scene->atPut(Class::Directive::Delete, cls); //delete this
        a = Class::exemplar.make(bottle);
        PrintMessage(owner, 8, a);
        owner->atPut(Class::Directive::Add, a);
      }
    }
    return 0;
  },
  [](Class * cls, Class * owner, Class * scene, Class * target_of) -> byte { //12
    if (target_of && (owner == player)) {
      scene->atPut(Class::Directive::Delete, cls); // delete this
      Rip(cls);
      Class * slp = Class::exemplar.make(sleep);
      owner->atPut(Class::Directive::Add, slp);
    }
    return 0;
  },
  [](Class * cls, Class * owner, Class * scene, Class * target_of) -> byte { //13
    if (target_of) {
    }
    return 0;
  },
  [](Class * cls, Class * owner, Class * scene, Class * target_of) -> byte { //14
    if (target_of == player) {
      if (scene_num == 0)
        NextScene(-1);
      else
        NextScene(1);
      return 1;
    }
    return 0;
  }
};
