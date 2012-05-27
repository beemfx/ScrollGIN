DECLARE SUB waitfor (waittime!)
DECLARE SUB gamover ()
DECLARE SUB PRINSC (b$, overest, downest)
DECLARE SUB rating ()
DECLARE SUB story2 ()
DECLARE SUB copmove ()
DECLARE SUB misslecom ()
DECLARE SUB boom ()
DECLARE SUB thegame ()
DECLARE SUB story ()
DECLARE SUB options ()
DECLARE SUB password ()
DECLARE SUB mainmenu ()
DECLARE SUB title ()
DECLARE SUB graphicsloadmain ()
DECLARE SUB presents ()
DECLARE SUB software ()
DECLARE SUB intro ()
DECLARE SUB gameloop ()
DECLARE SUB graphicsload ()

DIM SHARED i, j, x, y, espeed, ealive, copdead(160)
DIM SHARED speed, lmissle(50), alive, stateof
DIM SHARED copimage1(160), copimage2(160), lcopimage1(160), lcopimage2(160)
DIM SHARED missle(50), prop, lprop, missle2(50)
DIM SHARED man1, man2, cor, copimageb1(200), copimageb2(200)
DIM SHARED lcopimageb1(200), lcopimageb2(200), copdeadb(200)

graphicsload
graphicsloadmain
rating
mainmenu

SUB boom
copo = 1
jo = 60
CLS
FOR down = 60 TO 5 STEP -1
IF copo = 1 THEN
        LINE (jo - 2, down)-(jo + 12, down + 10), 0, BF
        PUT (jo, down), copimage1
        waitfor (30)
        LET copo = 2
        LET jo = jo + .5
        IF down < 30 THEN LET jo = jo - 1
        IF down < 20 THEN LET jo = jo + 1.5
ELSEIF copo = 2 THEN
        LINE (jo - 2, down)-(jo + 12, down + 10), 0, BF
        PUT (jo, down), lcopimage1
        waitfor (30)
        LET copo = 1
        LET jo = jo + .5
        IF down < 30 THEN LET jo = jo - 1
        IF jo < 20 THEN LET jo = jo + 1.5
END IF
LET x1 = RND * 1 * 2
LET x2 = RND * 1 * 2
CIRCLE (jo + x2, down + 12), x1, 18
NEXT down
FOR size = 1 TO 10 STEP .01
CIRCLE (78, 10), size, 4
waitfor (.5)
NEXT size
        story2
mainmenu
END SUB

SUB copmove
PUT (60, 60), copimage1
FOR j = 1 TO 1000: NEXT j
PUT (60, 60), copimage1
PUT (60, 60), copimage2
FOR j = 1 TO 1000: NEXT j
PUT (60, 60), copimage2

END SUB

SUB gamover
CLS
COLOR 15
LOCATE 12, 15: PRINT "GAME OVER"
COLOR 0
END
END SUB

SUB graphicsloadmain
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

SUB mainmenu
CLS
1640
COLOR 4
LOCATE 5, 10: PRINT "Main Menu"
COLOR 3
LOCATE 8, 8: PRINT "1. The Game"
LOCATE 10, 8: PRINT "2. The Story"
LOCATE 12, 8: PRINT "3. Options"
LOCATE 14, 8: PRINT "4. Password"
LOCATE 16, 8: PRINT "5. Quit"
LET man1 = 60: LET man2 = 50
DO
select$ = INKEY$
copmove
LOOP UNTIL select$ = "1" OR select$ = "2" OR select$ = "3" OR select$ = "4" OR select$ = "5"
COLOR 15
IF select$ = "1" THEN
        thegame
ELSEIF select$ = "2" THEN
        story
ELSEIF select$ = "3" THEN
        options
ELSEIF select$ = "4" THEN
        password
ELSEIF select$ = "5" THEN
        gamover
END IF

END SUB

SUB misslecom
LINE (cor - 1, 60)-(cor + 7, 64), 0, BF
PUT (cor, 60), missle2
END SUB

SUB options
CLS
PRINT "Options not done"
SLEEP 1
mainmenu
END SUB

SUB password
CLS
PRINT "Password not incorporated"
SLEEP 1
mainmenu
END SUB

SUB PRINSC (b$, overest, downest)
lens = LEN(b$)
FOR printes = 1 TO lens
LOCATE downest, overest: PRINT LEFT$(b$, printes)
waitfor (30)
NEXT printes
END SUB

SUB rating
CLS
COLOR 15
LOCATE 4, 5: PRINT "A Blaine Everett Myers Production"
LOCATE 6, 1: PRINT "B.M. Software Licence agreement: This"
LOCATE 7, 1: PRINT "product has been tested and will not "
LOCATE 8, 1: PRINT "effect you system in any way.  B.M."
LOCATE 9, 1: PRINT "Software is not responsible for errors"
LOCATE 10, 1: PRINT "you may find in the software.  Your"
LOCATE 11, 1: PRINT "product is in no way under warranty"
LOCATE 12, 1: PRINT "and if your not satisfied tuff. This"
LOCATE 13, 1: PRINT "software was intended as freeware and"
LOCATE 14, 1: PRINT "under no circumstances should any money"
LOCATE 15, 1: PRINT "be paid for this software. B.M. Software"
LOCATE 16, 1: PRINT "does hope that you enjoy thier product."
LOCATE 18, 1: PRINT "This product was rated by BM Software"
LOCATE 19, 1: PRINT "Rating Corps. (BMSRC). A sub corporation"
LOCATE 20, 1: PRINT "of B.M. Software."
LOCATE 22, 5: PRINT "R"
LOCATE 22, 7: PRINT "Restricted"
LOCATE 23, 7: PRINT "17+"
LINE (10, 12)-(10, 18)
LINE (10, 18)-(14, 18)
LINE (14, 18)-(14, 12)
LINE (14, 12)-(10, 12)
LINE (14, 18)-(41, 18)
LINE (41, 18)-(41, 8)
LINE (41, 8)-(10, 8)
LINE (10, 8)-(10, 12)
SLEEP 2
END SUB

SUB software
CLS

LOCATE 7, 8: PRINT "B.M. Software Presents"


FOR js = 1 TO 25000: NEXT js
CLS
FOR js = 1 TO 5000: NEXT js
END SUB

SUB story
CLS
COLOR 15
software
CLS
PLAY "l4t120o3mbmlddbddbddadfdefdedefaeed"
'DO
PUT (60, 60), copimage1
FOR cor = 2 TO 55
misslecom
IF cor = 30 THEN
        LOCATE 6, 4: PRINT "Oh No!  I can't shake it!"
        LOCATE 7, 28: PRINT "\"
END IF
'IF cor = 55 THEN
'   boom
'END IF
waitfor (40)
NEXT cor
'LOOP UNTIL cor = 55
boom
mainmenu
END SUB

SUB story2
CLS
PRINSC "Joe's courage had led to his fall.", 3, 8

waitfor (200)
PRINSC "But Joe had left something behind ", 3, 9
waitfor (200)
PRINSC "that was so precious, that it would", 3, 10
waitfor (200)
PRINSC "allow Joe's Spirit to live on.", 3, 11
waitfor (2000)
CLS
LINE (40, 40)-(60, 50), 1, BF
LINE (40, 40)-(38, 55), 1, BF
LINE (60, 40)-(62, 60), 1, BF
CIRCLE (56, 50), 3, 66, .1, 3
LINE (52.8, 50)-(59, 50), 66
PAINT (56, 51), 66
CIRCLE (46.8, 40), 10, 15, .9, 2.27
LINE (40.5, 50)-(53, 50)
PAINT (50, 51), 15
PSET (57, 52), 0
COLOR 8
LINE (58, 52)-(59, 52)
LINE (57.9, 52.5)-(58.5, 52.5)
LINE (57.8, 53)-(58.6, 53)
LINE (57.9, 51.5)-(58.6, 51.5)
LINE (57.8, 51)-(58.9, 51)
LINE (57.7, 50.5)-(59.2, 50.5)
LINE (58, 50.1)-(59, 50.1)
LINE (58.2, 50)-(59.1, 50)
COLOR 15
LOCATE 11, 22: PRINT "\"
waitfor (100)
LOCATE 10, 15: PRINT "Goo"
waitfor (500)
LOCATE 8, 20: PRINT "Goo"
waitfor (2000)
CLS
PRINSC "20 Years Later", 13, 10
waitfor (1500)
CLS
PRINSC "Joe Junior decided to take revenge!", 3, 12
waitfor (2000)

title
END SUB

SUB thegame
CLS
gameloop
mainmenu
END SUB

SUB title
CLS
LINE (30 - 10, 40)-(40 - 10, 50)
LINE (40 - 10, 50)-(60 - 10, 50)
LINE (60 - 10, 50)-(75 - 10, 40)
LINE (75 - 10, 40)-(90 - 10, 40)
LINE (90 - 10, 40)-(95 - 10, 50)
LINE (95 - 10, 50)-(100 - 10, 50)
LINE (100 - 10, 50)-(100 - 10, 35)
LINE (100 - 10, 35)-(75 - 10, 35)
LINE (75 - 10, 35)-(73 - 10, 33)
LINE (73 - 10, 33)-(35 - 10, 33)
LINE (35 - 10, 33)-(30 - 10, 35)
LINE (30 - 10, 35)-(30 - 10, 40)
LINE (50 - 10, 50)-(50 - 10, 55)
LINE (50 - 10, 55)-(55 - 10, 55)
LINE (55 - 10, 55)-(55 - 10, 50)
LINE (30 - 10, 55)-(75 - 10, 55)
LINE (35 - 10, 28)-(70 - 10, 28)
LINE (35 - 10, 28)-(33 - 10, 30)
LINE (40 - 10, 28)-(40 - 10, 33)
LINE (60 - 10, 28)-(60 - 10, 33)
CIRCLE (36 - 10, 45), 6, 15, 3.6, .6
PAINT (36 - 10, 45), 200, 15
PAINT (60 - 10, 40), 1009, 15
PAINT (54 - 10, 54), 1013, 15
LOCATE 6, 13: COLOR 12: PRINT "Joe's Copter II"
LOCATE 8, 13: PRINT "  The Revenge"
COLOR 15
PLAY "t200o4mfaabccdefal3bg08l1a"
SLEEP 1
END SUB

