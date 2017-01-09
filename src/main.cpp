#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "director.h"

//#include "res_path.h"

/*
 * Lesson 0: Test to make sure SDL is setup properly
 */
class MyScene : public Node {
private:
	NodeDrawable *s=nullptr;
	NodeDrawable *s_sub=nullptr; 
protected:
	void init();
	virtual void destroy() override;
	virtual void update(double) override; 
public:
	MyScene() : Node("Game Scene") {
		init();
	} 
	using Node::Node;
};

void MyScene::init() {
	//Sequence &walkSequence=AnimationManager::getInstance().addSequence("MAN_Walk");
	//walkSequence.addFrame(0,0,50,50);
	//walkSequence.addFrame(200,200,50,50);
	TextureManager &textureManager=TextureManager::getInstance();

	// load texture
	textureManager.loadTexture("assets/test.png", "test");
	// reuse loaded texture and make it available via new alias
	textureManager.loadTexture("assets/test.png", "test2");
	textureManager.loadTexture("assets/testschwein.png", "testschwein");

	textureManager.createAutomaticFramesAndSequence("testschwein", "testschwein.Walk", 11, 1);
	//textureManager.addFrameAutomaticallyToTexture("testschwein", "testschwein.frame", 11, 1);
	//textureManager.addFrameSequenceAutomatically("testschwein.Walk", "testschwein.frame", 11);


	// setup source frames for this texture
	//for (int i=0;i<11;i++) {
	//	textureManager.addFrameToTexture("testschwein","testschwein.Frame"+i,i*32,0,32,32);
	//	textureManager.addFrameSequence("testschwein.Walk")
	//								->addTextureFrame(textureManager.getTextureFrame("testschwein.Frame"+i));
	//}

	textureManager.addFrameToTexture("test","Test.Frame1",0,0,50,50);
	textureManager.addFrameToTexture("test","Test.Frame2",300,300,50,50);
	textureManager.addFrameToTexture("test","Test.Frame3",80,80,500,500);
	textureManager.addFrameToTexture("test","Test.Frame4",400,200,500,500);
	// create a sequence with previous added frames
	textureManager.addFrameSequence("Test.Walk")
								->addTextureFrame(textureManager.getTextureFrame("Test.Frame1"))
								->addTextureFrame(textureManager.getTextureFrame("Test.Frame2"));

	// create another sequence with previous added frames
	textureManager.addFrameSequence("Test.Walk2")
								->addTextureFrame(textureManager.getTextureFrame("Test.Frame3"))
								->addTextureFrame(textureManager.getTextureFrame("Test.Frame4"));
	// create one sprite with the full texture as image							
	s=new NodeDrawable(string("Sprite 1"), TextureManager::getInstance().getTextureFrame(string("test2")), 130, 30);
	addNode(s);
	// create a sub sprite of the first sprite s
	s_sub=(NodeDrawable*)s->addNode(new NodeDrawable(string("Sprite 2"), TextureManager::getInstance().getTextureFrame(string("test2")), 100, 100, 50, 50));
	// add two animation player to the second sprite and activate animation player 1 for now
	s_sub->addNode(new FramePlayer("testschwein.Walk", textureManager.getFrameSequence("testschwein.Walk"),50));
	s_sub->addNode(new FramePlayer("Test.Walk2", textureManager.getFrameSequence("Test.Walk2"),1000, PlayerType::Forward));
	s_sub->activateFramePlayer("testschwein.Walk");
	// make sure the mehtod update will be called for the root scene
	scheduleUpdate(true);
}

void MyScene::destroy() {
	// nothing to do
}

void MyScene::update(double deltaTime) {
	//s->moveDistanceXY(0.0, 20.0*(deltaTime/1000.0));
	if (s_sub) {
		s_sub->moveDistanceXY(90.0*(deltaTime/1000.0),0);
		if (s_sub->getX()>400) {
			s_sub->deleteMeScheduled();
			s_sub=(NodeDrawable*)s->addNode(new NodeDrawable(string("Sprite 33"), TextureManager::getInstance().getTextureFrame(string("test2")), 100, 100, 50, 50));;
			//s_sub->setX(0.0);
			// activate animation player 2 now
			//s_sub->activateFramePlayer("Test.Walk2");
		}
	}
	//s_sub->addAngle(180.0*(deltaTime/1000.0));
	//if (s_sub->getAngle()>180.0) {
	//	MyScene *s=new MyScene();
	//	Director::getInstance()->switchScene(s);
	//}
}

int main(int, char**) {
	Director::getInstance()->initialize();
	TextureManager::getInstance().loadTexture("assets/cursor.png", "cursor");
	Director::getInstance()->setMousePointer(TextureManager::getInstance().getTexture("cursor"), MousePointerAlignment::Middle);
	MyScene *s=new MyScene();
	Director::getInstance()->runWithNode(s);
	return 0;
}
