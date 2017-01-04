#include "node.h"


NodeDrawable::NodeDrawable(const string &name, SDL_Texture *rSDL_Texture, int x, int y) : Node(name) {
	mX=x;
	mY=y;
	mSDL_Texture=rSDL_Texture;

	queryTextureSize();
	mW=mTextureSize.x;
	mH=mTextureSize.y;
	setSourceFrame(0,0,mTextureSize.x, mTextureSize.y);
}  

NodeDrawable::NodeDrawable(const string &name, SDL_Texture *rSDL_Texture, int x, int y, int w, int h) : NodeDrawable(name, rSDL_Texture, x, y) {
	mW=w;
	mH=h;
}

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

void NodeDrawable::setSourceFrame(int x, int y, int w, int h) {
	mSourceRect.x=x;
	mSourceRect.y=y;
	mSourceRect.w=w;
	mSourceRect.h=h;
	keepSourceInTextureRange();
}

void NodeDrawable::draw(SDL_Renderer *rSDL_Renderer, const int parentX, const int parentY) {
	mDestRect.x=parentX+mX;
	mDestRect.y=parentY+mY;
	mDestRect.h=mH;
	mDestRect.w=mW;
	SDL_RenderCopy(rSDL_Renderer, mSDL_Texture, &mSourceRect, &mDestRect);
	// draw childs
	Node::draw(rSDL_Renderer, mDestRect.x, mDestRect.y);
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