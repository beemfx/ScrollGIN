SCREEN 13
WINDOW (1, 1)-(100, 100)
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

