#pragma once
#include "afxdialogex.h"

#include <opencv2/opencv.hpp>
#include <opencv2/core/types_c.h>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <atlimage.h>
#include <opencv2/imgproc/imgproc_c.h>
#include "opencv2/highgui/highgui.hpp"
#include <stdio.h>
#pragma comment (lib, "opencv_world454d")
#include <vfw.h>
#include "CRotateAngleDlg.h"
#pragma comment (lib,"vfw32.lib")
using namespace cv;
// CImageOpenDlg 대화 상자

class CImageOpenDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CImageOpenDlg)

public:
	CImageOpenDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CImageOpenDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_OPENCV_IMAGE };
#endif

public:
	Mat m_matImage; // 이미지 정보를 담고 있는 객체.
	Mat c_matImage;	// 이미지 정보 복사한 객체.
	BITMAPINFO* m_pBitmapInfo; // Bitmap 정보를 담고 있는 구조체.

	void CreateBitmapInfo(int w, int h, int bpp); // Bitmap 정보를 생성하는 함수.
	//void DrawImage(Mat img); // 그리는 작업을 수행하는 함수.
	void DrawImage(Mat img); // 그리는 작업을 수행하는 함수.
	long double trans_count_w = 1; // 가로 비율
	long double trans_count_h = 1; // 세로 비율
	POINT c;
	POINT origin;
	long double count=1;
	int Hpos;
	int Vpos;
	CRect rect;
	CRect rect_mini;
	VideoCapture* capture;
	Mat mat_frame;
	CImage cimage_mfc;

	int sourcex, sourcey;
	// 웹캠 연결 부분
	HWND hWindow;
	HDC hWindowDC, hWindowCompatibleDC;
	int height, width, srcHeight, srcWidth;
	HBITMAP hBitmap;
	BITMAPINFOHEADER bi;


protected:
	HICON m_hIcon;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonImage();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	CStatic m_TextSize;
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
	CStatic m_TextSize2;
	CStatic m_TextSize3;
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonImageEnlargement();
	afx_msg void OnBnClickedButtonImageReduction();
	afx_msg void OnBnClickedButtonImageOriginal();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CScrollBar m_VScroll;
	CScrollBar m_HScroll;
	afx_msg void OnBnClickedButtonSobel();
	afx_msg void OnBnClickedButtonCannyedge();
	//afx_msg void OnBnClickedButtonCircledetect();
	//afx_msg void OnBnClickedButtonPwewitt();
	//void maded_prewitt(const Mat& image, Mat& result, int thresh);
	//afx_msg void OnBnClickedButtonHoughline();
	afx_msg void OnBnClickedButtonBlur();
	afx_msg void OnBnClickedButtonImagesave();
//	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CStatic m_picture;
	afx_msg void OnBnClickedButtonCamera();
	CListBox m_List;
	int listcount=0;
	afx_msg void OnBnClickedButtonCamera2();
	afx_msg void OnBnClickedButtonEmbossing();
	afx_msg void OnBnClickedButtonUnsharp();
	afx_msg void OnBnClickedButtonBinary();
	afx_msg void OnBnClickedButtonHoughline();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	Mat hBitmapToMat(HBITMAP hBmp);
	HDC dc;
	CRotateAngleDlg dlg1;
	void BmpSobel();
	afx_msg void OnBnClickedButtonImageRotate();
	afx_msg void OnBnClickedButtonImageLabeling();
	afx_msg void OnBnClickedButtonImageFacedetect();
};
