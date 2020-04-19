#include "AVL.hpp"
#include <ctime>
class modelValue{
	private:
		int type;
		time_t expireTime;
		string value;
		setValue *set;
	public:
		modelValue(int type, int timeout);
		~modelValue();
		int getType();
		void deleteSet();
		void flipType();
		string getValue();
		void updateValue(string &s);
		int zadd(pair<double, string> &scoresValues);
		long long zrank(string &s);
		vector<string> zrange(long long &start, long long &end);
		string serialise();
		void deserialise(vector<string> &values);
		void setExpire(int timeout);
		bool isExpire();
};

class redis{
private:
	unordered_map<string, modelValue *> hashTable;
public:
	int SET(string key, string value);
	string GET(string key);
	int EXPIRE(string key, int timeout);
	int ZADD(string key, pair<double, string> scoresValues);
	long long ZRANK(string key, string value);
	vector<string> ZRANGE(string key, long long start, long long end);
	void removeExpiredKey(string key);
	void dump();
	void load();
	~redis();
	redis();
};
