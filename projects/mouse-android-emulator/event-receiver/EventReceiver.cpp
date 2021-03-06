#include <fstream>
#include <iostream>
#include <array>

#include "EventReceiverApplication.h"

#define NOMINMAX
#include <Windows.h>
#include <shellapi.h>
#include <windowsx.h>

static const char * s_connectionStatus;
void SwitchConnectionStatus(bool connected)
{
	if (connected)
	{
		s_connectionStatus = "Status - Connected";
	}
	else
	{
		s_connectionStatus = "Status - Disconnected";
	}
}

static std::unique_ptr<EventReceiverApplication> s_application;

UINT const WMAPP_NOTIFYCALLBACK = WM_APP + 1;

HINSTANCE g_hInst = nullptr;
NOTIFYICONDATA s_hNotifyIconData = { sizeof(NOTIFYICONDATA) };
const char* szWindowClass = "MouseRemoteClass";
const char* s_applicationIco = "D:\\Development\\pets\\build\\bin\\Debug\\MouseRemote_4.ico";
std::string_view s_iconTip = "Remote Mouse";
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

class MyBuf : public std::streambuf
{
public:
	std::streamsize xsputn(const char* _Ptr, std::streamsize _Count) override
	{
		OutputDebugStringA(_Ptr);
		return _Count;
	}

	int_type overflow(int_type c) override
	{
		if (c >= static_cast<int>(std::numeric_limits<char>::min()) &&
			c <= static_cast<int>(std::numeric_limits<char>::max()))
		{
			char buffer[2] = {static_cast<char>(c), '\0'};
			OutputDebugStringA(buffer);
		}
		return c;
	}
};

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, PSTR lpCmdLine, int nCmdShow)
{
	MyBuf myBuf;
	std::cout.set_rdbuf(&myBuf);
	std::cerr.set_rdbuf(&myBuf);

	std::string buffer(4096, '\0');
	const size_t bufferSize = buffer.size();
	size_t bytes = GetCurrentDirectoryA(buffer.size(), buffer.data());
	if (bytes != 0 && bytes < bufferSize)
	{
		buffer.resize(bytes);
	}
	std::string resourcesDir = buffer + "\\bin\\Resources\\";
	std::string icoPath = resourcesDir + "laptop_78931.ico";
	s_applicationIco = icoPath.data();

	SwitchConnectionStatus(false);

	g_hInst = hInstance;
	RegisterWindowClass(szWindowClass, "Mouse Remote Menu", WndProc);

	HWND hwnd = CreateWindowA(szWindowClass, "Mouse Remote", WS_OVERLAPPEDWINDOW,
	                          CW_USEDEFAULT, 0, 250, 200, NULL, NULL, g_hInst, NULL);

	if (hwnd)
	{
		ShowWindow(hwnd, SW_HIDE);
		SetTimer(hwnd, s_idTimer, USER_TIMER_MINIMUM, (TIMERPROC)MyTimerProc);

		s_application = std::make_unique<EventReceiverApplication>();

		s_application->ProcessCommandLine(1, &lpCmdLine);

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
		static DWORD lastTime;
		if (lastTime == 0)
		{
			lastTime = dwTime;
			return;
		}

		DWORD diff = dwTime - lastTime;
		lastTime = dwTime;

		if (s_application)
		{
			s_application->ProcessElapsedTime(TimeState::Milliseconds(diff));
		}
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

BOOL SendMouseClick(bool left)
{
	POINT mousePos;
 	if (GetCursorPos(&mousePos))
	{
		INPUT input[2] = {};
		input[0].type = INPUT_MOUSE;
		input[0].mi.dx = mousePos.x;
		input[0].mi.dy = mousePos.y;
 		DWORD buttonDown = left ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_RIGHTDOWN;
		input[0].mi.dwFlags = buttonDown;

		input[1].type = INPUT_MOUSE;
		input[1].mi.dx = mousePos.x;
		input[1].mi.dy = mousePos.y;
 		DWORD buttonUp = left ? MOUSEEVENTF_LEFTUP : MOUSEEVENTF_RIGHTUP;
		input[1].mi.dwFlags = buttonUp;
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
					InsertMenu(hPopMenu, 0xFFFFFFFF,MF_BYPOSITION | MF_STRING | MFS_DISABLED, s_idStatus, s_connectionStatus);
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
				// SendMouseClick();
			}
			break;
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

// int main(int argc, char** argv)
// {
// 	std::cout
// 		<< "Event Receiver Application started." << '\n'
// 		<< "I'll receive events from event-dispatcher application and handle it." << std::endl;
//
// 	application->ProcessCommandLine(argc, argv);
// 	
// 	sf::Window window(sf::VideoMode(600, 400), "Event Receiver App");
// 	
// 	sf::Clock clock;
// 	sf::Time timeOnFrame = sf::milliseconds(15);
// 	sf::Time elapsedTimeOnFrame;
// 	
// 	bool shouldClose = false;
// 	while (not shouldClose)
// 	{
// 		FrameMarkNamed("Frame");
// 		sf::Event event;
// 		while (!shouldClose && window.pollEvent(event))
// 		{
// 			application->ProcessEvent(event);
// 			shouldClose = application->ShouldTerminate();
// 		}
// 	
// 		auto elapsedTimeInternal = TimeState::Milliseconds(elapsedTimeOnFrame.asMilliseconds());
// 		application->ProcessElapsedTime(elapsedTimeInternal);
// 	
// 		elapsedTimeOnFrame = clock.restart();
// 		if (elapsedTimeOnFrame < timeOnFrame)
// 		{
// 			sleep(timeOnFrame - elapsedTimeOnFrame);
// 		}
// 	}
// 	
// 	window.close();
//
// 	return 0;
// }
