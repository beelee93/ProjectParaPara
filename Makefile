GAME_OBJS = GODefaultArrow.o GOHeader.o GOPinkFlash.o \
	GOScore.o GOShockwave.o GOStationaryArrow.o \
	GOTitle.o GOFontSheet.o

OBJS = ErrorHandler.o Game.o GameObject.o \
	   GameObjectManager.o Sprite.o SpriteLoader.o \
	   GameParaPara.o GOMParaPara.o Main.o \
	   SDL_FontCache.o RenderQueue.o Audio.o \
	   ArrowList.o InputParaPara.o \
	   $(GAME_OBJS)

GOH = Globals.h GameObject.h
NORMAL_LIBS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer 

ifeq ($(rpi), 1)
	LIBS = $(NORMAL_LIBS) -lwiringPi
	CFLAGS = -c -D RPI
else
	LIBS = $(NORMAL_LIBS)
	CFLAGS = -c
endif

CC = g++ -std=c++11

install : $(OBJS)
	$(CC) $(OBJS) $(DIRS) $(LIBS) -o para

ErrorHandler.o : ErrorHandler.h ErrorHandler.cpp
	$(CC) $(CFLAGS) ErrorHandler.cpp

Game.o : GameObject.h GameObjectManager.h Sprite.h \
		 SpriteLoader.h Game.h ErrorHandler.h Audio.h Game.cpp
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

RenderQueue.o : RenderQueue.h GameObject.h RenderQueue.cpp
	$(CC) $(CFLAGS) $(LIBS) RenderQueue.cpp

Audio.o : Audio.h Audio.cpp
	$(CC) $(CFLAGS) $(LIBS) Audio.cpp

ArrowList.o : ArrowList.h ArrowList.cpp
	$(CC) $(CFLAGS) ArrowList.cpp

SDL_FontCache.o : SDL_FontCache.h SDL_FontCache.c
	$(CC) $(CFLAGS) $(LIBS) SDL_FontCache.c

GOMParaPara.o : Globals.h GameObjectManager.h GOMParaPara.cpp
	$(CC) $(CFLAGS) $(LIBS) GOMParaPara.cpp

GODefaultArrow.o : $(GOH) GODefaultArrow.cpp
	$(CC) $(CFLAGS) $(LIBS) GODefaultArrow.cpp

GOShockwave.o : $(GOH) GOShockwave.cpp
	$(CC) $(CFLAGS) $(LIBS) GOShockwave.cpp

GOFontSheet.o : $(GOH) GOFontSheet.cpp
	$(CC) $(CFLAGS) $(LIBS) GOFontSheet.cpp

GOStationaryArrow.o : $(GOH) GOStationaryArrow.cpp
	$(CC) $(CFLAGS) $(LIBS) GOStationaryArrow.cpp

GOPinkFlash.o : $(GOH) GOPinkFlash.cpp
	$(CC) $(CFLAGS) $(LIBS) GOPinkFlash.cpp

GOHeader.o : $(GOH) GOHeader.cpp
	$(CC) $(CFLAGS) $(LIBS) GOHeader.cpp

GOTitle.o : $(GOH) GOTitle.cpp
	$(CC) $(CFLAGS) $(LIBS) GOTitle.cpp

GOScore.o : $(GOH) GOScore.cpp
	$(CC) $(CFLAGS) $(LIBS) GOScore.cpp

GameParaPara.o : Game.h Globals.h GameParaPara.h GameParaPara.cpp
	$(CC) $(CFLAGS) $(LIBS) GameParaPara.cpp

InputParaPara.o : InputParaPara.h InputParaPara.cpp
	$(CC) $(CFLAGS) $(LIBS) InputParaPara.cpp

Main.o : Game.h Globals.h Main.cpp
	$(CC) $(CFLAGS) $(LIBS) Main.cpp

clean :
	rm -rf *.o

