// CImageOpenDlg.cpp: 구현 파일
//

#include "pch.h"
#include "MFCOpenCV.h"
#include "afxdialogex.h"
#include "CImageOpenDlg.h"
using namespace std;


// CImageOpenDlg 대화 상자

IMPLEMENT_DYNAMIC(CImageOpenDlg, CDialogEx)

CImageOpenDlg::CImageOpenDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_OPENCV_IMAGE, pParent)
{

}

CImageOpenDlg::~CImageOpenDlg()
{
}

void CImageOpenDlg::CreateBitmapInfo(int w, int h, int bpp)
{
	if (m_pBitmapInfo != NULL)
	{
		delete m_pBitmapInfo;
		m_pBitmapInfo = NULL;
	}

	if (bpp == 8)
		m_pBitmapInfo = (BITMAPINFO*) new BYTE[sizeof(BITMAPINFO) + 255 * sizeof(RGBQUAD)];
	else if(bpp == 24)// 24 or 32bit
		m_pBitmapInfo = (BITMAPINFO*) new BYTE[sizeof(BITMAPINFO)];

	m_pBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_pBitmapInfo->bmiHeader.biPlanes = 1;
	m_pBitmapInfo->bmiHeader.biBitCount = bpp;
	m_pBitmapInfo->bmiHeader.biCompression = BI_RGB;
	m_pBitmapInfo->bmiHeader.biSizeImage = 0;
	m_pBitmapInfo->bmiHeader.biXPelsPerMeter = 0;
	m_pBitmapInfo->bmiHeader.biYPelsPerMeter = 0;
	m_pBitmapInfo->bmiHeader.biClrUsed = 0;
	m_pBitmapInfo->bmiHeader.biClrImportant = 0;

	if (bpp == 8)
	{
		for (int i = 0; i < 256; i++)
		{
			m_pBitmapInfo->bmiColors[i].rgbBlue = (BYTE)i;
			m_pBitmapInfo->bmiColors[i].rgbGreen = (BYTE)i;
			m_pBitmapInfo->bmiColors[i].rgbRed = (BYTE)i;
			m_pBitmapInfo->bmiColors[i].rgbReserved = 0;
		}
	}

	m_pBitmapInfo->bmiHeader.biWidth = w;
	m_pBitmapInfo->bmiHeader.biHeight = -h;


	
}

void CImageOpenDlg::DrawImage(Mat img)
{
	CClientDC dc(GetDlgItem(IDC_PICTURE));
	CClientDC dc2(GetDlgItem(IDC_PICTURE_MINI));

	//CClientDC dc(GetDlgItem(IDC_PICTURE));
	CDialogEx::OnPaint();
	CDC dcTemp; dcTemp.CreateCompatibleDC(&dc);
	HBITMAP hbmp = ::CreateCompatibleBitmap(dc, 501, rect.Height() - 20); // 가로 10000 크기로 생성 
	HBITMAP hbmpOld = (HBITMAP)dcTemp.SelectObject(hbmp);


	POINT t;
	t.x = c.x + origin.x;
	t.y = c.y + origin.y;
	// SetStretchBltMode(dc.GetSafeHdc(), COLORONCOLOR);
	// StretchDIBits(dc.GetSafeHdc(), 0, 0, t.x*trans_count_w, t.y* trans_count_h, 0, 0,
	//	c_matImage.cols * count, c_matImage.rows * count, c_matImage.data, m_pBitmapInfo, DIB_RGB_COLORS, SRCCOPY);

	CRect rectHScroll;
	GetDlgItem(IDC_PICTURE)->GetClientRect(&rectHScroll);
	GetDlgItem(IDC_PICTURE)->GetClientRect(&rect);
	SCROLLINFO scrInfo; int iSrcX = 0; if (NULL == m_HScroll.GetSafeHwnd()) {

		rectHScroll.SetRect(rect.left, rect.top, rect.right, rect.bottom);
		m_HScroll.Create(WS_CHILD | WS_VISIBLE | SBS_HORZ | SBS_BOTTOMALIGN // 속성 
			, rectHScroll // 위치 
			, GetDlgItem(IDC_PICTURE) // 부모 윈도우 
			, 0 // 스크롤 막대의 컨트롤 ID입니다 
		);
		m_HScroll.ShowScrollBar(TRUE);
		scrInfo.cbSize = sizeof(scrInfo);
		scrInfo.fMask = SIF_ALL;
		scrInfo.nMin = 0; // 스크롤 최소값 
		scrInfo.nMax = 1; // 스크롤 최대값 
		scrInfo.nPage = rect.Width(); // 페이지 번호 
		scrInfo.nTrackPos = 0; // 드래깅 상태의 트랙바 위치 
		scrInfo.nPos = 0; // 트랙바 위치 
		m_HScroll.SetScrollRange(scrInfo.nMin, scrInfo.nMax); // 범위 설정
		m_HScroll.SetScrollPos(scrInfo.nPos); // 위치 설정
		m_HScroll.SetScrollInfo(&scrInfo); // 스크롤바 정보 설정 
	}
	else {
		if (FALSE != m_HScroll.GetScrollInfo(&scrInfo))
		{
			iSrcX = scrInfo.nPos; // 현재 스크롤 위치 받아옴 } 
		}
		dc.BitBlt(0, 0, 501, 501, &dc, iSrcX, 0, SRCCOPY); // 더블 버퍼링

	}
	int pos = m_HScroll.GetScrollPos();
	//GetDlgItem(IDC_PICTURE)->ScreenToClient(&rect);
	GetDlgItem(IDC_PICTURE)->GetClientRect(&rect);
	//GetDlgItem(IDC_PICTURE_MINI)->GetClientRect(&rect);

	SetStretchBltMode(dc.GetSafeHdc(), COLORONCOLOR);
	StretchDIBits(dc.GetSafeHdc(), 0, 0, rect.Width(), rect.Height() , pos, 0,
		(c_matImage.cols+pos)/count, c_matImage.rows, img.data, m_pBitmapInfo, DIB_RGB_COLORS, SRCCOPY);

	//SetDIBitsToDevice(dc.GetSafeHdc(), 0, 0, rect.Width(), rect.Height(), 0, 0, 0,
	//	c_matImage.rows / count, c_matImage.data, m_pBitmapInfo, DIB_RGB_COLORS);

	SetStretchBltMode(dc2.GetSafeHdc(), COLORONCOLOR);
	StretchDIBits(dc2.GetSafeHdc(), 0, 0, 99, 99, 0, 0,
		c_matImage.cols, c_matImage.rows, img.data, m_pBitmapInfo, DIB_RGB_COLORS, SRCCOPY);

	//StretchDIBits(dc.GetSafeHdc(), 0, 0, rect.Width(), rect.Height(), 0, 0,
		//img.cols/count, img.rows/count, img.data, m_pBitmapInfo, DIB_RGB_COLORS, SRCCOPY);

	//if(count>1)
	//InvalidateRect(rect,TRUE );
	
	//InvalidateRect(&rect);
	

}

void CImageOpenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TEXT_SIZE, m_TextSize);
	DDX_Control(pDX, IDC_TEXT_SIZE2, m_TextSize2);
	DDX_Control(pDX, IDC_TEXT_SIZE3, m_TextSize3);
	DDX_Control(pDX, IDC_SCROLLBAR_VERTICAL, m_VScroll);
	DDX_Control(pDX, IDC_SCROLLBAR_HORIZON, m_HScroll);
}


BEGIN_MESSAGE_MAP(CImageOpenDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_IMAGE, &CImageOpenDlg::OnBnClickedButtonImage)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_NCMOUSEMOVE()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON_IMAGE_ENLARGEMENT, &CImageOpenDlg::OnBnClickedButtonImageEnlargement)
	ON_BN_CLICKED(IDC_BUTTON_IMAGE_REDUCTION, &CImageOpenDlg::OnBnClickedButtonImageReduction)
	ON_BN_CLICKED(IDC_BUTTON_IMAGE_ORIGINAL, &CImageOpenDlg::OnBnClickedButtonImageOriginal)
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON_SOBEL, &CImageOpenDlg::OnBnClickedButtonSobel)
	ON_BN_CLICKED(IDC_BUTTON_CANNYEDGE, &CImageOpenDlg::OnBnClickedButtonCannyedge)
	ON_BN_CLICKED(IDC_BUTTON_CIRCLEDETECT, &CImageOpenDlg::OnBnClickedButtonCircledetect)
	ON_BN_CLICKED(IDC_BUTTON_PWEWITT, &CImageOpenDlg::OnBnClickedButtonPwewitt)
	ON_BN_CLICKED(IDC_BUTTON_HOUGHLINE, &CImageOpenDlg::OnBnClickedButtonHoughline)
	ON_BN_CLICKED(IDC_BUTTON_BLUR, &CImageOpenDlg::OnBnClickedButtonBlur)
	ON_BN_CLICKED(IDC_BUTTON_IMAGESAVE, &CImageOpenDlg::OnBnClickedButtonImagesave)
END_MESSAGE_MAP()


// CImageOpenDlg 메시지 처리기


void CImageOpenDlg::OnBnClickedButtonImage()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_READONLY, _T("image file(*.jpg;)|*.jpg;|All Files(*.*)|*.*||"));
	if (fileDlg.DoModal() == IDOK)
	{
		CString path = fileDlg.GetPathName();

		CT2CA pszString(path);
		std::string strPath(pszString);

		m_matImage = imread(strPath, IMREAD_GRAYSCALE);

		c_matImage = m_matImage; // 객체에 이미지 복사

		CreateBitmapInfo(m_matImage.cols, m_matImage.rows, m_matImage.channels() * 8);

		DrawImage(m_matImage);
		Invalidate(false); // TRUE, FALSE는 알아서.
	}
	// 영상 출력 Picture Control 크기
	CStatic* staticSize = (CStatic*)GetDlgItem(IDC_PICTURE);
	CRect rect;
	staticSize->GetClientRect(rect);

	// Picture Control 너비 높이 받아오기
	int iWidth = rect.Width();
	int iHeight = rect.Height();

	// Picture Control 크기를 Static Text로 출력하기
	CString strPoint;
	strPoint.Format(L"%04d, %04d", iWidth, iHeight);
	m_TextSize.SetWindowTextW(strPoint);
	
	// 원본 사진 크기를 Static Text로 출력하기 
	int iWidth2 = m_matImage.cols;
	int iHeight2 = m_matImage.rows;
	m_matImage.channels();
	CString strPoint2;
	strPoint2.Format(L"%04d, %04d", iWidth2, iHeight2);
	m_TextSize2.SetWindowTextW(strPoint2);

	trans_count_w = static_cast<float>(501) / m_matImage.cols;	// 이미지 가로 비율 
	trans_count_h = static_cast<float>(501) / m_matImage.rows;  // 이미지 세로 비율

}
void CImageOpenDlg::OnBnClickedButtonImagesave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	

	CString szFilter = _T("image file(*.jpg;*.bmp;*.png;)|*.jpg;*.bmp;*.png;|All Files(*.*)|*.*||");

	CFileDialog dlg(FALSE,
		_T("*.*"),
		_T("*.*"),
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		(LPCTSTR)szFilter.GetBuffer(), //(LPCTSTR)(szFilter),
		this
	);
	CString path = dlg.GetPathName();


	dlg.m_ofn.lpstrTitle = _T("Image Save As");
	dlg.m_ofn.lpstrInitialDir = _T("");  //기본 경로 설정

	if (dlg.DoModal() == IDOK)
	{

		CString check = dlg.GetFileExt();  // 파일형식 얻기

		if (check == "bmp" || check == "jpg")

		{

			path = dlg.GetPathName();    // 파일 전체 경로 얻기

			CT2CA pszString(path);
			std::string strPath(pszString);
			imwrite(strPath, c_matImage);  // 이미지 저장


		}

		else AfxMessageBox(_T("확장자를 올바르게 선택하여 주십시오."));
	}
}


void CImageOpenDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		
		GetClientRect(&rect);
		int x = (rect.Width()  - cxIcon + 1) / 2;
		int y = (rect.Height()  - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
		
	}
	else
	{
		

		
	}
}


void CImageOpenDlg::OnDestroy()
{
	CDialogEx::OnDestroy();


	if (m_pBitmapInfo != NULL)
	{
		delete m_pBitmapInfo;
		m_pBitmapInfo = NULL;
	}
}


BOOL CImageOpenDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	CRect VScroll;
	CRect HScroll;

	m_VScroll.GetWindowRect(&VScroll); // 수직 스크롤바 위치 가져오기
	m_HScroll.GetWindowRect(&HScroll); // 수평 스크롤바 위치 가져오기

	m_VScroll.SetScrollRange(0, VScroll.Height()); // 수직 스크롤바 범위 설정
	m_HScroll.SetScrollRange(0, HScroll.Width());  // 수평 스크롤바 범위 설정

	c.x = 0;
	c.y = 0;

	origin.x = HScroll.left;
	origin.y = VScroll.top;


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CImageOpenDlg::OnNcMouseMove(UINT nHitTest, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
}


void CImageOpenDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CDialogEx::OnMouseMove(nFlags, point);
	ClientToScreen(&point);
	GetDlgItem(IDC_PICTURE)->ScreenToClient(&point);
	if (point.x > 501)
	{
		point.x = 501;
		
	}
	if (point.y > 501)
	{
		point.y = 501;
	}
	if (point.x < 0)
	{
		point.x = 0;
	}
	if (point.y < 0)
	{
		point.y = 0;
	}
	//CString strPoint5;
	//strPoint5.Format(L"%04d, %04d", point.x, point.y);
	//m_TextSize3.SetWindowTextW(strPoint5);

	CString strPoint6;
	strPoint6.Format(L"%04.1f, %04.1f", point.x/ trans_count_w, point.y/ trans_count_h);
	m_TextSize3.SetWindowTextW(strPoint6);
	

}


void CImageOpenDlg::OnBnClickedButtonImageEnlargement()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	

	count = count * 2;
	
	//CPoint point;
	Mat dst;
	//resize(m_matImage, dst1, Size(), 4, 4, INTER_NEAREST);
	resize(c_matImage, dst, Size(c_matImage.cols*count,c_matImage.rows*count), 0, 0, INTER_CUBIC);
	
	
	//imshow("dst1", dst2(Rect(point.x,point.y, m_matImage.cols/4,m_matImage.rows/4)));
	//waitKey();
	
	CreateBitmapInfo(dst.cols, dst.rows,dst.channels() * 8);

	c_matImage = dst;
	DrawImage(c_matImage);
	//DrawImage();
}


void CImageOpenDlg::OnBnClickedButtonImageReduction()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	Mat dst;

	count = count * 0.5;

	//resize(m_matImage, dst1, Size(), 4, 4, INTER_NEAREST);
	resize(c_matImage, dst, Size(c_matImage.cols*count , c_matImage.rows*count ), 0,0, INTER_CUBIC);
	CreateBitmapInfo(dst.cols, dst.rows, dst.channels() * 8);
	c_matImage = dst;
	DrawImage(c_matImage);
	//DrawImage();
}


void CImageOpenDlg::OnBnClickedButtonImageOriginal()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	count = 1;
	resize(m_matImage, c_matImage, Size(m_matImage.cols / count, m_matImage.rows / count), 0, 0, INTER_CUBIC);
	CreateBitmapInfo(c_matImage.cols, c_matImage.rows, c_matImage.channels() * 8);
	DrawImage(c_matImage);
	//DrawImage();
}


void CImageOpenDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	//m_VScroll.SetScrollRange(0, 572);
	//m_VScroll.SetScrollPos(572);

	if (pScrollBar->GetSafeHwnd() == m_VScroll.GetSafeHwnd()) {
		int pos;
		pos = m_VScroll.GetScrollPos();

		if (nSBCode == SB_LINEDOWN) m_VScroll.SetScrollPos(pos + 10);
		else if (nSBCode == SB_LINEUP) m_VScroll.SetScrollPos(pos - 10);
		else if (nSBCode == SB_PAGEUP) m_VScroll.SetScrollPos(pos - 50);
		else if (nSBCode == SB_PAGEDOWN) m_VScroll.SetScrollPos(pos + 50);
		else if (nSBCode == SB_THUMBTRACK) m_VScroll.SetScrollPos(nPos);

		c.y = m_VScroll.GetScrollPos();
		OnPaint();
		CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
	}



}


void CImageOpenDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//m_HScroll.SetScrollRange(0, 651);
	//m_HScroll.SetScrollPos(0);

	if (pScrollBar->GetSafeHwnd() == m_HScroll.GetSafeHwnd()) {
		// Scroll Bar ID 확인 (복수개의 Scroll Bar 가 있을수 있기 때문)
		int pos;
		pos = m_HScroll.GetScrollPos();   // 현재 위치 가져옴

		if (nSBCode == SB_LINEDOWN) m_HScroll.SetScrollPos(pos + 10);    // Line 단위 이동 event
		else if (nSBCode == SB_LINEUP) m_HScroll.SetScrollPos(pos - 10);
		else if (nSBCode == SB_PAGEUP) m_HScroll.SetScrollPos(pos - 50);    // Page 단위 이동 event
		else if (nSBCode == SB_PAGEDOWN) m_HScroll.SetScrollPos(pos + 50);
		else if (nSBCode == SB_THUMBTRACK) m_HScroll.SetScrollPos(nPos);   // 스크롤 헤더를 마우스로 끌때 event

		c.x = m_HScroll.GetScrollPos();  // 현재 위치를 Point 에 저장
		DrawImage(c_matImage);   // 윈도우 다시 그리기

	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);

}



BOOL CImageOpenDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return TRUE;
}


void CImageOpenDlg::OnBnClickedButtonSobel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Mat dx, dy;
	Sobel(c_matImage, dx, CV_32FC1, 1, 0);
	Sobel(c_matImage, dy, CV_32FC1, 0, 1);

	Mat fmag,mag;
	magnitude(dx, dy, fmag);
	fmag.convertTo(mag, CV_8UC1);

	c_matImage = mag ;


	DrawImage(c_matImage);
}


void CImageOpenDlg::OnBnClickedButtonCannyedge()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Mat dst1;
	Canny(c_matImage, dst1, 50, 100);

	c_matImage = dst1;

	DrawImage(c_matImage);
}


void CImageOpenDlg::OnBnClickedButtonCircledetect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Mat blurred;
	blur(c_matImage, blurred, Size(3, 3));

	std::vector<Vec3f> circles;
	HoughCircles(blurred, circles, HOUGH_GRADIENT, 1, 50, 150, 30);

	Mat dst;
	cvtColor(c_matImage, dst, COLOR_GRAY2BGR);

	for (Vec3f c : circles) {
		Point center(cvRound(c[0]), cvRound(c[1]));
		int radius = cvRound(c[2]);
		circle(dst, center, radius, Scalar(0, 0, 255), 2, LINE_AA);
	}
	//DrawImage(dst);
	imshow("dst", dst);
	waitKey();
}


void CImageOpenDlg::OnBnClickedButtonPwewitt()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	Mat prewitt;

	//maded_prewitt(m_matImage, prewitt, 100);
	//maded_prewitt(m_matImage, c_matImage, 100);

	

	
	DrawImage(c_matImage);
	//imshow("프리윗마스크",prewitt);
	//waitKey();
}

void CImageOpenDlg::maded_prewitt(const Mat& image, Mat& result, int thresh)
{
	// 수직마스크
	Mat maskX = (Mat_<double>(3, 3) << -1, 0, 1, -1, 0, 1, -1, 0, 1);
	// 수평마스크
	Mat maskY = (Mat_<double>(3, 3) << 1, 1, 1, 0, 0, 0, -1, -1, -1);

	int filterOffset = 3 / 2;

	result = Mat::zeros(image.rows - filterOffset * 2, image.cols - filterOffset * 2, image.type());

	double sumEdgeX;
	double sumEdgeY;
	double magnitude;

	for (int yimage = filterOffset; yimage < image.rows - filterOffset; ++yimage) {
		for (int ximage = filterOffset; ximage < image.cols - filterOffset; ++ximage) {

			sumEdgeX = 0;
			sumEdgeY = 0;
			for (int ymask = -filterOffset; ymask <= filterOffset; ++ymask) {
				for (int xmask = -filterOffset; xmask <= filterOffset; ++xmask) {
					sumEdgeX += image.at<uchar>(yimage + ymask, ximage + xmask) * maskX.at<double>(filterOffset + ymask, filterOffset + xmask);
					sumEdgeY += image.at<uchar>(yimage + ymask, ximage + xmask) * maskY.at<double>(filterOffset + ymask, filterOffset + xmask);
				}
			}
			magnitude = sqrt(pow(sumEdgeY, 2) + pow(sumEdgeX, 2));
			result.at<uchar>(yimage - filterOffset, ximage - filterOffset) = ((magnitude > thresh) ? 255 : 0);
		}
	}
}


void CImageOpenDlg::OnBnClickedButtonHoughline()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Mat edge;
	Canny(c_matImage, edge, 50, 150);

	vector<Vec2f> lines;
	HoughLines(edge, lines, 1, CV_PI / 180, 250);

	Mat dst;
	cvtColor(edge, dst, COLOR_GRAY2BGR);

	for (size_t i = 0; i < lines.size(); i++)
	{
		float r = lines[i][0], t = lines[i][1];
		double cos_t = cos(t), sin_t = sin(t);
		double x0 = r * cos_t, y0 = r * sin_t;
		double alpha = 1000;

		Point pt1(cvRound(x0 + alpha * (-sin_t)), cvRound(y0 + alpha * cos_t));
		Point pt2(cvRound(x0 - alpha * (-sin_t)), cvRound(y0 - alpha * cos_t));
		line(dst, pt1, pt2, Scalar(0, 0, 255), 2, LINE_AA);
	}
	c_matImage = dst;
	DrawImage(c_matImage);
	//imshow("허프직선", dst);
	//waitKey();
}


void CImageOpenDlg::OnBnClickedButtonBlur()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Mat blur;
	GaussianBlur(c_matImage, blur, Size(3, 3), 0);

	c_matImage = blur;

	DrawImage(c_matImage);
}


