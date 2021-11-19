#pragma once

constexpr char enIntro1[] PROGMEM = "You live in communist\nmining village.\n\n1 year equal 9 days.\n\nYour task to fulfill\nannual coal mining\nplan.";
constexpr char enIntro2[] PROGMEM = "In reward you can\norder food and picks\nfrom other villages\nfor next year.\n\nGood luck!";

constexpr const char *enIntro[] PROGMEM = {
  enIntro1, enIntro2
};

constexpr char enDrop[] PROGMEM = "drop";
constexpr char enWait[] PROGMEM = "wait one minute";
//constexpr char enInfo[] PROGMEM = "info";
constexpr char enUseOn[] PROGMEM = "show\xEE\xDB OR on\xEE\xDBuse\xEE\x02";
constexpr char enDestroy[] PROGMEM = "destroy\xEE\xDB if LOW mem";

constexpr const char *enMenuItems[] PROGMEM = {
  enWait,
//  enInfo,
  enUseOn,
  enDestroy
};

constexpr char enPissedWater[] PROGMEM = "pissed in waters";
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
  enBind//11
};
