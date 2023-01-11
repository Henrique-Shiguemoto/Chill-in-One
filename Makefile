CC=gcc
DBG=gdb
BIN=src/main.exe
SRC=src/main.c
OBJS=src/main.o
IFLAGS=-Isrc/include -Isrc/vendor/mthlibc
LFLAGS=-Lsrc/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
CFLAGS=-g -Wall -Wextra -pedantic -std=c11

all: compile link

compile: #Compiling .c files into .o object files
	@echo Compiling and Assembling source files...
	$(CC) $(SRC) $(CFLAGS) -c -o $(OBJS) $(IFLAGS)
	@echo Done compiling and assembling...

link: #Linking .o objects into .exe file
	@echo Linking object files...
	$(CC) $(OBJS) -o $(BIN) $(LFLAGS)
	@echo Done linking...

run: all
	@echo Running executable...
	$(BIN)
	@echo Done running...

debug: $(BIN)
	@echo Starting debugger...
	$(DBG) $(BIN)
	@echo Done debugging...

clean:
	@echo Cleaning garbage files...
	del src\*.o src\*.exe
	@echo Done cleaning...