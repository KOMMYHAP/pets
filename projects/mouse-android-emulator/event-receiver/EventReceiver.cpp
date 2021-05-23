#include <fstream>
#include <iostream>
// #include <SFML/Window.hpp>
// #include "SFML/System.hpp"
#include "EventReceiverApplication.h"

#include <Windows.h>
#include <shellapi.h>
#include <windowsx.h>

UINT const WMAPP_NOTIFYCALLBACK = WM_APP + 1;

HINSTANCE g_hInst = nullptr;
NOTIFYICONDATA s_hNotifyIconData = { sizeof(NOTIFYICONDATA) };
const char* szWindowClass = "MouseRemoteClass";
const char* s_applicationIco = "D:\\Development\\pets\\build\\bin\\Debug\\MouseRemote_4.ico";
std::string_view s_iconTip = "TestToolTip";
const int s_idIcon = 43;
const int s_idStatus = 100;
const int s_idSeparator = 101;
const int s_idExit = 102;
const int s_idTimer = 103;

void RegisterWindowClass(PCSTR pszClassName, PCSTR pszMenuName, WNDPROC lpfnWndProc);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL AddNotificationIcon(HWND hwnd);
BOOL DeleteNotificationIcon();
VOID CALLBACK MyTimerProc(
    HWND hwnd,        // handle to window for timer messages
    UINT message,     // WM_TIMER message
    UINT idTimer,     // timer identifier
    DWORD dwTime);     // current system time

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR /*lpCmdLine*/, int nCmdShow)
{
	std::ofstream ofs("D:\\Development\\pets\\build\\bin\\Debug\\log.txt");
	std::streambuf * rdtbuf = ofs.rdbuf();
	std::cout.set_rdbuf(rdtbuf);
	std::cerr.set_rdbuf(rdtbuf);

	g_hInst = hInstance;
	RegisterWindowClass(szWindowClass, "Mouse Remote Menu", WndProc);

	HWND hwnd = CreateWindowA(szWindowClass, "Mouse Remote", WS_OVERLAPPEDWINDOW,
	                          CW_USEDEFAULT, 0, 250, 200, NULL, NULL, g_hInst, NULL);

	if (hwnd)
	{
		ShowWindow(hwnd, SW_HIDE);
		SetTimer(hwnd, s_idTimer, USER_TIMER_MINIMUM, (TIMERPROC)MyTimerProc);

		// Main message loop:
		MSG msg;
		while (GetMessage(&msg, nullptr, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return 0;
}

float easeInSine(float n)
{
	return 1 - cos(n * 3.14 / 2);
}

float easeInOutQuint(float n)
{
	return n < 0.5 ? 16 * n * n * n * n * n : 1 - pow(-2 * n + 2, 5) / 2;
}

float easeOutBounce(float n)
{
	const float n1 = 7.5625;
	const float d1 = 2.75;

	if (n < 1 / d1) {
	    return n1 * n * n;
	} else if (n < 2 / d1) {
	    return n1 * (n -= 1.5 / d1) * n + 0.75;
	} else if (n < 2.5 / d1) {
	    return n1 * (n -= 2.25 / d1) * n + 0.9375;
	} else {
	    return n1 * (n -= 2.625 / d1) * n + 0.984375;
	}
}

float easeInBounce(float n)
{
	return 1 - easeOutBounce(1 - n);
}

void CALLBACK MyTimerProc(
    HWND hwnd,        // handle to window for timer messages
    UINT message,     // WM_TIMER message
    UINT idTimer,     // timer identifier
    DWORD dwTime)     // current system time
{
	if (idTimer == s_idTimer)
	{
		// const static DWORD s_length = 2 * 1000;
		// static DWORD s_start;
		// static DWORD s_end;
		// static bool s_forward = true;
		// if (dwTime > s_end)
		// {
		// 	s_forward = !s_forward;
		// 	s_start = dwTime;
		// 	s_end = s_start + s_length;
		// }
		//
		// float t = (dwTime - s_start) / static_cast<float>(s_length);
		// float v = easeOutBounce(t);
		//
		// float x = 500 + 400 * t;
		// float y = 200 + 400 * v;
		//
		// SetCursorPos(x, y);
		//
		// POINT point;
		// GetCursorPos(&point);
		// std::cout << "mouse moved to (" << point.x << ", " << point.y << ")\n";
	}
}

void RegisterWindowClass(PCSTR pszClassName, PCSTR pszMenuName, WNDPROC lpfnWndProc)
{
	WNDCLASSEX wcex = {sizeof(wcex)};
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = lpfnWndProc;
	wcex.hInstance = g_hInst;
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = pszMenuName;
	wcex.lpszClassName = pszClassName;
	ATOM r = RegisterClassEx(&wcex);
	if (!r)
	{
		int err = GetLastError();
	}
}

BOOL AddNotificationIcon(HWND hwnd)
{
	auto & nid = s_hNotifyIconData;
	nid.hWnd = hwnd;
	// add the icon, setting the icon, tooltip, and callback message.
	// the icon will be identified with the GUID
	nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
	nid.uID = s_idIcon;
	nid.uCallbackMessage = WMAPP_NOTIFYCALLBACK;
	nid.hIcon = (HICON) LoadImage( // returns a HANDLE so we have to cast to HICON
	  NULL,             // hInstance must be NULL when loading from a file
	  s_applicationIco,   // the icon file name
	  IMAGE_ICON,       // specifies that the file is an icon
	  0,                // width of the image (we'll specify default later on)
	  0,                // height of the image
	  LR_LOADFROMFILE|  // we want to load a file (as opposed to a resource)
	  LR_DEFAULTSIZE|   // default metrics based on the type (IMAGE_ICON, 32x32)
	  LR_SHARED         // let the system release the handle when it's no longer used
	);
	memcpy(nid.szTip, s_iconTip.data(), s_iconTip.size());
	return Shell_NotifyIcon(NIM_ADD, &nid);
}

BOOL SendMouseClick()
{
	POINT mousePos;
 	if (GetCursorPos(&mousePos))
	{
		INPUT input[2] = {};
		input[0].type = INPUT_MOUSE;
		input[0].mi.dx = mousePos.x;
		input[0].mi.dy = mousePos.y;
		input[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

		input[1].type = INPUT_MOUSE;
		input[1].mi.dx = mousePos.x;
		input[1].mi.dy = mousePos.y;
		input[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;
		SendInput(2, input, sizeof(INPUT));
 		return TRUE;
	}
	return FALSE;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	POINT lpClickPoint;

	switch (message)
	{
	case WM_CREATE:
		// add the notification icon
		if (!AddNotificationIcon(hwnd))
		{
			return -1;
		}
		break;
	case WMAPP_NOTIFYCALLBACK:
		{
			switch (LOWORD(lParam))
			{
			case WM_RBUTTONDOWN:
				{
					GetCursorPos(&lpClickPoint);
					HMENU hPopMenu = CreatePopupMenu();
					InsertMenu(hPopMenu, 0xFFFFFFFF,MF_BYPOSITION | MF_STRING, s_idStatus, ("Status"));
					InsertMenu(hPopMenu, 0xFFFFFFFF,MF_SEPARATOR, s_idSeparator, ("Separator"));
					InsertMenu(hPopMenu, 0xFFFFFFFF,MF_BYPOSITION | MF_STRING, s_idExit, ("Exit"));

					SetForegroundWindow(hwnd);
					TrackPopupMenu(hPopMenu,TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_BOTTOMALIGN, lpClickPoint.x,
					               lpClickPoint.y, 0,
					               hwnd, nullptr);
					return TRUE;
				}
			}
			break;
		}
	case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			int wmEvent = HIWORD(wParam);
			// Parse the menu selections:
			switch (wmId)
			{
			case s_idStatus:
				MessageBox(nullptr, ("This is a test for menu Test 1"), ("Test 1"),MB_OK);
				break;
			case s_idExit:
				DeleteNotificationIcon();
				DestroyWindow(hwnd);
				break;
			default:
				return DefWindowProc(hwnd, message, wParam, lParam);
			}
			break;
		}
	case WM_LBUTTONDOWN:
		{
			int xPos = GET_X_LPARAM(lParam);
			int yPos = GET_Y_LPARAM(lParam);
			std::cout << "clicked on (" << xPos << ", " << yPos << ")\n";
			break;
		}
	case WM_MOUSEMOVE:
		{
			int xPos = GET_X_LPARAM(lParam);
			int yPos = GET_Y_LPARAM(lParam);
			std::cout << "moved to (" << xPos << ", " << yPos << ")\n";
			break;
		}
	case WM_KEYDOWN:
		{
			if (wParam == VK_SPACE)
			{
				SendMouseClick();
			}
		}
	case WM_DESTROY:
		DeleteNotificationIcon();
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

BOOL DeleteNotificationIcon()
{
	// NOTIFYICONDATA nid = {sizeof(nid)};
	// nid.uID = s_idIcon;
	return Shell_NotifyIcon(NIM_DELETE, &s_hNotifyIconData);
}

int main(int argc, char** argv)
{
	std::cout
		<< "Event Receiver Application started." << '\n'
		<< "I'll receive events from event-dispatcher application and handle it." << std::endl;

	// auto application = std::make_unique<EventReceiverApplication>();
	// application->ProcessCommandLine(argc, argv);
	//
	// sf::Window window(sf::VideoMode(600, 400), "Event Receiver App");
	//
	// sf::Clock clock;
	// sf::Time timeOnFrame = sf::milliseconds(15);
	// sf::Time elapsedTimeOnFrame;
	//
	// bool shouldClose = false;
	// while (not shouldClose)
	// {
	// 	FrameMarkNamed("Frame");
	// 	sf::Event event;
	// 	while (!shouldClose && window.pollEvent(event))
	// 	{
	// 		application->ProcessEvent(event);
	// 		shouldClose = application->ShouldTerminate();
	// 	}
	//
	// 	auto elapsedTimeInternal = TimeState::Milliseconds(elapsedTimeOnFrame.asMilliseconds());
	// 	application->ProcessElapsedTime(elapsedTimeInternal);
	//
	// 	elapsedTimeOnFrame = clock.restart();
	// 	if (elapsedTimeOnFrame < timeOnFrame)
	// 	{
	// 		sleep(timeOnFrame - elapsedTimeOnFrame);
	// 	}
	// }
	//
	// window.close();

	return 0;
}
