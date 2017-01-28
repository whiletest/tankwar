// D3DTest.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "D3DTest.h"
#include "MyView.h"
#include "SObjIdentify.h"

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名
CMyView g_view;

// 此代码模块中包含的函数的前向声明:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: 在此放置代码。

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TANKWAR, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	g_view.Setup();
	int res = g_view.EnterMsgLoop();

	return res;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
//  注释:
//
//    仅当希望
//    此代码与添加到 Windows 95 中的“RegisterClassEx”
//    函数之前的 Win32 系统兼容时，才需要此函数及其用法。调用此函数十分重要，
//    这样应用程序就可以获得关联的
//    “格式正确的”小图标。
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
	wcex.hIcon			= LoadIcon(hInstance,MAKEINTRESOURCE(IDI_TANKWAR));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= 0;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 将实例句柄存储在全局变量中

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 800, 600, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   g_view.InitD3D(hInstance,hWnd,800,600,true,D3DDEVTYPE_HAL);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		case 'w':
		case 'W':
			g_view.P_ChangeCurState(CST_FORWARD);
			break;
		case 's':
		case 'S':
			g_view.P_ChangeCurState(CST_BACKWARD);
			break;
		case 'd':
		case 'D':
			g_view.P_ChangeCurState(CST_RIGHT);
			break;
		case 'a':
		case 'A':
			g_view.P_ChangeCurState(CST_LEFT);
			break;
		case VK_LEFT:
			g_view.P_ChangeCurState(CST_GR_NEGATIVE);
			break;
		case VK_RIGHT:
			g_view.P_ChangeCurState(CST_GR_POSITIVE);
			break;
		case VK_SPACE:
			g_view.P_Fire();
			break;
		}
		return 0;
	case WM_KEYUP:
		switch (wParam)
		{
		case 'w':
		case 'W':
		case 's':
		case 'S':
			g_view.P_ChangeCurState(CST_FB_SPEED_DOWN);
			break;
		case 'a':
		case 'A':
		case 'd':
		case 'D':
			g_view.P_ChangeCurState(CST_LR_STATIC);
			break;
		case VK_LEFT:
		case VK_RIGHT:
			g_view.P_ChangeCurState(CST_GRNP_STATIC);
			break;
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		 break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

// “关于”框的消息处理程序。
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
