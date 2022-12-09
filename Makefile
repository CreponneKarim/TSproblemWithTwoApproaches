IMGUI_DIR:=/home/creponnekarim/my_progs/cpp_progs/zegour/pvc/imgui
IMGUI_BACKEND:=/home/creponnekarim/my_progs/cpp_progs/zegour/pvc/imgui/backends
GLAD_H:=/home/creponnekarim/my_progs/cpp_progs/zegour/pvc/include
GLAD_H2:=/home/creponnekarim/my_progs/cpp_progs/zegour/pvc/include/glad
GLAD_C:=/home/creponnekarim/my_progs/cpp_progs/zegour/pvc/src
SDL2=/usr/local/include/SDL2
FLAGS:=-lSDL2 -lSDL2_ttf -lglfw -lGL -lm -ldl 
CC:=g++ -std=c++17 -g

all:$(GLAD_C)/glad.c $(IMGUI_DIR)/imgui*.cpp $(IMGUI_BACKEND)/imgui_impl_sdl.cpp $(IMGUI_BACKEND)/imgui_impl_sdlrenderer.cpp 
	$(CC) -I $(IMGUI_DIR) -I $(IMGUI_BACKEND) -I $(GLAD_H) -I $(GLAD_H2) -I $(GLAD_C) -I $(SDL2) $^ main.cpp -o run  $(FLAGS) 