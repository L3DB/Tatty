//�������
#include<Windows.h>
#include"Tatty3D.h"


int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,
				   PSTR szCmdLine,int iCmdShow)
{
	
	Tatty3D *pocher;
	pocher = new Tatty3D(60.0);
	pocher->Run();

}