#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;


class Node {
    private:
    string mName;
    vector<Node*> *mChildNodes;
    void deleteChilds();
    void initChildNodeList();
    public:
    Node(const string &name);
    Node();
    virtual ~Node();
    Node* addNode(Node *n);
    void deleteNode(Node *n);
    Node* searchNode(const string&name, bool searchInSub=false);
    void debugPrint();
};