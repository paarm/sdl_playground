#include "texturemanager.h"

TextureManager::TextureManager() {
	//mTextureMap = new map<string, SDL_Texture*>();
}

TextureManager::~TextureManager() {
	for(const auto &it : mTextureMap) {
		std::cout << "Texture destroyed: " << it.first << std::endl;

		SDL_DestroyTexture(it.second);
	}
	mTextureMap.clear();
	mAliasMap.clear();
//	std::map<string, SDL_Texture*>::iterator it=mTextureMap.begin();
//	while(it!=mTextureMap.end()) {
//		SDL_DestroyTexture(it.second());
//		it++;
//	}
}
void TextureManager::setSDLRenderer(SDL_Renderer *rSDL_Renderer) {
	mSDL_Renderer=rSDL_Renderer;
}

bool TextureManager::proofSDLRenderer() {
	if (!mSDL_Renderer) {
		std::cout << "SDL_Renderer not available. Set a SDL renderer with setSDLRenderer" << std::endl;
		return false;
	}
	return true;
}

SDL_Texture* TextureManager::loadTexture(const char *pathName, const char* alias) {
	SDL_Texture *rv=nullptr;
	if (pathName && strlen(pathName)>1) {
		if (alias) {
			string al=string(alias);
			rv=loadTexture(string(pathName), &al);
		} else {
			rv=loadTexture(string(pathName), nullptr);
		}
	}
	return rv;
}


SDL_Texture *TextureManager::loadTexture(const string &pathName, const string *alias) {
	SDL_Texture *rv=nullptr;
	if (proofSDLRenderer()) {
		rv=TextureManager::getTexture(pathName);
		if (!rv) {
			SDL_Surface *bmp = IMG_Load(pathName.c_str());//SDL_LoadBMP("test.png"/*imagePath.c_str()""*/);
			if (bmp == nullptr) {
				std::cout << "IMG_Load Error: " << SDL_GetError() << std::endl;
			} else {
				std::cout << "Load Texture: " << pathName;
				if (alias) {
					std::cout << " as alias: " <<*alias << std::endl;
				} else {
					std::cout << std::endl;
				}
				// create texture from surface (bitmap)
				rv = SDL_CreateTextureFromSurface(mSDL_Renderer, bmp);
				if (rv) {
					mTextureMap[pathName]=rv;
					if (alias) {
						mAliasMap[*alias]=pathName;
					}
				}
				SDL_FreeSurface(bmp);
			}
		} else if (alias) {
			// texture already exists. Add only the alias if not exist anymore
			std::cout << "Reuse already loaded Texture: " << pathName << " as alias: " << *alias << std::endl;
			mAliasMap[*alias]=pathName;
		}
	}
	return rv;
}

SDL_Texture* TextureManager::getTexture(const char*pathName) {
	SDL_Texture *rv=nullptr;
	if (pathName && strlen(pathName)>1) {
		rv=getTexture(string(pathName));
	}
	return rv;
}

string* TextureManager::getPathNameFromAlias(const string *alias) {
	string *rv=nullptr;
	if (alias) {
		const auto& itAlias=mAliasMap.find(*alias);
		if (itAlias!=mAliasMap.end()) {
			rv=&itAlias->second;
		}
	}
	return rv;
}

SDL_Texture* TextureManager::getTexture(const string &name) {
	SDL_Texture *rv=nullptr;
	string *pathName=getPathNameFromAlias(&name);
	if (pathName) {
		const auto &it=mTextureMap.find(*pathName);
		if (it!=mTextureMap.end()) {
			rv=it->second;
		}
	} else {
		const auto &it=mTextureMap.find(name);
		if (it!=mTextureMap.end()) {
			rv=it->second;
		}
	}
	return rv;
}
