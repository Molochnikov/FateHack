# FateHack
Roguelike game for [Arduboy](https://arduboy.com/) or [its emulator](https://github.com/felipemanga/ProjectABE/releases/latest) influenced by NetHack and WildTangent's FATE.

Game features:
* pet
* toilet
* thirst
* real-world logic
* no stupid HP, MP, EXP, SP, LVL or other lazy math abstractions of reality complexity
* priority of things relations over the soulless loot
* randomness only in generating scenes, no need to know game formulas and equations

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
* to drop something `UseOn` the floor tile (<kbd>.</kbd>)
