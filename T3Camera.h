#ifndef T3CAMERA_H
#define T3CAMERA_H

#include"T3Math.h"


//ŷ�������ת˳���
#define CAM_ROT_XYZ 0
#define CAM_ROT_YXZ 1
#define CAM_ROT_XZY 2
#define CAM_ROT_YZX 3
#define CAM_ROT_ZYX 4
#define CAM_ROT_ZXY 5


typedef struct T3CameraType
{
	//���״̬
	int32 state;
	//�����
	int32 attribute;

	//���λ��
	T3Point4D position;
	//�������Ƕ�
	T3Vector4D direction;

	//�Ӿ�
	float64 viewDistance;
	//ˮƽ�Ӿ�
	float64 viewDistanceHeorizontal;
	//��ֱ�Ӿ�
	float64 viewDistanceVertical;

	//��Ұ
	//�򵥰汾��Ĭ������Ϊ90��
	float64 FOV;

	//UVN���

	//�ü���

	//��ƽ����
	float64 viewPlaneWidth;
	float64 viewPlaneHeight;
    
	//�ӿ�/��Ļ�Ŀ��
	float64 viewPortWidth;
	float64 viewPortHeight;

	//�ӿ����ĵ�λ��/����
	float64 viewPortCenterX;
	float64 viewPortCenterY;

	//��Ļ��߱�
	float64 aspectRadio;

	//��������ת�����������ľ���
	T3Mat4X4 mCam;
	//������굽͸������
	T3Mat4X4 mPer;
	//͸�����굽��Ļ����
	T3Mat4X4 mScr;

}T3Camera, *T3CameraPtr;

//�����ʼ��
void T3CameraInit(T3CameraPtr cam, T3Point4DPtr pos, T3Vector4DPtr dir, float64 FOV, 
				  float64 viewPortWidth, float64 viewPortHeight);

//��������任����
void T3CameraBuildMat(T3CameraPtr cam, int32 rotationOrder);
#endif