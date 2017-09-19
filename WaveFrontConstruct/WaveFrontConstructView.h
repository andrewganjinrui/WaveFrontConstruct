// WaveFrontConstructView.h : CWaveFrontConstructView ��Ľӿ�
//


#pragma once
#include "WaveFrontConstructDoc.h"


class CWaveFrontConstructView : public CScrollView
{
protected: // �������л�����
	CWaveFrontConstructView();
	DECLARE_DYNCREATE(CWaveFrontConstructView)

// ����
public:
	CWaveFrontConstructDoc* GetDocument() const;

	ParameterVect m_ParameterVect;
	IplImage* m_pImgClone;
	IplImage* m_pDocImg;
	Parameter m_Parameter;
	CvPoint2D32f m_opt;
	CvPoint m_LineOpt;
	bool m_bLBtnDown;
	CPoint m_LeftPos;
	bool m_bWaveConstruct;
	RectVect m_RectVect;

	int m_NormalizeLength;

// ����
public:

	LPBITMAPINFO cv_CreatImgInfo(IplImage* pImg);
	void CalculateParameter(CvRect AnyRect);
	void NormalizeParameter();
	double GetZValue(int row, int col, CvPoint2D32f opt);
	void OnWavefrontconstruct();
	void OnLocate();
	CView *GetInfoView();
	CView *Get3DplayView();
	void DrawGrid();

	void ProduceRect(int Height, int Width, int n, int m);
	void MyThreshold(IplImage* pImg);
	void MyOtsu(IplImage* pImg);

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CWaveFrontConstructView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // WaveFrontConstructView.cpp �еĵ��԰汾
inline CWaveFrontConstructDoc* CWaveFrontConstructView::GetDocument() const
   { return reinterpret_cast<CWaveFrontConstructDoc*>(m_pDocument); }
#endif

