#include"WindowsBase.h"


//构造函数
WindowsBase::WindowsBase()
{
	//获得当前模块句柄
	hInstance=GetModuleHandle(NULL);

	//设置窗口类名称 窗口标题
	lstrcpy(szWindowClassName, TEXT("WindowClass"));
	lstrcpy(szCaptionName, TEXT("WindowCaption"));

	//设置窗口类
	wndclass.style=CS_HREDRAW|CS_VREDRAW;
	wndclass.lpfnWndProc=WndProc;
	wndclass.cbClsExtra=0;
	wndclass.cbWndExtra=0;
	wndclass.hInstance=hInstance;
	wndclass.hIcon=LoadIcon(NULL,IDI_APPLICATION);
	wndclass.hCursor=LoadCursor(NULL,IDC_ARROW);
	wndclass.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName=NULL;
	wndclass.lpszClassName=szWindowClassName;

}

BOOL WindowsBase::Resize(long width, long height)
{
	//边框高度宽度
	int xFrame = GetSystemMetrics(SM_CXSIZEFRAME);
	int yFrame = GetSystemMetrics(SM_CYSIZEFRAME);

	//边框高宽度
	int yCaption = GetSystemMetrics(SM_CYCAPTION);

	//整个屏幕高宽度
	int xScreen = GetSystemMetrics(SM_CXSCREEN);
	int yScreen = GetSystemMetrics(SM_CYSCREEN);

	//左边框右边框
	//从左到右
	int cx = xFrame + width +xFrame;
	//从上到下
	int cy = yFrame +yCaption +height +yFrame;

	SetWindowPos(hWnd, HWND_TOP, 
		(xScreen - cx) / 2, (yScreen -cy) / 2,
		cx, cy,
		SWP_SHOWWINDOW);

	return TRUE;
}


HWND WindowsBase::GethWnd()
{
	return hWnd;
}

HINSTANCE WindowsBase::GethInstance()
{
	return hInstance;
}

BOOL WindowsBase::Run()
{
	RegisterClass(&wndclass);

	hWnd=CreateWindow(szWindowClassName, szCaptionName, 
		WS_OVERLAPPEDWINDOW,
		0, 0,
		800, 600,
		NULL,
		NULL,
		hInstance,
		NULL);

	//重置窗口大小
	Resize(800, 600);

	//显示/更新窗口
	ShowWindow(hWnd, SW_NORMAL);
	UpdateWindow(hWnd);

	Init();

	ZeroMemory(&msg,sizeof(MSG));

	while(msg.message != WM_QUIT)
	{

		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//帧循环
			Frame();
		}
	}

	ShutDown();
	UnregisterClass(szWindowClassName, hInstance);
	return TRUE;
}

long _stdcall WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default:
		return DefWindowProc(hWnd,iMessage,wParam,lParam);
	}
}