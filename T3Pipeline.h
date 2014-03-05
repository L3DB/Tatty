#ifndef T3PIPELINE_H
#define T3PIPELINE_H

#include"T3Math.h"
#include"T3Primitive.h"
#include"T3Camera.h"

//渲染列表最大包含多边形数目
#define T3RENDERLIST_MAX_POLY 100


typedef struct T3RenderListType
{
	//这个渲染列表中含有的多边形个数
	int32 numPolys;

	//多边形的指针数组
	T3RenderPolyPtr polyPtr[T3RENDERLIST_MAX_POLY];

	//渲染列表中多边形的实际存储位置为此
	T3RenderPoly polyData[T3RENDERLIST_MAX_POLY];

}T3RenderList, *T3RenderListPtr;

//多边形状态
//活跃可见
#define POLY4DV1_STATE_ACTIVE             0
//被裁剪
#define POLY4DV1_STATE_CLIPPED            1
//处于背面 被消除无需变换
#define POLY4DV1_STATE_BACKFACE           2

//对模型本身/局部坐标进行转换
#define TRANSFORM_LOCAL 0
//对变换后的顶点列表进行变换
#define TRANSFORM_TRANS 1
//对模型坐标进行变换存储在变换列表内
#define TRANSFORM_LOCAL_TO_TRANS 2

//提供要变换的坐标 渲染列表以及矩阵进行转换
void Transform_RenderList(T3RenderListPtr rendList, T3Mat4X4Ptr mt, int32 coordinateSelect);

//与变换渲染列表不同在于是否要转换模型的朝向
void Transform_T3Object(T3ObjectPtr obj, T3Mat4X4Ptr mt, int32 coordinateSelect, int32 transformOrientation); 

//物体的局部坐标到世界坐标的转换
void ModelToWorld_T3Object(T3ObjectPtr obj, int32 coodinateSelect);

//渲染列表的局部坐标到世界坐标的转换
void ModelToWorld_T3RenderList(T3RenderListPtr rendList, T3Point4DPtr worldPosition, int32 coodinateSelect);

//物体世界坐标到相机坐标的变换
void WorldToCamera_T3Object(T3CameraPtr cam, T3ObjectPtr obj);

//渲染列表世界坐标到相机坐标的变换
void WorldToCamera_T3RenderList(T3CameraPtr cam, T3RenderListPtr rend);

//物体的相机坐标到透视坐标
void CameraToPerspective_T3Object(T3ObjectPtr obj, T3CameraPtr cam);

//渲染列表的相机坐标到透视坐标
void CameraToPerspective_T3RenderList(T3RenderListPtr obj, T3CameraPtr cam);

//物体的透视坐标到屏幕坐标的
void PersepectiveToScreen_T3Object(T3ObjectPtr obj, T3CameraPtr cam);

#endif