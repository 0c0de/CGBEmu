#include <SDL.h>
#include <stdio.h>
#include <iostream>
#include "CPU.h"

using namespace std;

void runApp() {
	SDL_Window* mainWindow;
	mainWindow = SDL_CreateWindow("CGBEmu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600,600, SDL_WINDOW_RESIZABLE);
	SDL_Renderer* render;
	render = SDL_CreateRenderer(mainWindow, 0, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(render, 255, 170, 170, 255);
	SDL_Rect rect;
	rect.h = 600;
	rect.w = 600;
	rect.x = 0;
	rect.y = 0;
	SDL_RenderFillRect(render, &rect);
	SDL_SetWindowSize(mainWindow, 600, 600);
	CPU gameboy;
	gameboy.init();
	//gameboy.loadBIOS();
	gameboy.loadGame("games/hello_world.gb");
	if (mainWindow != NULL) {
		bool isEmuRunning = true;

		while (isEmuRunning) {
			SDL_Event sdlEvent;
			gameboy.runLife();
			SDL_Delay(1);
			while (SDL_PollEvent(&sdlEvent)) {
				//Test things
				if(sdlEvent.type == SDL_WINDOWEVENT) {
					switch (sdlEvent.window.event)
					{
						case SDL_WINDOWEVENT_RESIZED:
							cout << "Window resized" << endl;
							int x, y;
							SDL_GetWindowSize(mainWindow, &x, &y);
							rect.h = y;
							rect.w = x;
							SDL_RenderFillRect(render, &rect);
							SDL_SetWindowSize(mainWindow, x, y);
							
							break;
						default:
							break;
					}
						
				}
			}
			SDL_RenderPresent(render);
		}
	}
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