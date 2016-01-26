OBJS = ErrorHandler.o Game.o GameObject.o \
	   GameObjectManager.o Sprite.o SpriteLoader.o \
	   GameParaPara.o GOMParaPara.o GODefaultArrow.o \
	   GOStationaryArrow.o Main.o SDL_FontCache.o \
	   RenderQueue.o GOPinkFlash.o Audio.o \
	   GOShockwave.o ArrowList.o

GOH = Globals.h GameObject.h

LIBS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
CFLAGS = -c
CC = g++

install : $(OBJS)
	$(CC) $(OBJS) $(DIRS) $(LIBS) -o para
	rm -rf *.o

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

GOStationaryArrow.o : $(GOH) GOStationaryArrow.cpp
	$(CC) $(CFLAGS) $(LIBS) GOStationaryArrow.cpp

GOPinkFlash.o : $(GOH) GOPinkFlash.cpp
	$(CC) $(CFLAGS) $(LIBS) GOPinkFlash.cpp

GameParaPara.o : Game.h Globals.h GameParaPara.h GameParaPara.cpp
	$(CC) $(CFLAGS) $(LIBS) GameParaPara.cpp

Main.o : Game.h Globals.h Main.cpp
	$(CC) $(CFLAGS) $(LIBS) Main.cpp

clean :
	rm -rf *.o

