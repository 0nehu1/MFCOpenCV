
// MFCOpenCVView.h: CMFCOpenCVView 클래스의 인터페이스
//

#pragma once


class CMFCOpenCVView : public CView
{
protected: // serialization에서만 만들어집니다.
	CMFCOpenCVView() noexcept;
	DECLARE_DYNCREATE(CMFCOpenCVView)

// 특성입니다.
public:
	CMFCOpenCVDoc* GetDocument() const;

// 작업입니다.
public:
	CImage m_bmpBitmap;
	CImage c_bmpBitmap;
	CImage m_image; // 원본 이미지 변수
	CDC* mDc;

	int width;
	int height;
	int width_change;
	int height_change;
	
	long double trans_count_w = 1;
	long double trans_count_h = 1;


// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CMFCOpenCVView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileOpen();
	afx_msg void OnPaint();

};

#ifndef _DEBUG  // MFCOpenCVView.cpp의 디버그 버전
inline CMFCOpenCVDoc* CMFCOpenCVView::GetDocument() const
   { return reinterpret_cast<CMFCOpenCVDoc*>(m_pDocument); }
#endif

