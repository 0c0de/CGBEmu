#include <SDL.h>
#include <SDL_opengl.h>
#include <stdio.h>
#include <iostream>
#include "CPU.h"
#include "GPU.h"
#include "GUI.h"
#include <bitset>

using namespace std;

void runApp() {
	//SDL Magic Thing
	SDL_Window* mainWindow;
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);


	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	mainWindow = SDL_CreateWindow("Gameboy Emulator C++", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, (SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI));

	SDL_GLContext gl_context = SDL_GL_CreateContext(mainWindow);
	SDL_GL_MakeCurrent(mainWindow, gl_context);
	SDL_GL_SetSwapInterval(1); // Enable vsync

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	ImGui_ImplSDL2_InitForOpenGL(mainWindow, gl_context);
	ImGui_ImplOpenGL2_Init();

	//Instantiate class GPU of the gameboy emulator
	GPU gpu;

	//Instantiate class CPU of the gameboy emulator
	CPU gameboy;
	//Init gameboy with some default values
	gameboy.init();
	//Load the bios of the GameBoy
	//gameboy.loadBIOS();
	//Load the game specified
	//gameboy.loadGame("games/test01CPU.gb");
	gameboy.loadGame("F:/Tetris.gb");
	std::thread t1(&CPU::runCPU, &gameboy);
	//gameboy.runLife();
	//gameboy.loadGame("F:/hello-world.gb");
	if (mainWindow != NULL) {
		bool isEmuRunning = true;
		//Infinite loop for running gameboy
		while (isEmuRunning) {
			SDL_Event sdlEvent;
			//SDL_Delay(150);
			//Handle event
			while (SDL_PollEvent(&sdlEvent)) {
				ImGui_ImplSDL2_ProcessEvent(&sdlEvent);
				//Test things
				if(sdlEvent.type == SDL_WINDOWEVENT) {
					switch (sdlEvent.window.event)
					{
						case SDL_WINDOWEVENT_RESIZED:
							cout << "Window resized" << endl;
							break;
						case SDL_WINDOWEVENT_CLOSE:
							cout << "Closing window" << endl;
							isEmuRunning = false;
						default:
							break;
					}
						
				}
			}

			ImGui_ImplOpenGL2_NewFrame();
			ImGui_ImplSDL2_NewFrame(mainWindow);
			ImGui::NewFrame();


			MMU *mmuValues = gameboy.getMMUValues();
			GameboyFlags *flagState = gameboy.getFlagState();
			GameboyRegisters *reg = gameboy.getGameboyRegisters();


			gpu.step(&gameboy, mmuValues);

			drawMMU(mmuValues);
			drawFlags(flagState, reg, &gpu);


			//Renders to the screen all things
			// Rendering
			ImGui::Render();
			glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
			glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
			glClear(GL_COLOR_BUFFER_BIT);
			//glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound
			ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
			SDL_GL_SwapWindow(mainWindow);
		}
	}

	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(mainWindow);
	SDL_Quit();
}


int main(int argc, char* argv[]) {
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		cout << "SDL Initiated correctly" << endl;
		runApp();
	}
	else {
		SDL_Quit();
	}
	SDL_Delay(500);
	return 0;
}