#pragma once

constexpr char enDrop[] PROGMEM = "drop";
constexpr char enWait[] PROGMEM = "wait";
constexpr char enInfo[] PROGMEM = "info";
constexpr char enUseOn[] PROGMEM = "use on";

constexpr const char *enMenuItems[] PROGMEM = {
  enWait,
  enInfo,
  enUseOn
};

constexpr char enPissedWater[] PROGMEM = " pissed in waters ";
constexpr char enSpace[] PROGMEM = " ";
constexpr char enHungry[] PROGMEM = " hungry ";
constexpr char enToilet[] PROGMEM = " toilet ";
constexpr char enThirst[] PROGMEM = " thirsty ";
constexpr char enDrowsy[] PROGMEM = " drowsy ";
constexpr char enDead[] PROGMEM = " disappeared ";
constexpr char enFar[] PROGMEM = " away from ";
constexpr char enOwner[] PROGMEM = " not owner ";
constexpr char enGot[] PROGMEM = " got ";

constexpr const char *enMessages[] PROGMEM = {
  enSpace,//0
  enHungry,//1
  enToilet,//2
  enThirst,//3
  enDrowsy,//4
  enDead,//5
  enFar,//6
  enOwner,//7
  enGot//8
};
