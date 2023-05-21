#include "LRU.h"

LRU* LRU::instance = NULL;

LRU::LRU(int maxSize)
{
	this->maxSize = maxSize;
	vec.reserve(maxSize);
}

void LRU::Init(int maxSize)
{
	getInstance()->maxSize = maxSize;
	getInstance()->vec.reserve(maxSize);
}

LRU* LRU::getInstance()
{
	if (instance == NULL)
		instance = new LRU(0);
	return instance;
}

void LRU::push(int in)
{
	// index가 0에 가까우면 오래된 것, 멀면 최신
	int point = -1;

	// 1. 이미 page가 메인 메모리에 올라가 있는 경우.
	for (int i = 0; i < vec.size(); i++)
		if (vec[i] == in)
			point = i;

	if (point != -1) {
		for (int i = point; i != vec.size() - 1; i++)
			vec[i] = vec[i + 1];
		vec[vec.size() - 1] = in;
		timeLine.push_back({ vec , in });	// for GUI
		return;
	}
	
	// 2. page가 메인 메모리에 올라 있지 않고, 메인 메모리가 가득 차 있지 않는 경우
	if (vec.size() < maxSize) {
		vec.push_back(in);
		timeLine.push_back({ vec , in });	// for GUI
		return;
	}

	// 3. page가 메인 메모리에 올라 있지 않고, 메인 메모리가 가득 차 있는 경우
	for (int i = 0; i < vec.size()-1; i++)
		vec[i] = vec[i+1];
	vec[vec.size() -1] = in;

	timeLine.push_back({ vec , in });	// for GUI
}

int LRU::getMaxSize()
{
	return maxSize;
}

int LRU::getInputCount()
{
	return timeLine.size();
}
