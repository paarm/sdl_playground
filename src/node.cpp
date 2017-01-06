#include "node.h"

Node::Node(const string &name) {
	mName=name;
	cout << "C " << mName << endl;
}

Node::Node(const char *name) : Node(string(name)) {
}

Node::Node() : mName("") {
	cout << "C " << mName << endl;
}

Node::~Node() {
	cout << "D " << mName << endl;
	deleteChilds();
}

void Node::deleteChilds() {
	if (mChildNodes) {
		for(Node* n : *mChildNodes) {
			n->destroy();
			delete n;
		}
		delete mChildNodes;
		mChildNodes=nullptr;
	}
}

void Node::initChildNodeList() {
	if (!mChildNodes) {
		mChildNodes=new vector<Node*>();
	}
}

Node* Node::addNode(Node *n) {
	initChildNodeList();
	mChildNodes->push_back(n);
	return n;
}

void Node::deleteNode(Node *n) {
	if (mChildNodes) {
		mChildNodes->erase(std::remove(std::begin(*mChildNodes), std::end(*mChildNodes), n), std::end(*mChildNodes));
		n->destroy();
		delete n;
	}
}

Node* Node::searchNode(const string &name, bool searchInSub) {
	Node *rv=nullptr;
	if (mChildNodes) {
		for(Node *n: *mChildNodes) {
			if (n->mName==name) {
				rv=n;
				break;
			}
		}
		if (!rv && searchInSub) {
			for(Node *n: *mChildNodes) {
				rv=n->searchNode(name, searchInSub);
				if (rv) {
					break;
				}
			}	
		}
	}
	return rv;
}
void Node::debugPrint() {
	cout << mName << endl;
	if (mChildNodes) {
		for(Node *n : *mChildNodes) {
			n->debugPrint();
			n->debugPrint();
		}
	}
}

void Node::updateInternal(double deltaTime) {
	if (mScheduledUpdate) {
		update(deltaTime);
	}
	if (mChildNodes) {
		for(Node *n : *mChildNodes) {
			n->updateInternal(deltaTime);
		}
	}
}


void Node::draw(SDL_Renderer *rSDL_Renderer, const int parentX, const int parentY) {
	if (mChildNodes) {
		for(const auto &n: *mChildNodes) {
			n->draw(rSDL_Renderer, parentX, parentY);
		}
	}
}

void Node::update(double deltaTime) {}
void Node::destroy() {}
