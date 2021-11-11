# FateHack
Roguelike game for [Arduboy](https://arduboy.com/) or [its emulator](https://github.com/felipemanga/ProjectABE/releases/latest) influenced by NetHack and WildTangent's FATE.

![screen](/screen.png)

Bottom panel from left to right:
* memory left (if `LOW` then you better start destroying objects)
* current year `14Y`
* current day `1D`
* current hour `1:`
* current minute `:1`
* current negative elevation

Game features:
* pet
* toilet
* thirst
* real-world logic
* communist economy
* no stupid HP, MP, EXP, SP, LVL or other lazy math abstractions of reality complexity
* priority of things relations over the soulless loot
* simplicity in randomness, no need to know game formulas and equations
* YASD is hard to do (except with destroy feature)

Programming features for using as engine for other Arduboy roguelikes:
* pathfinding algorithm for generating caves
* scene builder
* object builder
* dynamic memory allocation for every object (yes, you can run out of memory)
* full power of parametric polymorphism
* master design pattern at the core - prototype (exemplar)
* easy scripting for objects interactions

Controls:
* <kbd>Up</kbd> , <kbd>Down</kbd> , <kbd>Left</kbd> , <kbd>Right</kbd> - move cursor
* <kbd>A</kbd> or <kbd>Ctrl</kbd> + <kbd>Up</kbd> , <kbd>Down</kbd> , <kbd>Left</kbd> , <kbd>Right</kbd> - move player
* <kbd>B</kbd> or <kbd>Alt</kbd> - toggle menu at cursor position
