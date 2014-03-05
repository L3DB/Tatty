#include "Tatty3D.h"


Tatty3D::Tatty3D(double GameFrame):GameFrame(GameFrame)
{

	//计算性能和起始计数器
	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&StartCount);

}


Tatty3D::~Tatty3D(void) 
{

}


void Tatty3D::Render()
{
	
	//最终渲染
	Draw_T3Object(&obj, hMemDC);

}

void Tatty3D::RenderPipeline()
{
	//物体的局部坐标到世界坐标的转换
	ModelToWorld_T3Object(&obj, TRANSFORM_LOCAL_TO_TRANS);

	//物体世界坐标到相机坐标的变换
	WorldToCamera_T3Object(&cam, &obj);
	
	//物体的相机坐标到透视坐标
	CameraToPerspective_T3Object(&obj, &cam);

	//物体的透视坐标到屏幕坐标的
	PersepectiveToScreen_T3Object(&obj, &cam);

}



void Tatty3D::Clear()
{
	//获取设备DC
	hDC = GetDC(hWnd);
	//创建兼容的临时DC 解决屏闪
	hMemDC = CreateCompatibleDC(hDC);

	RECT rect;
	rect.left = 0;
	rect.top = 0; 
	rect.right = 1280;
	rect.bottom = 720;

	//在背景上创建一张兼容画布 
	//此后的绘图都在画布上进行
	HBITMAP hbm = CreateCompatibleBitmap(hDC, 1280, 720);
	SelectObject(hMemDC, hbm);
	DeleteObject(hbm);

	//填充背景为白色
	FillRect(hMemDC, &rect, WHITE_BRUSH);

}


void Tatty3D::Restore()
{

	ReleaseDC(hWnd, hMemDC);
	ReleaseDC(hWnd, hDC);
	//务必即时释放 否则内存会占满而溢出
	DeleteObject(hMemDC);
	DeleteObject(hDC);

}


void Tatty3D::UpdateView()
{

	LARGE_INTEGER CurrentCounter;

	//只有在实际帧数小于等于自定义帧数时才进行下一步
	do
	{
		//查询当前的计数器次数
		QueryPerformanceCounter(&CurrentCounter);

		//两帧之间的时间间隔 =(当前计数器 - 上一次计数器) / 运算频率
		double DeltaTime = (double) (CurrentCounter.LowPart - LastCounter.LowPart) / Frequency.LowPart ;

		//计算实际显示帧率
		DisplayFrame = 1.0 / DeltaTime;

	} while(DisplayFrame >= GameFrame);

	//上一帧的计数次数保存 
	LastCounter = CurrentCounter;


	//显示文本数组
	static TCHAR temp[50];

	//格式化输出帧率
	TextOut (hMemDC, 30, 550, temp, 	
		wsprintf (temp, TEXT ("FPS: %d"), (UINT)DisplayFrame));


	//一次更新避免屏闪
	BitBlt(hDC, 0, 0, 1280, 720, hMemDC, 0, 0, SRCCOPY);
}


void Tatty3D::Init()
{
	//旋转角度
	T3Vector4D dir = {40, 40, 0, 1};
	//相机世界坐标
	T3Point4D pos = {200, 2000, 100, 1};
	//物体世界坐标
	T3Point4D posObj = {0, 0, 100, 1};

	T3Load(obj, "tank1.txt");

	//建立SinCos查找表
	BuildSinCosTable();

	T3CameraInit(&cam, &pos, &dir, 90.0, 800, 600);
	T3CameraBuildMat(&cam, CAM_ROT_XYZ);

	obj.worldPosition = posObj;
	/*----------------Windows相关----------------*/

	//设置字体颜色
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
	//关闭的时候务必要释放
	/*----------------Windows相关----------------*/
	DeleteDC(hMemDC);              //释放缓存DC
	ReleaseDC(hWnd, hDC);       //释放设备DC
}


void Tatty3D::Frame()
{
	//清屏 进行循环
	Clear();

	//响应输入事件
	ReadInput();

	//渲染管线
	RenderPipeline();

	//绘制/渲染
	Render();

	//显示帧率锁定在自定义帧率之内 并更新界面视图
	UpdateView();

	//还原内存占用
	Restore();

}

