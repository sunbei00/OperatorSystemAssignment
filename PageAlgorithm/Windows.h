#pragma once
#include <Windows.h>
#include <vector>


#define SIZE_X 300
#define SIZE_Y 50
#define ID_hScroll (0x1000)


class Windows
{

	static Windows* instance;
	static HINSTANCE hInstance;

	HWND hwnd;
	int scrollPos = 0;
	MSG msg;
	TCHAR* windowClassName;
	HWND hScroll;
	HWND inputValue;
	HWND time;
	std::vector<HWND> memoryIndex;
	std::vector<HWND> memoryPage;
	
	static LRESULT CALLBACK sMainWindowProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
	Windows(HINSTANCE hInstance, TCHAR* windowClassName);
	void RegistMainWondwsClass();
	void CreateWindows(TCHAR* windowName);
public:
	void MsgFetch();
	static Windows* GetInstance();
	static void Init(HINSTANCE hInstance, TCHAR* windowClassName, TCHAR* windowName);

};

