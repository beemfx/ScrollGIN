ScrollGIN
=========

ScrollGIN(tm) is a 32 bit Scrolling engine intended for platform
based games, as well as overhead RPG type games.  It is heavily
based off 8 bit scrolling engines of the mid 80's, only much more
advanced.

Notes
=====
To build the game and tools you'll need the enviornment variable BEEMOUT set
to something useful such as "D:\BuildOut\".

Log
===
May 26, 2012

I'm not really resurrecting this project or anything. I just thought I'd make
the project open source. There isn't really much reason to develop this project
further as better technologies exist.

Basically this project was my introduction to Windows game development, as all
my previous experience was in DOS. Looking back on this code, it is very crude,
not well organized, and it runs really bad on modern computers due to the way
it used DirectDraw.

I may dabble around with this project a little more, but any major development
is unlinkely. I'm just happy that I got the game and tools building again.

== The log is now in chronological order ==

April 07, 2002

I finished the tile manager class.  Due to problems I had to use
a specific size, which can easily be changed.  The library is
still not implemented.

Strangely, when creating tiles, the program won't display the
tile that was last created, I'm not sure why, but I'll have to
find out.

I've created both a full screen, and windowed disply.  The
windowed display does not work correctly, the full screen does.
However when quitting after using full screen display an error
occurs, this does not happen with window mode.

That problem with an error in full screen mode, was simply because
I was releasing the backbuffer after the primary, I am so damn
happy that I figured it out.  Now I just need to get windowed
mode to work, and I'll be as happy as a cunt-penetrating penis.

April 08, 2002

Generated the viewport class.  It's heavily based off of Ian
Parberry's from ned's turkey farm, only designed for a real
2d univers with x and y.  I've tested it and it works quite
well.  I have not tested it's draw background feature yet.

I also added a timer.  I used the one from ned's turkey farm,
because it suits my needs, and if I wrote my own timer I'd do
something similar to that anyway.

May 02, 2002

I finally did some work on this.  I've mainly been working on
the tile library, but I haven't implimented it yet.  What I
have done is fixed up the windowed mode, sadly it doesn't clip
correctly.

May 04, 2002

I've been putting in a little work.  DirectInput is now active
and works.  You can use left and right arrows to scroll the
demo screen right and left.  I had to updated a problem with
the viewport manager.

May 07, 2002

Implimented the library feature.  Tiles can now be loaded from
a library built by Libellus ex Imago.

May 14, 2002

The problem with the last tile in the tile manager not being
displayed has been fixed.

June 06, 2002

Fixed the problem with the program not being active when
starting up in fullscreen mode.

June 24, 2002

I haven't worked on this program for a while because I've been
working on the Map Editor.  Now that the map editor is somewhat
complete, I have implimented the CMapBoard class and now the
programm will load a map and draw it on the screen.  I've also
made an optimization to the Viewport class.

June 29, 2002

Well I haven't really done to much, but I'd like to make a note of
what is left to do in the project.

Actual things that need to be done for ScrollGIN:
	Impliment Background Class ( CBackground ) This class
		will be used to manage the paralax scrolling of a 
		background image.  It will be able to do a number 
		of effects with a given background image.

	Impliment Sprite Class ( CSprite )  This class will manage 
		imagesused as sprites.

	Impliment Object Class ( CObject )  This class will represent 
		an object.  It will contain attached sprites, and what to 
		do with them.

	Impliment Pause functionality into the timer.

	Impliment Object Manager class ( CObjectManager ) This class 
		will manage objects, and report collisions between 
		objects, as well as destroy and create objects.  This 
		will only be a base class, and derived classes will be 
		required for each ScrollGIN game.

Things for actual ScrollGIN games:
	(These classes will be specific for each individual engine.)
	Intelligent Object class ( CIntelligentObject ) This will 
		be a derived class from CObject, it will be different 
		for each ScrollGIN game.  This will manage how gravity 
		works and such.

June 29, 2002 (Supplimental)
	I've created the spite class.  It works pretty damn good, 
	but I want to impliment the ability to reverse a sprite.

	Implimented all obove mentioned itmes.  I have tested all 
	member functions and they seem to work properly.

	Right now I have a test sprite and it displays the flying 
	ned from Ned's turkey farm wherever the mouse is.  It's 
	looks pretty damn cool.

July 02, 2002
	I've implimented a class called CImage.  This class contains 
	one image, and can do a few things to it.  It stores how 
	to successfully reload a given image.  This class is now 
	used by the CTileManager and CSprite classes.

	I beleive this will make it significantly easiers to upgrade 
	to Direct3D or even OpenGL for that matter.

July 02, 2002 (Supplimental)
	The joystick has been implimented.

July 03, 2002
	Object class has been implimented.  I created a really 
	retarded looking dude, but he is made up of several 
	sprites so it's pretty cool.

	I need to improve the object class somewhat for the multiple sprite thing.

July 04, 2002
	Thank God for our independence.  I've slightly upgraded 
	the CSprite and CSpriteManager classes.

	The CSprite class now allows you to name the sprite, that 
	way you can get a pointer to a sprite (using CSpriteManager) 
	without knowing which index it is.

	I've also implimented a createsprite function in the 
	CSpriteManager class that allows creation of a sprite 
	using a SSpriteCreateStruct structure.

	On onother note I've created new demo graphics for a 
	little guy that runs around he looks really cool.

July 04, 2002 (Supplimental)
	I finally fixed the problem with the directinput object 
	not being a member of the input class.  Now it is a member.

July 19, 2002
	Added background class.  It supports up to ten different 
	backgrounds.  It can automatically determine how to display
	the background based on the viewport. I'm so damn glade
	I got it working.

	One thing I'd like to do is make it so that the background 
	has a wider varitety of scrolling speeds.

August 06, 2002
	The CImage class now has a DrawClippedImage function which 
	does clipping itself regardless whether there is a Clipper 
	or not, so now windowed mode is clipped.

September 05, 2002
	I implimented a Direct3D8 engine as well as the DirectDraw7 
	engine.  The Direct3D engine seems to have some downsides, 
	so the DD7 version is prefered. To select which engine to 
	compite it is necessary to universally define either USED3D 
	(Direct3D) or USEDD (DirectDraw).

September 13, 2002
	Made some modifications to the code, now windowed mode is not 
	determined by a define. The Direct3D8 engine windowed mode 
	now works correctly.
	
October 02, 2002
	Added functions to CObject class for collision detection,
	also made other improvements to the code.
	
	For demo purposes there is a crow the flys along the screen,
	if the controllable object collides with it the crow moves
	downward.	

	Sprites can also be made to loop forward, then backward

October 05, 2002
	The graphics functions are now in a dll called GFXGin.dll which
	solidly maintains the graphics functions.

February 05, 2003
	Much has been done to advance the engine.  It should be 
	noted that Collision Detection of the environment is in
	the making.  I have made a default collision detection
	(at least part of it), which I will work on more as time
	goes by.  I'm about ready to convert "Joes Copter 2" to
	ScrollGIN technology.  Freedom Fighter will take
	significantly more work.

	Right now I need to work on the Image Library for more
	advanced support.  In order to do that I need to complete
	my packaging library.

	As for the engine I need to complete:
		The CObjectManager class (along with ability 
		to apply AI)
		Sound Engine (Music and FX)
		Tile Manager support for libraries (Need to create 
		better library format)

February 08, 2003
	I've completed the default archtitecture adjustment it
	works pretty well considering.  Though if the movement
	ratio is more than 1:2 versus x movement against y
	problems may persist.

February 14, 2003
	I've created a CObject derivation called CCrowObject to
	show a basic example of how artificial intelligence on
	an object type basis will work.

	The CCrowObject works the same as CObject except
	without architecture collision detection.

February 22, 2003
	Over the past couple of days I have completely redone
	the graphics engine.  It now uses GFXG7 (GraFiX enGine
	directraw7).  The game never uses any bitmap loading
	from offsets (excpet int the sense that ImageLib does).
	On that note ImageLib has been implimented to load
	both Sprites and Tiles.  The old TileLibrary mode
	is no longer supported at all (*.ilf), only (*ilb)
	is supported.  I don't beleive I have any ilf files
	under release so I didn't bother creating backward
	compatibility.

	What is left to do:
		CObjectManager class;
		Sound Engine;

	Thats pretty much it.  I will begin developing the Object
	Manager soon, and it will be designed to get an AI from the
	specific program and not the engine.  I have to do some
	research to develop the sound engine.  Joes copter 2 will
	begin the development process soon, then I'll work out
	freedom fighter.

February 26, 2003
	The CObjectManager class is complete.  Except of course
	for it's ability to play sounds (which will probably
	be part of the CObject class).  In the file TestAI.cpp
	it can be seen how to utilize the object manager
	and CObject derived objects.

	I just ordered a book on game audio programming, so once
	it arrives I will be able to further ScrollGIN technology.

Febrary 27, 2003
	Timer now supports pausing, as well as other updates
	to the code.

January 9, 2004
	Well it has been a while since I have done, much work
	on this project, but I suddenly got this riduculous
	urge to generate a working model of Joes Copter 2.
	In order to accomplish this I have made some serious
	revisions to the game, adding a new class CScrollGINGame
	which holds everything to do with ScrollGIN into a class
	that can easily be plugged into a windows application.
	In the processed of developing a Joes Copter 2 demo I
	also discovered some bugs in the code which have now been
	resolved, and I made some enhancements.  The Joes Copter
	2 demo I have created pretty much has all the featuers of
	the DOS version, except it lacks sound.  Plus now whether
	or not the game is windowed is determined by a paramer 
	(-w or -window on the command line to make the game
	windowed).

	Things that need to be done for ScrollGIN:
		1) Add a sound engine.
		2) Create a more advanced collision engine to
			go along with the DefaultArchAdjust.
		3) Turn the engine into a DLL hiding most of the
			complex stuff, making it more simple to base
			games off the ScrollGIN engine.
		4) The innability for objects to go past the edge
			of the map.

	It's pretty amazing to think that I have been working on
	this project for 2 and a half years and I still know how
	it works.  Unfortunately I will probably not put much
	more work into it for a while as I am about to serve a
	mission for the Church of Jesus Christ of Latter-day Saints
	for two years.  I hope I still have interest in game
	development when I get back, and I hope I finally put
	a sound engine in this engine.

About:

ScrollGIN(tm) copyright(c) 2002, 2003, 2004 Blaine Myers.  ScrollGIN(tm), 
Beem Software, and all respective logos are a trademark and/or 
registered trademark of Blaine Myers in the US and/or other
countries.  All other logos/trademarks belong to their respective 
owners.