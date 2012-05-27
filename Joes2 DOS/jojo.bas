DECLARE SUB bloodcome ()
DECLARE SUB resetdata ()
DECLARE SUB copsdead ()
DECLARE SUB copdie ()
DECLARE SUB edead ()
DECLARE SUB ecopdie ()
DECLARE SUB eshooter ()
DECLARE SUB ekillermov ()
DECLARE SUB graphicsload ()
DECLARE SUB shooter ()
DECLARE SUB killermov ()
DECLARE SUB waitfor (waittime!)
DECLARE SUB harmlessmissle ()
DECLARE SUB eharmlessmissle ()
DECLARE SUB gameloop ()
DECLARE SUB mainmenu ()

'Note to myself the FOR FALL = 1 to 5 may need to be change to for fall = 1
'an integer greater than 5 this may make the moveing easier.  Also I would
'really like to thank HACKSOFT for saying that I could use some of the code0
'because it really helped me out when it came to the enemy and the main
'character moving at the same time.  I better create the graphics really
'soon because I may need to edit the size of the POINT statement.  I would
'also like to thank HACKSOFT for the help with the POINT statement.  Space
'Pshcho has really helped with joes copter. (1, 27, 99)
'Using Created technology now the bullet will automatically stop when it
'hits any form of wall.  I think I still need to fix one thing, just tested
'it and it turns out that everything works fine.
'I also think that I could easily add a second player and that would work
'out really well.  I figure that the main character can move better if
'the enemys fall is less than the main characters. I have discovered a bug
'that will leave a red mark but I will ignore it because it occurs rarely
' Blaine out    (1, 28, 99).
'I have created the AI and also the enemys weapon systems all that is left
'is to apply the stuff and make it so that the enemys will be shot down
'rather that the enemy just move.  I don't play on making two copters at
'once that is enemy copters because then I'd have to copy over a bunch of
'files.  I wonder if the is too big to put in with the main program if it
'is I will just have to live with myself.  I think that I also might want
'to created the enemy copter so it doesn't look the same as the main
'characters this may be difficult but I will do it eventually. I am a little
'disapointed about the propeller spinning it didn't quite turn out the way
'I wanted it to but it still is pretty good.  I think I am done for today.
'Actually I just finished fixing the bug for the enemy which used to delete
'the back wall it wasn't too hard to do that.
'Blaine out (1, 29, 99)
'For right now I have only fixed the bug on the main characters side I still
'need to make it so the enemy cannot shoot through close walls and also so
'the enemy cannot move through thin walls so I better remember to do this
'I also need to make it so that when the enemy hits the main character the
'main character will crash. This engine is just about done.
'I have just added the lock feat if it is green that means if you shoot
'You will hit.  I have also added the enemy lock feature so if the enemy
'Shoots when it is locked then you are dead
'I just recently fix the helecoptor miss bug, now every time that it locks
'onto you you are garanteed to be hit.
'I now made it so that copters cannot shoot through walls thank you very much
'Blaine out (1/30/99)
'Note: Fix the bug so that when you are really close to a wall and locked
'onto an enemy you will not be able to shoot through the wall.
'Fixed it.  Now I think I have all the bugs worked out of the enjine.
'Blaine out (2\2\99)
'Estimated finish time originally 2 weeks from (1/28/99)
'now 1 weeks 1 days from (1/28/99) (Probally Less).
'I made it so the enemy either blows up or crashes
'I have not been working on J's Copter much but now both helicoptors
'either blow up or crash and also when you die blood comes over the screen
'it is actually really nice one day maybe I'll finish this game.
'Joes engine is complete and I have added the first section of the first
'level joes will be continued
'I have made it so the alive and dead stuff makes it so you are either
'true alive or false alive not 1 and 2
'I made it so blood comes
'I have fixed the bug so you can no longer shoot the enemy helicopter
'after it has died.  In the shooter sub I made sure that the point detector
'will no longer be 10 separate lines it is now one.
'05/8/99
'You can no longer go past the top of the screen 05/9/99
'06/18/99
'Haven't been working on it for a while. I got Qbasic 4.5 and tryed to make
'an .exe file but some of the lines were too long.  I was able to easily
'change that.  So now it can be made into an .exe file.  I made and .exe file
'called jotest.exe Still the problem is the game runs to fast.  So I need to
'figure something out.
'There is still an error that you can shoot the enemy helicoptor strait
'through a wall if your really close to that wall.

'If this game is to be completed the shooter sub should be redone.  It is
'extremely lousy right now.  The instead of using i, j for the coordinates
'x and y shoulde be used and xe and ye for the enemy.
'
'I completely redid the fucking shooter sub.  Now you can only shoot through
'walls less than 8 pixels wide.  I could repair this but I won't now
'I've also fixed a few speed issues.  I should redo the eshooter sub as well
'maybe later.
'
'Did major repairs on the cop and ecop dying subs.  Fixed up the blood come
'did more speed adjustment.  Why the hell am I still working on this?
'
'I redid the eshoot sub now it works just the opposite as the new shooter
'sub.  I've also taken away from the propeller right and left because it made
'too much flashiness.  Sadly it still is making flashes.  Other than that
'I'd have to say I'm done with the engine for Joes Copter 2.  Just making
'some levels is all there is left to the game.  As well as making an ending
'story.
'
'Improved bloodcome function
'
'=====================================================================
'=== Important Notice: When compiling lower the number in the      ===
'=== waitfor sub to about half as much.  see the sub for better    ===
'=== understanding. 500 good for compiling for a 450Mhz            ===
'=====================================================================
DIM SHARED i, j, x, y, espeed, ealive, copdead(160), true, false
DIM SHARED speed, lmissle(50), alive, stateof, stateofc
DIM SHARED copimage1(160), copimage2(160), lcopimage1(160), lcopimage2(160)
DIM SHARED missle(50), prop, lprop, missle2(50)
DIM SHARED man1, man2, cor, copimageb1(200), copimageb2(200)
DIM SHARED lcopimageb1(200), lcopimageb2(200), copdeadb(200)
DIM SHARED shootdist, eshootdist

SUB bloodcome
FOR comedown = 100 TO 1 STEP -.4
   LINE (1, comedown)-(100, comedown), 4
   waitfor (5)
NEXT
END SUB

SUB copdie
RANDOMIZE TIMER
LET eu = INT(RND * 2) + 1
'eu = 2
SOUND 200, 2
IF eu = 1 THEN

FOR downer = j TO 1 STEP -1
        PUT (i, downer), lcopimage1
        FOR wait1 = 1 TO 100: NEXT wait1
        PUT (i, downer), lcopimage1
        PUT (i, downer), copimage2
        FOR wait1 = 1 TO 100: NEXT wait1
        PUT (i, downer), copimage2
        LET i = i + .5
        IF downer < 20 THEN LET i = i - .5
        IF POINT(i + 11, downer) THEN LET i = i - 1
IF POINT(i, downer - 1) OR POINT(i + 10, downer - 1) THEN 48
                   
NEXT downer

48 LET i = i: LET j = downer
LET stateofc = 1
PUT (i, j), copdead

ELSEIF eu = 2 THEN
      
        FOR expl = 1 TO 7 STEP .1
           CIRCLE (i + 5, j + 4), expl, 4
           waitfor (3)
        NEXT expl
        waitfor (30)
        CIRCLE (i + 5, j + 4), 7, 15
        PAINT (i + 5, j + 4), 15, 15
        CIRCLE (i + 5, j + 4), 7, 0
        PAINT (i + 5, j + 4), 0, 0
LET stateofc = 2
END IF
copsdead
LINE (i, j)-(i + 10, j + 8), 0, BF
END SUB

SUB copsdead
PLAY "mbmlt200o2aao1gfo2ao1bcl2co2l1gl3o3"
bloodcome
COLOR 15
LOCATE 10, 10: PRINT "ÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜ"
LOCATE 11, 10: PRINT "Ý                 Þ"
LOCATE 12, 10: PRINT "Ý                 Þ"
LOCATE 13, 10: PRINT "Ý                 Þ"
LOCATE 14, 10: PRINT "ßßßßßßßßßßßßßßßßßßß"
COLOR 4
LOCATE 12, 14: PRINT "JOE IS DEAD"
COLOR 15
LOCATE 17, 9: PRINT "MAY HE REST IN PEACE!"
SLEEP 5
mainmenu
END SUB

SUB ecopdie
RANDOMIZE TIMER
LET eu = INT(RND * 2) + 1
'LET eu = 2
IF eu = 1 THEN

FOR downer = y TO 1 STEP -1
        PUT (x, downer), lcopimageb1
        FOR wait1 = 1 TO 100: NEXT wait1
        LINE (x, downer)-(x + 10, downer + 8), 0, BF
        PUT (x, downer), copimageb2
        FOR wait1 = 1 TO 100: NEXT wait1
        LINE (x, downer)-(x + 10, downer + 8), 0, BF
        LET x = x + .5
        IF downer < 20 THEN LET x = x - .5
        IF POINT(x + 11, downer) THEN LET x = x - 1
IF POINT(x, downer - 1) OR POINT(x + 10, downer - 1) THEN 38
                    
NEXT downer

38 LET x = x: LET y = downer
LET stateof = 1
ELSEIF eu = 2 THEN
       
        FOR expl = 1 TO 7 STEP .1
           CIRCLE (x + 5, y + 4), expl, 4
           'PAINT (x + 5, y + 4), 4
           waitfor (3)
        NEXT
        waitfor (30)
        CIRCLE (x + 5, y + 4), 7, 15
        PAINT (x + 5, y + 4), 15, 15
        CIRCLE (x + 5, y + 4), 7, 0
        PAINT (x + 5, y + 4), 0, 0
LET stateof = 2
END IF



END SUB

SUB edead
IF stateof = 1 THEN
PUT (x, y), copdeadb, PSET
LET x = 0: LET y = 0
LET stateof = 2
ELSEIF stateof = 2 THEN
LET x = 0: LET y = 0
END IF
END SUB

SUB eharmlessmissle
SOUND 200, 1
'Check to see if there is pixels in front of the chopper
'currently only checks on row so may no wok accurately
FOR cpoint = 1 TO 8
   IF POINT((x - cpoint), y) THEN GOTO ebreak
   'LOCATE 1, 1: PRINT "checkpoint is"; checkpoint
NEXT
FOR eshootdist = x - 8 TO 0 STEP -1
   IF POINT(eshootdist, y) THEN
       GOTO ebreak
   ELSE
      PUT (eshootdist, y), lmissle
      waitfor (2)
      PUT (eshootdist, y), lmissle
   END IF
NEXT
ebreak:
SOUND 50, 1

END SUB

SUB ekillermov
IF ealive = true THEN
FOR fall = 1 TO espeed
movec = INT(RND * 250) + 1
IF movec = 1 THEN
        LINE (x, y)-(x + 10, y + 8), 0, BF
        LET x = x + 1
        FOR yvari = 0 TO 8
        IF POINT(x + 10, y + yvari) THEN
                LET x = x - 1
        END IF
        NEXT yvari
        PUT (x, y), lcopimageb1

ELSEIF movec = 2 THEN
        LINE (x, y)-(x + 10, y + 8), 0, BF
        LET x = x - 1
        FOR yvari = 0 TO 8
        IF POINT(x, y + yvari) THEN LET x = x + 1
        NEXT yvari
        PUT (x, y), lcopimageb1

ELSEIF movec = 3 THEN
        LINE (x, y)-(x + 10, y + 8), 0, BF
        LET y = y + 1
        FOR xvary = 0 TO 10
        IF POINT(x + xvary, y + 8) THEN LET y = y - 1
        NEXT xvary
        PUT (x, y), lcopimageb1

ELSEIF movec = 4 THEN
        LINE (x, y)-(x + 10, y + 8), 0, BF
        LET y = y - 1
        FOR xvary = 0 TO 10
        IF POINT(x + xvary, y) THEN LET y = y + 1
        NEXT xvary
        PUT (x, y), lcopimageb1

ELSEIF movec < 5 THEN
        LINE (x, y)-(x + 10, y + 8), 0, BF
        PUT (x, y), lcopimageb1

ELSEIF movec = 5 THEN
      eshooter

END IF

NEXT fall
ELSEIF ealive = false THEN
        edead
END IF
END SUB

SUB eshooter
IF ((i + 10) < x) THEN
   IF (j + 8) < y THEN 'Higher than player
      'LOCATE 1, 1: PRINT "Higher"
      eharmlessmissle
   ELSEIF ((y + 4) < j) THEN 'Lower than player
      'LOCATE 1, 1: PRINT "lower"
      eharmlessmissle
   ELSE ' lined up with the enemy
      FOR cpoint = 1 TO 8
         IF POINT((x - cpoint), y) THEN nexttotile = true
         'LOCATE 1, 1: PRINT "Next to tile"
      NEXT
      IF nexttotile = true THEN
         IF (x - i < 20) THEN
            alive = false
            LINE (i, j)-(i + 10, j + 8), 0, BF
            copdie
         ELSE
            SOUND 200, 1
         END IF
      ELSE
      eharmlessmissle
         'LOCATE 1, 1: PRINT eshootdist
         IF eshootdist > (i + 10) THEN
         ELSE
            alive = false
            LINE (i, j)-(i + 10, j + 8), 0, BF
            copdie
         END IF
      END IF
   END IF
'LOCATE 1, 1: PRINT "checkpoint is"; checkpoint
ELSE
   'A Missle Flies Harmlessly
   'LOCATE 1, 1: PRINT "Passed"
   eharmlessmissle
END IF
'alldone:
  
END SUB

SUB gameloop
SCREEN 13
WINDOW (1, 1)-(100, 100)
RANDOMIZE TIMER
LET true = 1
LET false = NOT true

resetdata
'graphicsload
WINDOW (1, 1)-(100, 100)
CLS
LET prop = 1
LET lprop = 1
LET speed = 20
LET espeed = 10
LET ealive = true
LET alive = true

'First Map Structure
LET j = 38
LET i = 9
LINE (1, 1)-(100, 25), 2, BF
FOR pointset = 1 TO 100 STEP 3
PSET (pointset, 25), 45
NEXT pointset
COLOR 28
LINE (10, 23)-(10, 30)
LINE (10, 30)-(5, 35)
LINE (5, 35)-(5, 37)
LINE (5, 37)-(25, 37)
LINE (25, 37)-(25, 35)
LINE (25, 35)-(20, 30)
LINE (20, 30)-(20, 23)
LINE (20, 23)-(10, 23)
PAINT (11, 26), 28
COLOR 14
LINE (13, 27)-(13.6, 35), 14, BF
LINE (13.5, 31)-(16, 31.5), 14, BF
LINE (16, 27)-(16.5, 35), 14, BF
COLOR 15
FOR pointset = 5 TO 25 STEP 2
PSET (pointset, 37)
NEXT pointset


DO
killermov
LOOP UNTIL i + 10 > 99
CLS
'2nd Map
resetdata
LET i = 2
LET j = j
LET x = 60
LET y = 40
LINE (40, 0)-(50, 50), 4, BF
DO
killermov
ekillermov
LOOP
PRINT "DONE"
END SUB

SUB graphicsload
SCREEN 13
WINDOW (1, 1)-(100, 100)
CLS
LET ecolor = 170
LET ccolor = 230
COLOR 15

CLS
LINE (50, 40)-(55.5, 41), 202, BF
LINE (55.5, 41)-(57, 40.5), 15
LINE (57, 40.5)-(55.5, 40), 4
LINE (50, 40)-(48.5, 39), 15
LINE (50, 41)-(48.5, 42), 4
GET (48.5, 39)-(57, 42), missle2

LINE (10, 42)-(13, 42)
LINE (13, 42)-(15, 40)
LINE (15, 40)-(18, 40)
LINE (18, 40)-(20, 42)
LINE (20, 42)-(20, 43)
LINE (20, 43)-(19, 45)
LINE (19, 45)-(15, 45)
LINE (15, 45)-(14, 44)
LINE (14, 44)-(12, 44)
LINE (12, 44)-(11, 45)
LINE (11, 45)-(10, 45)
LINE (10, 45)-(10, 42)
LINE (16.5, 45)-(16.5, 47)
LINE (17.5, 40)-(17.5, 39)
LINE (15, 40)-(15, 39)
LINE (14, 39)-(19, 39)
LINE (19, 39)-(19.4, 39.5)
CIRCLE (19, 44), 1, 15, 2.5, 6
PAINT (19, 44), 1, 15
PAINT (17, 44), ccolor, 15
LINE (16.5, 47)-(19, 47)
GET (10, 39)-(20, 47), copimage1
PAINT (17, 44), ecolor, 15
GET (10, 39)-(20, 47), copimageb1
CLS
COLOR 15
LINE (10, 42)-(13, 42)
LINE (13, 42)-(15, 40)
LINE (15, 40)-(18, 40)
LINE (18, 40)-(20, 42)
LINE (20, 42)-(20, 43)
LINE (20, 43)-(19, 45)
LINE (19, 45)-(15, 45)
LINE (15, 45)-(14, 44)
LINE (14, 44)-(12, 44)
LINE (12, 44)-(11, 45)
LINE (11, 45)-(10, 45)
LINE (10, 45)-(10, 42)
LINE (16.5, 45)-(16.5, 47)
LINE (17.5, 40)-(17.5, 39)
LINE (15, 40)-(15, 39)
LINE (14, 39)-(19, 39)
LINE (19, 39)-(19.4, 39.5)
CIRCLE (19, 44), 1, 15, 2.5, 6
PAINT (19, 44), 1, 15
PAINT (17, 44), ccolor, 15
LINE (16.5, 47)-(14, 47)
GET (10, 39)-(20, 47), copimage2
PAINT (17, 44), ecolor, 15
GET (10, 39)-(20, 47), copimageb2
CLS
LINE (50, 40)-(55.5, 41), 7, BF
LINE (55.5, 41)-(57, 40.5), 15
LINE (57, 40.5)-(55.5, 40), 4
LINE (50, 40)-(48.5, 39), 15
LINE (50, 41)-(48.5, 42), 4
GET (48.5, 39)-(57, 42), missle
CLS
LINE (50, 40)-(55.5, 41), 2, BF
LINE (50, 40)-(48.5, 40.5), 4
LINE (50, 41)-(48.5, 40.5), 15
LINE (55.5, 41)-(57, 41.5), 4
LINE (55.6, 40)-(57, 39.5), 15
GET (48.5, 39.5)-(57, 41.5), lmissle

CLS
LINE (40, 40)-(43, 40)
LINE (40, 40)-(38, 42)
LINE (38, 42)-(38, 43.5)
LINE (38, 43.5)-(39, 45)
LINE (39, 45)-(43, 45)
LINE (43, 45)-(43.5, 44)
LINE (43.5, 44)-(46, 44)
LINE (46, 44)-(47, 45)
LINE (47, 45)-(48, 45)
LINE (48, 45)-(48, 42)
LINE (48, 42)-(44.5, 42)
LINE (44.5, 42)-(43, 40)
CIRCLE (39, 44), 1, 15, 3.5, 1
PAINT (39, 44), 1, 15
PAINT (40, 44), 1005, 15
LINE (41, 45)-(41, 47)
LINE (40, 40)-(40, 39)
LINE (42.5, 40)-(42.5, 39)
LINE (39.3, 39)-(43.5, 39)
LINE (39.3, 39)-(39, 39.5)
LINE (41, 47)-(39, 47)
GET (38, 39)-(48, 47), lcopimage1
PAINT (40, 44), ecolor, 15
GET (38, 39)-(48, 47), lcopimageb1
CLS
LINE (40, 40)-(43, 40)
LINE (40, 40)-(38, 42)
LINE (38, 42)-(38, 43.5)
LINE (38, 43.5)-(39, 45)
LINE (39, 45)-(43, 45)
LINE (43, 45)-(43.5, 44)
LINE (43.5, 44)-(46, 44)
LINE (46, 44)-(47, 45)
LINE (47, 45)-(48, 45)
LINE (48, 45)-(48, 42)
LINE (48, 42)-(44.5, 42)
LINE (44.5, 42)-(43, 40)
CIRCLE (39, 44), 1, 15, 3.5, 1
PAINT (39, 44), 1, 15
PAINT (40, 44), ccolor, 15
LINE (41, 45)-(41, 47)
LINE (40, 40)-(40, 39)
LINE (42.5, 40)-(42.5, 39)
LINE (39.3, 39)-(43.5, 39)
LINE (39.3, 39)-(39, 39.5)
LINE (41, 47)-(43, 47)
GET (38, 39)-(48, 47), lcopimage2
PAINT (40, 44), ecolor, 15
GET (38, 39)-(48, 47), lcopimageb2
CLS
LINE (20, 40)-(27, 40)
LINE (27, 40)-(28, 42)
LINE (28, 42)-(30, 40)
LINE (30, 40)-(30, 43)
LINE (30, 43)-(27, 45)
LINE (27, 45)-(26, 44)
LINE (26, 44)-(25, 45)
LINE (25, 45)-(22, 45)
CIRCLE (22, 43.5), 1, 15
PAINT (22, 43.5), 1, 15
LINE (22, 45)-(20, 40)
PAINT (24, 43), ccolor, 15
LINE (24, 45)-(23, 46.5)
LINE (23, 46.5)-(20, 43.6)
LINE (23, 46.5)-(27, 46)
LINE (21.7, 45)-(19.7, 40), 0
COLOR 15
GET (20, 40)-(30, 47), copdead
PAINT (24, 43), ecolor, 15
GET (20, 40)-(30, 47), copdeadb
CLS

END SUB

SUB harmlessmissle
SOUND 200, 1
'Check to see if there is pixels in front of the chopper
'currently only checks on row so may no wok accurately
FOR cpoint = 1 TO 8
   IF POINT((i + 10 + cpoint), j) THEN GOTO break
   'LOCATE 1, 1: PRINT "checkpoint is"; checkpoint
NEXT

FOR shootdist = (i + 10) TO 90
   IF POINT(shootdist + 8, j) THEN
       GOTO break
   ELSE
      PUT (shootdist, j), missle
      waitfor (2)
      PUT (shootdist, j), missle
   END IF
NEXT
break:
SOUND 50, 1
END SUB

SUB killermov
IF alive = true THEN

FOR fall = 1 TO speed
move$ = INKEY$
  IF move$ = CHR$(0) + "M" THEN  'Right
        LINE (i, j)-(i + 10, j + 8), 0, BF
        LET i = i + 1
        FOR jvari = 0 TO 8
        IF POINT(i + 10, j + jvari) THEN LET i = i - 1
        NEXT jvari
       
'        IF lprop = 1 THEN
           PUT (i, j), copimage1
'           lprop = 2
'        ELSEIF lprop = 2 THEN
'           PUT (i, j), copimage2
'           lprop = 1
'        END IF
   ELSEIF move$ = CHR$(0) + "K" THEN  'Left
        LINE (i, j)-(i + 10, j + 8), 0, BF
        LET i = i - 1
        FOR jvari = 0 TO 8
        IF POINT(i, j + jvari) THEN LET i = i + 1
        NEXT jvari
       
'        IF lprop = 1 THEN
           PUT (i, j), copimage1
'           lprop = 2
'        ELSEIF lprop = 2 THEN
'           PUT (i, j), copimage2
'           lprop = 1
'        END IF

   ELSEIF move$ = CHR$(0) + "H" THEN  'up
        LINE (i, j)-(i + 10, j + 8), 0, BF
        LET j = j + 1
        FOR ivari = 0 TO 10
        IF POINT(i + ivari, j + 8) THEN LET j = j - 1
        NEXT ivari
        IF j > 91 THEN LET j = j - 1
       
'        IF lprop = 1 THEN
           PUT (i, j), copimage1
'          lprop = 2
'       ELSEIF lprop = 2 THEN
'           PUT (i, j), copimage2
'           lprop = 1
'        END IF

   ELSEIF move$ = CHR$(0) + "P" THEN  'down
        LINE (i, j)-(i + 10, j + 8), 0, BF
        LET j = j - 1
        FOR ivari = 0 TO 10
        IF POINT(i + ivari, j) THEN LET j = j + 1
        NEXT ivari
       
'        IF lprop = 1 THEN
           PUT (i, j), copimage1
'           lprop = 2
'        ELSEIF lprop = 2 THEN
'           PUT (i, j), copimage2
'           lprop = 1
'        END IF

   ELSEIF move$ = "" THEN
'        IF lprop = 1 THEN
            LINE (i, j)-(i + 10, j + 8), 0, BF
            PUT (i, j), copimage1
'           PUT (i, j), copimage1
'           'lprop = 2
'        ELSEIF lprop = 2 THEN
'           PUT (i, j), copimage2
'           'lprop = 1
'        END IF
       
'        IF lprop = 1 THEN
           PUT (i, j), copimage1
'           lprop = 2
'        ELSEIF lprop = 2 THEN
'           PUT (i, j), copimage2
'           lprop = 1
'        END IF
   ELSEIF move$ = " " THEN
       LINE (i, j)-(i + 10, j + 8), 0, BF
       PUT (i, j), copimage1
       shooter
   ELSEIF move$ = CHR$(27) THEN
        LET alive = false
        LINE (i, j)-(i + 10, j + 8), 0, BF
        copdie
   END IF

NEXT fall
ELSEIF alive = false THEN
        copsdead
END IF
END SUB

SUB resetdata
LET ealive = true
LET stateof = 0

LET prop = 1
LET lprop = 1
LET alive = true
END SUB

SUB shooter
IF ((i + 10) < x) THEN
   IF j > (y + 8) THEN 'Higher than enemy
     ' LOCATE 1, 1: PRINT "Higher"
      harmlessmissle
   ELSEIF ((j + 4) < y) THEN 'Lower than enemy
     ' LOCATE 1, 1: PRINT "lower"
      harmlessmissle
   ELSE ' lined up with the enemy
      FOR cpoint = 1 TO 8
         IF POINT((i + 10 + cpoint), j) THEN nexttotile = true
      NEXT
     
      IF nexttotile = true THEN
         IF (x - i < 20) THEN
            ecopdie
            ealive = false
         ELSE
            SOUND 200, 1
         END IF
      ELSE
      harmlessmissle
      IF (shootdist + 8) < x THEN
         ELSE
            ecopdie
            ealive = false
         END IF
      END IF
   END IF
'LOCATE 1, 1: PRINT "checkpoint is"; checkpoint
ELSE
   'A Missle Flies Harmlessly
   'LOCATE 1, 1: PRINT "Passed"
   harmlessmissle
END IF
'alldone:
END SUB

SUB waitfor (waittime)
timetowait = waittime * 1000 '<--- This is the number to lower to change speed
FOR waiting = 1 TO timetowait: NEXT
END SUB

