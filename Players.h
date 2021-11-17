#pragma once

/*
   P - player mark
   @ - player symbol
   B/b - block/not block
   19 - script number
   name - name and description
*/

const char plr[] PROGMEM = "P@B0player";
const char descend[] PROGMEM = "P>B3descend";
const char ascend[] PROGMEM = "P<B4ascend";
const char wl[] PROGMEM = "P B0wall";
const char pt[] PROGMEM = "P%B0pit";
const char dor[] PROGMEM = "P+B0door";
const char water[] PROGMEM = "P~B0raw water";
const char vn[] PROGMEM = "P*B0vein";
const char life[] PROGMEM = "P B2life\nLiving thing. \nLife causes hunger, \nthirst, sleep, waste, \nillness and breathe. \nYou can lost it.";
const char mnd[] PROGMEM = "P B0mind\nIndividual that \nfeels, perceives, \nwills, and especially \nreasons and thinks.";
const char hunger[] PROGMEM = "PhB0hunger";
const char thirst[] PROGMEM = "PtB0thirst";
const char drowsy[] PROGMEM = "PdB12drowsy";
const char toilet[] PROGMEM = "PtB7toilet";
const char sand[] PROGMEM = "P.B0sand";
const char soil[] PROGMEM = "P,B0soil";
const char farm[] PROGMEM = "P=B0farm";
const char gold[] PROGMEM = "P$b0gold";
const char pet[] PROGMEM = "Ppb1friend";
const char threat[] PROGMEM = "PTB0threat";
const char boss[] PROGMEM = "PBB0boss";
const char filler[] PROGMEM = "PFB0filler";
const char hitter[] PROGMEM = "PHB0hitter";
const char dog[] PROGMEM = "PdB0undead dog";
const char collar[] PROGMEM = "P b0collar";
const char outside[] PROGMEM = "P9B5outside";
const char house1[] PROGMEM = "P1B0house";
const char house2[] PROGMEM = "P2B8bookkeeper";
const char house3[] PROGMEM = "P3B0house";
const char house4[] PROGMEM = "P4B0house";
const char house5[] PROGMEM = "P5B0house";
const char house6[] PROGMEM = "P6B0house";
const char house7[] PROGMEM = "P7B0house";
const char house8[] PROGMEM = "P8B0house";
const char mnr[] PROGMEM = "PcB0miner";
const char npc[] PROGMEM = "Pnb0minor NPC";
const char adventurer[] PROGMEM = "PtB6adventurer";
const char boozep[] PROGMEM = "P!B8booze";
const char oil[] PROGMEM = "P~B0oil";
const char blood[] PROGMEM = "P~B0blood";
const char oilp[] PROGMEM = "P!B0black";
const char bloodp[] PROGMEM = "P!B0red";
const char inkp[] PROGMEM = "P!B0blue ink";
const char milkp[] PROGMEM = "P!B0milk";
const char greenp[] PROGMEM = "P!B0green";
const char purplep[] PROGMEM = "P!B0purple";
const char orangep[] PROGMEM = "P!B0orange";
const char brownp[] PROGMEM = "P!B0brown";
const char pinkp[] PROGMEM = "P!B0pink";
const char grayp[] PROGMEM = "P!B0gray";
const char cyanp[] PROGMEM = "P!B0cyan";
const char waterp[] PROGMEM = "P!B11water";
const char rawwaterp[] PROGMEM = "P!B11raw water";
const char bottle[] PROGMEM = "P!B0empty bottle";
const char pick[] PROGMEM = "PTB0pick";
const char food[] PROGMEM = "P*B10food";
const char filter[] PROGMEM = "PfB0coal filter";
const char sleep[] PROGMEM = "PZB9sleep";

const char * const grounds[] PROGMEM = {
  soil, sand
};

const char * const waters[] PROGMEM = {
  water, blood, oil
};

const char * const potions[] PROGMEM = {
  boozep, oilp, bloodp, inkp, milkp, greenp, purplep, orangep, brownp, pinkp, grayp, cyanp
};

const char * const players[] PROGMEM = {
  plr,
  descend,
  ascend,
  wl,
  pt,
  dor,
  water,
  vn,
  life,
  mnd,
  hunger,
  thirst,
  drowsy,
  toilet,
  soil,
  farm,
  gold,
  pet,
  threat,
  boss,
  filler,
  hitter,
  dog,
  collar,
  outside,
  mnr,
  adventurer,
  npc,
  house1,
  house2,
  house3,
  house4,
  house5,
  house6,
  house7,
  house8
};
