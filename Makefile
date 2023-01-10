CC=gcc
DBG=gdb
BIN=main
SRC=main.c
OBJS=main.o
IFLAGS=-Isrc/include
LFLAGS=-Lsrc/lib
CFLAGS=-g -Wall -Wextra -pedantic -std=c11

all: compile link

compile: $(OBJS)
	@echo Compiling source files...
	$(CC) -o $(BIN) $(OBJS) $(IFLAGS) $(LFLAGS)
	@echo Done compiling...

link:
	@echo Linking object files...
	$(CC) -c $(SRC)
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
	del *.o *.exe
	@echo Done cleaning...