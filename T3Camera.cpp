#include"T3Camera.h"

void T3CameraInit(T3CameraPtr cam, T3Point4DPtr pos, T3Vector4DPtr dir, float64 FOV, 
				  float64 viewPortWidth, float64 viewPortHeight)
{
	//������Ϣ
	//����λ�ú���ת�Ƕ���Ϣ
	T3Vector4DCopy(&cam->position, pos);
	T3Vector4DCopy(&cam->direction, dir);

	//��ʼ����Ұ
	cam->FOV = FOV;

	//��ʼ���ӿ�
	cam->viewPortHeight = viewPortHeight;
	cam->viewPortWidth = viewPortWidth;

	//����������λ��
	T3MatIdentity4X4(&cam->mCam);
	T3MatIdentity4X4(&cam->mPer);
	T3MatIdentity4X4(&cam->mScr);

	//�ӿڵ����� �����0��ʼ���� ��˼�һ
	cam->viewPortCenterX = (viewPortWidth - 1)/2;
	cam->viewPortCenterY = (viewPortHeight - 1)/2;

	//��ʼ����Ļ��߱�
	cam->aspectRadio = viewPortWidth / viewPortHeight;

	//��ʼ����ƽ��Ϊ2X2/as
	cam->viewPlaneHeight = 2.0 / cam->aspectRadio;
	cam->viewPlaneWidth = 2.0;

	//�õ�FOV/2��tan
	float64 TanFOVDiv2 = tan(AngleToRadian(FOV / 2));

	//�����Ӿ�
	cam->viewDistance = 0.5 * cam->viewPlaneWidth * TanFOVDiv2;
}

void T3CameraBuildMat(T3CameraPtr cam, int32 rotationOrder)
{
	//ƽ�� xyz��ת�������
	T3Mat4X4 mTranslateInv, mXInv, mYInv, mZInv;
	//���絽�������任�ľ���
	T3Mat4X4 mCamInv;
	//��ʱ����
	T3Mat4X4 mTemp;

	//��ʼ��ƽ�ƾ���������
	T3MatInit4X4(&mTranslateInv,
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		-cam->position.x, -cam->position.y, -cam->position.z, 1);

	//��ȡ��Ҫ��ת�ĽǶ�
	float64 thetaX = cam->direction.x;
	float64 thetaY = cam->direction.y;
	float64 thetaZ = cam->direction.z;

	//��������ֵ
	float64 cosTheta = FastCos(thetaX);
	float64 sinTheta = -FastSin(thetaX);

	//��ʼ��X��������
	T3MatInit4X4(&mXInv, 
		1, 0, 0, 0,
		0, cosTheta, sinTheta, 0,
		0, -sinTheta, cosTheta, 0,
		0, 0, 0, 1);

	//��������ֵ
	cosTheta = FastCos(thetaY);
	sinTheta = -FastSin(thetaY);

	//��ʼ��Y��������
	T3MatInit4X4(&mYInv, 
		cosTheta, 0, -sinTheta, 0,
		0, 1, 0, 0,
		sinTheta, 0, cosTheta, 0,
		0, 0, 0, 1);

	//��������ֵ
	cosTheta = FastCos(thetaZ);
	sinTheta = -FastSin(thetaZ);

	//��ʼ��Y��������
	T3MatInit4X4(&mZInv, 
		cosTheta, sinTheta, 0, 0,
		-sinTheta, cosTheta, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);

	//������ת˳����в�ͬ����ת�任
	switch(rotationOrder)
	{
	case CAM_ROT_XYZ:
		{
			T3MatMul4X4(&mXInv, &mYInv, &mTemp);
			T3MatMul4X4(&mTemp, &mZInv, &mCamInv);
		} break;

	case CAM_ROT_YXZ:
		{
			T3MatMul4X4(&mYInv, &mXInv, &mTemp);
			T3MatMul4X4(&mTemp, &mZInv, &mCamInv);
		} break;

	case CAM_ROT_XZY:
		{
			T3MatMul4X4(&mXInv, &mZInv, &mTemp);
			T3MatMul4X4(&mTemp, &mYInv, &mCamInv);
		} break;

	case CAM_ROT_YZX:
		{
			T3MatMul4X4(&mYInv, &mZInv, &mTemp);
			T3MatMul4X4(&mTemp, &mXInv, &mCamInv);
		} break;

	case CAM_ROT_ZYX:
		{
			T3MatMul4X4(&mZInv, &mYInv, &mTemp);
			T3MatMul4X4(&mTemp, &mXInv, &mCamInv);
		} break;

	case CAM_ROT_ZXY:
		{
			T3MatMul4X4(&mZInv, &mXInv, &mTemp);
			T3MatMul4X4(&mTemp, &mYInv, &mCamInv);

		} break;

	}

	//����ת������ƽ�ƾ������
	T3MatMul4X4(&mTranslateInv, &mTemp, &mCamInv);

}