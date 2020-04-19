#include "models.hpp"
#include <fstream>
#include <sstream>
#include <ctime>

modelValue::modelValue(int typ, int timeout = 0){
	type = typ;
	expireTime = timeout;
	set = NULL;
	if(type == 1)
		set  = new setValue;
}

modelValue::~modelValue(){
	if(set)
		delete set;
}

void modelValue::deleteSet(){
	delete set;
}

void modelValue::flipType(){
	type ^= 1;
	expireTime = 0;
}

int modelValue::getType(){
	return type;
}

string modelValue::getValue(){
	return value ;
}

void modelValue::updateValue(string &s){
	value = s;
	return;
}

int modelValue::zadd(pair <double, string> &scoresValues){
	if(!set)
		set = new setValue();
	return set->addValue(scoresValues.first, scoresValues.second);
}

long long modelValue::zrank(string &s){
	return set->findRank(s);
}

vector<string> modelValue::zrange(long long &start, long long &end){
	return set->rangeValues(start, end);
}

string modelValue::serialise(){
	// cout<<type<<" "<<value<<"\n";
	if(!type) return to_string(this->type)+ " " +to_string(this->expireTime) + " " + value;
	else return to_string(this->type)+ " " +to_string(this->expireTime) + set->serialise();
}

void modelValue::deserialise(vector<string> &values){
	if(!type) value = values[0];
	else set->build(values);
}

void modelValue::setExpire(int timeout){
	expireTime = time(NULL) + timeout;
}

bool modelValue::isExpire(){
	if(!type && expireTime>0 && expireTime<time(NULL)) return true;
	return false;
}

redis::~redis(){
	this->dump();
	for(auto &temp: hashTable) delete temp.second;
}

redis::redis(){
	this->load();
}

void redis::removeExpiredKey(string key){
	if(!hashTable.count(key)) return;
	modelValue *temp = hashTable[key];
	if(!temp->isExpire()) return;
	delete temp;
	hashTable.erase(key);
	return;
}

string redis::GET(string key){
	this->removeExpiredKey(key);
	if(!hashTable.count(key)) return "Key Not Found";
	modelValue *temp = hashTable[key];
	if(temp->getType()) return "Invalid Value Type To Perform Operation";
	return temp->getValue();
}

int redis::SET(string key, string value){
	this->removeExpiredKey(key);
	modelValue *temp;
	if(hashTable.count(key)) temp = hashTable[key];
	else{
		temp = new modelValue(0);
		hashTable[key] = temp;
	}
	if(temp->getType()){
		temp->deleteSet();
		temp->flipType();	
	}
	temp->updateValue(value);
	return 1;
}

int redis::EXPIRE(string key, int timeout){
	this->removeExpiredKey(key);
	if(timeout<0) return -4;
	if(!hashTable.count(key)) return -1;
	modelValue *temp = hashTable[key];
	if(temp->getType()) return -2;
	temp->setExpire(timeout);
	return 1;
}
int redis::ZADD(string key, pair<double, string> scoresValues){
	this->removeExpiredKey(key);
	modelValue *temp;
	if(hashTable.count(key)) temp = hashTable[key];
	else{
		temp = new modelValue(1);
		hashTable[key] = temp;
	}
	if(!temp->getType()) return -2;
	return temp->zadd(scoresValues);
}

long long redis::ZRANK(string key, string value){
	this->removeExpiredKey(key);
	if(!hashTable.count(key)) return -1;
	modelValue *temp = hashTable[key];
	if(temp->getType()==0) return -2;
	return temp->zrank(value);
}

vector<string> redis::ZRANGE(string key, long long start, long long end){
	this->removeExpiredKey(key);
	if(!hashTable.count(key)) return {"Key Not Found"};
	modelValue *temp = hashTable[key];
	if(temp->getType()==0) return {"Invalid Value Type To Perform Operation"};
	return temp->zrange(start, end);
}

void redis::dump(){
	cout<<"Save In Progress\n";
	string seralisedData="";
	ofstream dbFile;
	dbFile.open("redisBackup.txt");
	for(auto &key_pair: hashTable){
		seralisedData = key_pair.second->serialise();
		dbFile << key_pair.first <<" "<< seralisedData<<"\n";
	}
	dbFile.close();
	cout<<"Completed\n";
}

void redis::load(){
	ifstream dbFile;
	dbFile.open("redisBackup.txt");
	string line;
	istringstream stream;
	vector<string> vec;
	string token, key, type, timeout;
	int ind;
	while(getline(dbFile, line)){
		ind = 0;
		if(!line.size()) break;
		vec.clear();
		stream = istringstream(line);
		while(getline(stream, token, ' ')){
			if(!ind)
				key = token;
			else if(ind==1)
				type = token;
			else if(ind==2)
				timeout=token;
			else
				vec.push_back(token);
			ind++;
		}
		modelValue *temp = new modelValue(stoi(type), stoi(timeout));
		hashTable[key] = temp;
		temp->deserialise(vec);
	}
	dbFile.close();
}