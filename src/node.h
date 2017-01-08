#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <SDL.h>
#include <math.h>
#include "texturemanager.h"

using namespace std;

enum class NodeType{
	Node=0,
	Sprite,
	Player
};

class NodeBase {
public:
	virtual NodeType getNodeType()=0;
	virtual void draw(SDL_Renderer *rSDL_Renderer, const int parentX, const int parentY)=0;
	virtual void setTextureFrame(TextureFrame *rTextureFrame)=0;
protected:
	virtual void update(double deltaTime)=0;
	virtual void destroy()=0;
};

class Node : NodeBase {
private:
	Node 	*mParentNode=nullptr;
	string 	mName;
	bool 	mScheduledUpdate=false;
	void initChildNodeList();
protected:
	vector<Node*> *mChildNodes=nullptr;
	vector<Node*> *mChildToDelete=nullptr;
	virtual void update(double deltaTime) override;
	virtual void destroy() override;
public:
	Node(const string &name);
	Node(const char *name);
	Node();
	virtual ~Node();
	Node* addNode(Node *n);
	void deleteMeScheduled();
	void scheduleChildToDelete(Node *rToDeleteNode);
	void deleteNode(Node *n);
	void deleteChilds();
	Node* searchNode(const string&name, bool searchInSub=false);
	void debugPrint();
	void scheduleUpdate(bool rScheduleUpdate) {
		mScheduledUpdate=rScheduleUpdate;
	}
	virtual void updateInternal(double deltaTime);
	virtual void draw(SDL_Renderer *rSDL_Renderer, const int parentX, const int parentY) override;
	virtual NodeType getNodeType() override { return NodeType::Node;}
	virtual void setTextureFrame(TextureFrame *rTextureFrame) override {}
};

enum class PlayerType {
	None=0,
	ForwardLoop,
	BackwardLoop,
	Forward,
	Backward
};

class FramePlayer;

class NodeDrawable : public Node {
private:
	double mX=0.0;
	double mY=0.0;
	double mAngle=0.0;
	int mW=0.0;
	int mH=0.0;
	SDL_Rect mSourceRect;
	SDL_Rect mDestRect;
	void keepSourceInTextureRange();
	void queryTextureSize(SDL_Texture *rSDL_Texture, int *dx, int *dy);
	void correctAngle();
	TextureFrame *mTextureFrame=nullptr;
	FramePlayer  *mFramePlayer=nullptr;
	//vector<TextureFrame*> mTextureFrames;	
public:
	NodeDrawable(const string &name, TextureFrame *rTextureFrame, int x, int y);
	NodeDrawable(const string &name, TextureFrame *rTextureFrame, int x, int y, int w, int h);
	double getX();
	double getY();
	int		getWidth();
	int		getHeight();
	void setX(double);
	void setY(double);
	void setAngle(double);
	double getAngle();
	void addAngle(double rAddAngle);
	void subAngle(double rAddAngle);

	void moveDistanceXY(double distanceX, double distanceY);
	virtual void updateInternal(double deltaTime);
	virtual void draw(SDL_Renderer *rSDL_Renderer, const int parentX, const int parentY) override;
	virtual NodeType getNodeType() override { return NodeType::Sprite;}

	void setTextureFrame(TextureFrame*);
	void activateFramePlayer(const string &name, double rDelayInMs=-1.0, PlayerType playerType=PlayerType::None);
	bool isFramePlayerFinish();
	void deactivateFramePlayer();
};


class FramePlayer : public Node {
private:
	PlayerType		mPlayerType;
	int				mCurrentFrame=0;
	double			mDelayInMs;
	FrameSequence 	*mFrameSequence;
	TextureFrame	*mCurrentTextureFrame=nullptr;
	double 			mCurrentDelayTime=0.0;
	bool			mIsFinish=false;
public:
	FramePlayer(const string& name, FrameSequence *rFrameSequence, double rDelayInMs, PlayerType playerType=PlayerType::ForwardLoop) : Node(name) {
		mPlayerType=playerType;
		mFrameSequence=rFrameSequence;
		mDelayInMs=rDelayInMs;
		mCurrentTextureFrame=mFrameSequence->getTextureFrame(mCurrentFrame);
		resetPlayer();
	}	
	void resetPlayer() {
		if (mPlayerType==PlayerType::Backward) {
			mCurrentFrame=getLastFrameIndex();
		} else {
			mCurrentFrame=0;
		}
		mIsFinish=false;
		mCurrentDelayTime=0.0;
	}
	void setPlayerType(PlayerType rPlayerType) {
		mPlayerType=rPlayerType;
	}
	void setDelayInMs(double rDelayInMs) {
		mDelayInMs=rDelayInMs;
	}

	void addFrame() {
		if (mCurrentFrame<mFrameSequence->countFrames()-1) {
			mCurrentFrame++;
		} else {
			mIsFinish=true;
		}
	}
	void subFrame() {
		if (mCurrentFrame>0) {
			mCurrentFrame--;
		} else {
			mIsFinish=true;
		}
	}
	void addFrameLoop() {
		mCurrentFrame++;
		if (mCurrentFrame>=mFrameSequence->countFrames()) {
			mCurrentFrame=0;
		}
	}
	void subFrameLoop() {
		mCurrentFrame--;
		if (mCurrentFrame<0) {
			mCurrentFrame=mFrameSequence->countFrames()-1;
		}
	}
	int getLastFrameIndex() {
		return mFrameSequence->countFrames()-1;
	}
	void updateFramePlayer(double deltaTime) {
		if (!mIsFinish) {
			mCurrentDelayTime+=deltaTime;
			if (mCurrentDelayTime>mDelayInMs) {
				mCurrentDelayTime=0.0;
				if (mPlayerType==PlayerType::ForwardLoop) {
					addFrameLoop();
				} else if (mPlayerType==PlayerType::BackwardLoop) {
					subFrameLoop();
				} else if (mPlayerType==PlayerType::Forward) {
					addFrame();
				} else if (mPlayerType==PlayerType::Backward) {
					subFrame();
				}
			}
		} 
	}
	bool isFinish() {
		return mIsFinish;
	}
	TextureFrame* getCurrentTextureFrame() {
		return mFrameSequence->getTextureFrame(mCurrentFrame);
	}
	virtual NodeType getNodeType() override { return NodeType::Player;}
	
};
