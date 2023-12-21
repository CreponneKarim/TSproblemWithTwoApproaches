#!/bin/bash

sudo apt install make
sudo apt install build-essential
sudo apt install libsdl2-dev 
sudo apt install libsdl2-ttf-dev
sudo apt-get install libglfw3
sudo apt install gh
gh repo clone ocornut/imgui

sed -i 's/SDL.h/SDL2\/SDL.h/g' ./imgui/backends/imgui_impl_sdlrenderer2.cpp
sed -i 's/SDL.h/SDL2\/SDL.h/g' ./imgui/backends/imgui_impl_sdl2.cpp
sed -i 's/SDL_syswm.h/SDL2\/SDL_syswm.h/g' ./imgui/backends/imgui_impl_sdlrenderer2.cpp
sed -i 's/SDL_syswm.h/SDL2\/SDL_syswm.h/g' ./imgui/backends/imgui_impl_sdl2.cpp
