OBJS = ErrorHandler.o Game.o GameObject.o \
	   GameObjectManager.o Sprite.o SpriteLoader.o \
	   GameParaPara.o GOMParaPara.o GODefaultArrow.o \
	   GOStationaryArrow.o Main.o SDL_FontCache.o Audio.o

GOH = Globals.h GameObject.h

LIBS = -lSDL2 -lSDL2_image -lSDL2_ttf -lIrrKlang
DIRS = -I/usr/include/SDL2 -I/usr/local/include/SDL2 -I./include -Llib
CFLAGS = -c $(DIRS)
CC = g++

install : $(OBJS)
	$(CC) $(OBJS) $(DIRS) $(LIBS) -o para
	echo "Copying libraries to /usr/local/lib"
	cp -f lib/* /usr/local/lib
	echo "Running ldconfig to register libraries"
	ldconfig
	rm -rf *.o

ErrorHandler.o : ErrorHandler.h ErrorHandler.cpp
	$(CC) $(CFLAGS) ErrorHandler.cpp

Game.o : GameObject.h GameObjectManager.h Sprite.h \
		 SpriteLoader.h Game.h ErrorHandler.h
	$(CC) $(CFLAGS) $(LIBS) Game.cpp

Sprite.o : Sprite.h ErrorHandler.h Sprite.cpp
	$(CC) $(CFLAGS) $(LIBS) Sprite.cpp

GameObject.o : Globals.h GameObject.h Sprite.h \
			   ErrorHandler.h GameObject.cpp
	$(CC) $(CFLAGS) $(LIBS) GameObject.cpp

GameObjectManager.o : GameObjectManager.h \
					  GameObject.h Sprite.h ErrorHandler.h \
					  GameObjectManager.cpp
	$(CC) $(CFLAGS) $(LIBS) GameObjectManager.cpp

SpriteLoader.o : SpriteLoader.h Sprite.h ErrorHandler.h SpriteLoader.cpp
	$(CC) $(CFLAGS) $(LIBS) SpriteLoader.cpp

Audio.o : ErrorHandler.h Audio.cpp
	$(CC) $(CFLAGS) $(LIBS) Audio.cpp

SDL_FontCache.o : SDL_FontCache.h SDL_FontCache.c
	$(CC) $(CFLAGS) $(LIBS) SDL_FontCache.c

GOMParaPara.o : Globals.h GameObjectManager.h GOMParaPara.cpp
	$(CC) $(CFLAGS) $(LIBS) GOMParaPara.cpp

GODefaultArrow.o : $(GOH) GODefaultArrow.cpp
	$(CC)  $(CFLAGS) $(LIBS) GODefaultArrow.cpp

GOStationaryArrow.o : $(GOH) GOStationaryArrow.cpp
	$(CC)  $(CFLAGS) $(LIBS) GOStationaryArrow.cpp

GameParaPara.o : Game.h Globals.h GameParaPara.cpp
	$(CC)  $(CFLAGS) $(LIBS) GameParaPara.cpp

Main.o : Game.h Globals.h Main.cpp
	$(CC)  $(CFLAGS) $(LIBS) Main.cpp

clean :
	rm -rf *.o

uninstall :
	echo "Removing libraries from /usr/local/lib"
	rm -rf /usr/local/lib/ikpMP3.so
	rm -rf /usr/local/lib/ikpFlac.so
	rm -rf /usr/local/lib/libIrrKlang.so
	rm -rf para
	ldconfig
	echo "Done"
