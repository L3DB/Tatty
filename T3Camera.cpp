#include"T3Camera.h"

void T3CameraInit(T3CameraPtr cam, T3Point4DPtr pos, T3Vector4DPtr dir, float64 FOV, 
				  float64 viewPortWidth, float64 viewPortHeight)
{
	//拷贝信息
	//拷贝位置和旋转角度信息
	T3Vector4DCopy(&cam->position, pos);
	T3Vector4DCopy(&cam->direction, dir);

	//初始化视野
	cam->FOV = FOV;

	//初始化视口
	cam->viewPortHeight = viewPortHeight;
	cam->viewPortWidth = viewPortWidth;

	//将三个矩阵单位化
	T3MatIdentity4X4(&cam->mCam);
	T3MatIdentity4X4(&cam->mPer);
	T3MatIdentity4X4(&cam->mScr);

	//视口的中心 坐标从0开始计数 因此减一
	cam->viewPortCenterX = (viewPortWidth - 1)/2;
	cam->viewPortCenterY = (viewPortHeight - 1)/2;

	//初始化屏幕宽高比
	cam->aspectRadio = viewPortWidth / viewPortHeight;

	//初始化视平面为2X2/as
	cam->viewPlaneHeight = 2.0 / cam->aspectRadio;
	cam->viewPlaneWidth = 2.0;

	//得到FOV/2的tan
	float64 TanFOVDiv2 = tan(AngleToRadian(FOV / 2));

	//计算视距
	cam->viewDistance = 0.5 * cam->viewPlaneWidth * TanFOVDiv2;
}

void T3CameraBuildMat(T3CameraPtr cam, int32 rotationOrder)
{
	//平移 xyz旋转的逆矩阵
	T3Mat4X4 mTranslateInv, mXInv, mYInv, mZInv;
	//世界到相机坐标变换的矩阵
	T3Mat4X4 mCamInv;
	//临时矩阵
	T3Mat4X4 mTemp;

	//初始化平移矩阵的逆矩阵
	T3MatInit4X4(&mTranslateInv,
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		-cam->position.x, -cam->position.y, -cam->position.z, 1);

	//提取需要旋转的角度
	float64 thetaX = cam->direction.x;
	float64 thetaY = cam->direction.y;
	float64 thetaZ = cam->direction.z;

	//正弦余弦值
	float64 cosTheta = FastCos(thetaX);
	float64 sinTheta = -FastSin(thetaX);

	//初始化X轴的逆矩阵
	T3MatInit4X4(&mXInv, 
		1, 0, 0, 0,
		0, cosTheta, sinTheta, 0,
		0, -sinTheta, cosTheta, 0,
		0, 0, 0, 1);

	//正弦余弦值
	cosTheta = FastCos(thetaY);
	sinTheta = -FastSin(thetaY);

	//初始化Y轴的逆矩阵
	T3MatInit4X4(&mYInv, 
		cosTheta, 0, -sinTheta, 0,
		0, 1, 0, 0,
		sinTheta, 0, cosTheta, 0,
		0, 0, 0, 1);

	//正弦余弦值
	cosTheta = FastCos(thetaZ);
	sinTheta = -FastSin(thetaZ);

	//初始化Y轴的逆矩阵
	T3MatInit4X4(&mZInv, 
		cosTheta, sinTheta, 0, 0,
		-sinTheta, cosTheta, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);

	//根据旋转顺序进行不同的旋转变换
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

	//将旋转矩阵与平移矩阵相乘
	T3MatMul4X4(&mTranslateInv, &mTemp, &mCamInv);

}