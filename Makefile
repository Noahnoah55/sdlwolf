OBJS = $(wildcard *.cpp)
OBJ_NAME = builds/game # Final executable name
IMGUI_SRCS = $(wildcard includes/imgui/*.cpp) $(wildcard includes/imgui/backends/*.cpp)
IMGUI_OBJS = $(addprefix builds/, $(addsuffix .o, $(basename $(notdir $(IMGUI_SRCS)))))
IMGUI_DIR = includes/imgui/
IMGUI_FLAGS = -I$(IMGUI_DIR) -I$(IMGUI_DIR)backends `sdl2-config --cflags`
IMGUI_LIBS = -lGL -ldl `sdl2-config --libs`
CC = g++
COMPILER_FLAGS = -Wall -Wextra -std=c++20
LINKER_FLAGS = -lSDL2_ttf -lSDL2_image -llua5.4


all: build

build: $(OBJS) imgui
	$(CC) $(OBJS) $(IMGUI_OBJS) $(COMPILER_FLAGS) $(IMGUI_LIBS) $(LINKER_FLAGS) -o $(OBJ_NAME)

builds/%.o: $(IMGUI_DIR)/%.cpp
	$(CC) $(IMGUI_FLAGS) -c -o $@ $<

builds/%.o: $(IMGUI_DIR)backends/%.cpp
	$(CC) $(IMGUI_FLAGS) -c -o $@ $<

imgui: $(IMGUI_OBJS)
	@echo Imgui Built

.PHONY : clean
clean:
	rm $(OBJ_NAME)

.PHONY : run
run:
	./$(OBJ_NAME)