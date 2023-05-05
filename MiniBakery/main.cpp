#include <Windows.h>
#include <stack>
#include <mutex>
#include <thread>
#include "BakeryArgorithm.h"


#pragma region Data
HWND producerHWND;
HWND consumerHWND[5];
HANDLE thread[5];
HINSTANCE hInst;
HWND text[5];
HWND producerText;
volatile int value[5] = {0,0,0,0,0};
volatile bool calculate = false;
volatile int shareData;
int count=0;
BakeryArgorithm lock(5);

int id[5] = { 0,1,2,3,4 };
HMENU ButtonID[5] = { (HMENU)0x1006 ,(HMENU)0x1007 ,(HMENU)0x1008 ,(HMENU)0x1009 ,(HMENU)0x100A };

std::mutex m;

#pragma endregion 
#pragma region ConsumnerWinProc
LRESULT CALLBACK consumerWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{

	WCHAR tmp[10];
	WCHAR str[20];
	switch (iMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CREATE:
		GetWindowText(hwnd, str, 20);
		wsprintf(tmp, L"%d", value[static_cast<int>(str[8])-48]);
		text[static_cast<int>(str[8])-48] = CreateWindow(L"static", tmp, WS_CHILD | WS_VISIBLE,
			50, 20, 100, 20, hwnd, NULL, hInst, NULL);
		break;
	case WM_COMMAND:
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
#pragma endregion
#pragma region ProducerWinProc
LRESULT CALLBACK producerWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static WCHAR buf[30];
	switch (iMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CREATE:
		for (int i = 0; i < 5; i++) {
			wsprintf(buf, L"%d俺狼 户 积己", (int)pow(10, i));
			CreateWindow(L"button", buf, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				40, 20 + (i * 50), 150, 25, hwnd, ButtonID[i], hInst, NULL);
		}
		wsprintf(buf, L"醚 积魂 户 荐 : %d",count);
		producerText = CreateWindow(L"static", buf, WS_CHILD | WS_VISIBLE,
			40, 270,200, 20, hwnd, NULL, hInst, NULL);
		break;
	case WM_COMMAND:
		if (calculate == false) {
			switch (LOWORD(wParam)) {
			case 0x1006:
				count += 1;
				shareData += 1;
				calculate = true;
				break;
			case 0x1007:
				count += 10;
				shareData += 10;
				calculate = true;
				break;
			case 0x1008:
				count += 100;
				shareData += 100;
				calculate = true;
				break;
			case 0x1009:
				count += 1000;
				shareData += 1000;
				calculate = true;
				break;
			case 0x100A:
				count += 10000;
				shareData = 10000;
				calculate = true;
				break;
			}
			wsprintf(buf, L"醚 积魂 户 荐 : %d", count);
			SetWindowText(producerText, buf);
		}
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
#pragma endregion
#pragma region RegistWindowClasses
void RegistWindowClasses(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) {
	hInst = hInstance;
	WNDCLASS    wndclass;

	wndclass.style = 0;
	wndclass.lpfnWndProc = producerWndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = NULL;
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wndclass.lpszMenuName = L"Producer";
	wndclass.lpszClassName = L"Producer";
	RegisterClass(&wndclass);

	wndclass.lpfnWndProc = consumerWndProc;
	wndclass.lpszMenuName = L"Consumer";
	wndclass.lpszClassName = L"Consumer";
	RegisterClass(&wndclass);
}
#pragma endregion

DWORD ConsumerEntry(void* id) {

	int tid = *((int*)id);

	WCHAR buf[20];
	wsprintf(buf, L"%s%d", L"Consumer", tid);

	consumerHWND[tid] = CreateWindow(L"Consumer",
		buf,
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		0, 150 * (tid), 200, 100,
		NULL, NULL, hInst, NULL);
	MSG msg;
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (calculate) {
			 m.lock();
			//lock.b_lock(tid,5);
			if (shareData > 0) {
				shareData--;
				value[tid]++;
				WCHAR buf2[20];
				wsprintf(buf2, L"%d", value[tid]);
				SetWindowText(text[tid], buf2);
			}
			else {
				calculate = false;
			}
			 m.unlock();
			//lock.b_unlock(tid);
			std::this_thread::yield();
		}
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine,  int iCmdShow) {

	RegistWindowClasses(hInstance, hPrevInstance, szCmdLine, iCmdShow);

	for (int i = 0; i < 5; i++)
		thread[i] = CreateThread(NULL, 0, ConsumerEntry, &id[i], 0, NULL);


#pragma region CreateProducerSystem
	producerHWND = CreateWindow(L"Producer",
		L"Producer System",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		500, 200, 250, 500,
		NULL, NULL, hInstance, szCmdLine);
#pragma endregion

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}