#ifndef T3PIPELINE_H
#define T3PIPELINE_H

#include"T3Math.h"
#include"T3Primitive.h"
#include"T3Camera.h"

//��Ⱦ�б��������������Ŀ
#define T3RENDERLIST_MAX_POLY 100


typedef struct T3RenderListType
{
	//�����Ⱦ�б��к��еĶ���θ���
	int32 numPolys;

	//����ε�ָ������
	T3RenderPolyPtr polyPtr[T3RENDERLIST_MAX_POLY];

	//��Ⱦ�б��ж���ε�ʵ�ʴ洢λ��Ϊ��
	T3RenderPoly polyData[T3RENDERLIST_MAX_POLY];

}T3RenderList, *T3RenderListPtr;

//�����״̬
//��Ծ�ɼ�
#define POLY4DV1_STATE_ACTIVE             0
//���ü�
#define POLY4DV1_STATE_CLIPPED            1
//���ڱ��� ����������任
#define POLY4DV1_STATE_BACKFACE           2

//��ģ�ͱ���/�ֲ��������ת��
#define TRANSFORM_LOCAL 0
//�Ա任��Ķ����б���б任
#define TRANSFORM_TRANS 1
//��ģ��������б任�洢�ڱ任�б���
#define TRANSFORM_LOCAL_TO_TRANS 2

//�ṩҪ�任������ ��Ⱦ�б��Լ��������ת��
void Transform_RenderList(T3RenderListPtr rendList, T3Mat4X4Ptr mt, int32 coordinateSelect);

//��任��Ⱦ�б�ͬ�����Ƿ�Ҫת��ģ�͵ĳ���
void Transform_T3Object(T3ObjectPtr obj, T3Mat4X4Ptr mt, int32 coordinateSelect, int32 transformOrientation); 

//����ľֲ����굽���������ת��
void ModelToWorld_T3Object(T3ObjectPtr obj, int32 coodinateSelect);

//��Ⱦ�б�ľֲ����굽���������ת��
void ModelToWorld_T3RenderList(T3RenderListPtr rendList, T3Point4DPtr worldPosition, int32 coodinateSelect);

//�����������굽�������ı任
void WorldToCamera_T3Object(T3CameraPtr cam, T3ObjectPtr obj);

//��Ⱦ�б��������굽�������ı任
void WorldToCamera_T3RenderList(T3CameraPtr cam, T3RenderListPtr rend);

//�����������굽͸������
void CameraToPerspective_T3Object(T3ObjectPtr obj, T3CameraPtr cam);

//��Ⱦ�б��������굽͸������
void CameraToPerspective_T3RenderList(T3RenderListPtr obj, T3CameraPtr cam);

//�����͸�����굽��Ļ�����
void PersepectiveToScreen_T3Object(T3ObjectPtr obj, T3CameraPtr cam);

#endif