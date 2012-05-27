Joes Copter II: The Revenge
Copyright (C) 1998, 2000, 2001, 2004 Blaine Myers

The executable for Joes Copter 2 is ScrollGIN.exe as
Joes Copter 2 is a ScrollGIN demo right now and not
a finished game.

Instructions:
	During gameplay use the following controls.

   --------   
   Keyboard
   --------

      'Up Arrow' - Fly Up
      'Down Arrow' - Fly Down
      'Right Arrow' - Fly to Right
      'Left Arrow' - Fly to left
      'Right Control' - Fire a Missile

      'ESC' - QUIT
   --------
   Joystick
   --------
      Use Joystick to Fly in various directions.
      'Button 0' - Shoot
	
To shoot down an enemy take careful aim and fire.  Missiles
do not have infinite range, they expire after 8/10 of a
second.

NOTES:

Technically right now Joes Copter 2 is the ScrollGIN test,
that is to say that it a demonstration of what ScrollGIN
technology will be.  It is the prefered version of the
game right now, but it is missing a few things.

Joes Copter 2 uses the first joystick it finds, so if it's
not using the joystick you want it to you'll have to disable
any others.

To play in windowed mode add -w or -window on the command
line.

WHAT JOES COPTER 2 IS MISSING:
1) The Introductory Sequence (where Joe Senior is shot down).
2) When the chopper is shot down, the screen should go red.
3) Sound Effects.
4) Main Menu (or any menu options).
5) The second level with the red thing (this will not be
   seen again :-) ).
6) Only the exploding death has been programmed as of now,
   the crashing death has not (but won't be too difficult).

WHAT JOES COPTER 2 NOW FEATURES:
1) Objects move independantly of each other (in the previous
   version, when a missile was fired it was the only object
   to move).
2) Missles leave a smoke trail.
3) Missles can shoot enemy missiles.
4) Missles blow up when hitting any surface (not just
   the enemy chopper).
5) The screen scrolls.
6) Joystick control.
7) You can shoot and move at the same time.
8) You can move diagnally.
9) You can turn around.
10) Explosions no longer make part of the map dissapear.
11) Maps are now made of tiles instead of bitmaps.
12) There can be more than one missile fired from the
    same chopper on the screen at any given time).
13) Missiles expire after 8/10 second instead of when
    they hit a wall, the enemy chopper, or the end of
    the level.
14) There are two enemies in the demo.
15) Paralaxed backgrounds (note starfield in first level).

WHAT IS THE SAME:
1) The artwork is the same.
2) The first map is mostly the same (but is now two
   screens long).
3) The enemy AI is pretty much the same.


==============================================================
=== Version History                                        ===
===       for Joes Copter II: The Revenge                  ===
==============================================================
v1.04b  (February, 2006)
	Fixed a bug where both enemy helicopters excercise
	the exact same AI.

v1.03b  (January 09, 2004)
	Joes copter 2 uses ScrollGIN technology.  No intro
	sequence or menu yet, as it is actually a ScrollGIN
	test right now.  No sound effects.

v1.02b	(August 29, 2003)
	Joes Copter 2 is now one .exe file becuase I figured
	out how to work QuickBasic's linker.  Not that it
	really maters because Joes Copter 2 will use ScrollGIN
	technology.

v1.01b  (July 21, 2001)
	I finally fixed that damn shoot through wall bug
	at least to a better extent than it was previously
	I know that if a wall is less than eight pixels wide
	you will be able to shoot through it.  But I know how
	to fix that for sure.  That is unless I scrap the
	project again and forget.
v1.00b
	Removed the lock feature. The shoot through wall bug 
	is still active. Improved some code.
	
v1.03c
	Made it so the mainmenu would run the game portion.

v1.02c
	Added a thing that told you if you were locked onto 
	the enemy.

v1.01c
	Fixed some errors tryed to make it so you can shoot 
	the enemy copter through walls, but failed.
	Created a neet animated story.
	
v1.00c
	First version major errors.