OBJS = $(wildcard *.cpp)
OBJ_NAME = builds/game # Final executable name
CC = g++
COMPILER_FLAGS = -Wall -Wextra -std=c++20
LINKER_FLAGS = -lSDL2 -lSDL2_ttf -lSDL2_image -llua5.4 -limgui


all: build

build: $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

.PHONY : clean
clean:
	rm $(OBJ_NAME)

.PHONY : run
run:
	./$(OBJ_NAME)