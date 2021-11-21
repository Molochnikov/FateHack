#pragma once

#define CODE_PAGE_437

constexpr char enIntro1[] PROGMEM = "You live in communist\nmining village.\n\n1 year equal 9 days.\n\nYour task to fulfill\nannual coal mining\nplan.";
constexpr char enIntro2[] PROGMEM = "In reward you can\norder food and picks\nfrom other villages\nfor next year.\n\nGood luck!";

constexpr const char *enIntro[] PROGMEM = {
  enIntro1, enIntro2
};

constexpr char enDrop[] PROGMEM = "drop";
constexpr char enWait[] PROGMEM = "wait one minute";

#ifdef CODE_PAGE_437
constexpr char enInfo[] PROGMEM = "next owner\xEE\x02 among\xEE\x02";
#endif
#ifndef CODE_PAGE_437
constexpr char enInfo[] PROGMEM = "next owner\xEE@ among\xEE@";
#endif

#ifdef CODE_PAGE_437
constexpr char enUseOn[] PROGMEM = "show\xEE\xDB OR on\xEE\xDBuse\xEE\x02";
#endif
#ifndef CODE_PAGE_437
constexpr char enUseOn[] PROGMEM = "show\xEE\xDB OR on\xEE\xDBuse\xEE@";
#endif

constexpr char enDestroy[] PROGMEM = "destroy\xEE\xDB if LOW mem";

constexpr const char *enMenuItems[] PROGMEM = {
  enWait,
  enInfo,
  enUseOn,
  enDestroy
};

#ifdef CODE_PAGE_437
constexpr char enNote1[] PROGMEM = "\x02 you(hold A to move)";
#endif
#ifndef CODE_PAGE_437
constexpr char enNote1[] PROGMEM = "@ you(hold A to move)";
#endif

constexpr char enNote2[] PROGMEM = "\xDB - obj under cursor";
constexpr char enNote3[] PROGMEM = ". - floor (nothing)";
constexpr char enNote4[] PROGMEM = "\xEE - 'element of'";

constexpr char enSpace[] PROGMEM = " ";
constexpr char enHungry[] PROGMEM = "hungry";
constexpr char enToilet[] PROGMEM = "toilet";
constexpr char enThirst[] PROGMEM = "thirsty";
constexpr char enDrowsy[] PROGMEM = "drowsy";
constexpr char enDead[] PROGMEM = "disappeared";
constexpr char enFar[] PROGMEM = "away from";
constexpr char enOwner[] PROGMEM = "not owner";
constexpr char enGot[] PROGMEM = "got";
constexpr char enSleep[] PROGMEM = "Zzz";
constexpr char enBind[] PROGMEM = "bind";
constexpr char enScene[] PROGMEM = "scene";
constexpr char enHome[] PROGMEM = "sweet home";

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
