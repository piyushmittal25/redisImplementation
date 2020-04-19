#include<string>
#include<iostream>
#include<unordered_map>
#include<vector>
#include<algorithm>
#include<cstring>
using namespace std;

class treeNode{
	public:
		double score;
		string value;
		int height;
		long long weight;
		treeNode *leftChild;    // To store address of left child
		treeNode *rightChild;   // To store address of right child
		treeNode *inOrderPred; // To store pointer of in order predecessor
		treeNode *inOrderSucc; // To store pointer of in order predecessor
		// int getHeight();
		// long long getWeight();
		void heightWeightUpdate();
		treeNode *LLrotation();
		treeNode *RRrotation();
		treeNode *LRrotation();
		treeNode *RLrotation();
		treeNode *balanceHeight();

		
		treeNode(double score, string value);
		~treeNode();
		treeNode *insertValue(double &score, string &value);
		treeNode *deleteValue(double &score, string &value);
		treeNode *findXthRankNode(long long x);
		long long getRank(double &score, string &value);
		void falttenNode(string &str);
};

class setValue{
private:
	treeNode *root;
	unordered_map<string, double> elemScoreMap;
public:
	setValue();
	~setValue();
	int addValue(double &score, string &value);
	long long findRank(string &value);
	vector<string> rangeValues(long long start, long long end);
	string serialise();
	void build(vector<string> &values);
};
