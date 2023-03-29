OBJS = main.cpp types.cpp render.cpp player.cpp map.cpp # All files needed to build
OBJ_NAME = builds/game # Final executable name
CC = g++
COMPILER_FLAGS = -Wall -Wextra -std=c++20
LINKER_FLAGS = -lSDL2 -lSDL2_ttf -lSDL2_image


all: build run

.PHONY : build
build: $(OBJ_NAME) $(OBJS)

$(OBJ_NAME): $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

.PHONY : clean
clean:
	rm $(OBJ_NAME)

.PHONY : run
run:
	./$(OBJ_NAME)