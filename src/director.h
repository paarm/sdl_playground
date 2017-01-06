#pragma once
#include "utils/clock.h"
#include "texturemanager.h"
#include "node.h"

class Director {
private:
	SDL_Window 		*mSDL_Window=nullptr;
	SDL_Renderer 	*mSDL_Renderer=nullptr;
	bool 			mIsInitialized=false;
	Node			mRootScene;
	Node			*mStageScene=nullptr;
	Node			*mCurrentScene=nullptr;
	Clock 			clock;
	Director();
	Director(const Director&) = delete;
	Director& operator=(const Director&)=delete;
public:
	static Director* getInstance() {
		static Director *instance=new Director(); 
		return instance;
	}
	~Director();
	void initialize();
	void switchScene(Node *n);
	void runWithNode(Node *n);
};