'This Program was created as a program to show the functions of directional
'keys it will be use in several programs but the butons may prove to do
'differently, this is the start of new technology this in itself is better
'than joes coptor,you can now shoot a red thing and it will do something good
'this tech has been incorporated into Joes Copter II
'because of this technology Joes Copter is probally going to do really well
'I change the lazer to a missle in Joes Copter so you won't be seeing that
'In Joes copter instead of using the number statements I changed that
'to IF statement THEN action ELSIF statement THEN action END IF and this
'prooved to be a lot more effective.

SCREEN 13
DIM killer(160)
DIM ekiller(160)
WINDOW (0, 0)-(100, 100)
LINE (50, 50)-(55, 55), 1, BF
GET (50, 50)-(55, 55), killer
CLS
LINE (50, 50)-(55, 55), 4, BF
GET (50, 50)-(55, 55), ekiller
CLS
LET move = RND * 1 * 4
LET x = 50
LET y = 50
LET j = 30
LET i = 10
PUT (x, y), ekiller
10 PUT (i, j), killer
15 DO: move$ = INKEY$: LOOP UNTIL move$ = "c" OR move$ = CHR$(0) + "M" OR move$ = CHR$(0) + "K" OR move$ = " " OR move$ = CHR$(0) + "H" OR move$ = CHR$(0) + "P"
IF move$ = CHR$(0) + "M" THEN 20 'left
IF move$ = CHR$(0) + "K" THEN 30 'right
IF move$ = " " THEN 60
IF move$ = "c" THEN 80
IF move$ = CHR$(0) + "H" THEN 40 'up
IF move$ = CHR$(0) + "P" THEN 50 'down
20 PUT (i, j), killer
IF i = 95 THEN LET i = 5
LET i = i + 1
GOTO 10
30 PUT (i, j), killer
IF i = 5 THEN LET i = 95
LET i = i + (-1)
GOTO 10
40 PUT (i, j), killer
IF j = 95 THEN LET j = 5
LET j = j + 1
GOTO 10
50 PUT (i, j), killer
IF j = 5 THEN LET j = 95
LET j = j - 1
GOTO 10
60 IF j = y OR j = y - 1 OR j = y + 1 OR j = y + 2 OR j = y + 3 OR j = y + 4 OR j = y + 5 THEN 70
65 FOR di = 6 TO 95
LINE (i + di, j)-(i + di + 5, j + 1), 14, BF
LINE (i + di, j)-(i + di + 1, j + 1), 0, BF
FOR time.delay1 = 1 TO 200: NEXT time.delay1: NEXT di
GOTO 15
70 IF i >= x - 11 THEN 15
FOR di = 6 TO x - i - 7
LINE (i + di, j)-(i + di + 5, j + 1), 14, BF
LINE (i + di, j)-(i + di + 1, j + 1), 0, BF
FOR time.delay1 = 1 TO 200: NEXT time.delay1: NEXT di
LINE (i + di, j)-(i + di + 5, j + 1), 0, BF
PUT (x, y), ekiller
IF x >= 90 THEN LET x = 15
IF y >= 90 THEN LET y = 15
LET x = x + 10
LET y = y + 5
PUT (x, y), ekiller
GOTO 15
80 END



