#pragma once
#include <vector>

typedef struct{
	std::vector<int> vec;
	int input;
} time;


class LRU {
private:
	static LRU* instance;
	int maxSize = 0;

	LRU(int maxSize);
public:
	std::vector<int> vec;
	std::vector<time> timeLine;

	static void Init(int maxSize);
	static LRU* getInstance();
	void push(int in);
	int getMaxSize();
	int getInputCount();
};

