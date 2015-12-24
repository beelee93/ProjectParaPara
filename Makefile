OBJS = ErrorHandler.o Game.o
LIBS = -lSDL2 -lSDL2_image
INCDIR = -I/usr/include/SDL2
CFLAGS = -c
CC = gcc

para : $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o para

ErrorHandler.o : ErrorHandler.h ErrorHandler.c
	$(CC) $(CFLAGS) ErrorHandler.c

Game.o : Game.h ErrorHandler.h Game.c
	$(CC) $(INCDIR) $(CFLAGS) $(LIBS) Game.c

clean :
	rm -rf *.o
