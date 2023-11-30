CC=gcc
DBG=gdb
BIN=src/main.exe
SRC=src/main.c src/input.c src/render.c src/sim.c src/parseLevel.c src/audio.c src/rmathlib.c
OBJS=main.o input.o render.o sim.o parseLevel.o audio.o rmathlib.o
IFLAGS=-Isrc/include
LFLAGS=-Lsrc/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
CFLAGS=-g -Wall -Wextra -pedantic -std=c11 -Wno-unused-parameter

all: compile link

#Compiling .c files into .o object files
compile: 
	$(CC) $(SRC) $(CFLAGS) -c $(IFLAGS)

#Linking .o objects into .exe file
link: 
	$(CC) $(OBJS) -o $(BIN) $(LFLAGS)

run: all
	$(BIN)

debug: all
	$(DBG) $(BIN)

clean:
	del *.o *.exe src\*.o src\*.exe