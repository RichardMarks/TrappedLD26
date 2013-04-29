Trapped LD26
A Ludum Dare 26 Compo Entry by Richard Marks (ccpsceo@gmail.com)
© Copyright 2013, Richard Marks, Bang Bang Attack Studios
www.ccpssolutions.com | www.bangbangattackstudios.com
====

### Objective

Reach the edge of the screen to escape the traps.

### Summary

In Trapped LD26, you must attempt to reach the edge of the screen.
There are laser beams (red lines) that are blocking your way.
If you touch a laser beam, you die.
There are switches (green boxes) that you must walk on to change
the direction that the lasers are pointing. Careful, some switches
control multiple lasers, and you may get fried!

### Controls

 + WASD, Arrow Keys to move player
 + ESC to quit
 + F4 to save
 + F5 to load
 + R to retry the current level

### LD26 Rules Compliance

 + Developed by a single person (Richard Marks)
 + All code and content developed within the 48 hours
 + Based on theme: Minimalism through use of minimalistic graphics and mechanics
 + Developed using publicly available technology: C++ and SDL
 + Source included: See trappedld26source.7z

### What? Only Five Levels?

If you are inclined, you can make your own levels. Here is how.

Open levels.txt in a text editor and change the 5 to however many levels you will have.
Now, create level-number-.txt in the levels folder, where -number- is the number of the level such as level6.txt

Now, open the level-number-.txt file in a text editor and use the information below to create your level.

### The Level Format

Each line starts with a command character 

 + l (lowercase L) to add a laser to the game board
 + s to add a switch to the game board
 + c to map a connection between a switch and a laser

The laser line definition consists of the coordinates and the direction mask.

Laser Coordinates are zero-based (start from zero) in the range of 0 - 4 because there are 5x5 positions for a laser.
The direction mask of a laser is a bitmask using 1, 2, 4, and 8 for UP, RIGHT, DOWN, and LEFT respectively.
You can bitwise-OR these values to create lasers which fire in multiple directions, such as using 15 to fire in all four.

The switch line definition consists of just coordinates of the switch.
Switch Coordinates are zero-based in the range of 0 - 3 because there are 4x4 positions for a switch.

The connection line definition consists of the zero-based id of the switch, the zero-based id of the laser, and the direction mask for the laser once the switch is flipped.

Look at the level1.txt - level5.txt in the levels folder to see how it is done.

A Switch can have any number of connections allowing a single switch to control multiple lasers.

### Fast Testing

The way it works, you can edit the level#.txt (where # is the level you are editing) and if you are playing the 
level in the game, you can press R to restart the level, and it will re-load the level data, allowing you to 
interactively build the level and playtest it at the same time. 

### BUGS
 + If a laser is set to 0 for it's direction, the beams are invisible and the player will still die.

 If you find a bug in the game, please let me know about it.
