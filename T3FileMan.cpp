#define _AFXDLL
#define _CRT_SECURE_NO_WARNINGS
#include "afx.h"
#include <Afxdlgs.h>
#include "stdio.h"
#include "string.h"
#include "math.h"
#include "T3FileMan.h"
#include "T3Graphics.h"
#include "T3Math.h"
#include "T3Primitive.h"



POINT T3O;//存放中心坐标
int FlagAntiAliasing=1;//标识是否抗锯齿

int T3Load(T3Object & LoadModle,char * cmf)
{
	//声明MFC防止报错
	if(!AfxWinInit(::GetModuleHandle(NULL),NULL,::GetCommandLine(),0)){return 1;}
	//地址、文件名、文件指针、参数序号
	CString dir="./model/",mf;
	FILE * pmf;
	mf=cmf;
	dir=dir+mf;
	if((_wfopen_s(&pmf,dir,_T("r")))!=0)
	{
		MessageBox (NULL, TEXT ("ERROR IN OPEN FILE"), TEXT ("ERROR"), 0);
		exit(0);
	}
	//读取模型参数
	int num=0;float64 temp;
	while(fscanf(pmf,"%lf",&temp)&&temp!=10)
	{
		LoadModle.vertexList_Local[num].x=temp;
		fscanf(pmf,"%lf %lf\n",
			&(LoadModle.vertexList_Local[num].y),
			&(LoadModle.vertexList_Local[num].z));
		num++;
		if (num>T3OBJECT_MAX_VERTICES)
			MessageBox (NULL, TEXT ("ERROR IN FILE"), TEXT ("ERROR"), 0);
	}
	LoadModle.numVertices=num;
	num=0;
	int index=0;
	while(!feof(pmf)&&temp==10)
	{
		fscanf(pmf,"%d",&index);
		if (index==3)
		{
			fscanf(pmf,"%d %d %d\n",
				&LoadModle.polyList[num].vertex[0],
				&LoadModle.polyList[num].vertex[1],
				&LoadModle.polyList[num].vertex[2]);
			num++;
		}
		else//nto3
		{
			fscanf(pmf,"%d %d %d",
			&LoadModle.polyList[num].vertex[0],
			&LoadModle.polyList[num].vertex[1],
			&LoadModle.polyList[num].vertex[2]);
			num++;
			for(int i=0;i<index-3;i++)
			{
				LoadModle.polyList[num].vertex[0]=LoadModle.polyList[num-1].vertex[0];
				LoadModle.polyList[num].vertex[1]=LoadModle.polyList[num-1].vertex[2];
				fscanf(pmf,"%d",&LoadModle.polyList[num].vertex[2]);
				num++;
			}
		}
		if(!feof(pmf))
			fscanf(pmf,"%lf",&temp);
		if (num>T3OBJECT_MAX_POLYS)
			MessageBox (NULL, TEXT ("ERROR IN FILE"), TEXT ("ERROR"), 0);
	}
	LoadModle.numPolygons=num;
	fclose(pmf);
	return 0;
}


	


int T3Load(T3PLGMODLE & LoadModle)
{
	//声明MFC防止报错
	if(!AfxWinInit(::GetModuleHandle(NULL),NULL,::GetCommandLine(),0)){return 1;}
	//地址、文件名、文件指针、参数序号
	CString dir="./model/";
	CString	mf;
	FILE * pmf;
	//打开文件通用对话框
	CFileDialog modle(TRUE);
	if(modle.DoModal()==IDOK)
		mf=modle.GetFileName();
	dir=dir+mf;
	if((pmf=_tfopen(dir,_T("r")))==NULL)
	{
		MessageBox (NULL, TEXT ("ERROR IN OPEN FILE"), TEXT ("ERROR"), 0);
		exit(0);
	}
	//读取模型所有参数
	int num=0;
	while(!feof(pmf))
			fscanf(pmf,"%d\n",&LoadModle.c[num++]);
	LoadModle.c[num]=11;
	fclose(pmf);
	//将顶点存入Point3
	int num1=0;
	double * pi=&LoadModle.Point3[0].x;
	while(LoadModle.c[num1]!=10)
	{		
		*pi++=LoadModle.c[num1++];
		*pi++=LoadModle.c[num1++];
		*pi++=LoadModle.c[num1++];
	}
	LoadModle.NumOfPoint=num1/3;
	//关于极坐标计算
	for(int i=0;i<LoadModle.NumOfPoint;i++)
	{
		LoadModle.RPoint[i].r=sqrt(
			(LoadModle.Point3[i].x)*(LoadModle.Point3[i].x)
			+(LoadModle.Point3[i].z)*(LoadModle.Point3[i].z));
		LoadModle.RPoint[i].theta=atan2(
			(LoadModle.Point3[i].z),
			(LoadModle.Point3[i].x));
		LoadModle.RPoint[i].y=LoadModle.Point3[i].y;
	}
	return 0;
}

int T3Draw(HDC hdc,HWND hwnd,T3PLGMODLE & PModle,COLORREF DColor)
{
	CTGraphics tattty;
	//点、矩形申明，用于存鼠标位置和窗口位置
	POINT ptc;
	LPRECT lpw;
	lpw=new RECT;
	//创建画笔神马的
	HPEN hPen;
	HPEN hPenOld;
	hPen = CreatePen( PS_SOLID, 3, DColor);
	hPenOld = ( HPEN )SelectObject ( hdc, hPen );
	//获取鼠标位置、窗口位置
	GetCursorPos(&ptc);
	GetWindowRect(hwnd,lpw);
	//相对位置计算，调教了一下
	T3O.x=ptc.x-lpw->left-8;
	T3O.y=ptc.y-lpw->top-30;
	
	//将点转化为二维
	for(int i=0;i<PModle.NumOfPoint;i++)
	{
		PModle.Point2[i].x=T3O.x+PModle.Point3[i].x+1/2*PModle.Point3[i].z;
		PModle.Point2[i].y=T3O.y-PModle.Point3[i].y-1/2*PModle.Point3[i].z;
	}
	//激动人心的时刻
	int index=3*PModle.NumOfPoint;
	int index2=0;
	while(PModle.c[index]!=11)
	{
		if(PModle.c[index]==10)
		{
			index++;
			index2=PModle.c[index];
			index++;
			for(int i=0;i<index2;i++)
			{
				PModle.DPoint[i]=PModle.Point2[PModle.c[index+i]];
			}
			if(FlagAntiAliasing)
				tattty.DrawPolyline(hdc, PModle.DPoint, index2, DColor);
			else
				Polyline(hdc,PModle.DPoint, index2);
			index+=index2;
		}
	}
	//释放画笔神马的
	SelectObject (hdc , hPenOld);
	DeleteObject (hPen );

	return 0;
}

int T3Revolve(HDC hdc,HWND hwnd,T3PLGMODLE & PModle,POINT RO,COLORREF RColor)
{
	CTGraphics tattty;
	static double thetar=0;
	POINT ptc;
	LPRECT lpw;
	lpw=new RECT;
	RECT   rect ;
	//创建画笔神马的
	HPEN hPen;
	HPEN hPenOld;
	hPen = CreatePen( PS_SOLID, 2, RColor);
	hPenOld = ( HPEN )SelectObject ( hdc, hPen );
	//获取鼠标位置、窗口位置
	GetCursorPos(&ptc);
	GetClientRect (hwnd, &rect) ;
	//中心位置计算
	T3O.x=RO.x-rect.left;
	T3O.y=RO.y-rect.top;
	//计算旋转后的点的极坐标
	for(int i=0;i<PModle.NumOfPoint;i++)
	{
		PModle.RPoint[i].x=PModle.RPoint[i].r*cos(thetar+PModle.RPoint[i].theta);
		PModle.RPoint[i].z=PModle.RPoint[i].r*sin(thetar+PModle.RPoint[i].theta);
	}
	thetar+=0.01;

	//将点转化为二维
	for(int i=0;i<PModle.NumOfPoint;i++)
	{
		PModle.Point2[i].x=T3O.x+PModle.RPoint[i].x+1/2*PModle.RPoint[i].z;
		PModle.Point2[i].y=T3O.y-PModle.RPoint[i].y-1/2*PModle.RPoint[i].z;
	}
	//激动人心的时刻
	int index=3*PModle.NumOfPoint;
	int index2=0;
	while(PModle.c[index]!=11)
	{
		if(PModle.c[index]==10)
		{
			index++;
			index2=PModle.c[index];
			index++;
			for(int i=0;i<index2;i++)
			{
				PModle.DPoint[i]=PModle.Point2[PModle.c[index+i]];
			}
			if(FlagAntiAliasing)
				tattty.DrawPolyline(hdc, PModle.DPoint, index2, RColor);
			else
				Polyline(hdc,PModle.DPoint, index2);
			index+=index2;
		}
	}
	//释放画笔神马的
	SelectObject (hdc , hPenOld);
	DeleteObject (hPen );
	return 0;
}

int T3Refresh(HDC hdc,HWND hwnd)
{
	CTGraphics tattty;
	//创建画笔神马的
	HPEN hPen;
	HPEN hPenOld;
	hPen = CreatePen( PS_SOLID, 1, RGB( 255 ,255, 255 ));
	hPenOld = ( HPEN )SelectObject ( hdc, hPen );
	HBRUSH hbrush;
	HBRUSH hbrushold;
	hbrush = CreateSolidBrush(RGB(0,0,0)); 
	hbrushold = ( HBRUSH )SelectObject ( hdc, hbrush );
	//画个圈圈
	Ellipse(hdc,100,100,700,500);
//	tattty.DrawEllipse(hdc, 100, 100, 700, 500, RGB(0, 0, 0));
	//释放画笔神马的
	SelectObject (hdc , hPenOld);
	DeleteObject (hPen );
	SelectObject (hdc , hbrush);
	DeleteObject (hbrush );
	return 0;
}
