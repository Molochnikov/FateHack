#pragma once

//#define CODE_PAGE_437

constexpr char enBot[] PROGMEM = "DEMO MODE? AY/BN";

constexpr char enIntro1[] PROGMEM = "YOU LIVE IN COMMUNIST\nMINING VILLAGE.\n\n1 YEAR EQUAL 9 DAYS.\n\nYOUR TASK TO FULFILL\nANNUAL ORE MINING\nPLAN.";
constexpr char enIntro2[] PROGMEM = "IN REWARD YOU CAN\nORDER FOOD AND PICKS\nFROM OTHER VILLAGES\nFOR NEXT YEAR.\n\nGOOD LUCK.";

constexpr const char *enIntro[] PROGMEM = {
  enIntro1, enIntro2
};

constexpr char enDrop[] PROGMEM = "DROP";
constexpr char enWait[] PROGMEM = "WAIT ONE TURN";

#ifdef CODE_PAGE_437
constexpr char enInfo[] PROGMEM = "NEXT OWNER\xEE\x02 AMONG\xEE\x02";
#endif
#ifndef CODE_PAGE_437
constexpr char enInfo[] PROGMEM = "NEXT OWNER\xEE@ AMONG\xEE@";
#endif

#ifdef CODE_PAGE_437
constexpr char enUseOn[] PROGMEM = "ON\xEE\xDBUSE\xEE\x02 OR SHOW\xEE\xDB";
#endif
#ifndef CODE_PAGE_437
constexpr char enUseOn[] PROGMEM = "ON\xEE\xDBUSE\xEE@ OR SHOW\xEE\xDB";
#endif

constexpr char enDestroy[] PROGMEM = "DESTROY\xEE\xDB IF LOW MEM";

constexpr const char *enMenuItems[] PROGMEM = {
  enWait,
  enInfo,
  enUseOn,
  enDestroy
};

#ifdef CODE_PAGE_437
constexpr char enNote1[] PROGMEM = "\x02 YOU(HOLD A TO MOVE)";
#endif
#ifndef CODE_PAGE_437
constexpr char enNote1[] PROGMEM = "@ YOU(HOLD A TO MOVE)";
#endif

constexpr char enNote2[] PROGMEM = "\xDB : OBJ UNDER CURSOR";
constexpr char enNote3[] PROGMEM = ". : FLOOR (NOTHING)";
constexpr char enNote4[] PROGMEM = "\xEE : 'ELEMENT OF'";

constexpr char enSpace[] PROGMEM = " ";
constexpr char enHungry[] PROGMEM = "HUNGRY";
constexpr char enToilet[] PROGMEM = "TOILET";
constexpr char enThirst[] PROGMEM = "THIRSTY";
constexpr char enDrowsy[] PROGMEM = "DROWSY";
constexpr char enDead[] PROGMEM = "DISAPPEARED";
constexpr char enFar[] PROGMEM = "AWAY FROM";
constexpr char enOwner[] PROGMEM = "NOT OWNER";
constexpr char enGot[] PROGMEM = "GOT";
constexpr char enSleep[] PROGMEM = "ZZZ";
constexpr char enBind[] PROGMEM = "BIND";
constexpr char enScene[] PROGMEM = "SCENE";
constexpr char enHome[] PROGMEM = "HOME";

constexpr const char *enMessages[] PROGMEM = {
  enSpace,//0
  enHungry,//1
  enToilet,//2
  enThirst,//3
  enDrowsy,//4
  enDead,//5
  enFar,//6
  enOwner,//7
  enGot,//8
  enDrop,//9
  enSleep,//10
  enBind,//11
  enScene,//12
  enHome//13
};
