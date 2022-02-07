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
const char exi[] PROGMEM = "PrB14EXIT";
const char door[] PROGMEM = "P+b0DOOR";
const char life[] PROGMEM = "P B2LIFE\nLIVING THING.\nLIFE CAUSES HUNGER,\nTHIRST, SLEEP, WASTE.\nYOU CAN LOST IT.";
const char hunger[] PROGMEM = "P B0HUNGER";
const char thirst[] PROGMEM = "P B0THIRST";
const char drowsy[] PROGMEM = "P B12DROWSY";
const char toilet[] PROGMEM = "Psb0TOILET";
const char pet[] PROGMEM = "Pfb1FRIEND";
const char dog[] PROGMEM = "PdB0UNDEAD DOG";
const char collar[] PROGMEM = "Pcb0COLLAR";
const char outside[] PROGMEM = "PoB0OUTSIDE";
const char house1[] PROGMEM = "PhB14HOUSE";
const char house2[] PROGMEM = "PbB8BOOKKEEPER";
const char adventurer[] PROGMEM = "PaB7MINER";
const char waterp[] PROGMEM = "P~B11WATER";
const char pick[] PROGMEM = "P/B13PICK";
const char food[] PROGMEM = "P%B10FOOD";
const char sleep[] PROGMEM = "PsB9SLEEP";
const char vein[] PROGMEM = "P B0VEIN";
const char partition[] PROGMEM = "P#b0PARTITION";

const char * const players[] PROGMEM = {
  waterp,
  partition,
  plr,
  descend,
  ascend,
  wl,
  door,
  exi,
  vein,
  life,
  thirst,
  drowsy,
  toilet,
  pet,
  dog,
  collar,
  outside,
  adventurer,
  house1,
  house2
};
