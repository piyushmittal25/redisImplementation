#include "AVL.hpp"

setValue::setValue(){
	root = NULL;
	elemScoreMap.clear();
}

setValue::~setValue(){
	if(root) delete root;
	elemScoreMap.clear();
}

int setValue::addValue(double &score, string &value){
	if(elemScoreMap.count(value) && elemScoreMap[value] == score) return 0;
	if(elemScoreMap.count(value)) root = root->deleteValue(elemScoreMap[value], value);
	elemScoreMap[value] = score;
	if(!root) root = new treeNode(score, value);
	else root = root->insertValue(score, value);
	return true;
}

long long setValue::findRank(string &value){
	if(!elemScoreMap.count(value)) return -3;
	return root->getRank(elemScoreMap[value], value)-1;
}

vector<string> setValue:: rangeValues(long long start, long long end){
	if(start < 0) start += elemScoreMap.size();
	if(end < 0) end += elemScoreMap.size();
	if(start < 0 || end < 0 || start>end || start>=elemScoreMap.size()) return {};
	end = min(end , (long long)elemScoreMap.size()-1);
	treeNode *startNode = root->findXthRankNode(start+1);
	vector<string> resValues;
	while(start<=end){
		resValues.push_back(startNode->value);
		startNode = startNode->inOrderSucc;
		start++;
	}
	return resValues;
}

string setValue::serialise(){
	string str = "";
	if(root) root->falttenNode(str);
	return str;
}


treeNode* buildBalancedBST(vector<string> &scoreVal, treeNode **prev,long long st, long long end){
	if(st>end) return NULL;
	int mid = (st+end)/2;
	treeNode *left = buildBalancedBST(scoreVal, prev, st, mid-1);
	treeNode *root = new treeNode(stod(scoreVal[2*mid]), scoreVal[2*mid+1]);
	root->inOrderPred = *prev;
	if(*prev)(*prev)->inOrderSucc = root;
	*prev = root;
	root->leftChild = left;
	root->rightChild = buildBalancedBST(scoreVal, prev, mid+1, end);
	root->heightWeightUpdate();
	return root;
}

void setValue::build(vector<string> &values){
	long long n = values.size()/2;
	for(long long i = 0; i < values.size(); i+=2)
		elemScoreMap[values[i+1]] = stod(values[i]);
	treeNode *prev = NULL;
	root = buildBalancedBST(values, &prev, 0, n-1);
}