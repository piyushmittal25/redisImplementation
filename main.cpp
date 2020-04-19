#include "models.hpp"
#include <sstream>

int commandValue(string s){
	if(s == "GET") return 1;
	if(s == "SET") return 2;
	if(s == "EXPIRE") return 3;
	if(s == "ZADD") return 4;
	if(s == "ZRANK") return 5;
	if(s == "ZRANGE") return 6;
	if(s == "SAVE") return 7;
	if(s == "EXIT") return 8;
	return 0;
}

string error_strings(int responseCode){
	switch(responseCode){
		case -1: return "Key Not Found";
				 break;
		case -2: return "Invalid Value Type To Perform Operation";
				 break;
		case -3: return "Key Exist but value didn't Exist";
				 break;
		case -4: return "Invalid Command";
				 break;
		default: return "Successfull";
				 break;
	}
}

bool isInteger(string &s){
	int ind = -1;
	for(auto &c: s){
		ind++;
		if(!ind && c=='-')continue;
		if(!isdigit(c)) return false;
	}
	return true;
}

bool isDouble(string &s){
	int dot =0;
	int ind = -1;
	for(auto &c: s){
		ind++;
		if(!ind && c=='-')continue;
		if(c=='.') dot++;
		else if(!isdigit(c)) return false;
	}
	return dot<2;
}

vector<string> tokenise(string s){
	istringstream stream(s);
	string token;
	vector<string> words;
	while(getline(stream, token, ' '))
	{
		if(token.size() && token[0]!=' ')	
			words.push_back(token);
	}
	return words;
}

bool isCommandValid(vector<string> &tokens){
	switch(commandValue(tokens[0])){
		case 1: if(tokens.size()!=2) return false;
					break;
		case 2: if(tokens.size()!=3) return false;
					break;
		case 3: if(tokens.size()!=3 || !isInteger(tokens[2])) return false;
					break;
		case 4: if(tokens.size()<4 || tokens.size()%2) return false;
			    for(int i = 2; i<tokens.size(); i+= 2) if(!isDouble(tokens[i])) return false;
					break;
		case 5: if(tokens.size()!=3) return false;
					break;
		case 6: if(tokens.size()!=4 || !isInteger(tokens[2]) || !isInteger(tokens[3])) return false;
					break;
		case 7: if(tokens.size()!=1) return false;
				    break;
		case 8: if(tokens.size()!=1) return false;
					break;
		default:  return false;
					   break;

	}
	return true;
}

int main(){
	redis *db = new redis();
	string command, key, value, line;
	int res, timeout, flag = true;
	long long start, end;
	vector<string> strs, ret;
	double score;
	cout<<"-------------- Redis ----------------\n";
	cout<<"Format for Giving Following Commands:\nGET string_key\nSET string_key string_value\nEXPIRE string_key int_timeout_sec\n";
	cout<<"ZADD string_key double_score string_values\nZRANK string_key string_value\nZRANGE string_key int_start_index int_end_index\nNote: You can add multiple pairs of score and value pair in single command\n";
	while (flag){
		cout<<">> ";
		getline(cin,line);
		strs = tokenise(line);
		cout<<"==> ";
		if(!isCommandValid(strs)){
			cout<<error_strings(-4)<<"\n";
			continue;
		}
		if(strs.size()>1)
			key = strs[1];
		switch(commandValue(strs[0])){
			case 1: cout<<db->GET(key)<<"\n";
						break;
			case 2: value = strs[2];
			        cout<<error_strings(db->SET(key, value))<<"\n";
			        break;
			case 3: timeout = stoi(strs[2]);
	                cout<<error_strings(db->EXPIRE(key, timeout))<<"\n";
	                break;
			case 4: for(int i = 2; i < strs.size(); i+=2)
					cout<<error_strings(db->ZADD(key, make_pair(stod(strs[i]), strs[i+1])))<<"\n";
						break;
			case 5: value = strs[2];
			        res = db->ZRANK(key, value);
			        if(res>=0)
					    cout<<res<<"\n";
					else
						cout<<error_strings(res)<<"\n";
						break;
			case 6: ret = db->ZRANGE(key, stoi(strs[2]), stoi(strs[3]));
					for(auto &s: ret) cout<<s<<"\n";
						break;
			case 7: db->dump();
			        break;
			case 8: flag = false;
					break;
			default: cout<<error_strings(-4)<<"\n";
 
		}
	}
	delete db;
	return 0;
}