#pragma once
#include "stdafx.h"
#include "WaveFrontConstructDoc.h"
#include "gl.h"
#include "glaux.h"
#include "glu.h"

// C3DplayView ��ͼ

class C3DplayView : public CView
{
	DECLARE_DYNCREATE(C3DplayView)

protected:
	C3DplayView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~C3DplayView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

private:
	HGLRC m_hglrc;	//add two variable
	CClientDC* m_pDC;

	BOOL m_bLeftDown;
	CPoint m_pLeftPos;

	float m_RotateZ;
	float m_RotateX;
	float m_RotateY;

public:
	CWaveFrontConstructDoc* GetDocument() const;

	BOOL OpenGLExit();
	BOOL OpenGLInit();
	void DrawScene();
	void Draw_Grid();
	void Draw_3D();
	void UpdateMyView();

protected:
	virtual void OnDraw(CDC* pDC);      // ��д�Ի��Ƹ���ͼ
	virtual void OnInitialUpdate();     // �����ĵ�һ��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};


