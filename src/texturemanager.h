#pragma once
#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>

using namespace std;


struct Frame {
    int x;
    int y;
    int dx;
    int dy;
};

struct TextureFrame {
	Frame 		mFrame;
	SDL_Texture *mSDL_Texture;
};

class FrameSequence {
private:
    vector<TextureFrame*> mTextureFrame;

public:
    FrameSequence() {}
    FrameSequence* addTextureFrame(TextureFrame *f) {
      mTextureFrame.push_back(f);
	  return this;
	}
    TextureFrame* getTextureFrame(int rIndex) {
        return mTextureFrame[rIndex];
    }
	int countFrames () {
        return mTextureFrame.size();
    }
};


class TextureManager {
private:
	map<string, SDL_Texture*> mTextureMap;
	map<string, string> mAliasMap;
	map<string, TextureFrame>mFrameMap;
    map<string,	FrameSequence> mFrameSequenceMap;
	
	SDL_Renderer *mSDL_Renderer;
	TextureManager();
	bool proofSDLRenderer();
	string* getPathNameFromAlias(const string *alias);
	void queryTextureSize(SDL_Texture *rSDL_Texture, int *dx, int *dy);
	void addDefaultFrame(const string &pathName, const string *&alias, SDL_Texture* rSDL_Texture);
	void keepSourceInTextureRange(Frame &rFrame, int textX, int textY) {
		rFrame.x=max(0,rFrame.x);
		rFrame.x=min(textX,rFrame.x);
		rFrame.y=max(0,rFrame.y);
		rFrame.y=min(textY,rFrame.y);
		// todo: proof dx, dy
	}	
public:
	static TextureManager & getInstance() {
		static TextureManager instance;
		return instance;
	}
	~TextureManager();
	TextureManager(const TextureManager&) = delete;
	void operator=(const TextureManager&) = delete;
	void setSDLRenderer(SDL_Renderer*);
	SDL_Texture* loadTexture(const string &pathName, const char*alias);
	SDL_Texture* loadTexture(const string &pathName, const string* alias=nullptr);
	SDL_Texture* getTexture(const string &name);

	TextureFrame *getTextureFrame(const string &frameName) {
		const auto &it=mFrameMap.find(frameName);
		if (it==mFrameMap.end()) {
			string* pathName=getPathNameFromAlias(&frameName);
			if (pathName) {
				const auto &it2=mFrameMap.find(frameName);
				if (it2!=mFrameMap.end()) {
					return &it2->second;
				}	
			}
		} else {
			return &it->second;
		}
		return nullptr;
	}

	FrameSequence *createAutomaticFramesAndSequence(const string &textureName, const string &sequenceName, int xFrameCount, int yFrameCount=1) {
		string frameNamePrefix=textureName;
		frameNamePrefix+=".frame";
		int frameCount=addFramesAutomaticallyToTexture(textureName, frameNamePrefix, xFrameCount, yFrameCount);
		return addFrameSequenceAutomatically(sequenceName, frameNamePrefix, frameCount);
	}

	int addFramesAutomaticallyToTexture(const string &textureName, const string &frameNamePrefix, int xFrameCount, int yFrameCount=1) {
		SDL_Texture* rSDL_Texture=getTexture(textureName);
		int frameCount=0;
		if (rSDL_Texture && xFrameCount>0 && yFrameCount>0) {
			int textX=0;
			int textY=0;
			queryTextureSize(rSDL_Texture, &textX, &textY);

			int xFrameSize=textX/xFrameCount;
			int yFrameSize=textY/yFrameCount;

			for (int y=0;y<yFrameCount;y++) {
				for (int x=0;x<xFrameCount;x++) {
					string frameName=frameNamePrefix;
					frameName+=frameCount;
					addFrameToTexture(textureName, frameName, x*xFrameSize, y*yFrameSize, xFrameSize, yFrameSize);
					frameCount++;
				}
			}
		}
		return frameCount;		
	}

	TextureFrame *addFrameToTexture(const string &textureName, const string &frameName, int x, int y, int dx, int dy) {
		TextureFrame*rv=nullptr;
		SDL_Texture* rSDL_Texture=getTexture(textureName);
		if (rSDL_Texture) {
			rv=getTextureFrame(frameName);
			if (!rv) {
				int textX=0;
				int textY=0;
				queryTextureSize(rSDL_Texture, &textX, &textY);
				
				Frame rFrame;
				rFrame.x=x;
				rFrame.y=y;
				rFrame.dx=dx;
				rFrame.dy=dy;
				keepSourceInTextureRange(rFrame, textX, textY);
				TextureFrame rTextureFrame;
				rTextureFrame.mFrame=rFrame;
				rTextureFrame.mSDL_Texture=rSDL_Texture;
				mFrameMap[frameName]=rTextureFrame;
				rv=&mFrameMap[frameName];
			}
		}
		return rv;
	}

    FrameSequence *addFrameSequenceAutomatically(const string& rSequenceName, const string &frameNamePrefix, int count) {
	    FrameSequence *rv=addFrameSequence(rSequenceName);
		if (rv) {
			for (int i=0;i<count;i++) {
				string frameName=frameNamePrefix;
				frameName+=i;
				TextureFrame *textureFrame=getTextureFrame(frameName);
				if (textureFrame) {
					rv->addTextureFrame(textureFrame);			
				} else {
					break;
				}
			}
		}
		return rv;
	}
	

    FrameSequence *addFrameSequence(const string& rSequenceName) {
        const auto &it=mFrameSequenceMap.find(rSequenceName);
        if (it!=mFrameSequenceMap.end()) {
            return &it->second;
        }
        FrameSequence rFrameSequence;
        mFrameSequenceMap[rSequenceName]=rFrameSequence;
        return &mFrameSequenceMap[rSequenceName];
    }
    bool isFrameSequenceExist(const string &rSequenceName) {
        bool rv=false;
        const auto &it=mFrameSequenceMap.find(rSequenceName);
        if (it!=mFrameSequenceMap.end()) {
            rv=true;
        }
        return rv;
    }
    FrameSequence *getFrameSequence(const string& rSequenceName) {
        const auto &it=mFrameSequenceMap.find(rSequenceName);
        if (it!=mFrameSequenceMap.end()) {
            return &it->second;
        }
        return nullptr;
    }
};