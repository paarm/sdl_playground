#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "node.h"
#include "texturemanager.h"
//#include "res_path.h"

/*
 * Lesson 0: Test to make sure SDL is setup properly
 */

int main(int, char**) {
	//init SDL2
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}
	// open window
	SDL_Window *win = SDL_CreateWindow("Hello World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
	if (win == nullptr) {
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}
	// create renderer
	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == nullptr) {
		SDL_DestroyWindow(win);
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	TextureManager::getInstance().setSDLRenderer(ren);
	TextureManager::getInstance().loadTexture("assets/test.png", "test");
	TextureManager::getInstance().loadTexture("assets/test.png", "test2");

	// load bitmap
	//std::string imagePath = getResourcePath("Lesson1") + "test.png";
	//SDL_Surface *bmp = SDL_LoadBMP("test.bmp");//SDL_LoadBMP("test.png"/*imagePath.c_str()""*/);
	SDL_Surface *bmp = IMG_Load("assets/test.png");//SDL_LoadBMP("test.png"/*imagePath.c_str()""*/);
	if (bmp == nullptr) {
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
		std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}
	// create texture from surface (bitmap)
	SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, bmp);
	SDL_FreeSurface(bmp);
	if (tex == nullptr) {
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
		std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}
	bool quit = false;

	Node* n=new Node(string("root"));
	n->addNode(new Node(string("N 1")))->addNode(new Node(string("N 1.1")));
	n->addNode(new Node(string("N 2")))->addNode(new Node(string("N 2.1")));
	n->searchNode(string("N 1.1"), true)->debugPrint();
	//n->debugPrint();
	

	delete n;

	while (!quit) {
		SDL_Event e;
		SDL_PollEvent(&e);
		if (e.type == SDL_QUIT) {
			quit = true;
			std::cout << "SDL_QUIT" << endl;
		}
		if (e.type == SDL_KEYDOWN) {
			quit = true;
			std::cout << "SDL_KEYDOWN" << endl;
		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {
			quit = true;
			std::cout << "SDL_MOUSEBUTTONDOWN" << endl;
		}
		//First clear the renderer
		SDL_RenderClear(ren);
		//Draw the texture
		SDL_RenderCopy(ren, TextureManager::getInstance().getTexture(string("test2")), NULL, NULL);
		//Update the screen
		SDL_RenderPresent(ren);
		if (quit==true) {
			break;
		}
	}
	SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}
