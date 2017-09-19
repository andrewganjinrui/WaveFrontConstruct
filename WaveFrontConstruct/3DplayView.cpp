// 3DplayView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WaveFrontConstruct.h"
#include "3DplayView.h"
#include "WaveFrontConstructDoc.h"
#include <math.h>

#define  PI 3.1415926
// C3DplayView

IMPLEMENT_DYNCREATE(C3DplayView, CView)

C3DplayView::C3DplayView()
{
	m_RotateX = -80;
	m_RotateY = 0;
	m_RotateZ = -40;

	m_bLeftDown = FALSE;
}

C3DplayView::~C3DplayView()
{
}


BEGIN_MESSAGE_MAP(C3DplayView, CView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// C3DplayView ��ͼ

void C3DplayView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	
	m_pDC = new CClientDC(this);		//Windows��ǰ�Ļ�ͼ�豸�����Ķ���ָ��
	ASSERT(m_pDC!=NULL);

	if(!OpenGLInit())		            //��ʼ��OpenGL����ʾ���
		return ;
}

void C3DplayView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: �ڴ���ӻ��ƴ���

	DrawScene();
}


// C3DplayView ���

#ifdef _DEBUG
void C3DplayView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void C3DplayView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// C3DplayView ��Ϣ�������

CWaveFrontConstructDoc* C3DplayView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWaveFrontConstructDoc)));
	return (CWaveFrontConstructDoc*)m_pDocument;
}

BOOL C3DplayView::OpenGLInit()
{
	int n;								//��ǰѡ������ظ�ʽ����

	static PIXELFORMATDESCRIPTOR pfd=
	{									//���ص��ʽ��Ϣ�����ṹ��
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |		//֧��Windows��OpenGL��������ͼ
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER,			//ʹ��˫���潻����ʾͼ��
		PFD_TYPE_RGBA,			    // RGBA type 
		24,							// 24-bit color depth 
		0, 0, 0, 0, 0, 0,			// color bits ignored 
		0,							// no alpha buffer 
		0,							// shift bit ignored 
		0,							// no accumulation buffer 
		0, 0, 0, 0,					// accum bits ignored 
		32,							// 32-bit z-buffer     
		0,							// no stencil buffer 
		0,							// no auxiliary buffer 
		PFD_MAIN_PLANE,				// main layer 
		0,							// reserved 
		0, 0, 0						// layer masks ignored 
	};

	int pixelformat;
	if((pixelformat=ChoosePixelFormat(m_pDC->GetSafeHdc(),&pfd))==0)
	{
		AfxMessageBox("ѡ�����ظ�ʽ��ʧ��");
		return FALSE;
	}

	if(SetPixelFormat(m_pDC->GetSafeHdc(),pixelformat,&pfd)==FALSE)
	{
		AfxMessageBox("�������ظ�ʽ��ʧ��");
		return FALSE;
	}

	n=::GetPixelFormat(m_pDC->GetSafeHdc());
	::DescribePixelFormat(m_pDC->GetSafeHdc(),n,sizeof(pfd),&pfd);

	m_hglrc = wglCreateContext(m_pDC->GetSafeHdc());	//���������龰����

	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hglrc);		//ѡ������龰����

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);

	return TRUE;
}

BOOL C3DplayView::OpenGLExit()
{
	::wglMakeCurrent(NULL,NULL);		//����ǰ��ͼ�龰�����ÿ�

	if(m_hglrc)
		::wglDeleteContext(m_hglrc);		//ɾ����ǰ��ͼ�龰����

	if(m_pDC)
		delete m_pDC;					//ɾ��������windows�豸������

	return TRUE;
}

void C3DplayView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	OpenGLExit();
}

void C3DplayView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	//�����ӿڴ�С
	glViewport(0,0,cx,cy);

	glMatrixMode(GL_PROJECTION);	//ͶӰ�任
	glLoadIdentity();

	if(cx <= cy)	//���ݴ��ڴ�С��������ͶӰ����
		glOrtho(-20.0,20.0,-20.0*(GLfloat)cy/(GLfloat)cx,
		20.0*(GLfloat)cy/(GLfloat)cx,-50.0,50.0);
	else
		glOrtho(-20.0*(GLfloat)cx/(GLfloat)cy,
		20.0*(GLfloat)cx/(GLfloat)cy,-20.0,20.0,-50.0,50.0);

	glMatrixMode(GL_MODELVIEW);		//����䲻���٣�����ͼ�ο�����Ч��
	glLoadIdentity();				//���ñ任ģʽΪģ�ͱ任
}

void C3DplayView::DrawScene()
{
	//����Ļ���Գ�ͼ��
	if(m_hglrc)					//���õ�ǰ��ͼ�豸ΪOpenGL���豸�龰����
		wglMakeCurrent(m_pDC->GetSafeHdc(),m_hglrc);
	else
		return;

	glDrawBuffer(GL_BACK);		//ָ���ں�̨�����л���ͼ��
	glLoadIdentity();			//��ʼ���任����

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	glShadeModel(GL_SMOOTH);

	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);

	glScalef(15.00, 15.00, 15.00);

	glRotated(m_RotateX,1.0,0.0,0.0);
	glRotated(m_RotateY,0.0,1.0,0.0);
	glRotated(m_RotateZ,0.0,0.0,1.0);

	//��ͼ
	Draw_Grid();
	Draw_3D();
	
	//��ͼ����
	glPopMatrix();
	glFinish();					    //������������

	SwapBuffers(wglGetCurrentDC());	//����ǰ�󻺴�
	glDrawBuffer(GL_FRONT);			//����ǰ��
}

void C3DplayView::Draw_Grid()
{
	glBegin(GL_LINES);
	//X axis
	glColor3f(0.0f, 1.0f, 0.0f);

	glVertex3f(-(GLdouble)1, 0.0f, 0.0f);
	glVertex3f((GLdouble)1, 0.0f, 0.0f);
	//Arrow
	glVertex3f((GLdouble)0.92, 0.08f, 0.0f);
	glVertex3f((GLdouble)1, 0.0f, 0.0f);

	glVertex3f((GLdouble)0.92, -0.08f, 0.0f);
	glVertex3f((GLdouble)1, 0.0f, 0.0f);	

	//Y  axis
	glColor3f(1.0f, 0.0f, 0.0f);

	glVertex3f(0.0f, -(GLdouble)1, 0.0f);
	glVertex3f(0.0f, (GLdouble)1, 0.0f);
	//Arrow
	glVertex3f(-0.08f, (GLdouble)0.92, 0.0f);
	glVertex3f(0, (GLdouble)1, 0.0f);

	glVertex3f(0.08f, (GLdouble)0.92, 0.0f);
	glVertex3f(0, (GLdouble)1, 0.0f);
	//Z  axis
	glColor3f(0.0f, 0.0f, 1.0f);

	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 1.0f);
	//Arrow
	glVertex3f(0, -0.08f, 0.92f);
	glVertex3f(0, 0, 1.0f);

	glVertex3f(0, 0.08f, 0.92f);
	glVertex3f(0, 0, 1.0f);
	glEnd();

	wglUseFontBitmaps(wglGetCurrentDC(),0,255,100);

	glListBase(100);

	glColor3f(0.0f,1.0f,0.0f);	
	glRasterPos3f((GLfloat)1,0.0f,0.0f);
	glCallLists(2,GL_UNSIGNED_BYTE,_T("+x"));

	glColor3f(1.0f,0.0f,0.0f);
	glRasterPos3f(0.0f,(GLfloat)1,0.0f);
	glCallLists(2,GL_UNSIGNED_BYTE,_T("+y"));

	glColor3f(0.0f,0.0f,1.0f);
	glRasterPos3f(0.0f,0.0f,1);
	glCallLists(2,GL_UNSIGNED_BYTE,_T("+z"));

}

void C3DplayView::Draw_3D()
{
	CWaveFrontConstructDoc* pDoc = GetDocument();
	float X[4], Y[4], Z[4];
	int i, j;
	float Max, Min;
	Max = -1.0;
	Min = 1.0;

	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 12; j++)
		{
			X[0] = pDoc->m_GridX[i][j];
			Y[0] = pDoc->m_GridY[i][j];
			if (sqrtf(pow(X[0], 2) + pow(Y[0], 2)) > 1)
			{
				Z[0] = 0;
				pDoc->m_WaveFront[i][j] = 0.0;
			}
			else
				Z[0] = pDoc->m_WaveFront[i][j];
			if (Z[0] > Max)
			{
				Max = Z[0];
			}
			if (Z[0] < Min)
			{
				Min = Z[0];
			}
		}
	}
	glBegin(GL_QUADS);
	for (i = 0; i < 7; i++)
	{
		for (j = 0; j < 11; j++)
		{
			X[0] = pDoc->m_GridX[i][j];
			Y[0] = pDoc->m_GridY[i][j];
			if(pDoc->m_WaveFront[i][j] == 0)
				Z[0] = 0.0;
			else
				Z[0] = (pDoc->m_WaveFront[i][j] - Min) * 1.0 / (Max - Min);

			X[1] = pDoc->m_GridX[i+1][j];
			Y[1] = pDoc->m_GridY[i+1][j];
			if(pDoc->m_WaveFront[i+1][j] == 0)
				Z[1] = 0.0;
			else
				Z[1] = (pDoc->m_WaveFront[i+1][j] - Min) * 1.0 / (Max - Min);

			X[2] = pDoc->m_GridX[i+1][j+1];
			Y[2] = pDoc->m_GridY[i+1][j+1];
			if(pDoc->m_WaveFront[i+1][j+1] == 0)
				Z[2] = 0.0;
			else
				Z[2] = (pDoc->m_WaveFront[i+1][j+1] - Min) * 1.0 / (Max - Min);

			X[3] = pDoc->m_GridX[i][j+1];
			Y[3] = pDoc->m_GridY[i][j+1];
			if(pDoc->m_WaveFront[i][j+1] == 0)
				Z[3] = 0.0;
			else
				Z[3] = (pDoc->m_WaveFront[i][j+1] - Min) * 1.0 / (Max - Min);

			for (int k = 0; k < 4; k++)
			{
				glColor3f(fabs(X[k]/2), fabs(Y[k]/2), k/4.0);
				glVertex3f(X[k], Y[k], Z[k]);
			}
		}
	}
	glEnd();
}

void C3DplayView::UpdateMyView()
{
	CRect rect;
	GetWindowRect(rect);
	C3DplayView::OnSize(0, rect.Width(), rect.Height());
 
	InvalidateRect(NULL, FALSE);
}

void C3DplayView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_bLeftDown = TRUE;
	m_pLeftPos = point;

	CView::OnLButtonDown(nFlags, point);
}

void C3DplayView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_bLeftDown = FALSE;

	CView::OnLButtonUp(nFlags, point);
}

void C3DplayView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(m_bLeftDown)
	{
		//����һ����Χ,��֤�����������»�����ת������ϵ5������
		int loffsety = point.y - m_pLeftPos.y;
		int loffsetx = point.x - m_pLeftPos.x;

		m_RotateX += loffsety;
		m_RotateZ += loffsetx;

		m_pLeftPos.x += loffsetx;
		m_pLeftPos.y += loffsety;

		InvalidateRect(NULL, FALSE);
	}

	CView::OnMouseMove(nFlags, point);
}
