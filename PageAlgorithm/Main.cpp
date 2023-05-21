#include <Windows.h>
#include "Windows.h"
#include "LRU.h"




int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) {
	
	LRU::Init(4);

	LRU::getInstance()->push(0);
	LRU::getInstance()->push(1);
	LRU::getInstance()->push(2);
	LRU::getInstance()->push(3);
	LRU::getInstance()->push(0);
	LRU::getInstance()->push(1);
	LRU::getInstance()->push(4);
	LRU::getInstance()->push(0);
	LRU::getInstance()->push(1);
	LRU::getInstance()->push(2);
	LRU::getInstance()->push(3);
	LRU::getInstance()->push(4);

	Windows::Init(hInstance, (TCHAR*) L"Main", (TCHAR*) L"LRU Algorithm");
	Windows::GetInstance()->MsgFetch();

	return 0;
}

