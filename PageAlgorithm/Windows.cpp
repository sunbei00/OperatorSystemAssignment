#include "Windows.h"
#include "LRU.h"
#include <algorithm>



Windows* Windows::instance;
HINSTANCE Windows::hInstance;

LRESULT Windows::sMainWindowProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	return GetInstance()->MainWindowProc(hwnd, iMsg, wParam, lParam);
}

LRESULT Windows::MainWindowProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static TCHAR text[30];
	int sdads = LRU::getInstance()->getInputCount();

	switch (iMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CREATE:
		time = CreateWindow(L"static", L"time : 0", WS_CHILD | WS_VISIBLE, 5 , 5, 100, 30, hwnd, NULL, hInstance, NULL);
		hScroll = ::CreateWindow(L"scrollbar", NULL, WS_CHILD | WS_VISIBLE | SBS_HORZ, 10, 70, 200, 20, hwnd, (HMENU)ID_hScroll, hInstance, NULL);
		inputValue = CreateWindow(L"static", L"Input Value : x", WS_CHILD | WS_VISIBLE, (SIZE_X / 2) - 60, 30, 100, 50, hwnd, NULL, hInstance, NULL);

		for (int i = 0; i < LRU::getInstance()->getMaxSize(); i++) {
			swprintf_s(text, 30, L"%d : ", i + 1);
			memoryIndex.push_back(CreateWindow(L"static", text, WS_CHILD | WS_VISIBLE, (SIZE_X / 2) - 100, 80 + 50 * i, 30, 30, hwnd, NULL, hInstance, NULL));
		}
		for (int i = 0; i < LRU::getInstance()->getMaxSize(); i++)
			memoryPage.push_back(CreateWindow(L"static", L"x", WS_CHILD | WS_VISIBLE, (SIZE_X / 2), 80 + 50 * i, 30, 30, hwnd, NULL, hInstance, NULL));


		::SetScrollPos(hScroll, SB_CTL, scrollPos, TRUE);
		break;
	case WM_SIZE:
		SetWindowPos(hScroll, NULL, 0, HIWORD(lParam) - 30, LOWORD(lParam), 30, 0);
		// SetWindowPos(DATA::thickProgressBar, NULL, (0 - ((float)scrollPos / 10)) * (DATA::window_size ? 430 : 800), 30, 30, 150, 0);
		::InvalidateRect(hwnd, NULL, TRUE);
		break;
	case WM_HSCROLL:
		::SetScrollRange(hScroll, SB_CTL, 0, LRU::getInstance()->getInputCount(), TRUE);
		switch (LOWORD(wParam))
		{
		case SB_LINELEFT:
			scrollPos = max(0, scrollPos - 1);
			break;
		case SB_PAGELEFT:
			scrollPos = max(0, scrollPos - 1);
			break;
		case SB_LINERIGHT:
			scrollPos = min(LRU::getInstance()->getInputCount(), scrollPos + 1);
			break;
		case SB_PAGERIGHT:
			scrollPos = min(LRU::getInstance()->getInputCount(), scrollPos + 1);
			break;
		case SB_THUMBTRACK:
			scrollPos = HIWORD(wParam);
			break;
		}
		::SetScrollPos(hScroll, SB_CTL, scrollPos, TRUE);

		if (scrollPos == 0) {
			SetWindowText(inputValue, L"Input Value : x");
			for (int i = 0; i < memoryPage.size(); i++)
				SetWindowText(memoryPage[0], L"x");
		}
		else {
			swprintf_s(text, 30, L"Input Value : %d", LRU::getInstance()->timeLine[scrollPos - 1].input);
			SetWindowText(inputValue, text);
			for (int i = 0; i < memoryPage.size(); i++) {
				if (LRU::getInstance()->timeLine[scrollPos - 1].vec.size() - 1 >= i) {
					swprintf_s(text, 30, L"%d", LRU::getInstance()->timeLine[scrollPos - 1].vec[i]);
					SetWindowText(memoryPage[i], text);
				}
				else
					SetWindowText(memoryPage[i], L"x");
			}
		}
		swprintf_s(text, 30, L"time : %d", scrollPos);
		SetWindowText(time, text);


		::InvalidateRect(hwnd, NULL, TRUE);
		break;
	case WM_GETMINMAXINFO:
		((LPMINMAXINFO)lParam)->ptMinTrackSize.x = SIZE_X;
		((LPMINMAXINFO)lParam)->ptMinTrackSize.y = SIZE_Y + LRU::getInstance()->getMaxSize() * 80;
		((LPMINMAXINFO)lParam)->ptMaxTrackSize.x = SIZE_X;
		((LPMINMAXINFO)lParam)->ptMaxTrackSize.y = SIZE_Y + LRU::getInstance()->getMaxSize() * 80;;
		((LPMINMAXINFO)lParam)->ptMaxSize.x = SIZE_X;
		((LPMINMAXINFO)lParam)->ptMaxSize.y = SIZE_Y + LRU::getInstance()->getMaxSize() * 80;
		break;
	case WM_SYSCOMMAND:
	case WM_COMMAND:
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

Windows::Windows(HINSTANCE hInstance, TCHAR* windowClassName)
{
	this->hInstance = hInstance;
	this->windowClassName = windowClassName;
}

void Windows::RegistMainWondwsClass()
{
	WNDCLASS wndclass;

	wndclass.style = 0;
	wndclass.lpfnWndProc = sMainWindowProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = NULL;
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wndclass.lpszMenuName = this->windowClassName;
	wndclass.lpszClassName = this->windowClassName;
	RegisterClass(&wndclass);
}

void Windows::CreateWindows(TCHAR* windowName)
{
	hwnd = ::CreateWindow(this->windowClassName,
		windowName,
		WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_SYSMENU,
		0, 0, 0, 0,
		NULL, NULL, hInstance, NULL);
}

void Windows::MsgFetch()
{
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

Windows* Windows::GetInstance()
{
	return Windows::instance;
}

void Windows::Init(HINSTANCE hInstance, TCHAR* windowClassName, TCHAR* windowName)
{
	if (Windows::instance == NULL) {
		Windows::instance = new Windows(hInstance, windowClassName);
		Windows::instance->RegistMainWondwsClass();
		Windows::instance->CreateWindows(windowName);
	}
}
