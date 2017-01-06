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

void TextureManager::queryTextureSize(SDL_Texture *rSDL_Texture, int *dx, int *dy) {
	Uint32 	format;
	int 	access;
	if (rSDL_Texture) {
		SDL_QueryTexture(rSDL_Texture, &format, &access, dx, dy);
	} else {
		(*dx)=0;
		(*dy)=0;
	}
}

void TextureManager::addDefaultFrame(const string &pathName, const string *&alias, SDL_Texture* rSDL_Texture) {
	int dx=0;
	int dy=0;
	queryTextureSize(rSDL_Texture, &dx, &dy);
	if (alias) {
		addFrameToTexture(pathName, *alias, 0, 0, dx, dy);
	} else {
		addFrameToTexture(pathName, pathName, 0, 0, dx, dy);
	}
}

SDL_Texture *TextureManager::loadTexture(const string &pathName, const char*alias) {
	if (alias) {
		string sAlias(alias);
		return loadTexture(pathName, &sAlias);
	} else {
		return loadTexture(pathName, (const string*)nullptr);
	}
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
				SDL_SetSurfaceBlendMode(bmp,SDL_BLENDMODE_BLEND);
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
					addDefaultFrame(pathName, alias, rv);					
				}
				SDL_FreeSurface(bmp);
			}
		} else if (alias) {
			// texture already exists. Add only the alias if not exist anymore
			std::cout << "Reuse already loaded Texture: " << pathName << " as alias: " << *alias << std::endl;
			mAliasMap[*alias]=pathName;
			addDefaultFrame(pathName, alias, rv);
		}
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
