#include"WindowsBase.h"


//���캯��
WindowsBase::WindowsBase()
{
	//��õ�ǰģ����
	hInstance=GetModuleHandle(NULL);

	//���ô��������� ���ڱ���
	lstrcpy(szWindowClassName, TEXT("WindowClass"));
	lstrcpy(szCaptionName, TEXT("WindowCaption"));

	//���ô�����
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
	//�߿�߶ȿ��
	int xFrame = GetSystemMetrics(SM_CXSIZEFRAME);
	int yFrame = GetSystemMetrics(SM_CYSIZEFRAME);

	//�߿�߿��
	int yCaption = GetSystemMetrics(SM_CYCAPTION);

	//������Ļ�߿��
	int xScreen = GetSystemMetrics(SM_CXSCREEN);
	int yScreen = GetSystemMetrics(SM_CYSCREEN);

	//��߿��ұ߿�
	//������
	int cx = xFrame + width +xFrame;
	//���ϵ���
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

	//���ô��ڴ�С
	Resize(800, 600);

	//��ʾ/���´���
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
			//֡ѭ��
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