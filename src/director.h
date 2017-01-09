#pragma once
#include "utils/clock.h"
#include "texturemanager.h"
#include "node.h"

enum class MousePointerAlignment {
	LeftTop=0,
	Middle,
};

struct CustomMouseIcon {
	MousePointerAlignment mMousePointerAlignment;
	SDL_Texture *mSDL_Texture;
	SDL_Rect 	mSDL_DestRect;
	int 		mOffsetX;
	int 		mOffsetY;
};

class Director {
private:
	SDL_Window 		*mSDL_Window=nullptr;
	SDL_Renderer 	*mSDL_Renderer=nullptr;
	bool 			mIsInitialized=false;
	Node			mRootScene;
	Node			*mStageScene=nullptr;
	Node			*mCurrentScene=nullptr;
	Clock 			clock;
	CustomMouseIcon mCustomMouseIcon;
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
	void setMousePointer(SDL_Texture *rSDL_Texture, MousePointerAlignment rMousePointerAlignment=MousePointerAlignment::LeftTop);
};