#include "DATA.h"
#include <algorithm>

std::unordered_map<int, std::vector<time_step>> DATA::TimeStep;
std::vector<ProcessData> DATA::processList;
std::vector<Progressbar> DATA::progressList;
int DATA::total_time = 0;
float DATA::waitAVG = 0;
float DATA::returnAVG = 0;


void DATA::initData() {
	processList.push_back(ProcessData(0, 10, 1));
	processList.push_back(ProcessData(1, 28, 2));
	processList.push_back(ProcessData(2, 6, 3));
	processList.push_back(ProcessData(3, 4, 4));
	processList.push_back(ProcessData(4, 14, 5));
	processList.push_back(ProcessData(5, 23, 6));
	processList.push_back(ProcessData(6, 9, 7));

	for (auto& it : processList) {
		total_time += it.needTime;
	}
}

bool ProcessData::isFinish() const
{
	if (needTime - exeTime <= 0)
		return true;

	return false;
}

bool ProcessData::isAccess(int time)
{
	if (time >= accessTime)
		return true;
	return false;
}

void ProcessData::operator=(ProcessData& pd)
{
	this->accessTime = pd.accessTime;
	this->exeTime = pd.exeTime;
	this->needTime = pd.needTime;
	this->pId = pd.pId;
}


ProcessData::ProcessData(int accessTime, int needTime, int pId) {
	this->accessTime = accessTime;
	this->exeTime = 0;
	this->needTime = needTime;
	this->pId = pId;
}

void DATA::initProgressBar(HINSTANCE hInstance)
{
	for (auto& it : processList)
		progressList.push_back(Progressbar(hInstance));
}

void DATA::RoundRobin(const int regulationTime)
{
	std::deque<ProcessData> FIFO;
	int currentTime = 654321; // inf

	for (auto& it : processList) // 처음 access가 0초가 아님을 가정
		currentTime = min(currentTime, it.accessTime);

	for (auto& it : processList) // 현재 시간의 access 프로세스 삽입.
		if (it.accessTime == currentTime)
			FIFO.push_back(it);

	while (FIFO.size() != 0) {
		auto it = FIFO.front();
		FIFO.pop_front();

		for (int i = 0; i < regulationTime; i++) {
			DATA::TimeStep[it.pId].push_back({ currentTime,currentTime + 1 });
			currentTime++;
			it.exeTime++;

			for (auto& newProcess : processList) // 현재 시간의 access 프로세스 삽입.
				if (newProcess.accessTime == currentTime)
					FIFO.push_back(newProcess);

			if (it.isFinish())
				break;
		}
		if (!it.isFinish())
			FIFO.push_back(it);
	}

}

void DATA::SJF()
{
	std::vector<ProcessData> vec;
	int currentTime = 654321; // inf

	for (auto& it : processList) // 처음 access가 0초가 아님을 가정
		currentTime = min(currentTime, it.accessTime);

	for (auto& it : processList) // 현재 시간의 access 프로세스 삽입.
		if (it.accessTime == currentTime)
			vec.push_back(it);

	while (vec.size() != 0) {
		int minTime = 987654;
		ProcessData pd(0,0,0);

		for (auto& it : vec)
			if (minTime > it.needTime)
				minTime = it.needTime;
		for (auto it = vec.begin(); it != vec.end(); it++) // erase
			if (minTime == it->needTime) {
				pd = *it;
				for (auto copy = it; copy != vec.end(); copy++) {
					auto next = copy + 1;
					if (next == vec.end())
						break;
					*copy = *next;
				}
				vec.pop_back();
				break;
			}		

		for (int i = 0; i < pd.needTime; i++) {
			DATA::TimeStep[pd.pId].push_back({ currentTime,currentTime + 1 });
			currentTime++;
			pd.exeTime++;

			for (auto& newProcess : processList) // 현재 시간의 access 프로세스 삽입.
				if (newProcess.accessTime == currentTime)
					vec.push_back(newProcess);
		}
	}

}

void DATA::calcAVG() {
	DATA::returnAVG = 0;
	DATA::waitAVG = 0;

	for (auto& it : DATA::TimeStep) {
		auto& vec = it.second;
		int finish = vec[vec.size() - 1].end;
		int firstCalc = vec[0].start;
		int pid = it.first;
		int accessTime = 0;
		for (auto& process : processList)
			if (pid == process.pId)
				accessTime = process.accessTime;

		waitAVG += finish - accessTime - it.second.size();
		returnAVG += finish - accessTime;
	}
	DATA::returnAVG /= TimeStep.size();
	DATA::waitAVG /= TimeStep.size();

}
