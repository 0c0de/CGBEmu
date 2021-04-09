#include <SDL.h>
#include <stdio.h>
#include <iostream>
#include "CPU.h"
#include "GUI.h"
#include <bitset>


using namespace std;

void runApp() {
	//SDL Magic Thing
	SDL_Window* mainWindow = NULL;
	SDL_Window *debuggerWindow = NULL;
	SDL_Renderer *render = NULL;
	SDL_Renderer *debuggerRender = NULL;

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	glewInit();

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	int cyclesMainLoop = 0;


	mainWindow = SDL_CreateWindow("CGB++", 0, 100, 160, 144, (SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI));
	debuggerWindow = SDL_CreateWindow("Debugger CGB++", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, (SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI));
	render = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_ACCELERATED);
	debuggerRender = SDL_CreateRenderer(debuggerWindow, 0, SDL_RENDERER_ACCELERATED);

	int MAXCYCLES = 70224;
	float FPS = 59.73f;
	float DELAY_TIME = 1000.0f / FPS;

	SDL_GLContext gl_context = SDL_GL_CreateContext(debuggerWindow);
	SDL_GL_MakeCurrent(debuggerWindow, gl_context);
	SDL_GL_SetSwapInterval(1); // Enable vsync

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	ImGui_ImplSDL2_InitForOpenGL(debuggerWindow, gl_context);
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
	//gameboy.loadGame("games/hello_world.gb");
	//For PC
	//gameboy.loadGame("E:/tetris.gb");
	gameboy.loadGame("E:/Dr. Mario (World).gb");
	//gameboy.loadGame("E:/cpu_instrs.gb");
	//For laptop
	//gameboy.loadGame("C:/ROMS/Tetris.gb");
	std::thread t1(&CPU::runCPU, &gameboy, &gpu, render);
	//gameboy.runLife();
	//gameboy.loadGame("E:/hello-world.gb");


	if (mainWindow != NULL) {


		SDL_RenderClear(render);
		bool isEmuRunning = true;
		SDL_Event sdlEvent;

		//Infinite loop for running gameboy
		while (isEmuRunning) {

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
							break;
						default:
							break;
					}
						
				}

				if (sdlEvent.type == SDL_KEYDOWN) {
					if (sdlEvent.key.keysym.sym == SDLK_z) {
						gameboy.setKey(4);
					}

					if (sdlEvent.key.keysym.sym == SDLK_x) {
						gameboy.setKey(5);
					}

					if (sdlEvent.key.keysym.sym == SDLK_SPACE) {
						gameboy.setKey(6);
					}

					if (sdlEvent.key.keysym.sym == SDLK_KP_ENTER) {
						gameboy.setKey(7);
					}

					if (sdlEvent.key.keysym.sym == SDLK_UP) {
						gameboy.setKey(2);
					}

					if (sdlEvent.key.keysym.sym == SDLK_DOWN) {
						gameboy.setKey(3);
					}

					if (sdlEvent.key.keysym.sym == SDLK_LEFT) {
						gameboy.setKey(1);
					}

					if (sdlEvent.key.keysym.sym == SDLK_RIGHT) {
						gameboy.setKey(0);
					}
				}


				if (sdlEvent.type == SDL_KEYUP) {
					if (sdlEvent.key.keysym.sym == SDLK_z) {
						gameboy.releaseKey(4);
					}

					if (sdlEvent.key.keysym.sym == SDLK_x) {
						gameboy.releaseKey(5);
					}

					if (sdlEvent.key.keysym.sym == SDLK_SPACE) {
						gameboy.releaseKey(6);
					}

					if (sdlEvent.key.keysym.sym == SDLK_KP_ENTER) {
						gameboy.releaseKey(7);
					}

					if (sdlEvent.key.keysym.sym == SDLK_UP) {
						gameboy.releaseKey(2);
					}

					if (sdlEvent.key.keysym.sym == SDLK_DOWN) {
						gameboy.releaseKey(3);
					}

					if (sdlEvent.key.keysym.sym == SDLK_LEFT) {
						gameboy.releaseKey(1);
					}

					if (sdlEvent.key.keysym.sym == SDLK_RIGHT) {
						gameboy.releaseKey(0);
					}
				}
			}


			//gameboy.runCPU(&gpu, render);


			MMU *mmuValues = gameboy.getMMUValues();

			ImGui_ImplOpenGL2_NewFrame();
			ImGui_ImplSDL2_NewFrame(debuggerWindow);
			ImGui::NewFrame();

			GameboyFlags *flagState = gameboy.getFlagState();
			GameboyRegisters *reg = gameboy.getGameboyRegisters();

			drawMMU(mmuValues);
			drawFlags(flagState, reg, &gpu, &gameboy);

			//Renders to the screen all things
			// Rendering
			ImGui::Render();
			glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
			glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
			glClear(GL_COLOR_BUFFER_BIT);
			//glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound
			ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
			SDL_GL_SwapWindow(debuggerWindow);
		}
	}

	//SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(mainWindow);
	SDL_Quit();
}


int main(int argc, char* argv[]) {
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		cout << "SDL Initiated correctly" << endl;
		runApp();
	}
	else {
		cout << "An error ocurred when initiating SDL: " << SDL_GetError() << endl;
		SDL_Quit();
	}
	SDL_Delay(500);
	return 0;
}