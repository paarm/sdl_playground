#pragma once
#include <string>
#include <iostream>
#include <map>
#include <SDL.h>
#include <SDL_image.h>

using namespace std;

class TextureManager {
private:
	map<string, SDL_Texture*> mTextureMap;
	map<string, string> mAliasMap;
	SDL_Renderer *mSDL_Renderer;
	TextureManager();
	bool proofSDLRenderer();
	string* getPathNameFromAlias(const string *alias);
	
public:
	static TextureManager & getInstance() {
		static TextureManager instance;
		return instance;
	}
	~TextureManager();
	TextureManager(const TextureManager&) = delete;
	void operator=(const TextureManager&) = delete;
	void setSDLRenderer(SDL_Renderer*);
	SDL_Texture* loadTexture(const string &pathName, const string* alias=nullptr);
	SDL_Texture* loadTexture(const char *pathName, const char *alias=nullptr);
	SDL_Texture* getTexture(const string &name);
	SDL_Texture* getTexture(const char*name);
};



