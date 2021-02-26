#pragma once
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl2.h"
#include "mmu.h"
#include <string>  
#include <iostream> 
#include <sstream>
#include <SDL.h>
#include "CPU.h"
#include "GPU.h"
#include "imgui_memory_editor.h"

void drawMMU(MMU *mmu);

void drawFlags(GameboyFlags *flags, GameboyRegisters *reg, GPU *gpu, CPU *gameboy);

void drawGame(GLuint gameTexture);