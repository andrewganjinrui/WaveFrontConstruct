#pragma once


// CMyWaveFrontView ��ͼ

class CMyWaveFrontView : public CListView
{
	DECLARE_DYNCREATE(CMyWaveFrontView)

protected:
	CMyWaveFrontView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CMyWaveFrontView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	void InitializeListView();
	void DisplayWaveFront();
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};


