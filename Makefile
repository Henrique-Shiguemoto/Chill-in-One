CC=gcc
DBG=gdb
BIN=src/main.exe
SRC=src/main.c src/vendor/mthlibc/mthlib.c src/input.c src/render.c src/sim.c src/parseLevel.c 
OBJS=main.o mthlib.o input.o render.o sim.o parseLevel.o
IFLAGS=-Isrc/include -Isrc/vendor/mthlibc
LFLAGS=-Lsrc/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
CFLAGS=-g -Wall -Wextra -pedantic -std=c11

all: compile link

#Compiling .c files into .o object files
compile: 
	@echo Compiling and Assembling source files...
	$(CC) $(SRC) $(CFLAGS) -c $(IFLAGS)
	@echo Done compiling and assembling...

#Linking .o objects into .exe file
link: 
	@echo Linking object files...
	$(CC) $(OBJS) -o $(BIN) $(LFLAGS)
	@echo Done linking...

run: all
	@echo Running executable...
	$(BIN)
	@echo Done running...

debug: all
	@echo Starting debugger...
	$(DBG) $(BIN)
	@echo Done debugging...

clean:
	@echo Cleaning garbage files...
	del *.o *.exe src\*.o src\*.exe
	@echo Done cleaning...