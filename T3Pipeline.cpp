#include"T3Pipeline.h"

//提供要变换的坐标 渲染列表以及矩阵进行转换
void TransformRenderList(T3RenderListPtr rendList, T3Mat4X4Ptr mt, int32 coordinateSelect)
{
	switch (coordinateSelect)
	{
	//直接更改局部坐标
	case TRANSFORM_LOCAL:
		for(int32 poly=0; poly<rendList->numPolys; poly++)
		{
			//获得当前索引指向的多边形
			T3RenderPolyPtr currentPoly = rendList->polyPtr[poly];

			//对于多边形的状态进行判断
			if(currentPoly == nullptr || 
				currentPoly->state != POLY4DV1_STATE_ACTIVE || 
				currentPoly->state == POLY4DV1_STATE_CLIPPED ||
				currentPoly->state == POLY4DV1_STATE_BACKFACE)
				//无需转换直接跳过此多边形
				continue;

			for(int32 vertex=0; vertex<3; vertex++)
			{
				T3Point4D result;

				//进行矩阵变换
				T3Mat_T3Vector4D_Mul_4X4(&currentPoly->vertexList[vertex], mt, &result);
				//将结果保存到局部坐标中
				T3Vector4DCopy(&currentPoly->vertexList[vertex], &result);
			}
		}
		break;

	//对于转换后的坐标进行转换
	case TRANSFORM_TRANS:
		for(int32 poly=0; poly<rendList->numPolys; poly++)
		{
			//获得当前索引指向的多边形
			T3RenderPolyPtr currentPoly = rendList->polyPtr[poly];

			//对于多边形的状态进行判断
			if(currentPoly == nullptr || 
				currentPoly->state != POLY4DV1_STATE_ACTIVE || 
				currentPoly->state == POLY4DV1_STATE_CLIPPED ||
				currentPoly->state == POLY4DV1_STATE_BACKFACE)
				//无需转换直接跳过此多边形
				continue;

			for(int32 vertex=0; vertex<3; vertex++)
			{
				T3Point4D result;

				//进行矩阵变换
				T3Mat_T3Vector4D_Mul_4X4(&currentPoly->transVertexList[vertex], mt, &result);
				//将结果保存到局部坐标中
				T3Vector4DCopy(&currentPoly->transVertexList[vertex], &result);
			}
		}
		break;

	//对模型坐标进行变换存储在变换列表内
	case TRANSFORM_LOCAL_TO_TRANS:
		for(int32 poly=0; poly<rendList->numPolys; poly++)
		{
			//获得当前索引指向的多边形
			T3RenderPolyPtr currentPoly = rendList->polyPtr[poly];

			//对于多边形的状态进行判断
			if(currentPoly == nullptr || 
				currentPoly->state != POLY4DV1_STATE_ACTIVE || 
				currentPoly->state == POLY4DV1_STATE_CLIPPED ||
				currentPoly->state == POLY4DV1_STATE_BACKFACE)
				//无需转换直接跳过此多边形
				continue;

			for(int32 vertex=0; vertex<3; vertex++)
			{
				T3Point4D result;

				//进行矩阵变换
				T3Mat_T3Vector4D_Mul_4X4(&currentPoly->vertexList[vertex], mt, &result);
				//将结果保存到局部坐标中
				T3Vector4DCopy(&currentPoly->transVertexList[vertex], &result);
			}
		}
		break;

	default:
		break;
	}
}

//与变换渲染列表不同在于是否要转换模型的朝向
void TransformT3Object(T3ObjectPtr obj, T3Mat4X4Ptr mt, int32 coordinateSelect, int32 transformOrientation)
{
	switch (coordinateSelect)
	{
	case TRANSFORM_LOCAL:
		for(int32 vertex=0; vertex<obj->numVertices; vertex++)
		{
			T3Point4D result;

			//对局部坐标转换
			T3Mat_T3Vector4D_Mul_4X4(&obj->vertexList_Local[vertex], mt, &result);

			T3Vector4DCopy(&obj->vertexList_Local[vertex], &result);
		}
		break;

	case TRANSFORM_TRANS:
		for(int32 vertex=0; vertex<obj->numVertices; vertex++)
		{
			T3Point4D result;

			//对转换后的坐标进行二次转换
			T3Mat_T3Vector4D_Mul_4X4(&obj->vertexList_Local[vertex], mt, &result);

			T3Vector4DCopy(&obj->vertexList_Trans[vertex], &result);
		}
		break;

	case TRANSFORM_LOCAL_TO_TRANS:
		for(int32 vertex=0; vertex<obj->numVertices; vertex++)
		{
			T3Point4D result;

			//对转换后的坐标进行二次转换
			T3Mat_T3Vector4D_Mul_4X4(&obj->vertexList_Local[vertex], mt, &result);

			T3Vector4DCopy(&obj->vertexList_Trans[vertex], &result);
		}
		break;

	default:
		break;
	}

	//判断是否需要旋转朝向
	if(transformOrientation)
	{
		//保存物体朝向向量的计算结果
		T3Vector4D result;

		//ux转换
		T3Mat_T3Vector4D_Mul_4X4(&obj->ux, mt, &result);
		T3Vector4DCopy(&obj->ux, &result);

		//uy转换
		T3Mat_T3Vector4D_Mul_4X4(&obj->uy, mt, &result);
		T3Vector4DCopy(&obj->uy, &result);

		//uz转换
		T3Mat_T3Vector4D_Mul_4X4(&obj->uz, mt, &result);
		T3Vector4DCopy(&obj->uz, &result);
	}
}

//物体的局部坐标到世界坐标的转换
void ModelToWorld_T3Object(T3ObjectPtr obj, int32 coodinateSelect)
{
	if(coodinateSelect == TRANSFORM_LOCAL_TO_TRANS)
	{
		for(int32 vertex=0; vertex < obj->numVertices; vertex++)
		{
			//直接加上世界坐标进行变换
			T3Vector4DAdd(&obj->vertexList_Local[vertex], &obj->worldPosition, &obj->vertexList_Trans[vertex]);
		}
	}
	else 
	{
		for(int32 vertex=0; vertex < obj->numVertices; vertex++)
		{
			//直接加上世界坐标进行变换
			T3Vector4DAdd(&obj->vertexList_Local[vertex], &obj->worldPosition, &obj->vertexList_Trans[vertex]);
		}
	}
}

//渲染列表的局部坐标到世界坐标的转换
void ModelToWorld_T3RenderList(T3RenderListPtr rendList, T3Point4DPtr worldPosition, int32 coodinateSelect)
{
	if(coodinateSelect == TRANSFORM_LOCAL_TO_TRANS)
	{
		for(int32 poly=0; poly<rendList->numPolys; poly++)
		{
			//获得当前索引指向的多边形
			T3RenderPolyPtr currentPoly = rendList->polyPtr[poly];

			//对于多边形的状态进行判断
			if(currentPoly == nullptr || 
				currentPoly->state != POLY4DV1_STATE_ACTIVE || 
				currentPoly->state == POLY4DV1_STATE_CLIPPED ||
				currentPoly->state == POLY4DV1_STATE_BACKFACE)
				//无需转换直接跳过此多边形
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
			//获得当前索引指向的多边形
			T3RenderPolyPtr currentPoly = rendList->polyPtr[poly];

			//对于多边形的状态进行判断
			if(currentPoly == nullptr || 
				currentPoly->state != POLY4DV1_STATE_ACTIVE || 
				currentPoly->state == POLY4DV1_STATE_CLIPPED ||
				currentPoly->state == POLY4DV1_STATE_BACKFACE)
				//无需转换直接跳过此多边形
				continue;

			for(int32 vertex=0; vertex<3; vertex++)
			{
				T3Vector4DAdd(&currentPoly->transVertexList[vertex], worldPosition, &currentPoly->transVertexList[vertex]);
			}
		}
	}
}

//世界坐标到相机坐标的变换
void WorldToCamera_T3Object(T3CameraPtr cam, T3ObjectPtr obj)
{
	for(int32 vertex=0; vertex<obj->numVertices; vertex++)
	{
		T3Vector4D result;

		//计算组合矩阵
		T3Mat_T3Vector4D_Mul_4X4(&obj->vertexList_Trans[vertex], &cam->mCam, &result);
		
		//计算结果拷贝回去
		T3Vector4DCopy(&obj->vertexList_Trans[vertex], &result);
	}
}

//物体的相机坐标到透视坐标
void CameraToPerspective_T3Object(T3ObjectPtr obj, T3CameraPtr cam)
{
	for(int32 vertex=0; vertex<obj->numVertices; vertex++)
	{
		float64 z = obj->vertexList_Trans[vertex].z;

		//对每个点进行透视变换
		obj->vertexList_Trans[vertex].x = cam->viewDistance * obj->vertexList_Trans[vertex].x / z;
		obj->vertexList_Trans[vertex].y = cam->viewDistance * obj->vertexList_Trans[vertex].y * cam->aspectRadio / z;

	}
}

//物体的透视坐标到屏幕坐标的
void PersepectiveToScreen_T3Object(T3ObjectPtr obj, T3CameraPtr cam)
{
	float64 alpha = (0.5 * cam->viewPortWidth - 0.5);
	float64 beta = (0.5 * cam->viewPortHeight -0.5);
	for(int32 vertex=0; vertex<obj->numVertices; vertex++)
	{
		//对视平面的坐标进行缩放 并反转Y轴
		obj->vertexList_Trans[vertex].x = alpha + alpha * obj->vertexList_Trans[vertex].x;
		obj->vertexList_Trans[vertex].y = beta - beta * obj->vertexList_Trans[vertex].y;
	}
}
