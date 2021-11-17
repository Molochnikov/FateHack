#include "Class.h"

Arduboy2 Class::arduboy;
Class *Class::list = 0;
Class Class::exemplar = Class(Exemplar());
char Class::errFlag = 0;

#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__

int Class::getFreeMemory() {
  char top;
#ifdef __arm__
  return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  return &top - __brkval;
#else  // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}

Class::Class(Exemplar) {
  next = list;
  list = this;
}

Class *Class::make(const char* s) {
  Class *retval = 0;
  for (Class *a = list; a; a = a->next) {
    if (a != (&Class::exemplar)) {
      if ((retval = a->make(s))) {
        retval->_init = s;
        return retval;
      }
    }
  }
  return 0;
}

Class::Class() {
  next = 0;
  Class::errFlag = 0;
}

Class::~Class() {

}

void Class::die( void ) {
  set_sleep_mode( SLEEP_MODE_PWR_DOWN );
  while (1) {
    sleep_enable();
    cli();
    sleep_cpu();
  }
}

Class *Class::atPut(Directive, Class *) {
  return 0;
}

Class *Class::atGet(Directive) {
  return 0;
}

Class *Class::clone() const {
  return Class::exemplar.make(_init);
}

char* Class::toStr() {
  return 0;
}

//byte Class::DropDice(byte d = 2, byte n = 1) {
//  int sum = 0;
//  for (int i = 0; i < n; i++) {
//    sum += random(d);
//  }
//  return sum;
//}

byte Class::getBits(byte x, byte p, byte n) {
  return (x >> (p + 1 - n)) & (~((~0U) << n));
}

byte Class::setBits(byte x, byte pos_new_bits, byte new_bits, byte num_new_bits) {
  return (x & (~(((byte)(1 << num_new_bits) - 1) << (pos_new_bits + 1 - num_new_bits)))) | (new_bits << (pos_new_bits + 1 - num_new_bits));
}

int Class::toInt() {
  return 0;
}

char Class::getTypeChar() {
  return 0;
}
