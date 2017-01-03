#include "node.h"

Node::Node(const string &name) :mChildNodes(nullptr) {
	mName=name;
	cout << "C " << mName << endl;
}
Node::Node() : mName(""), mChildNodes(nullptr) {
}

Node::~Node() {
	cout << "D " << mName << endl;
	deleteChilds();
}

void Node::deleteChilds() {
	if (mChildNodes) {
		for(Node* n : *mChildNodes) {
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
	initChildNodeList();
	mChildNodes->erase(std::remove(std::begin(*mChildNodes), std::end(*mChildNodes), n), std::end(*mChildNodes));
	delete n;
}
Node* Node::searchNode(const string &name, bool searchInSub) {
	Node *rv=nullptr;
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
