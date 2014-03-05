#ifndef _WINDOWS_BASE_H_
#define _WINDOWS_BASE_H_

#include<Windows.h>

class WindowsBase
{
protected:
	//��ǰʵ�����
	HINSTANCE hInstance;
	//���ھ��
	HWND hWnd;
	//����������
	TCHAR szWindowClassName[20];
	//���ڱ�������
	TCHAR szCaptionName[20];
	//������
	WNDCLASS wndclass;
	//��Ϣ
	MSG msg;

public:
	//���캯����ʼ������
	WindowsBase();

	//������ʹ��
	HWND GethWnd();
	HINSTANCE GethInstance();

	//��Ϸ������
	BOOL Run();
	//�ı䴰�ڴ�С
	BOOL Resize(long width, long height);

	//��ͬ��GameApplicationʵ������������
	//�˴�Ϊ���麯�� ֻ������һ��������
	virtual void Init(){};
	virtual void ShutDown(){};
	virtual void Frame(){};
};

static long _stdcall WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif


