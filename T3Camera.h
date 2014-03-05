#ifndef T3CAMERA_H
#define T3CAMERA_H

#include"T3Math.h"


//欧拉相机旋转顺序宏
#define CAM_ROT_XYZ 0
#define CAM_ROT_YXZ 1
#define CAM_ROT_XZY 2
#define CAM_ROT_YZX 3
#define CAM_ROT_ZYX 4
#define CAM_ROT_ZXY 5


typedef struct T3CameraType
{
	//相机状态
	int32 state;
	//项技属性
	int32 attribute;

	//相机位置
	T3Point4D position;
	//相机朝向角度
	T3Vector4D direction;

	//视距
	float64 viewDistance;
	//水平视距
	float64 viewDistanceHeorizontal;
	//垂直视距
	float64 viewDistanceVertical;

	//视野
	//简单版本中默认设置为90度
	float64 FOV;

	//UVN相机

	//裁剪面

	//视平面宽高
	float64 viewPlaneWidth;
	float64 viewPlaneHeight;
    
	//视口/屏幕的宽高
	float64 viewPortWidth;
	float64 viewPortHeight;

	//视口中心的位置/坐标
	float64 viewPortCenterX;
	float64 viewPortCenterY;

	//屏幕宽高比
	float64 aspectRadio;

	//世界坐标转换到相机坐标的矩阵
	T3Mat4X4 mCam;
	//相机坐标到透视坐标
	T3Mat4X4 mPer;
	//透视坐标到屏幕坐标
	T3Mat4X4 mScr;

}T3Camera, *T3CameraPtr;

//相机初始化
void T3CameraInit(T3CameraPtr cam, T3Point4DPtr pos, T3Vector4DPtr dir, float64 FOV, 
				  float64 viewPortWidth, float64 viewPortHeight);

//创建相机变换矩阵
void T3CameraBuildMat(T3CameraPtr cam, int32 rotationOrder);
#endif