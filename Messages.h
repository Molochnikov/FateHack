#pragma once

constexpr char enDrop[] PROGMEM = "drop";
constexpr char enWait[] PROGMEM = "wait";
constexpr char enInfo[] PROGMEM = "info";
constexpr char enForceInto[] PROGMEM = "force into";
constexpr char enUseOn[] PROGMEM = "use on";
constexpr char enMix[] PROGMEM = "mix";
constexpr char enOvercome[] PROGMEM = "overcome";
constexpr char enAttack[] PROGMEM = "attack";
constexpr char enCreateAnAdvantage[] PROGMEM = "create an advantage";

constexpr const char *enMenuItems[] PROGMEM = {
  enWait,
  enInfo,
//  enForceInto,
  enUseOn
//  enMix
};

constexpr char enPissedWater[] PROGMEM = " pissed in waters ";
constexpr char enHungry[] PROGMEM = " hungry ";
constexpr char enToilet[] PROGMEM = " needs toilet ";
constexpr char enThirst[] PROGMEM = " thirsty ";
constexpr char enDrowsy[] PROGMEM = " feeling drowsy ";
constexpr char enDead[] PROGMEM = " disappeared ";
constexpr char enFar[] PROGMEM = " away from ";
constexpr char enOwner[] PROGMEM = " not owner ";

constexpr const char *enMessages[] PROGMEM = {
  enPissedWater,//0
  enHungry,//1
  enToilet,//2
  enThirst,//3
  enDrowsy,//4
  enDead,//5
  enFar,//6
  enOwner//7
};
