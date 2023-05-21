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
	// index�� 0�� ������ ������ ��, �ָ� �ֽ�
	int point = -1;

	// 1. �̹� page�� ���� �޸𸮿� �ö� �ִ� ���.
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
	
	// 2. page�� ���� �޸𸮿� �ö� ���� �ʰ�, ���� �޸𸮰� ���� �� ���� �ʴ� ���
	if (vec.size() < maxSize) {
		vec.push_back(in);
		timeLine.push_back({ vec , in });	// for GUI
		return;
	}

	// 3. page�� ���� �޸𸮿� �ö� ���� �ʰ�, ���� �޸𸮰� ���� �� �ִ� ���
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
