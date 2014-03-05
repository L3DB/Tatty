//由WindowBase继承来 会使用到部分Windows风格的参数
//#include"LoadHead.h"
#include"WindowsBase.h"
#include<Windows.h>
#include"T3Pipeline.h"
#include"T3FileMan.h"
#include"T3Draw.h"
#include"T3IO.h"

class Tatty3D : public WindowsBase
{
public:
	//默认构造函数
	//不需要是因为需要给定帧率
	//GameApplication();

	//构造函数
	Tatty3D(double GameFrame);
	//析构函数
	~Tatty3D(void);

	//实现基类函数
	void Init();
	void ShutDown();
	void Frame();

	//响应鼠标键盘事件
	void ReadInput();
	//渲染管线
	void RenderPipeline();
	//更新空间渲染
	void Render();
	//清屏 进行循环
	void Clear();
	//即时释放内存
	void Restore();
	//显示帧率锁定在自定义帧率之内 并更新界面视图
	void UpdateView();


	//设备DC
	HDC hDC;
	//临时DC
	HDC hMemDC;
private:
	//自定义游戏的帧数
	double GameFrame;
	//实际显示的帧数
	double DisplayFrame;
	//性能运算数
	LARGE_INTEGER Frequency;
	//刚开始(程序一开始/某个循环中的一开始)的计数次数
	LARGE_INTEGER StartCount;
	//上一帧计数次数
	LARGE_INTEGER LastCounter;

	//读取模型信息
	T3Object obj;
	//摄像机
	T3Camera cam;
};




