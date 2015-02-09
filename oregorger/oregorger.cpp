// oregorger.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "oregorger.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
ULONG_PTR gdiplusToken;
HWND ghwnd;
std::thread * thr = 0;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;
	GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL); 
	init();

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_OREGORGER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_OREGORGER));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_OREGORGER));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_OREGORGER);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      400, 400, 440, 520, NULL, NULL, hInstance, NULL);
   ghwnd = hWnd;

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

void refresh1();
//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		Graphics *myGraphics;

		Pen *myPen;
		Brush* myBrush;

		hdc = BeginPaint(hWnd, &ps);

		myGraphics = new Graphics(hdc);

		myPen = new Pen(Color(255, 0, 0, 0), 1);
		myBrush = new SolidBrush(Color(255, 0, 0, 0));

		for (int i = 0; i < 360; i += 40)

			myGraphics->DrawLine(myPen, 20, 60 + i, 340, 60 + i);

		for (int i = 0; i < 360; i += 40)

			myGraphics->DrawLine(myPen, 20 + i, 60, 20 + i, 380);
		for (int i = 0; i < 9; i++) for (int j = 0; j < 9; j++){
			if (!roadmap[i][j]){
				myGraphics->FillRectangle(myBrush, Rect(20 + j * 40, 60 + i * 40, 40, 40));
			}
		}
		delete myGraphics;

		delete myPen;

		EndPaint(hWnd, &ps);
		refresh1();
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		if (!thr){
			init();
			thr = new std::thread(bossroutine);
		}
		else{
			switch (wParam){
			case 87: case 38: case 104:
				charmove(0); break;
			case 65: case 37: case 100:
				charmove(1); break;
			case 83: case 40: case 98:
				charmove(2); break;
			case 68: case 39: case 102:
				charmove(3); break;
			}
			refresh();
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

void refresh1(){
	static HDC hdc = GetDC(ghwnd);
	static Graphics *myGraphics = new ::Graphics(hdc);
	static Brush *myPen1 = new Gdiplus::SolidBrush(Color(255, 255, 0, 0));
	static Brush *myPen2 = new Gdiplus::SolidBrush(Color(255, 0, 0, 255));
	static Brush *myPen3 = new Gdiplus::SolidBrush(Color(255, 255, 255, 0));

	myGraphics->FillRectangle(myPen3, Rect(20 + boxy * 40, 60 + boxx * 40, 40, 40));
	myGraphics->FillEllipse(myPen2, Rect(20 + chary * 40, 60 + charx * 40, 40, 40));
	myGraphics->FillEllipse(myPen1, Rect(20 + bossy * 40, 60 + bossx * 40, 40, 40));

	char* str = (char*)alloca(256);
	sprintf(str, "Score: %d", score);
	static WCHAR* drawString = (WCHAR*)calloc(256, sizeof(WCHAR));
	MultiByteToWideChar(CP_ACP, 0, str, (int)strlen(str), drawString, 255);
	// Create font and brush.
	static Font* drawFont = new Font(L"Tahoma", 8);
	static SolidBrush* drawBrush = new SolidBrush(Color::Black);
	// Create point for upper-left corner of drawing.
	PointF drawPoint = PointF(0.0F, 0.0F);
	// Draw string to screen.
	myGraphics->DrawString(drawString, -1, drawFont, drawPoint, drawBrush);

}

void refresh(){
	InvalidateRect(ghwnd, NULL, TRUE);
	//refresh1();
}

void startcallback(){
	static HDC hdc = GetDC(ghwnd);
	static Graphics *myGraphics = new ::Graphics(hdc);
	std::chrono::milliseconds dur(1000);
	WCHAR three[] = L"3";
	WCHAR two[] = L"2";
	WCHAR one[] = L"1";
	static Font* drawFont = new Font(L"Tahoma", 36);
	static SolidBrush* drawBrush = new SolidBrush(Color::YellowGreen);
	PointF drawPoint = PointF(180.0F, 0.0F);
	myGraphics->DrawString(three, -1, drawFont, drawPoint, drawBrush);
	std::this_thread::sleep_for(dur); 
	refresh();
	myGraphics->DrawString(two, -1, drawFont, drawPoint, drawBrush);
	std::this_thread::sleep_for(dur); 
	refresh();
	myGraphics->DrawString(one, -1, drawFont, drawPoint, drawBrush);
	std::this_thread::sleep_for(dur); 
	refresh();
}

void endcallback(){
	static char str[32];
	sprintf(str, "Your score: %d", score);
	MessageBoxA(ghwnd, str, "Game Over", 0);
	init();
	thr = nullptr;
}