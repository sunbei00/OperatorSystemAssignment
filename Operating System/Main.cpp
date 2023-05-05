#include <Windows.h>
#include <winnt.h>
#include <stdlib.h>
#include <string>



#pragma region data
enum class threadState {
	None, Run, Dead, Block
};
const int zero = 0;
const int one = 1;
const int two = 2;
HINSTANCE oper_hInstance;
HWND start[3];
HWND stop[3];
HWND suspend[3];
HWND name[3];
HWND stateLabel[3];
threadState state[3];
HANDLE hThread[3];

#define First 0x1001 
// First ~ First+2 : Start, First + 3 ~ Fisrt + 5 : stop, First + 6 ~ First + 8 : wait 
// First+9 ~ First+11 : name, First+12 ~ First + 14 : stateLabel
// First - 3 ~ First - 1 : I/O , First -6 ~ Fisrt - 4 : Terminate

#pragma endregion

void setStateLabel() {
	for (int i = 0; i < 3; i++) {
		switch (state[i]) {
		case threadState::None:
			SetWindowText(stateLabel[i], L"None");
			break;
		case threadState::Run:
			SetWindowText(stateLabel[i], L"Run / Ready");
			break;
		case threadState::Dead:
			SetWindowText(stateLabel[i], L"Dead");
			break;
		case threadState::Block:
			SetWindowText(stateLabel[i], L"Block");
			break;
		}
	}
}

DWORD subThreadEntry(void* value) {

#pragma region CreateSubWin
	WCHAR buf[20];
	wsprintf(buf, L"%s%d", L"SubThread", *((int*)value) + 1);
	HWND hwnd = CreateWindow(L"SubThread",
		buf,
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		0, 0, 230, 200,
		NULL, NULL, oper_hInstance, NULL);
#pragma endregion
#pragma region MSGfetcher
	MSG msg;
	while (state[*((int*)value)] == threadState::Run || state[*((int*)value)] == threadState::Block)
	{
		GetMessage(&msg, NULL, 0, 0);
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
#pragma endregion
	return 0;
}

#pragma region OperatingSystemWndProc
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
		for (int i = 0; i < 3; i++) {
			start[i] = CreateWindow(L"button", L"Start / Run", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				220, 20 + (i * 50), 100, 25, hwnd, (HMENU)(First + i), oper_hInstance, NULL);
			stop[i] = CreateWindow(L"button", L"Stop", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				320, 20 + (i * 50), 100, 25, hwnd, (HMENU)(First + 3 + i), oper_hInstance, NULL);
			suspend[i] = CreateWindow(L"button", L"Suspend", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				420, 20 + (i * 50), 100, 25, hwnd, (HMENU)(First + 6 + i), oper_hInstance, NULL);
			WCHAR nameBuf[10];
			wsprintf(nameBuf, L"%s%d", L"Thread", i + 1);
			name[i] = CreateWindow(L"static", nameBuf, WS_CHILD | WS_VISIBLE,
				20, 25 + (i * 50), 100, 25, hwnd, (HMENU)(First + 9 + i), oper_hInstance, NULL);
			stateLabel[i] = CreateWindow(L"static", L"-", WS_CHILD | WS_VISIBLE,
				120, 25 + (i * 50), 100, 25, hwnd, (HMENU)(First + 12 + i), oper_hInstance, NULL);
			state[i] = threadState::None;
			hThread[i] = (HANDLE)NULL;
		}
		setStateLabel();
		break;
#pragma endregion
#pragma region WM_COMMAND
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
			// start
		case (First):
		case (First + 1):
		case (First + 2):
			if (state[LOWORD(wParam) - First] == threadState::None 
					|| state[LOWORD(wParam) - First] == threadState::Dead) {
				state[LOWORD(wParam) - First] = threadState::Run;
				if (LOWORD(wParam) - First == 0)
					hThread[LOWORD(wParam) - First] = 
						CreateThread(NULL, 0, subThreadEntry, (LPVOID)&zero, 0, NULL);
				if (LOWORD(wParam) - First == 1)
					hThread[LOWORD(wParam) - First] = 
						CreateThread(NULL, 0, subThreadEntry, (LPVOID)&one, 0, NULL);
				if (LOWORD(wParam) - First == 2)
					hThread[LOWORD(wParam) - First] = 
						CreateThread(NULL, 0, subThreadEntry, (LPVOID)&two, 0, NULL);
			}
			if (state[LOWORD(wParam) - First] == threadState::Block) {
				state[LOWORD(wParam) - First] = threadState::Run;
				ResumeThread(hThread[LOWORD(wParam) - First]);
			}
			setStateLabel();
			break;
			// stop
		case (First + 3):
		case (First + 4):
		case (First + 5):
			if (state[LOWORD(wParam) - (First + 3)] == threadState::Run 
					|| state[LOWORD(wParam) - (First + 3)] == threadState::Block) {
				state[LOWORD(wParam) - (First + 3)] = threadState::Dead;
				TerminateThread(hThread[LOWORD(wParam) - (First + 3)], -1);
				CloseHandle(hThread[LOWORD(wParam) - (First + 3)]);
			}
			setStateLabel();
			break;
			// wait
		case (First + 6):
		case (First + 7):
		case (First + 8):
			if (state[LOWORD(wParam) - (First + 6)] == threadState::Run) {
				state[LOWORD(wParam) - (First + 6)] = threadState::Block;
				SuspendThread(hThread[LOWORD(wParam) - (First + 6)]);
			}
			setStateLabel();
			break;
		}
		break;
#pragma endregion
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
#pragma endregion

#pragma region SubWndProc
LRESULT CALLBACK subWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static WCHAR str[20];
	switch (iMsg)
	{
#pragma region WM_DESTROY
	case WM_DESTROY:
		GetWindowText(hwnd, str, 20);
		state[static_cast<int>(str[9]) - 49] = threadState::Dead;
		setStateLabel();
		PostQuitMessage(0);
		break;
#pragma endregion
#pragma region WM_CREATE
	case WM_CREATE:
		GetWindowText(hwnd, str, 20);
		CreateWindow(L"button", L"I/O", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			10, 10, 100, 150, hwnd, (HMENU)(First - (static_cast<int>(str[9]) - 48)), oper_hInstance, NULL);
		CreateWindow(L"button", L"Terminate", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			110, 10, 100, 150, hwnd, (HMENU)(First - (static_cast<int>(str[9]) - 48) - 3), oper_hInstance, NULL);
		break;
#pragma endregion
#pragma region WM_COMMAND
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case First - 3:
			state[2] = threadState::Block;
			setStateLabel();
			Sleep(1000);
			state[2] = threadState::Run;
			setStateLabel();
			break;
		case First - 2:
			state[1] = threadState::Block;
			setStateLabel();
			Sleep(1000);
			state[1] = threadState::Run;
			setStateLabel();
			break;
		case First - 1:
			state[0] = threadState::Block;
			setStateLabel();
			Sleep(1000);
			state[0] = threadState::Run;
			setStateLabel();
			break;
		case First - 6:
			state[2] = threadState::Dead;
			setStateLabel();
			break;
		case First - 5:
			state[1] = threadState::Dead;
			setStateLabel();
			break;
		case First - 4:
			state[0] = threadState::Dead;
			setStateLabel();
			break;
		}
#pragma endregion
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
#pragma endregion


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	oper_hInstance = hInstance;

#pragma region WindowsClass
	HWND        hwnd;
	MSG         msg;
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
#pragma endregion
#pragma region SubWindowsClass
	wndclass.lpfnWndProc = subWndProc;
	wndclass.lpszMenuName = L"SubThread";
	wndclass.lpszClassName = L"SubThread";
	RegisterClass(&wndclass);
#pragma endregion
#pragma region CreateOperatingSystem
	hwnd = CreateWindow(L"Operating",
		L"Operating System",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
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