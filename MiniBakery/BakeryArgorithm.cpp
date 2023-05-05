#include "BakeryArgorithm.h"

BakeryArgorithm::BakeryArgorithm(int cnt) {
	int count = cnt;
	g_Flag = new int[cnt]; // flag
	g_Label = new int[cnt]; // 번호
	for (int i = 0; i < cnt; ++i)
	{
		g_Label[i] = 0;
		g_Flag[i] = false;
	}
}

void BakeryArgorithm::setFlag(int i, bool flag) {
	g_Flag[i] = flag;
}

void BakeryArgorithm::setLabel(int i, int label) {
	g_Label[i] = label;
}

bool BakeryArgorithm::getFlag(int i) {
	return g_Flag[i];
}

int BakeryArgorithm::getLabel(int i) {
	return g_Label[i];
}

int BakeryArgorithm::Max(int thread_num) // 가장 큰 번호 찾기
{
	int maximum = getLabel(0);
	for (int i = 0; i < thread_num; ++i)
	{
		if (maximum < getLabel(i))
			maximum = getLabel(i);
	}
	return maximum;
}

int BakeryArgorithm::Compare(int i, int myId) {
	if (getLabel(i) < getLabel(myId)) return 1; // 대기
	else if (getLabel(i) > getLabel(myId)) return 0; // 패스
	else
	{
		// 두 번호가 같을경우
		if (i < myId) return 1;
		else if (i > myId) return 0;
		else return 0;
	}

}

void BakeryArgorithm::b_lock(int myId, int thread_num) {
	// 번호표 받을 준비
	setFlag(myId, true);
	// 다음 번호를 받음
	setLabel(myId, Max(thread_num) + 1);
	setFlag(myId, false);
	// 모든 프로세스에 대한 번호표 비교루프
	for (int i = 0; i < thread_num; ++i)
	{
		while (getFlag(i)); // 누군가 티켓을 받고있으면 대기.
		while ((getLabel(i) != 0) && Compare(i, myId));
	}
}

void BakeryArgorithm::b_unlock(int myId) {
	setLabel(myId, 0);
}