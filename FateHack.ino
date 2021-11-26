#include "Logo.h"

#include "Players.h"
#include "Scenes.h"

#include "Class.h"
#include "Scene.h"
#include "Player.h"
#include "Coordinate.h"

#include "Messages.h"

//ARDUBOY_NO_USB

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

static Class* pcur = 0; //pointer
static Class* target = 0; //target
static Class* use = 0; //use
static Class* on = 0; //on
static Class* scene = 0; //map
static Class* player = 0; //you
static Class* wall = 0; //wall
static Class* xcur = 0; //cursor x
static Class* ycur = 0; //cursor y
static Class* maxp = 0; //max path length
static Class* path = 0; //pathfinding distance

byte scene_num = 255;
byte day = 1;
byte hour = 1;
byte minute = 1;
unsigned long turn = 1;
byte age = 14;
byte left = 0;
byte up = 0;
byte make_choice = 0;
byte isBotMode = 1;

byte population_stock = 10;
byte pick_stock = 10;
byte ore_stock = 0;
byte ore_need = 10;
byte food_stock = 100;

const byte max_scene_num = 254;
const char space_tile = ' ';
const char wall_tile = '=';

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
  IsBot,
  Logo,
  MenuMIN = Wait,
  MenuMAX = Destroy
};

size_t currentState = State::Logo;
size_t currentSelection = State::MenuMIN;
size_t freeMem = 0;
size_t death_reason = 0;

void PrintMessage(Class* src = 0, size_t num = 0, Class* trg = 0) {
  for (int i = 0; i < 500; i++) {
    Class::arduboy.clear();
    Class::exemplar.setCursor(0, 0);
    if (src)
      src->atGet(Class::Directive::Draw);
    else
      Class::exemplar.print(asFlashStringHelper(enScene));
    Class::exemplar.println();
    Class::exemplar.println(readFlashStringPointer(&enMessages[num]));
    if (trg)
      Class::exemplar.print(asFlashStringHelper(trg->toStr()));
    Class::arduboy.display();
  }
}

void Rip(Class* c, size_t reason = 0) {
  if (c == player) {
    currentState = State::Dead;
    death_reason = reason;
    Class::arduboy.clear();
    Class::exemplar.setCursor(0, 0);
    c->atGet(Class::Directive::Draw);
    Class::exemplar.println(readFlashStringPointer(&enMessages[0]));
    Class::exemplar.println(readFlashStringPointer(&enMessages[5]));
    Class::exemplar.print(readFlashStringPointer(&enMessages[6]));
    Class::exemplar.println(readFlashStringPointer(&enMessages[0]));
    Class::exemplar.print(readFlashStringPointer(&enMessages[reason]));
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

void NextScene(int portal, byte make_blocks = 0) {
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
    is_predefined = RebuildScene(scene_minetown);
  } else if (scene_num == 255) {
    is_predefined = RebuildScene(scene_home);
    PrintMessage(player, 13);
  } else {
    RebuildScene(scene_clear);
  }

  Class * asc = 0;
  Class * desc = 0;

  scene->atPut(Class::Directive::Greater, maxp); //or equal min path
  scene->atPut(Class::Directive::Less, maxp); //or equal max path

  if (is_predefined == 0) {
    if (scene_num != max_scene_num) {
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
      scene->atPut(Class::Directive::Greater, maxp); //or equal min path
      scene->atPut(Class::Directive::Less, maxp); //or equal max path
      //PrintMessage(0, 8, player);
    }
    while ((scene->atPut(Class::Directive::Build, 0)) == 0); //generate scene

    if (scene_num != max_scene_num) {
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

  scene->atPut(Class::Directive::Clear, path);
  scene->atPut(Class::Directive::Map, desc); //set pathfinding map to downstairs

  if (make_blocks && (is_predefined == 0)) {
    int r = random((max_scene_num - scene_num) / 100);
    if (r == 0) {
      scene->atPut(Class::Directive::Block, Class::exemplar.make(waterp));
      scene->atGet(Class::Directive::Place);
    }
  }

  scene->atPut(Class::Directive::Clear, path);
  scene->atPut(Class::Directive::Block, wall); //setting blocks for a scene
  scene->atGet(Class::Directive::Cursor);
  scene->atPut(Class::Directive::Search, player);
  SaveCursor();
}

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
  if (s != scene_clear)
    s++;
  do {
    do {
      ch = (char) (pgm_read_byte_near(s + ((y * 20) + x)));
      if ((ch == space_tile) || (s == scene_clear)) {
      } else if (ch == pgm_read_byte_near((player->_init) + 1)) {
        scene->atPut(Class::Directive::Character, player);
        is_predefined = 1;
      } else if (ch == wall_tile) {
        if ((scene->atGet(Class::Directive::Block)) == 0) {
          scene->atPut(Class::Directive::Block, wall);
        }
        scene->atPut(Class::Directive::Character, (scene->atGet(Class::Directive::Block)));
      } else {
        for (unsigned int i = 0; i < size(players); i++) {
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

#include "Scripts.h"

void setup() {
  //while (!Serial) {}
  Class::arduboy.boot();
  Class::arduboy.display();
  Class::arduboy.flashlight();
  Class::arduboy.setFrameRate(15);
  Class::arduboy.initRandomSeed();

  player = Class::exemplar.make(plr);
  player->atPut(Class::Directive::Place, player);

  pcur = player;
  pcur = (pcur->atPut(Class::Directive::Next, Class::exemplar.make(life)));
  //pcur = (pcur->atPut(Class::Directive::Next, Class::exemplar.make(mnd)));
  pcur = (pcur->atPut(Class::Directive::Next, Class::exemplar.make(waterp)));
  //  pcur = (pcur->atPut(Class::Directive::Next, Class::exemplar.make(filter)));

  pcur = (pcur->atPut(Class::Directive::Next, Class::exemplar.make(pet)));
  pcur->atPut(Class::Directive::Place, pcur);
  pcur->atPut(Class::Directive::Hidden, pcur);
  pcur = (pcur->atPut(Class::Directive::Next, Class::exemplar.make(dog)));
  pcur = (pcur->atPut(Class::Directive::Next, Class::exemplar.make(collar)));

  pcur = 0;

  //player->atPut(Class::Directive::Add, Class::exemplar.make(drowsy)); //debug drowsy
  //player->atPut(Class::Directive::Add, Class::exemplar.make(sleep)); //debug sleep
  //player->atPut(Class::Directive::Add, Class::exemplar.make(thirst)); //debug thirst
  //player->atPut(Class::Directive::Add, Class::exemplar.make(hunger)); //debug hunger

  wall = Class::exemplar.make(wl);
  wall->atPut(Class::Directive::Place, wall);

  path = Class::exemplar.make("C1");
  maxp = Class::exemplar.make("C8"); //TODO if <7 then bug with max_path_length = 0. Don't set < 7!

  NextScene(1);
}

Class * ShowInfo(Class * c, byte is_select = 0) { //you don't need to understand this
  Class::arduboy.clear();
  Class::exemplar.setCursor(0, 0);
  Class::exemplar.print(F("(DOWN:NEXT"));
  if (is_select) {
    Class::exemplar.print(F(",A:SELECT"));
  } else if (player == c) {
    Class::exemplar.print(F(",A:OWNER"));
  }
  Class::exemplar.println(F(")"));

  if ((pcur == 0) && (target == 0)) {
    pcur = c; // owner of target aspect
    target = (pcur->atGet(Class::Directive::Next)); // target aspect
  }
  Class::exemplar.print(F("\xEE "));
  Class::exemplar.println(asFlashStringHelper(pcur->toStr()));

  Class::exemplar.print(F("\x19 "));
  if (target && ((target->atGet(Class::Directive::Place)) == 0)) {
    if (target->atGet(Class::Directive::Block)) {
      Class::exemplar.print(F("("));
      Class::exemplar.print(asFlashStringHelper(enBind));
      Class::exemplar.print(F(")"));
      Class::exemplar.print(asFlashStringHelper(enSpace));
    }
    Class::exemplar.println(asFlashStringHelper(target->toStr()));
  } else if (target) {
    Class::exemplar.println(asFlashStringHelper(target->toStr()));
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
      PrintMessage(a, 8, target);
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
  Class::exemplar.setCursor(0, 0);

  scene->atPut(Class::Directive::Map, player);
  scene->atPut(Class::Directive::Draw, player);
  freeMem = Class::getFreeMemory();
  if (freeMem) {
    if (freeMem < 256)
      Class::exemplar.print(F("LOW"));
    else
      Class::exemplar.print(freeMem);
    Class::exemplar.print(readFlashStringPointer(&enMessages[0]));
  }
  scene->atPut(Class::Directive::Clear, path);

  Class::exemplar.print(age);
  Class::exemplar.print(F("Y"));
  Class::exemplar.print(day);
  Class::exemplar.print(F("D"));
  Class::exemplar.print(readFlashStringPointer(&enMessages[0]));
  Class::exemplar.print(hour);
  Class::exemplar.print(F(":"));
  Class::exemplar.print(minute);
  Class::exemplar.print(readFlashStringPointer(&enMessages[0]));
  Class::exemplar.print(scene_num);
  Class::exemplar.println();

  switch (currentState) {
    case State::Logo:
      Class::arduboy.clear();
      Class::sprites.drawSelfMasked(0, 0, glove, 0);
      Class::sprites.drawSelfMasked(80, 20, fate, 0);
      Class::sprites.drawSelfMasked(90, 30, hack, 0);
      Class::arduboy.display();
      if (Class::arduboy.justPressed(B_BUTTON) || Class::arduboy.justPressed(A_BUTTON)) {
        currentState = State::Intro;
      }
      break;
    case State::Bookkeeper:
      Class::exemplar.setCursor(0, 0);
      Class::exemplar.println(F("BOOKKEEPER:"));
      Class::exemplar.print(population_stock);
      Class::exemplar.print(readFlashStringPointer(&enMessages[0]));
      Class::exemplar.println(F("MAN(1 FOOD EACH)"));
      Class::exemplar.print(food_stock);
      Class::exemplar.print(readFlashStringPointer(&enMessages[0]));
      Class::exemplar.println(F("FOOD | A : TAKE"));
      Class::exemplar.print(ore_stock);
      Class::exemplar.print(readFlashStringPointer(&enMessages[0]));
      Class::exemplar.print(F("COAL | NEED"));
      Class::exemplar.print(readFlashStringPointer(&enMessages[0]));
      Class::exemplar.println(ore_need);
      Class::exemplar.print(pick_stock);
      Class::exemplar.print(readFlashStringPointer(&enMessages[0]));
      Class::exemplar.print(F("PICKS | AUTOPICKUP"));
      Class::exemplar.println(readFlashStringPointer(&enMessages[0]));
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
      Class::exemplar.setCursor(0, 0);
      Class::exemplar.print(readFlashStringPointer(&enIntro[make_choice]));
      Class::arduboy.display();
      if (Class::arduboy.justPressed(B_BUTTON) || Class::arduboy.justPressed(A_BUTTON)) {
        make_choice++;
      }
      if (make_choice == size(enIntro)) {
        make_choice = 0;
#ifndef DEMO_VERSION
        currentState = State::IsBot;
#endif
#ifdef DEMO_VERSION
        currentState = State::OtherTurn;
#endif
      }
      break;
#ifndef DEMO_VERSION
    case State::IsBot:
      Class::arduboy.clear();
      Class::exemplar.setCursor(0, 0);
      Class::exemplar.print(asFlashStringHelper(enBot));
      Class::arduboy.display();
      if (Class::arduboy.justPressed(B_BUTTON)) {
        isBotMode = 0;
        make_choice = 0;
        currentState = State::OtherTurn;
      } else if (Class::arduboy.justPressed(A_BUTTON)) {
        make_choice = 0;
        currentState = State::OtherTurn;
      }
      break;
#endif
    case State::Dead:
      Rip(player, death_reason);
      Class::arduboy.display();
      break;
    case State::UseOn:
      {
        if (on == 0 && make_choice == 0) {
          Class * c = (scene->atGet(Class::Directive::Character));
          if (c && ((c->atGet(Class::Directive::Hidden)) == 0)) {
            on = ShowInfo(c, 1);
          }
          if ((c == 0) || (on != 0)) {
            make_choice = 1;
          }
        } else if (use == 0) {
          use = ShowInfo(player, 1);
        }
        if (use && make_choice)
        {
          if (on && (use != player)) { //use
            (*scripts[use->toInt()]) (use, player, scene, on);
          } else if (on) { //take
            player->atPut(Class::Directive::Add, on);
            scene->atPut(Class::Directive::Character, 0);
            PrintMessage(player, 8, on);
          } else { //drop
            if ((use->atGet(Class::Directive::Block)) == 0) {
              PrintMessage(player, 9, use);
              player->atPut(Class::Directive::Delete, use);
              scene->atPut(Class::Directive::Character, use);
            } else {
              PrintMessage(use, 11);
            }
          }
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
        Class::arduboy.clear();
        Class::exemplar.setCursor(0, 0);
        for (unsigned int i = 0; i < size(enMenuItems); i++) {
          if (currentSelection == i) {
            Class::exemplar.print('\x1A');
          } else {
            Class::exemplar.print(' ');
          }
          Class::exemplar.println(readFlashStringPointer(&enMenuItems[i]));
        }
        Class::exemplar.println(asFlashStringHelper(enNote1));
        Class::exemplar.println(asFlashStringHelper(enNote2));
        Class::exemplar.println(asFlashStringHelper(enNote3));
        Class::exemplar.println(asFlashStringHelper(enNote4));

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
        //Class * c = (scene->atGet(Class::Directive::Character));
        Class * c = player;
        if (c && ((c->atGet(Class::Directive::Hidden)) == 0) && (c != (scene->atGet(Class::Directive::Block)))) {
          //ShowInfo(scene->atGet(Class::Directive::Character));
          ShowInfo(player);
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
          Class *owner = pcur;
          Class *scene_owner = (scene->atPut(Class::Directive::Owner, pcur));
          if (scene_owner == 0)
            scene_owner = owner;

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

          if (is_next_scene == 0)
            is_next_scene = (*scripts[owner->toInt()]) (owner, scene_owner, scene, target);

          if (owner && (owner == player)) {
            if (player->atGet(Class::Directive::Turn)) {
              currentState = State::Turn;
              RestoreCursor();
            } else {
              EndTurn(player);
            }
          }

          scene->atPut(Class::Directive::Clear, path);

          if (owner && (is_next_scene == 0))
            owner->atPut(Class::Directive::Turn, 0);
        }
        pcur = 0;
      }
      break;
  }
}
