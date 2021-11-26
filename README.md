# FateHack
<!--## Warning! This version uses “remove USB stack” technique. When you upload a new game, you need to hold down the DOWN button while ARDUBOY is starting up.->
<!--### <a href="https://felipemanga.github.io/ProjectABE/?url=https://raw.githubusercontent.com/Molochnikov/FateHack/main/FateHack.ino.leonardo.hex&skin=BareFit">Play it in your browser using ProjectABE!</a>-->

Roguelike game and probably tiniest roguelike library for [Arduboy](https://arduboy.com/) or [its emulator](https://github.com/felipemanga/ProjectABE/releases/latest) influenced by NetHack and WildTangent's FATE.

![screen](/screen.png)

## Bottom panel:
* memory left in bytes `556` (if `LOW` then you better start destroying most useless objects)
* current year `14Y`
* current day `1D`
* current hour `1:`
* current minute `:1`
* current negative elevation `0`

## Controls:
* <kbd>Up</kbd> , <kbd>Down</kbd> , <kbd>Left</kbd> , <kbd>Right</kbd> - move cursor
* <kbd>A</kbd> or <kbd>Ctrl</kbd> + <kbd>Up</kbd> , <kbd>Down</kbd> , <kbd>Left</kbd> , <kbd>Right</kbd> - move player
* <kbd>A</kbd> or <kbd>Ctrl</kbd> - select object for action or give object to the next potential owner in your possessions
* <kbd>B</kbd> or <kbd>Alt</kbd> - toggle menu at cursor position or cancel action
* to take something 'on this use player'
* to drop something 'on floor `.` use this'

## Game features:
* bot
* cursor
* pet
* toilet
* thirst
* real-world logic
* communist economy
* no stupid HP, MP, EXP, SP, LVL or other lazy math abstractions of reality complexity
* priority of things relations over the soulless loot
* simplicity in randomness, no need to know game formulas and equations
* YASD is hard to do (except with destroy feature)

## Programming features for using as library for other Arduboy roguelikes:
* pathfinding algorithm for generating caves
* scene builder
* object builder
* dynamic memory allocation for every object (yes, you can run out of memory)
* full power of parametric polymorphism
* master OOP pattern at the core - prototype (exemplar)
* easy scripting for objects interactions

## How to build:

## Roguelike library documentation:
Library uses dynamic memory allocation and prototype polymorphism principles. There is no memory for smart pointers implementation so don't forget to `delete` objects when you don't need them anymore.
### Class
Base class and interface for all objects. Is must be used everywhere. Inherit all your new classes from Class.
#### Class::exemplar
Object maker instance with Print custom tiles functionality.

`virtual Class *make(const char* s); //always create all your objects with this method. DONT FORGET TO destroy THEM AFTER! First char is the object type.`

`static int hasMoreMemory() { //returns free memory size or 0 if the size is under 300`

`static void printDebug(char* c); //use this for debuging purposes with itoa`

`static void setCursor(int8_t x, int8_t y); //Arduboy2 library analog with custom tiles`

`static void setTextColor(const uint8_t color); //Arduboy2 library analog with custom tiles`

`static void setTextBackground(const uint8_t color); //Arduboy2 library analog with custom tiles`

#### Class::arduboy
#### Class::sprites
### Screen
Map on the screen. Dynamically allocated array of pointers to Class.
## How to register your classes as prototypes:
Use a macro:
`#define REGISTER_PROTOTYPE(CLASS) static CLASS CLASS##Instance = CLASS(Exemplar())`
Then register it:
`REGISTER_PROTOTYPE(YourSubClass);`