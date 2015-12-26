OBJS = ErrorHandler.o Game.o GameObject.o \
	   GameObjectManager.o Sprite.o SpriteLoader.o \
	   GameParaPara.o GOMParaPara.o GODefaultArrow.o \
	   GODownArrow.o Main.o

GOH = Globals.h GameObject.h

LIBS = -lSDL2 -lSDL2_image
CFLAGS = -c -I/usr/include/SDL2
CC = g++

para : $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o para
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

GameObjectManager.o : Queue.h GameObjectManager.h \
					  GameObject.h Sprite.h ErrorHandler.h \
					  GameObjectManager.cpp
	$(CC) $(CFLAGS) $(LIBS) GameObjectManager.cpp

SpriteLoader.o : SpriteLoader.h Sprite.h ErrorHandler.h SpriteLoader.cpp
	$(CC) $(CFLAGS) $(LIBS) SpriteLoader.cpp

GOMParaPara.o : Globals.h GameObjectManager.h GOMParaPara.cpp
	$(CC) $(CFLAGS) $(LIBS) GOMParaPara.cpp

GODefaultArrow.o : $(GOH) GODefaultArrow.cpp
	$(CC)  $(CFLAGS) $(LIBS) GODefaultArrow.cpp

GODownArrow.o : $(GOH) GODownArrow.cpp
	$(CC)  $(CFLAGS) $(LIBS) GODownArrow.cpp

GameParaPara.o : Game.h Globals.h GameParaPara.cpp
	$(CC)  $(CFLAGS) $(LIBS) GameParaPara.cpp

Main.o : Game.h Globals.h Main.cpp
	$(CC)  $(CFLAGS) $(LIBS) Main.cpp

clean :
	rm -rf *.o
