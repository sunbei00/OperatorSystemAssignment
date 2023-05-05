#pragma once
#include <Windows.h>

class Progressbar
{
private:
	HINSTANCE hInstance;
	HWND hwnd;
	HWND caption;
public:
	Progressbar(HINSTANCE hInstance);
	void registClass();
	void createWindow(HWND hwnd, RECT rect, int id, WCHAR* text);
};

