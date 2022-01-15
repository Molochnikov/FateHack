#pragma once

/*
   P - player mark
   @ - player symbol
   B/b - block/not block
   19 - script number
   name - name and description
*/

const char plr[] PROGMEM = "P@b7PLAYER";
const char descend[] PROGMEM = "P>B3DESCEND";
const char ascend[] PROGMEM = "P<B4ASCEND";
const char wl[] PROGMEM = "P B0WALL";
const char door[] PROGMEM = "P+b0DOOR";
//const char water[] PROGMEM = "P~b0RAW WATER";
const char life[] PROGMEM = "P B2LIFE\nLIVING THING.\nLIFE CAUSES HUNGER,\nTHIRST, SLEEP, WASTE.\nYOU CAN LOST IT.";
//const char mnd[] PROGMEM = "PmB0MIND//\nINDIVIDUAL THAT \nFEELS, PERCEIVES, \nWILLS, AND ESPECIALLY \nREASONS AND THINKS.";
const char hunger[] PROGMEM = "P B0HUNGER";
const char thirst[] PROGMEM = "P B0THIRST";
const char drowsy[] PROGMEM = "P B12DROWSY";
const char toilet[] PROGMEM = "P B0TOILET";
//const char soil[] PROGMEM = "P,B0SOIL";
const char pet[] PROGMEM = "Pfb1FRIEND";
const char dog[] PROGMEM = "PdB0UNDEAD DOG";
const char collar[] PROGMEM = "Pcb0COLLAR";
const char outside[] PROGMEM = "P9B5OUTSIDE";
const char house1[] PROGMEM = "P1B14HOUSE";
const char house2[] PROGMEM = "P2B8BOOKKEEPER";
//const char mnr[] PROGMEM = "PmB0MINER";
//const char npc[] PROGMEM = "Pnb0NPC";
const char adventurer[] PROGMEM = "PaB7MINER";
//const char oil[] PROGMEM = "P~B0oil";
//const char blood[] PROGMEM = "P~B0blood";
//const char oilp[] PROGMEM = "P!B0black";
//const char bloodp[] PROGMEM = "P!B0red";
//const char inkp[] PROGMEM = "P!B0blue ink";
//const char milkp[] PROGMEM = "P!B0milk";
//const char greenp[] PROGMEM = "P!B0green";
//const char purplep[] PROGMEM = "P!B0purple";
//const char orangep[] PROGMEM = "P!B0orange";
//const char brownp[] PROGMEM = "P!B0brown";
//const char pinkp[] PROGMEM = "P!B0pink";
//const char grayp[] PROGMEM = "P!B0gray";
//const char cyanp[] PROGMEM = "P!B0cyan";
const char waterp[] PROGMEM = "P~b11WATER";
//const char rawwaterp[] PROGMEM = "P!B0RAW WATER";
//const char bottle[] PROGMEM = "P!B0BOTTLE";
const char pick[] PROGMEM = "P/B13PICK";
const char food[] PROGMEM = "P%B10FOOD";
//const char filter[] PROGMEM = "Pfb0FILTER";
const char sleep[] PROGMEM = "PsB9SLEEP";
const char vein[] PROGMEM = "P B0VEIN";
const char partition[] PROGMEM = "P#b0PARTITION";
//const char table[] PROGMEM = "Pob0TABLE";
//const char chair[] PROGMEM = "Phb0CHAIR";
//const char bed[] PROGMEM = "Pbb0BED";

//const char * const grounds[] PROGMEM = {
//  soil
//};

//const char * const waters[] PROGMEM = {
//  water//, oil
//};

//const char * const potions[] PROGMEM = {
//  boozep, oilp, bloodp, inkp, milkp, greenp, purplep, orangep, brownp, pinkp, grayp, cyanp
//};

const char * const players[] PROGMEM = {
  //  table,
  //  chair,
  //  bed,
  waterp,
  partition,
  plr,
  descend,
  ascend,
  wl,
  door,
  //  water,
  vein,
  life,
  //  mnd,
  thirst,
  drowsy,
  toilet,
  //  soil,
  pet,
  dog,
  collar,
  outside,
  //  mnr,
  adventurer,
  //  npc,
  house1,
  house2
};
