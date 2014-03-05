#include"T3Pipeline.h"

//�ṩҪ�任������ ��Ⱦ�б��Լ��������ת��
void TransformRenderList(T3RenderListPtr rendList, T3Mat4X4Ptr mt, int32 coordinateSelect)
{
	switch (coordinateSelect)
	{
	//ֱ�Ӹ��ľֲ�����
	case TRANSFORM_LOCAL:
		for(int32 poly=0; poly<rendList->numPolys; poly++)
		{
			//��õ�ǰ����ָ��Ķ����
			T3RenderPolyPtr currentPoly = rendList->polyPtr[poly];

			//���ڶ���ε�״̬�����ж�
			if(currentPoly == nullptr || 
				currentPoly->state != POLY4DV1_STATE_ACTIVE || 
				currentPoly->state == POLY4DV1_STATE_CLIPPED ||
				currentPoly->state == POLY4DV1_STATE_BACKFACE)
				//����ת��ֱ�������˶����
				continue;

			for(int32 vertex=0; vertex<3; vertex++)
			{
				T3Point4D result;

				//���о���任
				T3Mat_T3Vector4D_Mul_4X4(&currentPoly->vertexList[vertex], mt, &result);
				//��������浽�ֲ�������
				T3Vector4DCopy(&currentPoly->vertexList[vertex], &result);
			}
		}
		break;

	//����ת������������ת��
	case TRANSFORM_TRANS:
		for(int32 poly=0; poly<rendList->numPolys; poly++)
		{
			//��õ�ǰ����ָ��Ķ����
			T3RenderPolyPtr currentPoly = rendList->polyPtr[poly];

			//���ڶ���ε�״̬�����ж�
			if(currentPoly == nullptr || 
				currentPoly->state != POLY4DV1_STATE_ACTIVE || 
				currentPoly->state == POLY4DV1_STATE_CLIPPED ||
				currentPoly->state == POLY4DV1_STATE_BACKFACE)
				//����ת��ֱ�������˶����
				continue;

			for(int32 vertex=0; vertex<3; vertex++)
			{
				T3Point4D result;

				//���о���任
				T3Mat_T3Vector4D_Mul_4X4(&currentPoly->transVertexList[vertex], mt, &result);
				//��������浽�ֲ�������
				T3Vector4DCopy(&currentPoly->transVertexList[vertex], &result);
			}
		}
		break;

	//��ģ��������б任�洢�ڱ任�б���
	case TRANSFORM_LOCAL_TO_TRANS:
		for(int32 poly=0; poly<rendList->numPolys; poly++)
		{
			//��õ�ǰ����ָ��Ķ����
			T3RenderPolyPtr currentPoly = rendList->polyPtr[poly];

			//���ڶ���ε�״̬�����ж�
			if(currentPoly == nullptr || 
				currentPoly->state != POLY4DV1_STATE_ACTIVE || 
				currentPoly->state == POLY4DV1_STATE_CLIPPED ||
				currentPoly->state == POLY4DV1_STATE_BACKFACE)
				//����ת��ֱ�������˶����
				continue;

			for(int32 vertex=0; vertex<3; vertex++)
			{
				T3Point4D result;

				//���о���任
				T3Mat_T3Vector4D_Mul_4X4(&currentPoly->vertexList[vertex], mt, &result);
				//��������浽�ֲ�������
				T3Vector4DCopy(&currentPoly->transVertexList[vertex], &result);
			}
		}
		break;

	default:
		break;
	}
}

//��任��Ⱦ�б�ͬ�����Ƿ�Ҫת��ģ�͵ĳ���
void TransformT3Object(T3ObjectPtr obj, T3Mat4X4Ptr mt, int32 coordinateSelect, int32 transformOrientation)
{
	switch (coordinateSelect)
	{
	case TRANSFORM_LOCAL:
		for(int32 vertex=0; vertex<obj->numVertices; vertex++)
		{
			T3Point4D result;

			//�Ծֲ�����ת��
			T3Mat_T3Vector4D_Mul_4X4(&obj->vertexList_Local[vertex], mt, &result);

			T3Vector4DCopy(&obj->vertexList_Local[vertex], &result);
		}
		break;

	case TRANSFORM_TRANS:
		for(int32 vertex=0; vertex<obj->numVertices; vertex++)
		{
			T3Point4D result;

			//��ת�����������ж���ת��
			T3Mat_T3Vector4D_Mul_4X4(&obj->vertexList_Local[vertex], mt, &result);

			T3Vector4DCopy(&obj->vertexList_Trans[vertex], &result);
		}
		break;

	case TRANSFORM_LOCAL_TO_TRANS:
		for(int32 vertex=0; vertex<obj->numVertices; vertex++)
		{
			T3Point4D result;

			//��ת�����������ж���ת��
			T3Mat_T3Vector4D_Mul_4X4(&obj->vertexList_Local[vertex], mt, &result);

			T3Vector4DCopy(&obj->vertexList_Trans[vertex], &result);
		}
		break;

	default:
		break;
	}

	//�ж��Ƿ���Ҫ��ת����
	if(transformOrientation)
	{
		//�������峯�������ļ�����
		T3Vector4D result;

		//uxת��
		T3Mat_T3Vector4D_Mul_4X4(&obj->ux, mt, &result);
		T3Vector4DCopy(&obj->ux, &result);

		//uyת��
		T3Mat_T3Vector4D_Mul_4X4(&obj->uy, mt, &result);
		T3Vector4DCopy(&obj->uy, &result);

		//uzת��
		T3Mat_T3Vector4D_Mul_4X4(&obj->uz, mt, &result);
		T3Vector4DCopy(&obj->uz, &result);
	}
}

//����ľֲ����굽���������ת��
void ModelToWorld_T3Object(T3ObjectPtr obj, int32 coodinateSelect)
{
	if(coodinateSelect == TRANSFORM_LOCAL_TO_TRANS)
	{
		for(int32 vertex=0; vertex < obj->numVertices; vertex++)
		{
			//ֱ�Ӽ�������������б任
			T3Vector4DAdd(&obj->vertexList_Local[vertex], &obj->worldPosition, &obj->vertexList_Trans[vertex]);
		}
	}
	else 
	{
		for(int32 vertex=0; vertex < obj->numVertices; vertex++)
		{
			//ֱ�Ӽ�������������б任
			T3Vector4DAdd(&obj->vertexList_Local[vertex], &obj->worldPosition, &obj->vertexList_Trans[vertex]);
		}
	}
}

//��Ⱦ�б�ľֲ����굽���������ת��
void ModelToWorld_T3RenderList(T3RenderListPtr rendList, T3Point4DPtr worldPosition, int32 coodinateSelect)
{
	if(coodinateSelect == TRANSFORM_LOCAL_TO_TRANS)
	{
		for(int32 poly=0; poly<rendList->numPolys; poly++)
		{
			//��õ�ǰ����ָ��Ķ����
			T3RenderPolyPtr currentPoly = rendList->polyPtr[poly];

			//���ڶ���ε�״̬�����ж�
			if(currentPoly == nullptr || 
				currentPoly->state != POLY4DV1_STATE_ACTIVE || 
				currentPoly->state == POLY4DV1_STATE_CLIPPED ||
				currentPoly->state == POLY4DV1_STATE_BACKFACE)
				//����ת��ֱ�������˶����
				continue;

			for(int32 vertex=0; vertex<3; vertex++)
			{
				T3Vector4DAdd(&currentPoly->vertexList[vertex], worldPosition, &currentPoly->transVertexList[vertex]);
			}
		}
	}
	else 
	{
		for(int32 poly=0; poly<rendList->numPolys; poly++)
		{
			//��õ�ǰ����ָ��Ķ����
			T3RenderPolyPtr currentPoly = rendList->polyPtr[poly];

			//���ڶ���ε�״̬�����ж�
			if(currentPoly == nullptr || 
				currentPoly->state != POLY4DV1_STATE_ACTIVE || 
				currentPoly->state == POLY4DV1_STATE_CLIPPED ||
				currentPoly->state == POLY4DV1_STATE_BACKFACE)
				//����ת��ֱ�������˶����
				continue;

			for(int32 vertex=0; vertex<3; vertex++)
			{
				T3Vector4DAdd(&currentPoly->transVertexList[vertex], worldPosition, &currentPoly->transVertexList[vertex]);
			}
		}
	}
}

//�������굽�������ı任
void WorldToCamera_T3Object(T3CameraPtr cam, T3ObjectPtr obj)
{
	for(int32 vertex=0; vertex<obj->numVertices; vertex++)
	{
		T3Vector4D result;

		//������Ͼ���
		T3Mat_T3Vector4D_Mul_4X4(&obj->vertexList_Trans[vertex], &cam->mCam, &result);
		
		//������������ȥ
		T3Vector4DCopy(&obj->vertexList_Trans[vertex], &result);
	}
}

//�����������굽͸������
void CameraToPerspective_T3Object(T3ObjectPtr obj, T3CameraPtr cam)
{
	for(int32 vertex=0; vertex<obj->numVertices; vertex++)
	{
		float64 z = obj->vertexList_Trans[vertex].z;

		//��ÿ�������͸�ӱ任
		obj->vertexList_Trans[vertex].x = cam->viewDistance * obj->vertexList_Trans[vertex].x / z;
		obj->vertexList_Trans[vertex].y = cam->viewDistance * obj->vertexList_Trans[vertex].y * cam->aspectRadio / z;

	}
}

//�����͸�����굽��Ļ�����
void PersepectiveToScreen_T3Object(T3ObjectPtr obj, T3CameraPtr cam)
{
	float64 alpha = (0.5 * cam->viewPortWidth - 0.5);
	float64 beta = (0.5 * cam->viewPortHeight -0.5);
	for(int32 vertex=0; vertex<obj->numVertices; vertex++)
	{
		//����ƽ�������������� ����תY��
		obj->vertexList_Trans[vertex].x = alpha + alpha * obj->vertexList_Trans[vertex].x;
		obj->vertexList_Trans[vertex].y = beta - beta * obj->vertexList_Trans[vertex].y;
	}
}
