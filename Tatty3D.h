//��WindowBase�̳��� ��ʹ�õ�����Windows���Ĳ���
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
	//Ĭ�Ϲ��캯��
	//����Ҫ����Ϊ��Ҫ����֡��
	//GameApplication();

	//���캯��
	Tatty3D(double GameFrame);
	//��������
	~Tatty3D(void);

	//ʵ�ֻ��ຯ��
	void Init();
	void ShutDown();
	void Frame();

	//��Ӧ�������¼�
	void ReadInput();
	//��Ⱦ����
	void RenderPipeline();
	//���¿ռ���Ⱦ
	void Render();
	//���� ����ѭ��
	void Clear();
	//��ʱ�ͷ��ڴ�
	void Restore();
	//��ʾ֡���������Զ���֡��֮�� �����½�����ͼ
	void UpdateView();


	//�豸DC
	HDC hDC;
	//��ʱDC
	HDC hMemDC;
private:
	//�Զ�����Ϸ��֡��
	double GameFrame;
	//ʵ����ʾ��֡��
	double DisplayFrame;
	//����������
	LARGE_INTEGER Frequency;
	//�տ�ʼ(����һ��ʼ/ĳ��ѭ���е�һ��ʼ)�ļ�������
	LARGE_INTEGER StartCount;
	//��һ֡��������
	LARGE_INTEGER LastCounter;

	//��ȡģ����Ϣ
	T3Object obj;
	//�����
	T3Camera cam;
};




