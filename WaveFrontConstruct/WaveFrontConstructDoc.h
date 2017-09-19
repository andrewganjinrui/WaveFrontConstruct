// WaveFrontConstructDoc.h : CWaveFrontConstructDoc ��Ľӿ�
//


#pragma once


class CWaveFrontConstructDoc : public CDocument
{
protected: // �������л�����
	CWaveFrontConstructDoc();
	DECLARE_DYNCREATE(CWaveFrontConstructDoc)

// ����
public:

private:
	IplImage* m_pImage;
	CSize m_SizeDoc;
	
	ParameterVect m_ParaDocVect;
public:
	float m_GridX[8][12];
	float m_GridY[8][12];
	double m_Zernike[15];
	float m_WaveFront[8][12];

	// ����
public:

	inline void FreeImage(){if (m_pImage) {cvReleaseImage(&m_pImage); m_pImage = NULL;}}
	void UpdateImage(IplImage* pImage);
	IplImage* GetIplImg(){return m_pImage;}
	CSize GetDocDimension() const//��ȡ�ĵ���С
	{ return m_SizeDoc; }

	void SetZernikeData(CvMat* Mat, ParameterVect& ParaDocVect);
	void SaveRequiredData();
	float Get_Z(float x, float y);
	void UpdateWaveFront();

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CWaveFrontConstructDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
};


