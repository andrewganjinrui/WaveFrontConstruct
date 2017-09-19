// WaveFrontConstructView.cpp : CWaveFrontConstructView 类的实现
//

#include "stdafx.h"
#include "WaveFrontConstruct.h"
#include "MainFrm.h"

#include "WaveFrontConstructDoc.h"
#include "WaveFrontConstructView.h"
#include "InfoView.h"
#include "3DplayView.h"
#include "MyWaveFrontView.h"
#include <math.h>

#define  KNum 15
#define Space 15
#define min(a, b) (((a)<(b))?(a):(b))

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWaveFrontConstructView

IMPLEMENT_DYNCREATE(CWaveFrontConstructView, CScrollView)

BEGIN_MESSAGE_MAP(CWaveFrontConstructView, CScrollView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CWaveFrontConstructView 构造/析构

CWaveFrontConstructView::CWaveFrontConstructView()
{
	// TODO: 在此处添加构造代码
	m_pImgClone = NULL;
	m_pDocImg = NULL;
	m_LineOpt = cvPoint(250, 280);
	m_bLBtnDown = false;
	m_bWaveConstruct = false;
}

CWaveFrontConstructView::~CWaveFrontConstructView()
{
	if (m_pImgClone)
	{
		cvReleaseImage(&m_pImgClone);
		m_pImgClone = NULL;
	}
	if (m_pDocImg)
	{
		cvReleaseImage(&m_pDocImg);
		m_pDocImg = NULL;
	}
	m_RectVect.clear();
}

BOOL CWaveFrontConstructView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CScrollView::PreCreateWindow(cs);
}

// CWaveFrontConstructView 绘制

void CWaveFrontConstructView::OnDraw(CDC* pDC)
{
	CWaveFrontConstructDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	IplImage* pImg = pDoc->GetIplImg();

	LPBYTE pDibBites = (LPBYTE)pImg->imageData;
	LPBITMAPINFO pInfo = cv_CreatImgInfo(pImg);
	int lHeight = pImg->height;
	int lWidth = pImg->width;

	CDC MemDc;
	MemDc.CreateCompatibleDC(pDC);
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(pDC, lWidth, lHeight);
	CBitmap* pOldBmp = MemDc.SelectObject(&bitmap);

	::SetStretchBltMode(MemDc.GetSafeHdc(), COLORONCOLOR);// 设置显示模式		
	::StretchDIBits(MemDc.GetSafeHdc(),					// 设备环境句柄
					0,                 //目标X坐标
					0,                  // 目标Y坐标
					lWidth,            // 目标宽度
					lHeight,           // 目标高度
					0,                // 源X坐标
					0,                 // 源Y坐标
					lWidth,           // 源宽度
					lHeight,          // 源高度
					pDibBites,                      // 指向dib像素的指针
					pInfo,           // 指向位图信息结构的指针
					DIB_RGB_COLORS,                 // 使用的颜色数目
					SRCCOPY);     // 光栅操作类型

	pDC->StretchBlt(0, 0, lWidth, lHeight, &MemDc, 0, 0, lWidth, lHeight, SRCCOPY);

	pDC->SelectObject(pOldBmp);
	SelectObject(MemDc, pOldBmp);
	DeleteDC(MemDc);
}


// CWaveFrontConstructView 打印

BOOL CWaveFrontConstructView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CWaveFrontConstructView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CWaveFrontConstructView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CWaveFrontConstructView 诊断

#ifdef _DEBUG
void CWaveFrontConstructView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CWaveFrontConstructView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CWaveFrontConstructDoc* CWaveFrontConstructView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWaveFrontConstructDoc)));
	return (CWaveFrontConstructDoc*)m_pDocument;
}
#endif //_DEBUG


// CWaveFrontConstructView 消息处理程序

void CWaveFrontConstructView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	CWaveFrontConstructDoc* pDoc = GetDocument();
	SetScrollSizes(MM_TEXT, pDoc->GetDocDimension());
}

LPBITMAPINFO CWaveFrontConstructView::cv_CreatImgInfo(IplImage* pImg)
{
	if(pImg == NULL)
		return NULL;

	BITMAPINFOHEADER BIH = {40,1,1,1,8,0,0,0,0,0,0};
	LPBITMAPINFO lpBmInfo;
	int          wid, hei, bits, colors, i;
	RGBQUAD  ColorTab[256];
	wid =pImg->width;
	hei =pImg->height;
	bits = pImg->depth * pImg->nChannels;
	if (bits > 8) colors=0;
	else colors = 1<<bits;
	lpBmInfo = (LPBITMAPINFO) malloc(40 + 4*colors);
	BIH.biWidth = wid;
	BIH.biHeight = hei;
	BIH.biBitCount = (BYTE) bits;
	memcpy(lpBmInfo, &BIH, 40);                   //  复制位图信息头

	if (bits == 8)
	{                           //  256 色位图
		for (i = 0; i < 256; i++)
		{                                    //  设置灰阶调色板
			ColorTab[i].rgbRed = ColorTab[i].rgbGreen = ColorTab[i].rgbBlue = (BYTE) i;
			ColorTab[i].rgbReserved = 0;
		}
		memcpy(lpBmInfo->bmiColors, ColorTab, 1024);
	}
	return lpBmInfo;
}

void CWaveFrontConstructView::OnLocate()
{
	// TODO: 在此添加命令处理程序代码
	m_ParameterVect.clear();

	CWaveFrontConstructDoc* pDoc = GetDocument();
	IplImage* pImg = pDoc->GetIplImg();
	if(pImg == NULL)
		return;

	IplImage* pImgCopy = cvCreateImage( cvGetSize(pImg), IPL_DEPTH_8U, 1 );
	cvCvtColor(pImg, pImgCopy, CV_BGR2GRAY);
	
	m_pDocImg = cvCloneImage(pImg);
	m_pImgClone = cvCloneImage(pImgCopy);
	IplImage* markers = cvCreateImage( cvGetSize(pImg), IPL_DEPTH_32S, 1 );

//	cvSmooth(pImgCopy, pImgCopy);
//	MyThreshold(pImgCopy);
//	MyOtsu(pImgCopy);
//	cvDilate(pImgCopy, pImgCopy);
//	cvErode(pImgCopy, pImgCopy);
	cvThreshold(pImgCopy, pImgCopy, 30, 255, CV_THRESH_BINARY);
//	cvCanny(pImgCopy, pImgCopy, 40, 150);

	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* contours = 0;
	int i, j, comp_count = 0;
	cvFindContours(pImgCopy, storage, &contours, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	cvZero(markers);
	for( ; contours != 0; contours = contours->h_next, comp_count++ )
	{
		cvDrawContours( markers, contours, cvScalarAll(comp_count+1),
			cvScalarAll(comp_count+1), -1, -1, 8, cvPoint(0,0) );
	}
	cvWatershed(pImg, markers);

	cvZero(pImgCopy);
	for (i = 0; i < markers->height; i++)
	{
		for (j = 0; j < markers->width; j++)
		{
			int ndex = CV_IMAGE_ELEM(markers, int, i, j);
			if (ndex < 0 || ndex > comp_count)
				CV_IMAGE_ELEM(pImgCopy, char, i, j) = 255;
			else
				CV_IMAGE_ELEM(pImgCopy, char, i, j) = 0;

		}
	}
//	cvNamedWindow("qq");
//	cvShowImage("qq", pImgCopy);

	CvPoint pt1, pt2;
	CvRect AnyRect = cvRect(0, 0, 0, 0);
	cvFindContours(pImgCopy, storage, &contours, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	for( ; contours != 0; contours = contours->h_next)
	{
		AnyRect = cvBoundingRect(contours);
		CalculateParameter(AnyRect);

		pt1.x = AnyRect.x-2;
		pt1.y = AnyRect.y-2;
		pt2.x = pt1.x + AnyRect.width+3;
		pt2.y = pt1.y + AnyRect.height+3;

		cvRectangle(pImg, pt1, pt2, cvScalar(0, 0, 255));
	}
	cvSaveImage("D:\\2.tif", pImg);
	cvReleaseMemStorage(&storage);
	cvReleaseImage(&markers);
	cvReleaseImage(&pImgCopy);
	
//	pDoc->UpdateImage(pImgCopy);
	Invalidate(TRUE);
}

void CWaveFrontConstructView::ProduceRect(int Height, int Width, int n, int m)
{
	m_RectVect.clear();

	int stepx = Width / m;
	int stepy = Height / n;
	CvRect AnyRect = cvRect(0, 0, 0, 0);
	int Xlong, Ylong;

	for (int i = 0; i < n; i++)
	{
		Ylong = i * stepy;
		if(i == n-1)
			stepy = Height - (n-1)*stepy;
		for (int j = 0; j < m; j++)
		{
			if (j != m-1)
			{
				Xlong = j * stepx;
				AnyRect = cvRect(Xlong, Ylong, stepx, stepy);
				m_RectVect.push_back(AnyRect);
			}
			else
			{
				AnyRect = cvRect(Xlong, Ylong, Width - (m-1)*stepx, stepy);
				m_RectVect.push_back(AnyRect);
			}
		}
	}
}

void CWaveFrontConstructView::MyThreshold(IplImage* pImg)
{
	int lHeight = pImg->height;
	int lWidth = pImg->width;

	ProduceRect(lHeight, lWidth, 24, 21);
	if (m_RectVect.empty())
		return;
	
	CvRect AnyRect = cvRect(0, 0, 0, 0);
	for (int i = 0; i < m_RectVect.size(); i++)
	{
		AnyRect = m_RectVect[i];
		cvSetImageROI(pImg, AnyRect);

		IplImage* pMyImg = cvCreateImage(cvGetSize(pImg), 8, 1);
		cvCopyImage(pImg, pMyImg);
		MyOtsu(pMyImg);
		cvCopyImage(pMyImg, pImg);

		cvResetImageROI(pImg);
		cvReleaseImage(&pMyImg);
		pMyImg = NULL;
	}		
}

void CWaveFrontConstructView::MyOtsu(IplImage* pImg) 
{
	// TODO: Add your control notification handler code here
	LPBYTE pDibBites = (LPBYTE)pImg->imageData;
	int lHeight = pImg->height;
	int lWidth = pImg->width;
	int LineBytes = pImg->widthStep;

	double nHistogram[256];                                //灰度直方图
	double dVariance[256];                                 //类间方差
	int N = lHeight * lWidth;           //总像素数
	int i;
	// 初始化数组
	for(i=0; i<256; i++)
	{
		nHistogram[i] = 0.0;
		dVariance[i] = 0.0;
	}

	// 直方图数据统计
	for (int m = 0; m < lHeight; m++)
	{
		for (int n = 0; n < lWidth; n++)
		{
			int nData = *(pDibBites + (m * LineBytes) + n);
			nHistogram[nData]++;
		}
	}

	double Pa = 0.0;                                       //背景出现概率
	double Pb = 0.0;                                       //目标出现概率
	double Wa = 0.0;                                       //背景平均灰度值
	double Wb = 0.0;                                       //目标平均灰度值
	double W0 = 0.0;                                       //全局平均灰度值
	double dData1 = 0.0,  dData2 = 0.0;

	//计算全局平均灰
	for(i = 0; i < 256; i++)                               
	{
		nHistogram[i] /= N;
		W0 += i * nHistogram[i];
	}

	//对每个灰度值计算类间方差
	for(i = 0; i < 256; i++)                               
	{
		Pa += nHistogram[i];
		Pb = 1 - Pa;
		dData1 += i*nHistogram[i];
		dData2 = W0-dData1;
		Wa = dData1 / Pa;
		Wb = dData2 / Pb;
		dVariance[i] = (Pa * Pb * pow((Wb - Wa), 2));
	}

	//遍历每个方差，求取类间最大方差所对应的灰度值
	double temp = 0.0;
	int nThreshold = 0;

	for(i = 0; i < 256; i++)
	{
		if(dVariance[i] > temp)
		{
			temp = dVariance[i];
			nThreshold = i;
		}
	}

	cvThreshold(pImg, pImg, nThreshold, 255, CV_THRESH_BINARY);
}

void CWaveFrontConstructView::CalculateParameter(CvRect AnyRect)
{
	m_Parameter.CenterOpt = cvPoint2D32f(AnyRect.x + AnyRect.width / 2.0, AnyRect.y + AnyRect.height / 2.0);

	LPBYTE lpData = (LPBYTE)m_pImgClone->imageData;
	int LineBytes = m_pImgClone->widthStep;
	int WidthRoi = AnyRect.x + AnyRect.width;
	int HeightRoi = AnyRect.y + AnyRect.height;
	double Sum = 0.0;
	double Sumx = 0.0;           double Sumy = 0.0;
	BYTE pixel;
	for (int i = AnyRect.y; i < HeightRoi; i++)
	{
		for (int j = AnyRect.x; j < WidthRoi; j++)
		{
			pixel = *(lpData + i * LineBytes + j);
			Sum += pixel;
			Sumx += j * pixel;
			Sumy += i * pixel;
		}
	}
	m_Parameter.WeightOpt = cvPoint2D32f(Sumx / Sum, Sumy / Sum);


//	cvLine(m_pDocImg, cvPoint(int(m_Parameter.WeightOpt.x+0.5) -3, int(m_Parameter.WeightOpt.y+0.5)), cvPoint(int(m_Parameter.WeightOpt.x+0.5) +3, int(m_Parameter.WeightOpt.y+0.5)), cvScalar(0,0,255));
//	cvLine(m_pDocImg, cvPoint(int(m_Parameter.WeightOpt.x+0.5), int(m_Parameter.WeightOpt.y+0.5) -3), cvPoint(int(m_Parameter.WeightOpt.x+0.5), int(m_Parameter.WeightOpt.y+0.5) +3), cvScalar(0,0,255));

	m_ParameterVect.push_back(m_Parameter);
}

void CWaveFrontConstructView::OnWavefrontconstruct()
{
	// TODO: 在此添加命令处理程序代码
	CWaveFrontConstructDoc* pDoc = GetDocument();

	if(m_ParameterVect.empty())
		return;

	NormalizeParameter();
	int N = m_ParameterVect.size();

	CvMat* MatG = cvCreateMat(2*N, 1, CV_64FC1);
	double loffsetx, loffsety;
	int i, j;
	for (i = 0; i < N; i++)
	{
		loffsetx = /*fabs*/(m_ParameterVect[i].WeightOpt.x - m_ParameterVect[i].CenterOpt.x) * m_NormalizeLength / 1000.0;
		cvmSet(MatG, i*2, 0, loffsetx);
		loffsety = /*fabs*/(m_ParameterVect[i].WeightOpt.y - m_ParameterVect[i].CenterOpt.y)*m_NormalizeLength / 1000.0;
		cvmSet(MatG, i*2+1, 0, loffsety);
	}

	CvMat* MatZ = cvCreateMat(2*N, KNum, CV_64FC1);
	CvPoint2D32f Opt;
	double Value;
	for (i = 0; i < N; i++)
	{
		Opt = m_ParameterVect[i].CenterOpt;
		for (j = 0; j < KNum; j++)
		{
			Value = GetZValue(i*2, j, Opt);
			cvmSet(MatZ, i*2, j, Value);
			Value = GetZValue(i*2+1, j, Opt);
			cvmSet(MatZ, i*2+1, j, Value);
		}
	}
	CvMat* MatX = cvCreateMat(KNum, 1, CV_64FC1);

	cvSolve(MatZ, MatG, MatX, CV_SVD);
	pDoc->SetZernikeData(MatX, m_ParameterVect);

	CInfoView* pInfoView = (CInfoView*)GetInfoView();
	pInfoView->SetInfoZernike();

	m_bWaveConstruct = true;

	cvReleaseMat(&MatG);
	cvReleaseMat(&MatZ);
}

double CWaveFrontConstructView::GetZValue(int row, int col, CvPoint2D32f opt)
{
	float x = opt.x;
	float y = opt.y;

	double Value;
	switch(col)
	{
	case 0:
		Value = 0.0;
		break;
	case 1:
		if(row % 2 == 0)
			Value = 1.0;
		else
			Value = 0.0;
		break;
	case 2:
		if(row % 2 == 0)
			Value = 0.0;
		else
			Value = 1.0;
		break;
	case 3:
		if(row % 2 == 0)
			Value = 2 * y;
		else
			Value = 2 * x;
		break;
	case 4:
		if(row % 2 == 0)
			Value = 4 * x;
		else
			Value = 4 * y;
		break;
	case 5:
		if(row % 2 == 0)
			Value = -2 * x;
		else
			Value = 2 * y;
		break;
	case 6:
		if(row % 2 == 0)
			Value = -3 * x * x + 3 * y * y;
		else
			Value = 6 * x * y;
		break;
	case 7:
		if(row % 2 == 0)
			Value = -2 + 9 * x * x + 3 * y * y;
		else
			Value = 6 * x * y * y;
		break;
	case 8:
		if(row % 2 == 0)
			Value = 6 * x * y;
		else
			Value = -2 + 9 * y * y + 3 * x * x;
		break;
	case 9:
		if(row % 2 == 0)
			Value = -6 * x * y;
		else
			Value = 3 * y * y - 3 * x * x;
		break;
	case 10:
		if(row % 2 == 0)
			Value = -12 * x * x * y + 4 * y * y * y;
		else
			Value = -4 * pow(x, 3) + 12 * x * y * y;
		break;
	case 11:
		if(row % 2 == 0)
			Value = -6 * y + 24 * x * x * y + 8 * pow(y, 3);
		else
			Value = -6 * x + 24 * x * y * y + 8 * pow(x, 3);
		break;
	case 12:
		if(row % 2 == 0)
			Value = -12 * x + 24 * pow(x, 3) + 24 * x * y * y;
		else
			Value = -12 * y + 24 * pow(y, 3) + 24 * x * x * y;
		break;
	case 13:
		if(row % 2 == 0)
			Value = 6*x - 16 * pow(x, 3);
		else
			Value = -6 * y + 16 * pow(y, 3);
		break;
	case 14:
		if(row % 2 == 0)
			Value = 4 * pow(x,4) - 12*x*y*y;
		else
			Value = -12*x*x*y + 4*y*y*y;
		break;
	default:
		break;			
	}
	return Value;
}

void CWaveFrontConstructView::NormalizeParameter()
{
	if(m_ParameterVect.empty())
		return;

	CWaveFrontConstructDoc* pDoc = GetDocument();
	IplImage* pImg = pDoc->GetIplImg();

	
	double sumx, sumy;
	sumx = sumy = 0.0;
	int N = m_ParameterVect.size();
	int i;
	for (i = 0; i < N; i++)
	{
		sumx += m_ParameterVect[i].CenterOpt.x;
		sumy += m_ParameterVect[i].CenterOpt.y;
	}
	m_opt = cvPoint2D32f(sumx / N, sumy / N);
	int bb = min(m_opt.x, pImg->width-m_opt.x);
	int aa = min(m_opt.y, pImg->height-m_opt.y);
	m_NormalizeLength = min(aa, bb);
	cvCircle(pImg, cvPoint(m_opt.x, m_opt.y), m_NormalizeLength, CV_RGB(0, 255, 0));
	for (i = 0; i < N; i++)
	{
		m_ParameterVect[i].CenterOpt.x = (m_ParameterVect[i].CenterOpt.x - m_opt.x) / m_NormalizeLength;
		m_ParameterVect[i].CenterOpt.y = (m_ParameterVect[i].CenterOpt.y - m_opt.y) / m_NormalizeLength;
		m_ParameterVect[i].WeightOpt.x = (m_ParameterVect[i].WeightOpt.x - m_opt.x) / m_NormalizeLength;
		m_ParameterVect[i].WeightOpt.y = (m_ParameterVect[i].WeightOpt.y - m_opt.y) / m_NormalizeLength;
	}

	ParameterVect TempVect;
	TempVect.clear();
	double distance;
	for (i = 0; i < N; i++)
	{
		m_Parameter.CenterOpt.x = m_ParameterVect[i].CenterOpt.x;
		m_Parameter.CenterOpt.y = m_ParameterVect[i].CenterOpt.y;
		m_Parameter.WeightOpt.x = m_ParameterVect[i].WeightOpt.x;
		m_Parameter.WeightOpt.y = m_ParameterVect[i].WeightOpt.y;
		distance = sqrtf(pow(m_Parameter.CenterOpt.x, 2) + pow(m_Parameter.CenterOpt.y, 2));
		if (distance <= 1.0)
		{
			TempVect.push_back(m_Parameter);
		}
	}
	m_ParameterVect.clear();
	m_ParameterVect = TempVect;

	TempVect.clear();
	Invalidate(TRUE);
}

CView *CWaveFrontConstructView::GetInfoView() 
{
	CWaveFrontConstructApp   *pApp   = (CWaveFrontConstructApp *)AfxGetApp();		//取得当前应用程序对象句柄
	CMainFrame *pFrame = (CMainFrame *)pApp->m_pMainWnd;	            //取得应用程序主窗口句柄
	//取得displayView子视图即(0，0)句柄
	CView      *pView  = (CView *)pFrame->m_wndSplitter2.GetPane(0, 0);
	return pView;
}

CView *CWaveFrontConstructView::Get3DplayView() 
{
	CWaveFrontConstructApp   *pApp   = (CWaveFrontConstructApp *)AfxGetApp();		//取得当前应用程序对象句柄
	CMainFrame *pFrame = (CMainFrame *)pApp->m_pMainWnd;	            //取得应用程序主窗口句柄
	//取得displayView子视图即(0，0)句柄
	CView      *pView  = (CView *)pFrame->m_wndSplitter2.GetPane(1, 0);
	return pView;
}

void CWaveFrontConstructView::DrawGrid()
{
	CWaveFrontConstructDoc* pDoc = GetDocument();
	C3DplayView* p3DPlayView = (C3DplayView*)Get3DplayView();
	CMyWaveFrontView*  pListView = ((CMainFrame*)AfxGetMainWnd())->m_pListView;
	
	if(!m_bWaveConstruct)
		return;
	IplImage* pImg = cvCloneImage(m_pDocImg);

	for (int i = 0; i <= 8; i++)
	{
		cvLine(pImg, cvPoint(m_LineOpt.x, m_LineOpt.y + i * Space), cvPoint(m_LineOpt.x + 12 * Space, m_LineOpt.y + i * Space), CV_RGB(0, 255, 0));
	}
	for (int j = 0; j <= 12; j++)
	{
		cvLine(pImg, cvPoint(m_LineOpt.x + j * Space, m_LineOpt.y), cvPoint(m_LineOpt.x + j * Space, m_LineOpt.y + Space * 8), CV_RGB(0, 255, 0));
	}
	CvPoint2D32f Pt;
	for (int i = 0; i < 8; i++)
	{
		if(i == 0)
			Pt.y = (float)(m_LineOpt.y + Space/2.0 - m_opt.y) / m_NormalizeLength;
		else
			Pt.y = (float)(m_LineOpt.y + Space/2.0 + i * Space - m_opt.y) / m_NormalizeLength;
		for (int j = 0; j < 12; j++)
		{
			if (j == 0)
				Pt.x = (float)(m_LineOpt.x + Space/2.0 - m_opt.x) / m_NormalizeLength;
			else
				Pt.x = (float)(m_LineOpt.x + Space/2.0 + j * Space - m_opt.x) / m_NormalizeLength;

				pDoc->m_GridX[i][j] = Pt.x;
				pDoc->m_GridY[i][j] = Pt.y;
		}
	}
	pDoc->UpdateImage(pImg);
	pDoc->UpdateWaveFront();
	
	if(((CMainFrame*)AfxGetMainWnd())->m_bListView)
		pListView->DisplayWaveFront();
 	p3DPlayView->UpdateMyView();	
 	Invalidate(FALSE);
}

void CWaveFrontConstructView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CScrollView::OnLButtonDown(nFlags, point);
	m_bLBtnDown = true;
	m_LeftPos = point;
}

void CWaveFrontConstructView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CScrollView::OnLButtonUp(nFlags, point);
	m_bLBtnDown = false;
}

void CWaveFrontConstructView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CScrollView::OnMouseMove(nFlags, point);
	if (m_bLBtnDown)
	{
		int loffsetx = point.x - m_LeftPos.x;
		int loffsety = point.y - m_LeftPos.y;

		m_LineOpt.x += loffsetx;
		m_LineOpt.y -= loffsety;

		m_LeftPos.x += loffsetx;
		m_LeftPos.y += loffsety;

		DrawGrid();
	}
}
