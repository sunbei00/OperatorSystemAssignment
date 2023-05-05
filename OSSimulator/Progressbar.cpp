#include "Progressbar.h"
#include "DATA.h"
#include <stdlib.h>


WCHAR buffer[40];


LRESULT CALLBACK ProgressProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static HDC hdc;
	static PAINTSTRUCT ps;
	static RECT rc;
	static RECT tmp;
	static HBRUSH hBrush;
	switch (iMsg)
	{
#pragma region WM_DESTROY
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
#pragma endregion
#pragma region WM_COMMAND
	case WM_PAINT:
		hBrush = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
		hdc = BeginPaint(hwnd, &ps);

		GetClientRect(hwnd, &rc);
		GetWindowText(hwnd, buffer, 10);
		if (DATA::TimeStep.find(_wtoi(buffer)) != DATA::TimeStep.end()) {
			std::vector<time_step> vec = DATA::TimeStep[_wtoi(buffer)];
			for (auto it : vec) {
				tmp = rc;
				tmp.left = ((float)it.start / DATA::total_time) * rc.right;
				tmp.right = ((float)it.end / DATA::total_time) * rc.right;
				FillRect(hdc, &tmp, hBrush);
			}
		}

		hBrush = CreateSolidBrush(RGB(0, 0, 0));
		MoveToEx(hdc, rc.left, rc.top, NULL);
		LineTo(hdc, rc.right, rc.top);
		MoveToEx(hdc, rc.right - 1, rc.top, NULL);
		LineTo(hdc, rc.right - 1, rc.bottom);
		MoveToEx(hdc, rc.left, rc.bottom - 1, NULL);
		LineTo(hdc, rc.right, rc.bottom - 1);
		MoveToEx(hdc, rc.left, rc.bottom, NULL);
		LineTo(hdc, rc.left, rc.top);

		EndPaint(hwnd, &ps);
		break;
	}
#pragma endregion

	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}


Progressbar::Progressbar(HINSTANCE hInstance)
{
	this->hInstance = hInstance;
}

void Progressbar::registClass() {
	WNDCLASS    wndclass;

	wndclass.style = 0;
	wndclass.lpfnWndProc = ProgressProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = NULL;
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wndclass.lpszMenuName = L"ProgressBar";
	wndclass.lpszClassName = L"ProgressBar";
	RegisterClass(&wndclass);
}

void Progressbar::createWindow(HWND parent, RECT rect, int pid, WCHAR* text) {
	
	wsprintf(buffer,L"%d", pid);
	hwnd = CreateWindow(L"ProgressBar",
		buffer,
		WS_CHILD | WS_VISIBLE,
		rect.left, rect.top, rect.right, rect.bottom,
		parent, NULL, hInstance, NULL);

	caption = CreateWindow(L"static", text, WS_CHILD | WS_VISIBLE,
		rect.left - 100, rect.top, 100, rect.bottom, parent, NULL, hInstance, NULL);

	int accessTime = 0;
	int needTime = 0;
	for(auto it : DATA::processList)
		if (it.pId == pid) {
			accessTime = it.accessTime;
			needTime = it.needTime;
		}

	swprintf_s(buffer,40,L"Access Time : %d\nNeed Time : %d", accessTime, needTime);
	caption = CreateWindow(L"static", buffer, WS_CHILD | WS_VISIBLE,
		rect.left - 250, rect.top, 150, rect.bottom, parent, NULL, hInstance, NULL);
}

