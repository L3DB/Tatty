#include "Tatty3D.h"


Tatty3D::Tatty3D(double GameFrame):GameFrame(GameFrame)
{

	//�������ܺ���ʼ������
	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&StartCount);

}


Tatty3D::~Tatty3D(void) 
{

}


void Tatty3D::Render()
{
	
	//������Ⱦ
	Draw_T3Object(&obj, hMemDC);

}

void Tatty3D::RenderPipeline()
{
	//����ľֲ����굽���������ת��
	ModelToWorld_T3Object(&obj, TRANSFORM_LOCAL_TO_TRANS);

	//�����������굽�������ı任
	WorldToCamera_T3Object(&cam, &obj);
	
	//�����������굽͸������
	CameraToPerspective_T3Object(&obj, &cam);

	//�����͸�����굽��Ļ�����
	PersepectiveToScreen_T3Object(&obj, &cam);

}



void Tatty3D::Clear()
{
	//��ȡ�豸DC
	hDC = GetDC(hWnd);
	//�������ݵ���ʱDC �������
	hMemDC = CreateCompatibleDC(hDC);

	RECT rect;
	rect.left = 0;
	rect.top = 0; 
	rect.right = 1280;
	rect.bottom = 720;

	//�ڱ����ϴ���һ�ż��ݻ��� 
	//�˺�Ļ�ͼ���ڻ����Ͻ���
	HBITMAP hbm = CreateCompatibleBitmap(hDC, 1280, 720);
	SelectObject(hMemDC, hbm);
	DeleteObject(hbm);

	//��䱳��Ϊ��ɫ
	FillRect(hMemDC, &rect, WHITE_BRUSH);

}


void Tatty3D::Restore()
{

	ReleaseDC(hWnd, hMemDC);
	ReleaseDC(hWnd, hDC);
	//��ؼ�ʱ�ͷ� �����ڴ��ռ�������
	DeleteObject(hMemDC);
	DeleteObject(hDC);

}


void Tatty3D::UpdateView()
{

	LARGE_INTEGER CurrentCounter;

	//ֻ����ʵ��֡��С�ڵ����Զ���֡��ʱ�Ž�����һ��
	do
	{
		//��ѯ��ǰ�ļ���������
		QueryPerformanceCounter(&CurrentCounter);

		//��֮֡���ʱ���� =(��ǰ������ - ��һ�μ�����) / ����Ƶ��
		double DeltaTime = (double) (CurrentCounter.LowPart - LastCounter.LowPart) / Frequency.LowPart ;

		//����ʵ����ʾ֡��
		DisplayFrame = 1.0 / DeltaTime;

	} while(DisplayFrame >= GameFrame);

	//��һ֡�ļ����������� 
	LastCounter = CurrentCounter;


	//��ʾ�ı�����
	static TCHAR temp[50];

	//��ʽ�����֡��
	TextOut (hMemDC, 30, 550, temp, 	
		wsprintf (temp, TEXT ("FPS: %d"), (UINT)DisplayFrame));


	//һ�θ��±�������
	BitBlt(hDC, 0, 0, 1280, 720, hMemDC, 0, 0, SRCCOPY);
}


void Tatty3D::Init()
{
	//��ת�Ƕ�
	T3Vector4D dir = {40, 40, 0, 1};
	//�����������
	T3Point4D pos = {200, 2000, 100, 1};
	//������������
	T3Point4D posObj = {0, 0, 100, 1};

	T3Load(obj, "tank1.txt");

	//����SinCos���ұ�
	BuildSinCosTable();

	T3CameraInit(&cam, &pos, &dir, 90.0, 800, 600);
	T3CameraBuildMat(&cam, CAM_ROT_XYZ);

	obj.worldPosition = posObj;
	/*----------------Windows���----------------*/

	//����������ɫ
	SetTextColor(hMemDC, RGB(124, 45, 3));
	SetBkMode(hMemDC, TRANSPARENT);

	LastCounter = StartCount;
	DisplayFrame = 0;
}

void Tatty3D::ReadInput()
{
	if(KeyDown(W_DOWN))
	{
		obj.worldPosition.z++;
		//cam.direction.y++;
		//MessageBeep(1);
	}
	if(KeyDown(S_DOWN))
	{
		obj.worldPosition.z--;
		//cam.direction.y--;
	}
	if(KeyDown(A_DOWN))
	{
		obj.worldPosition.x--;
		//cam.direction.x--;
	}
	if(KeyDown(D_DOWN))
	{
		obj.worldPosition.x++;
		//cam.direction.x++;
	}
}


void Tatty3D::ShutDown()
{
	//�رյ�ʱ�����Ҫ�ͷ�
	/*----------------Windows���----------------*/
	DeleteDC(hMemDC);              //�ͷŻ���DC
	ReleaseDC(hWnd, hDC);       //�ͷ��豸DC
}


void Tatty3D::Frame()
{
	//���� ����ѭ��
	Clear();

	//��Ӧ�����¼�
	ReadInput();

	//��Ⱦ����
	RenderPipeline();

	//����/��Ⱦ
	Render();

	//��ʾ֡���������Զ���֡��֮�� �����½�����ͼ
	UpdateView();

	//��ԭ�ڴ�ռ��
	Restore();

}

