// WaveFrontConstructDoc.h : CWaveFrontConstructDoc 类的接口
//


#pragma once


class CWaveFrontConstructDoc : public CDocument
{
protected: // 仅从序列化创建
	CWaveFrontConstructDoc();
	DECLARE_DYNCREATE(CWaveFrontConstructDoc)

// 属性
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

	// 操作
public:

	inline void FreeImage(){if (m_pImage) {cvReleaseImage(&m_pImage); m_pImage = NULL;}}
	void UpdateImage(IplImage* pImage);
	IplImage* GetIplImg(){return m_pImage;}
	CSize GetDocDimension() const//获取文档大小
	{ return m_SizeDoc; }

	void SetZernikeData(CvMat* Mat, ParameterVect& ParaDocVect);
	void SaveRequiredData();
	float Get_Z(float x, float y);
	void UpdateWaveFront();

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// 实现
public:
	virtual ~CWaveFrontConstructDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
};


