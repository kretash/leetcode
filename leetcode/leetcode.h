#pragma once
#include <string>
#include <sstream>
#include <iterator>
#include <ostream>
#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <limits>
#include <queue>
#include <map>
#include <fstream>
#include <deque>
#include <iomanip>

using namespace std;

struct TreeNode {
	int val = 0;
	TreeNode *left = nullptr;
	TreeNode *right = nullptr;
	TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
	TreeNode() {}
};

struct ListNode {
	int val;
	ListNode *next;
	ListNode(int x) : val(x), next(nullptr) {}
};

void trimLeftTrailingSpaces(string &input) {
	input.erase(input.begin(), find_if(input.begin(), input.end(), [](int ch) {
		return !isspace(ch);
	}));
}

void trimRightTrailingSpaces(string &input) {
	input.erase(find_if(input.rbegin(), input.rend(), [](int ch) {
		return !isspace(ch);
	}).base(), input.end());
}

TreeNode* stringToTreeNode(string input) {
	trimLeftTrailingSpaces(input);
	trimRightTrailingSpaces(input);
	input = input.substr(1, input.length() - 2);
	if (!input.size()) {
		return nullptr;
	}

	string item;
	stringstream ss;
	ss.str(input);

	getline(ss, item, ',');
	TreeNode* root = new TreeNode(stoi(item));
	queue<TreeNode*> nodeQueue;
	nodeQueue.push(root);

	while (true) {
		TreeNode* node = nodeQueue.front();
		nodeQueue.pop();

		if (!getline(ss, item, ',')) {
			break;
		}

		trimLeftTrailingSpaces(item);
		if (item != "null") {
			int leftNumber = stoi(item);
			node->left = new TreeNode(leftNumber);
			nodeQueue.push(node->left);
		}

		if (!getline(ss, item, ',')) {
			break;
		}

		trimLeftTrailingSpaces(item);
		if (item != "null") {
			int rightNumber = stoi(item);
			node->right = new TreeNode(rightNumber);
			nodeQueue.push(node->right);
		}
	}
	return root;
}

int stringToInteger(string input) {
	return stoi(input);
}

void deleteTree(TreeNode* root)
{
	if (root == nullptr) return;

	deleteTree(root->right);
	deleteTree(root->left);

	delete root;
}

// Find the maximum height of the binary tree
int maxHeight(TreeNode *p) {
	if (!p) return 0;
	int leftHeight = maxHeight(p->left);
	int rightHeight = maxHeight(p->right);
	return (leftHeight > rightHeight) ? leftHeight + 1 : rightHeight + 1;
}

// Convert an integer value to string
string intToString(int val) {
	ostringstream ss;
	ss << val;
	return ss.str();
}

// Print the arm branches (eg, /    \ ) on a line
void printBranches(int branchLen, int nodeSpaceLen, int startLen, int nodesInThisLevel, const deque<TreeNode*>& nodesQueue, ostream& out) {
	deque<TreeNode*>::const_iterator iter = nodesQueue.begin();
	for (int i = 0; i < nodesInThisLevel / 2; i++) {
		out << ((i == 0) ? setw(startLen - 1) : setw(nodeSpaceLen - 2)) << "" << ((*iter++) ? "/" : " ");
		out << setw(2 * branchLen + 2) << "" << ((*iter++) ? "\\" : " ");
	}
	out << endl;
}

// Print the branches and node (eg, ___10___ )
void printNodes(int branchLen, int nodeSpaceLen, int startLen, int nodesInThisLevel, const deque<TreeNode*>& nodesQueue, ostream& out) {
	deque<TreeNode*>::const_iterator iter = nodesQueue.begin();
	for (int i = 0; i < nodesInThisLevel; i++, iter++) {
		out << ((i == 0) ? setw(startLen) : setw(nodeSpaceLen)) << "" << ((*iter && (*iter)->left) ? setfill('_') : setfill(' '));
		out << setw(branchLen + 2) << ((*iter) ? intToString((*iter)->val) : "");
		out << ((*iter && (*iter)->right) ? setfill('_') : setfill(' ')) << setw(branchLen) << "" << setfill(' ');
	}
	out << endl;
}

// Print the leaves only (just for the bottom row)
void printLeaves(int indentSpace, int level, int nodesInThisLevel, const deque<TreeNode*>& nodesQueue, ostream& out) {
	deque<TreeNode*>::const_iterator iter = nodesQueue.begin();
	for (int i = 0; i < nodesInThisLevel; i++, iter++) {
		out << ((i == 0) ? setw(indentSpace + 2) : setw(2 * level + 2)) << ((*iter) ? intToString((*iter)->val) : "");
	}
	out << endl;
}

// Pretty formatting of a binary tree to the output stream
// @ param
// level  Control how wide you want the tree to sparse (eg, level 1 has the minimum space between nodes, while level 2 has a larger space between nodes)
// indentSpace  Change this to add some indent space to the left (eg, indentSpace of 0 means the lowest level of the left node will stick to the left margin)
void printPretty(TreeNode *root, int level, int indentSpace, ostream& out) {
	int h = maxHeight(root);
	int nodesInThisLevel = 1;

	int branchLen = 2 * ((int)pow(2.0, h) - 1) - (3 - level)*(int)pow(2.0, h - 1);  // eq of the length of branch for each node of each level
	int nodeSpaceLen = 2 + (level + 1)*(int)pow(2.0, h);  // distance between left neighbor node's right arm and right neighbor node's left arm
	int startLen = branchLen + (3 - level) + indentSpace;  // starting space to the first node to print of each level (for the left most node of each level only)

	deque<TreeNode*> nodesQueue;
	nodesQueue.push_back(root);
	for (int r = 1; r < h; r++) {
		printBranches(branchLen, nodeSpaceLen, startLen, nodesInThisLevel, nodesQueue, out);
		branchLen = branchLen / 2 - 1;
		nodeSpaceLen = nodeSpaceLen / 2 + 1;
		startLen = branchLen + (3 - level) + indentSpace;
		printNodes(branchLen, nodeSpaceLen, startLen, nodesInThisLevel, nodesQueue, out);

		for (int i = 0; i < nodesInThisLevel; i++) {
			TreeNode *currNode = nodesQueue.front();
			nodesQueue.pop_front();
			if (currNode) {
				nodesQueue.push_back(currNode->left);
				nodesQueue.push_back(currNode->right);
			}
			else {
				nodesQueue.push_back(NULL);
				nodesQueue.push_back(NULL);
			}
		}
		nodesInThisLevel *= 2;
	}
	printBranches(branchLen, nodeSpaceLen, startLen, nodesInThisLevel, nodesQueue, out);
	printLeaves(indentSpace, level, nodesInThisLevel, nodesQueue, out);
}

#if 0


std::vector<std::string> split(const std::string& s, char delimiter)
{
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter))
	{
		tokens.push_back(token);
	}
	return tokens;
}

TreeNode* r_CreateTree(vector<string> nodes)
{
	if (nodes.size() != 0)
	{
		TreeNode* root = nullptr;

		if (nodes[0] != "null")
		{
			root = new TreeNode(stoi(nodes[0]));

			nodes.erase(nodes.begin());

			if (nodes.size() > 0)
			{
				vector<string> lv(nodes.begin(), nodes.end());
				lv.erase(lv.begin() + 1);
				if (lv.size() > 3)
				{
					lv.erase(lv.begin() + 3);
					lv.erase(lv.begin() + 3);
				}
				vector<string> rv(nodes.begin() + 1, nodes.end());
				if (rv.size() > 3)
				{
					rv.erase(rv.begin() + 1);
					rv.erase(rv.begin() + 1);
				}

				root->left = r_CreateTree(lv);
				root->right = r_CreateTree(rv);
			}

			return root;
		}

		return root;
	}

	return nullptr;
}

TreeNode* CreateTree(string nodes)
{
	return r_CreateTree(split(string(nodes.begin() + 1, nodes.end() - 1), ','));
}

void DeleteTree(TreeNode* root)
{
	if (root == nullptr) return;

	DeleteTree(root->right);
	DeleteTree(root->left);

	delete root;
}

#endif