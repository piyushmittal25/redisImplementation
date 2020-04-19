# redisImplementation
A C++14 program which simulate some core features of redis database and also have persistent behaviour.

This is a basic implementation of Redis with some basic functionalities like -
1. **GET**(https://redis.io/commands/get)
2. **SET**(https://redis.io/commands/set)
3. **EXPIRE**(https://redis.io/commands/expire)
4. **ZADD**(https://redis.io/commands/zadd)
5. **ZRANK**(https://redis.io/commands/zrank)
6. **ZRANGE**(https://redis.io/commands/zrange)

To Run the code in your system you must have g++(x>=11):
**STEP 1:** Clone the repo from https://github.com/piyushmittal25/redisImplementation.git
**STEP 2:** g++ -std=c++14 main.cpp RedisObj.cpp AVLTree.cpp Node.cpp -o redis
**STEP 3:** By above command you will get executable file **redis**
**STEP 4:** OS Based: Linux => **./redis** in terminal in same directory
                      Windows => **redis** in command prompt

**Note:** For making it persistant, I am saving serialised data of our redis in **radisBackup.txt**. If you delete that file you will loss all your data.

