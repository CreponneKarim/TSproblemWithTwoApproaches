IMGUI_DIR:=./imgui
IMGUI_BACKEND:=./imgui/backends
GLAD_H:=./include
GLAD_H2:=./include/glad
GLAD_C:=./src
SDL2=/usr/local/include/SDL2
FLAGS:=-lSDL2 -lSDL2_ttf -lglfw -lGL -lm -ldl 
CC:=g++ -std=c++17 -g

all:$(GLAD_C)/glad.c $(IMGUI_DIR)/imgui*.cpp $(IMGUI_BACKEND)/imgui_impl_sdl2.cpp $(IMGUI_BACKEND)/imgui_impl_sdlrenderer2.cpp 
	$(CC) -I $(IMGUI_DIR) -I $(IMGUI_BACKEND) -I $(GLAD_H) -I $(GLAD_H2) -I $(GLAD_C) -I $(SDL2) $^ main.cpp -o run  $(FLAGS) 
