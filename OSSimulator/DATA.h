#pragma once
#include <unordered_map>
#include <vector>
#include <Windows.h>
#include <deque>
#include "Progressbar.h"


struct time_step {
	int start;
	int end;
};

class ProcessData {
public:
	int pId;
	int accessTime;
	int exeTime;
	int needTime;
	bool isFinish() const;
	bool isAccess(int time);
	void operator=(ProcessData& pd);
	ProcessData(int accessTime, int needTime,int pId);
};

class DATA
{
public:
	static std::unordered_map<int, std::vector<time_step>> TimeStep;
	static int total_time;
	static float returnAVG;
	static float waitAVG;
	static std::vector<ProcessData> processList;
	static std::vector<Progressbar> progressList;
	static void initData();
	static void initProgressBar(HINSTANCE hInstance);
	static void RoundRobin(const int);
	static void SJF();
	static void calcAVG();
};

