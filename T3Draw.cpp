#include"T3Draw.h"

//渲染物体
void Draw_T3Object(T3ObjectPtr obj, HDC hdc)
{
	CTGraphics tatty;

	for(int32 poly=0; poly<obj->numPolygons; poly++)
	{
		//if(obj->polyList[poly].state != T3POLY_STATE_ACTIVE ||
		//	obj->polyList[poly].state == T3POLY_STATE_CLIPED ||
		//	obj->polyList[poly].state == T3POLY_STATE_BACKFACE)
		//	continue;

		//读取当前多边形的三个顶点索引
		int32 index1 = obj->polyList[poly].vertex[0];
		int32 index2 = obj->polyList[poly].vertex[1];
		int32 index3 = obj->polyList[poly].vertex[2];

		tatty.DrawLine(hdc, obj->vertexList_Trans[index1].x,
			obj->vertexList_Trans[index1].y,
			obj->vertexList_Trans[index2].x,
			obj->vertexList_Trans[index2].y,
			RGB(1.0, 1.0, 0.0));

		tatty.DrawLine(hdc, obj->vertexList_Trans[index2].x,
			obj->vertexList_Trans[index2].y,
			obj->vertexList_Trans[index3].x,
			obj->vertexList_Trans[index3].y,
			RGB(1.0, 1.0, 0.0));

		tatty.DrawLine(hdc, obj->vertexList_Trans[index3].x,
			obj->vertexList_Trans[index3].y,
			obj->vertexList_Trans[index1].x,
			obj->vertexList_Trans[index1].y,
			RGB(1.0, 1.0, 0.0));
	}

	//tatty.DrawLine(hdc, 0, 0, 130, 100, RGB(1, 1, 0));
}