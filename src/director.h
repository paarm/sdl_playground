#pragma once
#include "utils/clock.h"
#include "texturemanager.h"

class Director {
private:
	Node			mRootScene;
	Node			*mStageScene=nullptr;
	Node			*mCurrentScene=nullptr;
	bool 			mIsInitialized=false;
	SDL_Window 		*mSDL_Window=nullptr;
	SDL_Renderer 	*mSDL_Renderer=nullptr;

	Clock clock;
	Director() {
		mRootScene.scheduleUpdate(true);
	}
	Director(const Director&) = delete;

public:
	~Director() {
		if (mSDL_Renderer) SDL_DestroyRenderer(mSDL_Renderer);
		mSDL_Renderer=nullptr;
		if (mSDL_Window) SDL_DestroyWindow(mSDL_Window);
		mSDL_Window=nullptr;
		SDL_Quit();
	}
	void initialize() {
		//init SDL2
		if (!SDL_Init(SDL_INIT_VIDEO)) {
			// open window
			mSDL_Window = SDL_CreateWindow("Hello World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
			if (mSDL_Window) {
				mSDL_Renderer = SDL_CreateRenderer(mSDL_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
				if (mSDL_Renderer) {
					TextureManager::getInstance().setSDLRenderer(mSDL_Renderer);
					mIsInitialized=true;
				} else {
					std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
				}
			} else {
				std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
			}
		} else {
			std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		}
	}
	void switchScene(Node *n) {
		mStageScene=n;
	}

	void runWithNode(Node *n) {
		switchScene(n);
		if (mIsInitialized) {
			bool quit=false;
			while (!quit) {
				clock.tick();
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
				// update the nodes
				mRootScene.updateInternal(clock.getDelta());
				//First clear the renderer
				SDL_RenderClear(mSDL_Renderer);
				// draw the hole node tree
				mRootScene.draw(mSDL_Renderer,0,0);
				//Update the screen
				SDL_RenderPresent(mSDL_Renderer);
				if (quit==true) {
					break;
				}
				if (mStageScene) {
					mRootScene.deleteChilds();
					mRootScene.addNode(mStageScene);
					mStageScene=nullptr;
				}
			}
			mRootScene.deleteChilds();
		}
	}
	static Director* getInstance() {
		static Director *instance=new Director(); 
		return instance;
	}
};