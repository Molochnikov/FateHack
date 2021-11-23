# FateHack
## Warning! Don't load this sketch in your Arduboy until you know how to upload sketches after **ARDUBOY_NO_USB**!!!
<a href="https://felipemanga.github.io/ProjectABE/?url=https://raw.githubusercontent.com/Molochnikov/FateHack/main/FateHack.ino.leonardo.hex&skin=BareFit">Play it in your browser using ProjectABE!</a>

Roguelike game and probably tiniest roguelike engine for [Arduboy](https://arduboy.com/) or [its emulator](https://github.com/felipemanga/ProjectABE/releases/latest) influenced by NetHack and WildTangent's FATE.

![screen](/screen.png)

Bottom panel:
* memory left in bytes `556` (if `LOW` then you better start destroying most useless objects)
* current year `14Y`
* current day `1D`
* current hour `1:`
* current minute `:1`
* current negative elevation `0`

Controls:
* <kbd>Up</kbd> , <kbd>Down</kbd> , <kbd>Left</kbd> , <kbd>Right</kbd> - move cursor
* <kbd>A</kbd> or <kbd>Ctrl</kbd> + <kbd>Up</kbd> , <kbd>Down</kbd> , <kbd>Left</kbd> , <kbd>Right</kbd> - move player
* <kbd>A</kbd> or <kbd>Ctrl</kbd> - select object for action or give object to the next potential owner in your possessions
* <kbd>B</kbd> or <kbd>Alt</kbd> - toggle menu at cursor position or cancel action
* to take something 'on this use player'
* to drop something 'on floor `.` use this'

Game features:
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

Programming features for using as engine for other Arduboy roguelikes:
* pathfinding algorithm for generating caves
* scene builder
* object builder
* dynamic memory allocation for every object (yes, you can run out of memory)
* full power of parametric polymorphism
* master OOP pattern at the core - prototype (exemplar)
* easy scripting for objects interactions

