#pragma once

#include "Class.h"
#include <Arduino.h>

class Player : public Class {
  public:
    Player(Exemplar a);
    Player(const char* s);
    ~Player();
    Class *atPut(Directive key, Class *arg);
    Class *atGet(Directive key);
    Class *make(const char* s);
    const char* toStr();
    const int toInt();
    const char getTypeChar();
    virtual Class *clone() const;
  private:
    //void (*_script) (Class* cls, Class* owner, Class* scene) = 0;
    //byte _script = 0; //?

    Class * _next = 0;
    //static Player *_exemplar;
    //byte _has_to_be_placed = 0;
    //byte _is_hidden = 1;
    //byte _has_turn = 1;
    //byte _is_block = 0;
    byte _bit_mask = 0;
    /*7 6 5 4 3 2 1 0
     *+-+ + + | | | +- is hidden bit
     *  | | | | | +--- has turn bit
     *  | | | | +----- has to be placed bit 
     *  | | | +------- is block/bind bit
     *  | | +--------- armor bonus bit
     *  + +----------- weapon bonus bit
     */           
    static const char _typeChar = 'P';

    //char _symbol = ' ';
    //byte _countdown = 0;
    //byte _symbol = 0;
    //unsigned char Fight = 0;
    //unsigned char Will = 0;
    //unsigned char Physique = 0;
    //unsigned char Provoke = 0;

    //unsigned char FatePoints = 0 ;
    //unsigned char NextSceneFatePoints = 0;

    //unsigned char PhysicalStress = 0;
    //unsigned char PhysicalStressMax = 0;

    //unsigned char MentalStress = 0;
    //unsigned char MentalStressMax = 0;

    //const char* _name = 0; //?

    //const char* MildConsequence = 0;
    //const char* ModerateConsequence = 0;
    //const char* SevereConsequence = 0;

    //const char* HighConceptAspect = 0;
    //const char* TroubleAspect = 0;
    //const char* RelationshipAspect = 0;
    //const char* FreeAspect1 = 0;
    //const char* FreeAspect2 = 0;

};
