#include "Players.h"
#include "Scenes.h"

#include "Class.h"
#include "Scene.h"
#include "Player.h"
#include "Coordinate.h"

#include "Messages.h"

template<typename Type, size_t arraySize> constexpr size_t size(const Type (&)[arraySize]) noexcept
{
  return arraySize;
}

template<typename T> T * pgm_read_pointer(T * const * pointer) noexcept
{
  return reinterpret_cast<T *>(pgm_read_ptr(pointer));
}

using FlashStringHelper = const __FlashStringHelper *;

inline FlashStringHelper asFlashStringHelper(const char * flashString) noexcept
{
  return static_cast<FlashStringHelper>(static_cast<const void *>(flashString));
}

inline FlashStringHelper readFlashStringPointer(const char * const * flashString) noexcept
{
  return static_cast<FlashStringHelper>(pgm_read_ptr(flashString));
}

#define REGISTER_PROTOTYPE(CLASS) static CLASS CLASS##Instance = CLASS(Exemplar())

REGISTER_PROTOTYPE(Scene);
REGISTER_PROTOTYPE(Player);
REGISTER_PROTOTYPE(Coordinate);

static Class* target = 0; //target
static Class* use = 0; //use
static Class* on = 0; //on
static Class* scene = 0; //map
static Class* player = 0; //you
static Class* wall = 0; //wall
static Class* xcur = 0; //cursor x
static Class* ycur = 0; //cursor y
static Class* path = 0; //pathfinding distance
static Class* pcur = 0; //current turn player

byte scene_num = 255;
byte day = 1;
byte hour = 1;
byte minute = 1;
unsigned long turn = 1;
byte age = 14;
byte left = 0;
byte up = 0;
byte make_choice = 0;

byte population_stock = 10;
byte pick_stock = 10;
byte ore_stock = 0;
byte ore_need = 10;
byte food_stock = 100;

enum State {
  Wait = 0,
  Info,
  UseOn,
  Destroy,
  Menu,
  Turn,
  OtherTurn,
  Dead,
  Intro,
  Bookkeeper,
  MenuMIN = Wait,
  MenuMAX = Destroy
};

size_t currentState = State::Intro;
size_t currentSelection = State::MenuMIN;
size_t freeMem = 0;
size_t death_reason = 0;

void exit( int ignored ) {
  cli();
  while (1);
}

void PrintDebug(FlashStringHelper msg) {
  for (int i = 0; i < 200; i++) {
    Class::arduboy.clear();
    Class::arduboy.setCursor(0, 0);
    Class::arduboy.println(msg);
    Class::arduboy.display();
  }
}

void PrintMessage(Class* src = 0, size_t num = 0, Class* trg = 0) {
  for (int i = 0; i < 500; i++) {
    Class::arduboy.clear();
    Class::arduboy.setCursor(0, 0);
    if (src)
      src->atGet(Class::Directive::Draw);
    else
      Class::arduboy.print(F("scene"));
    Class::arduboy.println();
    Class::arduboy.println(readFlashStringPointer(&enMessages[num]));
    if (trg)
      Class::arduboy.print(asFlashStringHelper(trg->toStr()));
    Class::arduboy.display();
  }
}

void Rip(Class* c, size_t reason = 0) {
  if (c == player) {
    currentState = State::Dead;
    death_reason = reason;
    Class::arduboy.clear();
    Class::arduboy.setCursor(0, 0);
    c->atGet(Class::Directive::Draw);
    Class::arduboy.println(readFlashStringPointer(&enMessages[5]));
    Class::arduboy.print(readFlashStringPointer(&enMessages[6]));
    Class::arduboy.print(readFlashStringPointer(&enMessages[reason]));
    Class::arduboy.display();
  } else {
    PrintMessage(c, 5);
  }
}

void ExchangeOreStock() {
  byte incr_need = 1;
  byte min_need = 10;
  byte pick_rate = 3;
  if (ore_stock >= ore_need) {
    food_stock = food_stock + ore_need;
    pick_stock = pick_stock + (ore_need / pick_rate);
    ore_need = ore_need + random(incr_need);
    ore_stock = ore_stock - ore_need;
  } else {
    food_stock = food_stock + ore_stock;
    pick_stock = pick_stock + ((ore_stock / ore_need) * (ore_need / pick_rate));
    ore_need = ore_need - incr_need;
    if (ore_need < min_need)
      ore_need = ore_need + random(incr_need);
    ore_stock = 0;
  }
}

void SpendFoodStock() {
  if (food_stock < population_stock) {
    population_stock = food_stock + 1;
    food_stock = 0;
  } else {
    food_stock = food_stock - population_stock;
  }
}

void EndTurn(Class* pl) {
  pl->atPut(Class::Directive::Turn, 0);
  currentState = State::OtherTurn;
  turn++;
  minute = turn % 10;
  if (minute == 0) {
    hour++;
  }
  if (hour == 10) {
    hour = 0;
    day++;
  }
  if (day == 10) {
    day = 0;
    age++;
    ExchangeOreStock();
    SpendFoodStock();
  }
}

void SaveCursor() {
  left = 0;
  up = 0;
  while (scene->atGet(Class::Directive::Left))
    left++;
  while (scene->atGet(Class::Directive::Up))
    up++;
}

void RestoreCursor() {
  while (scene->atGet(Class::Directive::Left)) {};
  while (scene->atGet(Class::Directive::Up)) {};
  while (left) {
    scene->atGet(Class::Directive::Right);
    left--;
  }
  while (up) {
    scene->atGet(Class::Directive::Down);
    up--;
  }
}

Class * SetCursor(char * c) {
  Class * cls = 0;
  while (scene->atGet(Class::Directive::Left)) {};
  while (scene->atGet(Class::Directive::Up)) {};
  do {
    do {
      cls = scene->atGet(Class::Directive::Character);
      while (cls) {
        if ((cls->_init) == c)
          return (scene->atGet(Class::Directive::Character));
        cls = (cls->atGet(Class::Directive::Next));
      }
    } while (scene->atGet(Class::Directive::Right));
    while (scene->atGet(Class::Directive::Left)) {};
  } while (scene->atGet(Class::Directive::Down));
  return 0;
}

//void DropPlayer(Class* player, Class* owner, Class* scene) {

//}

void NextScene(int portal, byte make_blocks = 0, byte make_soil = 1) {
  int block_chance = 2;
  int soil_chance = 10;
  int num = 4;
  int is_down = 0;
  byte is_predefined = 0;

  if (portal == -1) {
    scene_num--;
  } else if (portal == 1) {
    scene_num++;
    is_down = 1;
  }

  if (scene)
    scene->atPut(Class::Directive::Next, player);
  if (scene_num == 0) {
    is_predefined = RebuildScene(scene_latium_mines4);
  } else {
    is_predefined = RebuildScene(clear_scene);
  }


  //  if (desc == 0)
  //    desc = Class::exemplar.make(descend);
  //  if (asc == 0)
  //    asc = Class::exemplar.make(ascend);
  Class * asc = 0;
  Class * desc = 0;

  if (is_predefined == 0) {
    if (scene_num != 255) {
      desc = Class::exemplar.make(descend);
      desc->atPut(Class::Directive::Place, desc);
    }
    asc = Class::exemplar.make(ascend);
    asc->atPut(Class::Directive::Place, asc);
    asc->atPut(Class::Directive::Next, Class::exemplar.make(outside));

    do {
      scene->atPut(Class::Directive::Next, player); //clear the scene and go next scene
      scene->atPut(Class::Directive::Path, path); //setting paths for a scene
      scene->atPut(Class::Directive::Block, wall); //setting blocks for a scene
    }
    while ((scene->atPut(Class::Directive::Build, 0)) == 0); //generate scene

    if (scene_num != 255) {
      if (is_down) {
        scene->atPut(Class::Directive::Far, desc); //setting downstairs
        scene->atPut(Class::Directive::Clear, path);
        scene->atPut(Class::Directive::Map, desc); //set pathfinding map to downstairs
      } else {
        scene->atPut(Class::Directive::Far, asc); //setting downstairs
        scene->atPut(Class::Directive::Clear, path);
        scene->atPut(Class::Directive::Map, asc); //set pathfinding map to downstairs
      }
    }

    if (is_down) {
      scene->atPut(Class::Directive::Far, asc);  //setting upstairs far from downstairs
      scene->atPut(Class::Directive::Clear, path);
      scene->atPut(Class::Directive::Map, asc); //set pathfinding map to upstairs
    } else {
      scene->atPut(Class::Directive::Far, desc);  //setting upstairs far from downstairs
      scene->atPut(Class::Directive::Clear, path);
      scene->atPut(Class::Directive::Map, desc); //set pathfinding map to upstairs
    }

    scene->atPut(Class::Directive::Close, player);
  } else {
    scene->atPut(Class::Directive::Path, path); //setting paths for a scene
    scene->atPut(Class::Directive::Map, player);
  }

  pcur = (player->atGet(Class::Directive::Next));
  while (pcur) {
    if (pcur->atGet(Class::Directive::Place))
      scene->atPut(Class::Directive::Close, pcur);
    pcur = (pcur->atGet(Class::Directive::Next));
  }
  pcur = 0;

  //scene->atPut(Class::Directive::Clear, path);
  //scene->atPut(Class::Directive::Map, desc); //set pathfinding map to downstairs

  //  if (make_blocks) {
  //    int blocks_num = random(num);
  //    for (int i = 0; i < blocks_num; i++) { //choose random block
  //      if (random(block_chance) == 0)
  //        scene->atPut(Class::Directive::Block, pit);
  //      block_chance++;
  //      if (random(block_chance) == 0)
  //        scene->atPut(Class::Directive::Block, door);
  //      block_chance++;
  //      if (random(block_chance) == 0)
  //        scene->atPut(Class::Directive::Block, water);
  //      scene->atGet(Class::Directive::Block); //set chosen block close to downstairs
  //    }
  //  }
  //  if (make_soil && (random(soil_chance) == 0)) {
  //    scene->atPut(Class::Directive::Place, Class::exemplar.make(sl));
  //    scene->atGet(Class::Directive::Block); //set chosen block close to downstairs
  //  }

  scene->atPut(Class::Directive::Clear, path);
  //scene->atPut(Class::Directive::Block, wall); //setting blocks for a scene
  SetCursor(plr);
  SaveCursor();
}

byte (*scripts[]) (Class* cls, Class* owner, Class* scene, Class* target_of) = {
  [](Class * cls, Class * owner, Class * scene, Class * target_of) -> byte { //0
    return 0;
  },
  [](Class * cls, Class * owner, Class * scene, Class * target_of) -> byte { //1
    scene->atPut(Class::Directive::Clear, path);
    scene->atPut(Class::Directive::Map, owner);
    scene->atPut(Class::Directive::Move, cls);
    return 0;
  },
  [](Class * cls, Class * owner, Class * scene, Class * target_of) -> byte { //2
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
    } else if ((hour == 0) && (minute == 0) && ((owner->atPut(Class::Directive::Character, drws)))) {
      owner->atPut(Class::Directive::Add, sleep);
      PrintMessage(owner, 8, sleep);
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
  [](Class * cls, Class * owner, Class * scene, Class * target_of) -> byte { //3
    if (target_of == player) {
      NextScene(1, 1);
      return 1;
    } else if (target_of) { //dissapear to next scene
      Rip(target_of);
      scene->atPut(Class::Directive::Delete, target_of);
    }
    return 0;
  },
  [](Class * cls, Class * owner, Class * scene, Class * target_of) -> byte { //4
    if (target_of == player) {
      NextScene(-1, 1);
      return 1;
    } else if (target_of) { //dissapear to next scene
      Rip(target_of);
      scene->atPut(Class::Directive::Delete, target_of);
    }
    return 0;
  },
  [](Class * cls, Class * owner, Class * scene, Class * target_of) -> byte { //5
    if (scene_num == 255) {
      return 0;
    }
    int r = random(scene_num + 5);
    scene->atPut(Class::Directive::Clear, path);  //clear pathes from scene
    if ((r == 0) && (SetCursor(adventurer) == 0) && SetCursor(outside) && ((scene->atPut(Class::Directive::Near, 0)) == 0)) {  //if scene without adventurer and near space is not blocked
      Class * c  = Class::exemplar.make(npc); //create npc
      PrintMessage(0, 8, c);
      scene->atPut(Class::Directive::Map, owner); //create paths to this
      scene->atPut(Class::Directive::Close, c); //set npc close to this
      c->atPut(Class::Directive::Hidden, c); //reveal npc
      c = (c->atPut(Class::Directive::Next, Class::exemplar.make(adventurer))); //npc is adventurer
      c = (c->atPut(Class::Directive::Next, Class::exemplar.make(mnr))); //adventurer has class (miner)
      c = (c->atPut(Class::Directive::Next, Class::exemplar.make(life))); //alive
    }
    return 0;
  },
  [](Class * cls, Class * owner, Class * scene, Class * target_of) -> byte { //6
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
    if (target_of) {
      for (int i = 0; i < size(waters); i++) { //find target in waters
        if ((target_of->_init) == waters[i]) { //found
          scene->atPut(Class::Directive::Cursor, owner); //set cursor on owner
          if ((scene->atPut(Class::Directive::Near, target_of)) == target_of) { //if near target
            PrintMessage(owner, 0); //print message
            owner->atPut(Class::Directive::Delete, cls); //delete this
            Rip(cls); //print message
          } else {
            PrintMessage(owner, 6, target_of);
          }
        }
      }
      for (int i = 0; i < size(potions); i++) { //find target in potions
        if ((target_of->_init) == potions[i]) { //found
          if ((scene->atPut(Class::Directive::Owner, target_of)) == owner) { //if owner of target
            (*scripts[target_of->toInt()]) (target_of, owner, scene, cls); //run potion script
            owner->atPut(Class::Directive::Delete, cls); //delete this
            Rip(cls); //print message
          } else {
            PrintMessage(owner, 7, target_of);
          }
        }
      }
    }
    return 0;
  },
  [](Class * cls, Class * owner, Class * scene, Class * target_of) -> byte { //8
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
  [](Class * cls, Class * owner, Class * scene, Class * target_of) -> byte { //9
    return 0;
  },
  [](Class * cls, Class * owner, Class * scene, Class * target_of) -> byte { //10
    if (target_of) {
      Class* hngr = Class::exemplar.make(hunger); //create
      Class* a = target_of->atPut(Class::Directive::Character, hngr); //find
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
    return 0;
  },
};

byte RebuildScene(const char* s) {
  char ch = 0;
  byte is_predefined = 0;
  Class* w = 0;
  if (scene)
    delete scene;
  if (xcur)
    delete xcur;
  if (ycur)
    delete ycur;
  xcur = Class::exemplar.make("C20");
  ycur = Class::exemplar.make("C7");
  scene = Class::exemplar.make(s);
  scene->atPut(Class::Directive::X, xcur);
  scene->atPut(Class::Directive::Y, ycur);
  scene->atGet(Class::Directive::Build);
  while (scene->atGet(Class::Directive::Left)) {};
  while (scene->atGet(Class::Directive::Up)) {};
  int x = xcur->toInt();
  int y = ycur->toInt();
  s++;
  do {
    do {
      ch = (char) (pgm_read_byte_near(s + ((y * 20) + x)));
      if (ch == ' ') {
      } else if (ch == pgm_read_byte_near((player->_init) + 1)) {
        scene->atPut(Class::Directive::Character, player);
        is_predefined = 1;
      } else if (ch == '#') {
        if ((scene->atGet(Class::Directive::Block)) == 0) {
          scene->atPut(Class::Directive::Block, wall);
        }
        scene->atPut(Class::Directive::Character, (scene->atGet(Class::Directive::Block)));
      } else {
        for (int i = 0; i < size(players); i++) {
          if (ch == pgm_read_byte_near(pgm_read_word(&(players[i])) + 1)) {
            w = Class::exemplar.make(pgm_read_word(&(players[i])));
            w->atPut(Class::Directive::Hidden, w);
            scene->atPut(Class::Directive::Character, w);
          }
        }
      }
      x++;
    } while (scene->atGet(Class::Directive::Right));
    while (scene->atGet(Class::Directive::Left)) {};
    x = xcur->toInt();
    y++;
  } while (scene->atGet(Class::Directive::Down));
  return is_predefined;
}

void setup() {
  //while (!Serial) {}
  Class::arduboy.boot();
  Class::arduboy.display();
  Class::arduboy.flashlight();
  Class::arduboy.setFrameRate(15);
  Class::arduboy.initRandomSeed();

  player = Class::exemplar.make(plr);
  player->atPut(Class::Directive::Place, player);
  //player->atPut(Class::Directive::Block, player);
  pcur = player;
  pcur = (pcur->atPut(Class::Directive::Next, Class::exemplar.make(life)));
  pcur->atPut(Class::Directive::Block, pcur);
  pcur = (pcur->atPut(Class::Directive::Next, Class::exemplar.make(mnd)));
  pcur->atPut(Class::Directive::Block, pcur);
  pcur = (pcur->atPut(Class::Directive::Next, Class::exemplar.make(waterp)));
  pcur = (pcur->atPut(Class::Directive::Next, Class::exemplar.make(filter)));

  pcur = (pcur->atPut(Class::Directive::Next, Class::exemplar.make(pet)));
  pcur->atPut(Class::Directive::Place, pcur);
  pcur->atPut(Class::Directive::Hidden, pcur);
  pcur = (pcur->atPut(Class::Directive::Next, Class::exemplar.make(dog)));
  pcur->atPut(Class::Directive::Block, pcur);
  pcur = (pcur->atPut(Class::Directive::Next, Class::exemplar.make(collar)));

  //soil = Class::exemplar.make(sl);

  wall = Class::exemplar.make(wl);
  //wall->atPut(Class::Directive::Block, wall);
  wall->atPut(Class::Directive::Place, wall);

  //pit = Class::exemplar.make(pt);

  //water = Class::exemplar.make(wtr);

  //door = Class::exemplar.make(dor);

  //  delete xcur;
  //  delete ycur;
  //  xcur = Class::exemplar.make("C10");
  //  ycur = Class::exemplar.make("C3");
  //  scene->atPut(Class::Directive::X, xcur);
  //  scene->atPut(Class::Directive::Y, ycur);

  path = Class::exemplar.make("C1");

  pcur = 0;
  //desc = Class::exemplar.make(descend);
  NextScene(1, 1);
}

Class * ShowInfo(Class * c, byte is_select = 0) { //you don't need to understand this
  Class::arduboy.clear();
  Class::arduboy.setCursor(0, 0);
  Class::arduboy.print(F("[DOWN-next"));
  if (is_select) {
    Class::arduboy.print(F(",A-select"));
  } else if (player == c) {
    Class::arduboy.print(F(",A-owner"));
  }
  Class::arduboy.println(F("]"));

  if ((pcur == 0) && (target == 0)) {
    pcur = c; // owner of target aspect
    target = (pcur->atGet(Class::Directive::Next)); // target aspect
  }
  Class::arduboy.print('\x1f');
  Class::arduboy.println(asFlashStringHelper(pcur->toStr()));

  Class::arduboy.print('\x17');
  if (target && ((target->atGet(Class::Directive::Place)) == 0)) {
    if (target->atGet(Class::Directive::Block))
      Class::arduboy.print(F("(bind) "));
    Class::arduboy.println(asFlashStringHelper(target->toStr()));
  } else if (target) {
    Class::arduboy.println(asFlashStringHelper(target->toStr()));
  }

  if (Class::arduboy.justPressed(DOWN_BUTTON)) {
    if (target) { //more than one aspect
      if (target->atGet(Class::Directive::Place)) {
        pcur = target;
        target = (pcur->atGet(Class::Directive::Next));
        if (target == 0) {
          target = c;
        }
      } else {
        target = (target->atGet(Class::Directive::Next));
        if (target == 0) {
          target = c;
        }
      }
    }
  }
  if (Class::arduboy.justPressed(A_BUTTON)) {
    if (is_select) {
      Class * ret = 0;
      if (target)
        ret = target;
      else
        ret = pcur;
      pcur = 0;
      target = 0;
      return ret;
    } else if ((player == c) && target && ((target->atGet(Class::Directive::Block)) == 0)) {
      Class * a = (pcur->atGet(Class::Directive::Next));
      pcur->atPut(Class::Directive::Delete, target);
      while (a && ((a->atGet(Class::Directive::Place)) == 0))
        a = (a->atGet(Class::Directive::Next));
      if (a == 0) {
        a = (scene->atPut(Class::Directive::Owner, pcur));
      }
      a->atPut(Class::Directive::Add, target);
      pcur = 0;
      target = 0;
      currentState = State::Menu;
    }
  }
  if (Class::arduboy.justPressed(B_BUTTON)) {
    pcur = 0;
    target = 0;
    use = 0;
    on = 0;
    make_choice = 0;
    currentState = State::Menu;
  }
  return 0;
}

void loop() {
  if (!(Class::arduboy.nextFrame()))
    return;
  Class::arduboy.pollButtons();
  Class::arduboy.clear();

  scene->atPut(Class::Directive::Map, player);
  scene->atPut(Class::Directive::Draw, player);
  if (freeMem < 300)
    Class::arduboy.print(F("LOW"));
  else
    Class::arduboy.print(freeMem);
  scene->atPut(Class::Directive::Clear, path);
  Class::arduboy.print(readFlashStringPointer(&enMessages[0]));
  Class::arduboy.print(age);
  Class::arduboy.print(F("Y"));
  Class::arduboy.print(day);
  Class::arduboy.print(F("D"));
  Class::arduboy.print(readFlashStringPointer(&enMessages[0]));
  Class::arduboy.print(hour);
  Class::arduboy.print(F(":"));
  Class::arduboy.print(minute);
  Class::arduboy.print(readFlashStringPointer(&enMessages[0]));
  Class::arduboy.print(scene_num);
  Class::arduboy.println();

  switch (currentState) {
    case State::Bookkeeper:
      Class::arduboy.setCursor(0, 0);
      Class::arduboy.println(F("Bookkeeper:"));
      Class::arduboy.print(population_stock);
      Class::arduboy.print(readFlashStringPointer(&enMessages[0]));
      Class::arduboy.println(F("man(1 food each)"));
      Class::arduboy.print(food_stock);
      Class::arduboy.print(readFlashStringPointer(&enMessages[0]));
      Class::arduboy.println(F("food | A - take"));
      Class::arduboy.print(ore_stock);
      Class::arduboy.print(readFlashStringPointer(&enMessages[0]));
      Class::arduboy.print(F("coal | need"));
      Class::arduboy.print(readFlashStringPointer(&enMessages[0]));
      Class::arduboy.println(ore_need);
      Class::arduboy.print(pick_stock);
      Class::arduboy.print(readFlashStringPointer(&enMessages[0]));
      Class::arduboy.print(F("picks | autopickup"));
      Class::arduboy.println(readFlashStringPointer(&enMessages[0]));
      Class::arduboy.display();
      if (Class::arduboy.justPressed(A_BUTTON)) {
        if (food_stock > 0) {
          Class * fd = Class::exemplar.make(food);
          PrintMessage(player, 8, fd);
          player->atPut(Class::Directive::Add, fd);
          food_stock--;
        }
      } else if (Class::arduboy.justPressed(B_BUTTON)) {
        EndTurn(player);
      }
      break;
    case State::Intro:
      Class::arduboy.clear();
      Class::arduboy.print(readFlashStringPointer(&enIntro[make_choice]));
      Class::arduboy.display();
      if (Class::arduboy.justPressed(B_BUTTON) || Class::arduboy.justPressed(A_BUTTON)) {
        make_choice++;
      }
      if (make_choice == size(enIntro)) {
        make_choice = 0;
        currentState = State::OtherTurn;
      }
      break;
    case State::Dead:
      Rip(player, death_reason);
      Class::arduboy.display();
      break;
    case State::UseOn:
      {
        if (use == 0) {
          use = ShowInfo(player, 1);
        } else if (on == 0) {
          Class * c = (scene->atGet(Class::Directive::Character));
          if (c && ((c->atGet(Class::Directive::Hidden)) == 0) && (c != (scene->atGet(Class::Directive::Block)))) {
            on = ShowInfo(c, 1);
          }
          if ((c == 0) || (on != 0))
            make_choice = 1;
        }
        if (use && make_choice)
        {
          (*scripts[use->toInt()]) (use, player, scene, on);
          use = 0;
          on = 0;
          make_choice = 0;
          SaveCursor();
          EndTurn(player);
        }
      }
      Class::arduboy.display();
      break;
    case State::Destroy: //TODO destroying wall refs
      {
        if (use == 0) {
          if (scene->atGet(Class::Directive::Character))
            use = ShowInfo(scene->atGet(Class::Directive::Character), 1);
          else
            currentState = State::Menu;
        } else {
          Rip(use);
          scene->atPut(Class::Directive::Delete, use);
          use = 0;
          currentState = State::Menu;
        }
      }
      Class::arduboy.display();
      break;
    case State::Wait:
      SaveCursor();
      EndTurn(player);
      Class::arduboy.display();
      break;
    case State::Menu:
      {
        Class::arduboy.setCursor(0, 0);
        for (int i = 0; i < size(enMenuItems); i++) {
          if (currentSelection == i) {
            Class::arduboy.print('\x1A');
          } else {
            Class::arduboy.print(' ');
          }
          Class::arduboy.print(readFlashStringPointer(&enMenuItems[i]));
          Class::arduboy.println(' ');
        }
        if (Class::arduboy.justPressed(DOWN_BUTTON) && currentSelection < State::MenuMAX) {
          currentSelection++;
        }
        if (Class::arduboy.justPressed(UP_BUTTON) && currentSelection > State::MenuMIN) {
          currentSelection--;
        }
        if (Class::arduboy.justPressed(B_BUTTON)) {
          currentState = State::Turn;
        }
        if (Class::arduboy.justPressed(A_BUTTON)) {
          currentState = currentSelection;
        }
      }
      Class::arduboy.display();
      break;
    case State::Info:
      {
        Class * c = (scene->atGet(Class::Directive::Character));
        if (c && ((c->atGet(Class::Directive::Hidden)) == 0) && (c != (scene->atGet(Class::Directive::Block)))) {
          ShowInfo(scene->atGet(Class::Directive::Character));
        } else {
          currentState = State::Menu;
        }
      }
      Class::arduboy.display();
      break;
    case State::Turn:
      {
        target = 0;
        Class* c = 0;
        if (Class::arduboy.justPressed(UP_BUTTON)) {
          if (Class::arduboy.anyPressed(A_BUTTON)) {
            c = scene->atPut(Class::Directive::Up, player);
            EndTurn(player);
            SaveCursor();
          } else {
            scene->atGet(Class::Directive::Up);
          }
        }
        if (Class::arduboy.justPressed(DOWN_BUTTON)) {
          if (Class::arduboy.anyPressed(A_BUTTON)) {
            c = scene->atPut(Class::Directive::Down, player);
            EndTurn(player);
            SaveCursor();
          } else {
            scene->atGet(Class::Directive::Down);
          }
        }
        if (Class::arduboy.justPressed(LEFT_BUTTON)) {
          if (Class::arduboy.anyPressed(A_BUTTON)) {
            c = scene->atPut(Class::Directive::Left, player);
            EndTurn(player);
            SaveCursor();
          } else {
            scene->atGet(Class::Directive::Left);
          }
        }
        if (Class::arduboy.justPressed(RIGHT_BUTTON)) {
          if (Class::arduboy.anyPressed(A_BUTTON)) {
            c = scene->atPut(Class::Directive::Right, player);
            EndTurn(player);
            SaveCursor();
          } else {
            scene->atGet(Class::Directive::Right);
          }
        }
        if (Class::arduboy.justPressed(B_BUTTON)) {
          currentState = State::Menu;
        }
        if (c)
          (*scripts[c->toInt()]) (c, c, scene, player);
      }
      Class::arduboy.display();
      break;
    case State::OtherTurn:
      {
        byte is_next_scene = 0;
        pcur = scene->atGet(Class::Directive::Turn);
        if (pcur == 0) {
          scene->atPut(Class::Directive::Turn, 0);
        } else {
          if (pcur == player) {
            freeMem = Class::getFreeMemory();
            currentState = State::Turn;
            RestoreCursor();
          }
          Class *owner = pcur;
          Class *scene_owner = (scene->atPut(Class::Directive::Owner, pcur));
          if (scene_owner == 0)
            scene_owner = owner;
          is_next_scene = (*scripts[owner->toInt()]) (owner, scene_owner, scene, target);
          if (is_next_scene != 0)
            break;
          scene->atPut(Class::Directive::Clear, path);
          if (pcur)
            pcur = (pcur->atGet(Class::Directive::Next));
          while (pcur && ((pcur->atGet(Class::Directive::Place)) == 0)) {
            is_next_scene = (*scripts[pcur->toInt()]) (pcur, owner, scene, target);
            if (is_next_scene != 0)
              break;
            scene->atPut(Class::Directive::Clear, path);
            if (pcur) {
              pcur->atPut(Class::Directive::Turn, 0);
              pcur = (pcur->atGet(Class::Directive::Next));
            }
          }
          if (is_next_scene != 0)
            break;
          if (owner)
            owner->atPut(Class::Directive::Turn, 0);
        }
        pcur = 0;
      }
      break;
  }
}
