#include <Windows.h>
#include "Progressbar.h"
#include "Position.h"
#include "DATA.h"
#include <unordered_map>
#include <stdlib.h>
#include <time.h>


enum class Scheldular {RoundRobin, SJF};

HINSTANCE hInst;
TCHAR tmpBuffer[30];
Scheldular schedular = Scheldular::RoundRobin;
const int regulationTime = 2;


LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

void RegistMainWondwsClass(HINSTANCE hInstance) {
	WNDCLASS    wndclass;
	RECT        rect;

	wndclass.style = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = NULL;
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wndclass.lpszMenuName = L"Operating";
	wndclass.lpszClassName = L"Operating";
	RegisterClass(&wndclass);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)

{
	hInst = hInstance;
	srand(time(0));
	RegistMainWondwsClass(hInstance);
	Progressbar(hInstance).registClass();
	DATA::initData();
	DATA::initProgressBar(hInstance);
	if (schedular == Scheldular::RoundRobin)
		DATA::RoundRobin(regulationTime);
	else if (schedular == Scheldular::SJF)
		DATA::SJF();
	else
		exit(1);
	DATA::calcAVG();
	HWND hwnd;
	MSG msg;

#pragma region CreateOperatingSystem
	hwnd = CreateWindow(L"Operating",
		L"Prcoess Scheduling",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_SYSMENU,
		0, 0, 550, 200,
		NULL, NULL, hInstance, szCmdLine);
#pragma endregion
#pragma region MSGfetcher
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
#pragma endregion

	return (int)msg.wParam;
}




LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
#pragma region WM_DESTROY
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
#pragma endregion
#pragma region WM_CREATE
	case WM_CREATE:
		for (long i = 0; i < DATA::progressList.size(); i++) {
			swprintf_s(tmpBuffer, 30, L"Process%d", i+1);
			DATA::progressList[i].createWindow(hwnd, { POS_X,POS_Y(i),LEN_X, LEN_Y }, i + 1, tmpBuffer);
		}
		swprintf_s(tmpBuffer, 30, L"��� ��ȯ �ð� : %0.2f", DATA::returnAVG);
		CreateWindow(L"static", tmpBuffer, WS_CHILD | WS_VISIBLE, POS_X, POS_Y(DATA::processList.size()), LEN_X, LEN_Y, hwnd, NULL, hInst, NULL);
		swprintf_s(tmpBuffer, 30, L"��� ��� �ð� : %0.2f", DATA::waitAVG);
		CreateWindow(L"static", tmpBuffer, WS_CHILD | WS_VISIBLE, POS_X, POS_Y(DATA::processList.size()) + 40, LEN_X, LEN_Y, hwnd, NULL, hInst, NULL);
		if (schedular == Scheldular::RoundRobin) {
			swprintf_s(tmpBuffer, 30, L"RoundRobin");
			CreateWindow(L"static", tmpBuffer, WS_CHILD | WS_VISIBLE, POS_X - 250, POS_Y(DATA::processList.size()), 100, LEN_Y, hwnd, NULL, hInst, NULL);
			swprintf_s(tmpBuffer, 30, L"���� �ð� : %d", regulationTime);
			CreateWindow(L"static", tmpBuffer, WS_CHILD | WS_VISIBLE, POS_X - 250, POS_Y(DATA::processList.size()) + 40, 100, LEN_Y, hwnd, NULL, hInst, NULL);
		}
		else if (schedular == Scheldular::SJF) {
			swprintf_s(tmpBuffer, 30, L"SJF");
			CreateWindow(L"static", tmpBuffer, WS_CHILD | WS_VISIBLE, POS_X - 250, POS_Y(DATA::processList.size()), 100, LEN_Y, hwnd, NULL, hInst, NULL);
		}
		break;
#pragma endregion
#pragma region Invarient_SIZE
	case WM_GETMINMAXINFO:        // ������ ������ ����� �߻��ϴ� �̺�Ʈ
		((LPMINMAXINFO)lParam)->ptMinTrackSize.x = 1280;        // ������ �ּ� ������ x
		((LPMINMAXINFO)lParam)->ptMinTrackSize.y = POS_Y(DATA::processList.size()) + 125;        // ������ �ּ� ������ y
		((LPMINMAXINFO)lParam)->ptMaxTrackSize.x = 1280;        // ������ �ִ� ������ x
		((LPMINMAXINFO)lParam)->ptMaxTrackSize.y = POS_Y(DATA::processList.size()) + 125;        // ������ �ִ� ������ y

		((LPMINMAXINFO)lParam)->ptMaxSize.x = 500;            // MAX ��ư Ŭ���� �ִ� ������ x
		((LPMINMAXINFO)lParam)->ptMaxSize.y = 300;            // MAX ��ư Ŭ���� �ִ� ������ y
		((LPMINMAXINFO)lParam)->ptMaxPosition.x = 100;        // MAX ��ư Ŭ���� x��ġ
		((LPMINMAXINFO)lParam)->ptMaxPosition.y = 100;        // MAX ��ư Ŭ���� y��ġ
		break;
#pragma endregion
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
