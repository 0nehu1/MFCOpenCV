
// MFCOpenCVView.cpp: CMFCOpenCVView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MFCOpenCV.h"
#endif

#include "MFCOpenCVDoc.h"
#include "MFCOpenCVView.h"


#include <opencv2/opencv.hpp>
#include <opencv2/core/types_c.h>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <string>

using namespace cv;

// CMFCOpenCVView

IMPLEMENT_DYNCREATE(CMFCOpenCVView, CView)

BEGIN_MESSAGE_MAP(CMFCOpenCVView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_FILE_OPEN, &CMFCOpenCVView::OnFileOpen)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CMFCOpenCVView 생성/소멸

CMFCOpenCVView::CMFCOpenCVView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CMFCOpenCVView::~CMFCOpenCVView()
{
}

BOOL CMFCOpenCVView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMFCOpenCVView 그리기

void CMFCOpenCVView::OnDraw(CDC* /*pDC*/)
{
	CMFCOpenCVDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CMFCOpenCVView 인쇄

BOOL CMFCOpenCVView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMFCOpenCVView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMFCOpenCVView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CMFCOpenCVView 진단

#ifdef _DEBUG
void CMFCOpenCVView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCOpenCVView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCOpenCVDoc* CMFCOpenCVView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCOpenCVDoc)));
	return (CMFCOpenCVDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFCOpenCVView 메시지 처리기


void CMFCOpenCVView::OnFileOpen()
{
	CString filter = _T("Bitmap(*.BMP)|*.BMP|JPEG(*.JPG)|*.JPG|All Files(*.*)|*.*||");
	// FileDialog 클래스의 인스턴스 정의
	CFileDialog dlg(TRUE, _T(""), _T(""), OFN_HIDEREADONLY, filter);

	int iReturn = dlg.DoModal();



	// [열기] 버튼을 눌렀을 때
	if (iReturn == IDOK)
	{
		Mat img;
		String cvPathName;
		//OnOpenDocument(dlg.GetPathName());
		CString strPathName = dlg.GetPathName();
		std::string strDst = (LPSTR)(LPCTSTR)strPathName;	// CString -> std::string형변환
		cvPathName = strDst;							// cv::String 에 주소 입력

		CDC* pDc = GetWindowDC();
		CDC memdc;

		Invalidate(true);
		m_bmpBitmap.Destroy();
		m_bmpBitmap.Load(strPathName);
		width = m_bmpBitmap.GetWidth();
		height = m_bmpBitmap.GetHeight();

		trans_count_w = static_cast<float>(1024) / m_bmpBitmap.GetWidth();	// 이미지 가로 비율 
		trans_count_h = static_cast<float>(768) / m_bmpBitmap.GetHeight();  // 이미지 세로 비율

		width_change = width;
		height_change = height;

		CSize sizeTotal;
		// TODO: 이 뷰의 전체 크기를 계산합니다.


		//SetScrollSizes(MM_TEXT, sizeTotal);

		memdc.CreateCompatibleDC(pDc);
		//m_bmpBitmap.Draw(/*memdc.*/pDc->m_hDC, 0, 0, 1024 , 768);
		m_bmpBitmap.Draw(/*memdc.*/pDc->m_hDC, 0, 0, width_change * trans_count_w, height_change * trans_count_h);
		m_image = m_bmpBitmap;


		pDc->StretchBlt(0, 0, width_change * trans_count_w, height_change * trans_count_h, &memdc, 0, 0, width_change * trans_count_w, height_change * trans_count_h, SRCCOPY);



		ReleaseDC(pDc);
		mDc = pDc;
		pDc->DeleteDC();
		pDc = NULL;


	}
}


void CMFCOpenCVView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CView::OnPaint()을(를) 호출하지 마십시오.
	


}


