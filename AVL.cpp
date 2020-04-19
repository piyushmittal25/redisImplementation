#include "AVL.hpp"

treeNode::treeNode(double score, string value){
	this->score = score;
	this->value = value;
	height = 1;
	weight = 1;
	leftChild = NULL;
	rightChild = NULL;
	inOrderPred = NULL;
	inOrderSucc = NULL;
}

treeNode::~treeNode(){
	if(this->leftChild)
		delete this->leftChild;
	if(this->rightChild)
		delete this->rightChild;
}

// int treeNode::getHeight(){
// 	if(!this) return 0;
// 	return this->height;
// }

// long long treeNode::getWeight(){
// 	if(!this) return 0;
// 	return this->weight;
// }

int getHeight(treeNode *root){
	if(!root) return 0;
	return root->height;
}
int getWeight(treeNode *root){
	if(!root) return 0;
	return root->weight;
}

void treeNode::heightWeightUpdate(){
	this->height = 1 + max(getHeight(this->leftChild) , getHeight(this->rightChild));
	this->weight = 1 +  getWeight(this->leftChild) + getWeight(this->rightChild);
	return;
}

treeNode* treeNode::LLrotation(){
	treeNode *temp   = this->leftChild;
	this->leftChild  = temp->rightChild;
	temp->rightChild = this;
	this->heightWeightUpdate();
	temp->heightWeightUpdate();
	return temp;
}

treeNode* treeNode::RRrotation(){
	treeNode *temp   = this->rightChild;
	this->rightChild  = temp->leftChild;
	temp->leftChild = this;
	this->heightWeightUpdate();
	temp->heightWeightUpdate();
	return temp;
}

treeNode* treeNode::LRrotation(){
	this->leftChild = this->leftChild->RRrotation();
	return this->LLrotation();
}

treeNode* treeNode::RLrotation(){
	this->rightChild = this->rightChild->LLrotation();
	return this->RRrotation();
}

treeNode* treeNode::balanceHeight(){
	treeNode *temp;
	if(abs(getHeight(this->leftChild) - getHeight(this->rightChild)) <= 1)
		temp =  this;
	else if(getHeight(this->leftChild) > getHeight(this->rightChild)){
		if(getHeight(this->leftChild->rightChild) > getHeight(this->leftChild->leftChild))
			temp =  this->LRrotation();
		else
			temp = this->LLrotation();
	}
	else{
		if(getHeight(this->rightChild->rightChild) >= getHeight(this->rightChild->leftChild))
			temp = this->RRrotation();
		else
			temp = this->RLrotation();
	}
	temp->heightWeightUpdate();
	return temp;
}

treeNode* treeNode::insertValue(double &score, string &value){
	if(this->score > score || (this->score == score && this->value > value))
	{	
		if(getWeight(this->leftChild) == 0){
			this->leftChild = new treeNode(score, value);
			this->leftChild->inOrderSucc = this;
			this->leftChild->inOrderPred = this->inOrderPred;
			if(this->inOrderPred)
				this->inOrderPred->inOrderSucc = this->leftChild;
			this->inOrderPred = this->leftChild;
		}
		else
			this->leftChild = this->leftChild->insertValue(score, value);
		
	}
	else{
		if(getHeight(this->rightChild) == 0){
			this->rightChild = new treeNode(score, value);
			this->rightChild->inOrderPred = this;
			this->rightChild->inOrderSucc = this->inOrderSucc;
			if(this->inOrderSucc)
				this->inOrderSucc->inOrderPred = this->rightChild;
			this->inOrderSucc = this->rightChild;
		}
		else
			this->rightChild = this->rightChild->insertValue(score, value);
		
	}
	this->heightWeightUpdate();
	return this->balanceHeight();
}

treeNode* treeNode::deleteValue(double &score, string &value){
	if(this->score > score || (this->score == score && this->value > value))
	{
		if(getWeight(this->leftChild) == 1){
			this->inOrderPred = this->leftChild->inOrderPred;
			if(this->inOrderPred)
				this->inOrderPred->inOrderSucc = this;
			delete this->leftChild;
			this->leftChild = NULL;
		}
		else
			this->leftChild = this->leftChild->deleteValue(score, value);
	}
	else if(this->score < score || (this->score == score && this->value < value)){
		if(getWeight(this->rightChild) == 1){
			this->inOrderSucc = this->rightChild->inOrderSucc;
			if(this->inOrderSucc)
				this->inOrderSucc->inOrderPred = this;
			delete this->rightChild;
			this->rightChild = NULL;
		}
		else
			this->rightChild = this->rightChild->deleteValue(score, value);
	}
	else{
		if(this->leftChild){
			treeNode *temp = this->inOrderPred;
			this->score = temp->score;
			this->value = temp->value;
			if(getWeight(this->leftChild) == 1){
				this->inOrderPred = this->leftChild->inOrderPred;
				if(this->inOrderPred)
					this->inOrderPred->inOrderSucc = this;
				delete this->leftChild;
				this->leftChild = NULL;
			}
			else
				this->leftChild = this->leftChild->deleteValue(temp->score, temp->value);
		}
		else if(this->rightChild){
			treeNode *temp = this->inOrderSucc;
			this->score = temp->score;
			this->value = temp->value;
			if(getWeight(this->rightChild) == 1){
				this->inOrderSucc = this->rightChild->inOrderSucc;
				if(this->inOrderSucc)
					this->inOrderSucc->inOrderPred = this;
				delete this->rightChild;
				this->rightChild = NULL;
			}
			else
				this->rightChild = this->rightChild->deleteValue(temp->score, temp->value);
		}
		else{
			delete this;
			return NULL;
		}
	}
	this->heightWeightUpdate();
	return this->balanceHeight();
}

long long treeNode::getRank(double &score, string &value){
	if(this->score > score || (this->score == score && this->value > value))
		return (this->leftChild?this->leftChild->getRank(score, value):-10000000);	
	else if(this->score < score || (this->score == score && this->value < value))
		return 1 + getWeight(this->leftChild) + (this->rightChild?this->rightChild->getRank(score, value):-10000000);
	else
		return 1 + getWeight(this->leftChild);
	return 0; 
}

treeNode* treeNode::findXthRankNode(long long x){
	if(getWeight(this->leftChild) == (x-1))
		return this;
	else if(getWeight(this->leftChild) >= x)
		return this->leftChild?this->leftChild->findXthRankNode(x):NULL;
	else
		return this->rightChild?this->rightChild->findXthRankNode(x - getWeight(this->leftChild) - 1):NULL;
	return NULL;
}

void treeNode::falttenNode(string &str){
	if(this->leftChild) this->leftChild->falttenNode(str);
	str.append(" " + to_string(score) + " " + value);
	if(this->rightChild) this->rightChild->falttenNode(str);
}
