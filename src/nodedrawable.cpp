#include "node.h"


NodeDrawable::NodeDrawable(const string &name, TextureFrame *rTextureFrame, int x, int y) : Node(name) {
	mX=x;
	mY=y;
	mTextureFrame=rTextureFrame;

	queryTextureSize(mTextureFrame->mSDL_Texture, &mW, &mH);
	//setSourceFrame(0,0,mTextureSize.x, mTextureSize.y);
}  

NodeDrawable::NodeDrawable(const string &name, TextureFrame *rTextureFrame, int x, int y, int w, int h) : NodeDrawable(name, rTextureFrame, x, y) {
	mW=w;
	mH=h;
}

void NodeDrawable::queryTextureSize(SDL_Texture *rSDL_Texture, int *dx, int *dy) {
	Uint32 	format;
	int 	access;
	if (rSDL_Texture) {
		SDL_QueryTexture(rSDL_Texture, &format, &access, dx, dy);
	} else {
		(*dx)=0;
		(*dy)=0;
	}
}

/*
void NodeDrawable::keepSourceInTextureRange() {
	mSourceRect.x=max(0,mSourceRect.x);
	mSourceRect.x=min(mTextureSize.x,mSourceRect.x);
	mSourceRect.y=max(0,mSourceRect.y);
	mSourceRect.y=min(mTextureSize.y,mSourceRect.y);
}

void NodeDrawable::queryTextureSize() {
	Uint32 	format;
	int 	access;
	if (mSDL_Texture) {
		SDL_QueryTexture(mSDL_Texture, &format, &access, &mTextureSize.x, &mTextureSize.y);
	} else {
		mTextureSize.x=0;
		mTextureSize.y=0;
	}
}
*/
void NodeDrawable::setTextureFrame(TextureFrame *rTextureFrame) {
	mTextureFrame=rTextureFrame;
}

void NodeDrawable::draw(SDL_Renderer *rSDL_Renderer, const int parentX, const int parentY) {
	if (mTextureFrame) {
		mDestRect.x=parentX+mX;
		mDestRect.y=parentY+mY;
		mDestRect.h=mH;
		mDestRect.w=mW;

		mSourceRect.x=mTextureFrame->mFrame.x;
		mSourceRect.y=mTextureFrame->mFrame.y;
		mSourceRect.h=mTextureFrame->mFrame.dx;
		mSourceRect.w=mTextureFrame->mFrame.dy;
		if (mAngle==0.0) {
			SDL_RenderCopy(rSDL_Renderer, mTextureFrame->mSDL_Texture, &mSourceRect, &mDestRect);
		} else {
			SDL_RenderCopyEx(rSDL_Renderer, mTextureFrame->mSDL_Texture, &mSourceRect, &mDestRect, mAngle, nullptr, SDL_FLIP_NONE);
		}
	}
	// draw childs
	Node::draw(rSDL_Renderer, mDestRect.x, mDestRect.y);
}

void NodeDrawable::updateInternal(double deltaTime) {
	if (mFramePlayer) {
		mFramePlayer->updateFramePlayer(deltaTime);
		setTextureFrame(mFramePlayer->getCurrentTextureFrame());
	}
	Node::updateInternal(deltaTime);
}

void NodeDrawable::activateFramePlayer(const string &name, double rDelayInMs, PlayerType playerType) {
	Node *n=searchNode(name);
	if (n && n->getNodeType()==NodeType::Player) {
		mFramePlayer=(FramePlayer*)n;
		if (rDelayInMs!=-1.0) {
			mFramePlayer->setDelayInMs(rDelayInMs);
		}
		if (playerType!=PlayerType::None) {
			mFramePlayer->setPlayerType(playerType);
		}
		mFramePlayer->resetPlayer();
	}
}

bool NodeDrawable::isFramePlayerFinish() {
	bool rv=true;
	if (mFramePlayer) {
		rv=mFramePlayer->isFinish();
	}
	return rv;
}
void NodeDrawable::deactivateFramePlayer() {
	mFramePlayer=nullptr;
}

void NodeDrawable::moveDistanceXY(double distanceX, double distanceY) {
	mX+=distanceX;
	mY+=distanceY;
}

void NodeDrawable::setX(double rX) {
	mX=rX;
}

double NodeDrawable::getX() {
	return mX;
}

void NodeDrawable::setY(double rY) {
	mY=rY;
}

double NodeDrawable::getY() {
	return mY;
}

int NodeDrawable::getHeight() {
	return mH;
}

int NodeDrawable::getWidth() {
	return mW;
}

void NodeDrawable::correctAngle() {
	mAngle=fmod(mAngle,360.0);
	if (mAngle<0.0) {
		mAngle=360.0+mAngle;
	}
}

void NodeDrawable::setAngle(double rAngle) {
	mAngle=rAngle;
	correctAngle();
}

double NodeDrawable::getAngle() {
	return mAngle;
}

void NodeDrawable::addAngle(double rAddAngle) {
	mAngle+=rAddAngle;
	correctAngle();
}

void NodeDrawable::subAngle(double rAddAngle) {
	mAngle-=rAddAngle;
	correctAngle();
}