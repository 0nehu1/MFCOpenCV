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
	//CDialogEx::OnPaint();
	//CDC dcTemp; dcTemp.CreateCompatibleDC(&dc);
	//HBITMAP hbmp = ::CreateCompatibleBitmap(dc, 501, rect.Height() - 20); // 가로 10000 크기로 생성 
	//HBITMAP hbmpOld = (HBITMAP)dcTemp.SelectObject(hbmp);


	POINT t;
	t.x = c.x + origin.x;
	t.y = c.y + origin.y;
	// SetStretchBltMode(dc.GetSafeHdc(), COLORONCOLOR);
	// StretchDIBits(dc.GetSafeHdc(), 0, 0, t.x*trans_count_w, t.y* trans_count_h, 0, 0,
	//	c_matImage.cols * count, c_matImage.rows * count, c_matImage.data, m_pBitmapInfo, DIB_RGB_COLORS, SRCCOPY);

	CRect rectHScroll;
	CRect rectVScroll;
	GetDlgItem(IDC_PICTURE)->GetClientRect(&rectHScroll);
	GetDlgItem(IDC_PICTURE)->GetClientRect(&rect);
	GetDlgItem(IDC_PICTURE)->GetClientRect(&rectVScroll);

	SCROLLINFO VscrInfo{};
	SCROLLINFO HscrInfo{};
	int iSrcX = 0;
	int iSrcY = 0;
	if (NULL == m_HScroll.GetSafeHwnd() && NULL == m_VScroll.GetSafeHwnd()) {

		rectHScroll.SetRect(rect.left, rect.top, rect.right, rect.bottom);
		m_HScroll.Create(WS_CHILD | WS_VISIBLE | SBS_HORZ | SBS_BOTTOMALIGN // 속성 
			, rectHScroll // 위치 
			, GetDlgItem(IDC_PICTURE) // 부모 윈도우 
			, 0 // 스크롤 막대의 컨트롤 ID입니다 
		);
		m_HScroll.ShowScrollBar(TRUE);
		HscrInfo.cbSize = sizeof(HscrInfo);
		HscrInfo.fMask = SIF_ALL;
		HscrInfo.nMin = 0; // 스크롤 최소값 
		HscrInfo.nMax = 501; // 스크롤 최대값 
		HscrInfo.nPage = rect.Width(); // 페이지 번호 
		HscrInfo.nTrackPos = 0; // 드래깅 상태의 트랙바 위치 
		HscrInfo.nPos = 0; // 트랙바 위치 
		m_HScroll.SetScrollRange(HscrInfo.nMin, HscrInfo.nMax); // 범위 설정
		m_HScroll.SetScrollPos(HscrInfo.nPos); // 위치 설정
		m_HScroll.SetScrollInfo(&HscrInfo); // 스크롤바 정보 설정 


		rectVScroll.SetRect(rect.left, rect.top, rect.right, rect.bottom);
		m_VScroll.Create(WS_CHILD | WS_VISIBLE | SBS_VERT | SBS_RIGHTALIGN // 속성 
			, rectVScroll // 위치 
			, GetDlgItem(IDC_PICTURE) // 부모 윈도우 
			, 1 // 스크롤 막대의 컨트롤 ID입니다 
		);
	
		m_VScroll.ShowScrollBar(TRUE); 
		VscrInfo.cbSize = sizeof(VscrInfo);
		VscrInfo.fMask = SIF_ALL;
		VscrInfo.nMin = 0; // 스크롤 최소값 
		VscrInfo.nMax = 501; // 스크롤 최대값 
		VscrInfo.nPage = rect.Height(); // 페이지 번호 
		VscrInfo.nTrackPos = 0; // 드래깅 상태의 트랙바 위치 
		VscrInfo.nPos = 0; // 트랙바 위치 
		m_VScroll.SetScrollRange(VscrInfo.nMin, VscrInfo.nMax); // 범위 설정
		m_VScroll.SetScrollPos(VscrInfo.nPos); // 위치 설정
		
		m_VScroll.SetScrollInfo(&VscrInfo); // 스크롤바 정보 설정 
	}
	else {
		if (FALSE != m_HScroll.GetScrollInfo(&HscrInfo) && FALSE != m_VScroll.GetScrollInfo(&VscrInfo))
		{
			iSrcX = HscrInfo.nPos; // 현재 스크롤 위치 받아옴 
			iSrcY = VscrInfo.nPos;
		}
		//dc.BitBlt(0, 0, 501, 501, &dc, iSrcX, iSrcY, SRCCOPY); // 더블 버퍼링

	}

	//int Hpos = m_HScroll.GetScrollPos();
	//int Vpos = m_VScroll.GetScrollPos();

	 Hpos = m_HScroll.GetScrollPos();
	 Vpos = m_VScroll.GetScrollPos();

	//GetDlgItem(IDC_PICTURE)->ScreenToClient(&rect);
	GetDlgItem(IDC_PICTURE)->GetClientRect(&rect);
	//GetDlgItem(IDC_PICTURE_MINI)->GetClientRect(&rect);
	
		//  스크롤 가로세로출력
	CString strPoint2;
	strPoint2.Format(L"%04d, %04d", Hpos, Vpos);
	m_TextSize2.SetWindowTextW(strPoint2);

		SetStretchBltMode(dc.GetSafeHdc(), COLORONCOLOR);
		StretchDIBits(dc.GetSafeHdc(), 0, 0, rect.Width(), rect.Height(), Hpos / trans_count_w, Vpos / trans_count_h,
			c_matImage.cols / count + Hpos*trans_count_w, c_matImage.rows /count + Vpos  * trans_count_h,
			img.data, m_pBitmapInfo, DIB_RGB_COLORS, SRCCOPY);
	
		CString strPoint;
		strPoint.Format(L"%04f, %04f", Hpos * count / trans_count_w, Vpos * count / trans_count_h);
		m_TextSize.SetWindowTextW(strPoint);

	//SetDIBitsToDevice(dc.GetSafeHdc(), 0, 0, rect.Width(), rect.Height(), 0, 0, 0,
	//	c_matImage.rows / count, c_matImage.data, m_pBitmapInfo, DIB_RGB_COLORS);
		 
	SetStretchBltMode(dc2.GetSafeHdc(), COLORONCOLOR);
	StretchDIBits(dc2.GetSafeHdc(), 0, 0, 99, 99, 0, 0,
		c_matImage.cols, c_matImage.rows, img.data, m_pBitmapInfo, DIB_RGB_COLORS, SRCCOPY);

	//CClientDC dc(this);

	CPen pen;
	CBrush brush;
	brush.CreateStockObject(NULL_BRUSH);
	CBrush* pOldBrush = dc2.SelectObject(&brush);
	pen.CreatePen(PS_DEFAULT, 3, RGB(255, 0 ,0));    // 펜을 생성
	CPen* oldPen = dc2.SelectObject(&pen);
	if (count > 1)
	{
		//dc2.SetBkMode(TRANSPARENT);
		dc2.Rectangle((0+Hpos)/ count * trans_count_w, (501-Vpos)/ count * trans_count_w, 99 / count+Hpos / count * trans_count_w, 99 / count + (501-Vpos) / count * trans_count_w);
		dc2.SelectObject(&pOldBrush);
		brush.DeleteObject();
		pen.DeleteObject();
	}
	
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
	DDX_Control(pDX, IDC_PICTURE, m_picture);
	DDX_Control(pDX, IDC_LIST1, m_List);
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
	//ON_BN_CLICKED(IDC_BUTTON_CIRCLEDETECT, &CImageOpenDlg::OnBnClickedButtonCircledetect)
	//ON_BN_CLICKED(IDC_BUTTON_PWEWITT, &CImageOpenDlg::OnBnClickedButtonPwewitt)
	//ON_BN_CLICKED(IDC_BUTTON_HOUGHLINE, &CImageOpenDlg::OnBnClickedButtonHoughline)
	ON_BN_CLICKED(IDC_BUTTON_BLUR, &CImageOpenDlg::OnBnClickedButtonBlur)
	ON_BN_CLICKED(IDC_BUTTON_IMAGESAVE, &CImageOpenDlg::OnBnClickedButtonImagesave)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_CAMERA, &CImageOpenDlg::OnBnClickedButtonCamera)
	ON_BN_CLICKED(IDC_BUTTON_CAMERA2, &CImageOpenDlg::OnBnClickedButtonCamera2)
	ON_BN_CLICKED(IDC_BUTTON_EMBOSSING, &CImageOpenDlg::OnBnClickedButtonEmbossing)
	ON_BN_CLICKED(IDC_BUTTON_UNSHARP, &CImageOpenDlg::OnBnClickedButtonUnsharp)
	ON_BN_CLICKED(IDC_BUTTON_BINARY, &CImageOpenDlg::OnBnClickedButtonBinary)
	ON_BN_CLICKED(IDC_BUTTON_HOUGHLINE, &CImageOpenDlg::OnBnClickedButtonHoughline)
END_MESSAGE_MAP()


// CImageOpenDlg 메시지 처리기


void CImageOpenDlg::OnBnClickedButtonImage()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CWnd* pWnd1 = (CWnd*)GetDlgItem(IDC_BUTTON_IMAGE_ENLARGEMENT);
	CWnd* pWnd2 = (CWnd*)GetDlgItem(IDC_BUTTON_IMAGE_REDUCTION);
	CWnd* pWnd3 = (CWnd*)GetDlgItem(IDC_BUTTON_IMAGE_ORIGINAL);
	CWnd* pWnd4 = (CWnd*)GetDlgItem(IDC_BUTTON_SOBEL);
	CWnd* pWnd5 = (CWnd*)GetDlgItem(IDC_BUTTON_CANNYEDGE);
	CWnd* pWnd6 = (CWnd*)GetDlgItem(IDC_BUTTON_BLUR);
	CWnd* pWnd7 = (CWnd*)GetDlgItem(IDC_BUTTON_EMBOSSING);
	CWnd* pWnd8 = (CWnd*)GetDlgItem(IDC_BUTTON_UNSHARP);
	CWnd* pWnd9 = (CWnd*)GetDlgItem(IDC_BUTTON_BINARY);
	pWnd1->EnableWindow(TRUE);
	pWnd2->EnableWindow(TRUE);
	pWnd3->EnableWindow(TRUE);
	pWnd4->EnableWindow(TRUE);
	pWnd5->EnableWindow(TRUE);
	pWnd6->EnableWindow(TRUE);
	pWnd7->EnableWindow(TRUE);
	pWnd8->EnableWindow(TRUE);
	pWnd9->EnableWindow(TRUE);

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

	

	CString szFilter = _T("image file(*.jpg;)|*.jpg;");

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


	capture = new VideoCapture(0);

	if (!capture->isOpened())
	{

		MessageBox(_T("웹캠을 열수 없습니다. \n"));

	}

	//웹캠 크기를  320x240으로 지정    
	capture->set(CAP_PROP_FRAME_WIDTH, 501);
	capture->set(CAP_PROP_FRAME_HEIGHT, 501);

	SetTimer(1000, 30, NULL);

	CWnd* pWnd1 = (CWnd*)GetDlgItem(IDC_BUTTON_IMAGE_ENLARGEMENT);
	CWnd* pWnd2 = (CWnd*)GetDlgItem(IDC_BUTTON_IMAGE_REDUCTION);
	CWnd* pWnd3 = (CWnd*)GetDlgItem(IDC_BUTTON_IMAGE_ORIGINAL);
	CWnd* pWnd4 = (CWnd*)GetDlgItem(IDC_BUTTON_SOBEL);
	CWnd* pWnd5 = (CWnd*)GetDlgItem(IDC_BUTTON_CANNYEDGE);
	CWnd* pWnd6 = (CWnd*)GetDlgItem(IDC_BUTTON_BLUR);
	CWnd* pWnd7 = (CWnd*)GetDlgItem(IDC_BUTTON_EMBOSSING);
	CWnd * pWnd8 = (CWnd*)GetDlgItem(IDC_BUTTON_UNSHARP);
	CWnd* pWnd9 = (CWnd*)GetDlgItem(IDC_BUTTON_BINARY);
	pWnd1->EnableWindow(FALSE);
	pWnd2->EnableWindow(FALSE);
	pWnd3->EnableWindow(FALSE);
	pWnd4->EnableWindow(FALSE);
	pWnd5->EnableWindow(FALSE);
	pWnd6->EnableWindow(FALSE);
	pWnd7->EnableWindow(FALSE);
	pWnd8->EnableWindow(FALSE);
	pWnd9->EnableWindow(FALSE);
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
	//GetDlgItem(IDC_BUTTON_IMAGE_ENLARGEMENT)->ShowWindow(SW_HIDE);
	//GetDlgItem(IDC_BUTTON_IMAGE)->EnableWindow(TRUE);
	//GetDlgItem(IDC_BUTTON_CAMERA)->EnableWindow(TRUE);
	
	if (count < 8)
		count = count * 2;
	else
		count = 8;
	//CPoint point;
	Mat dst;
	//resize(m_matImage, dst1, Size(), 4, 4, INTER_NEAREST);
	resize(m_matImage, dst, Size(m_matImage.cols*count,m_matImage.rows*count), 0, 0, INTER_CUBIC);
	
	
	//imshow("dst1", dst2(Rect(point.x,point.y, m_matImage.cols/4,m_matImage.rows/4)));
	//waitKey();
	
	CreateBitmapInfo(dst.cols, dst.rows,dst.channels() * 8);

	c_matImage = dst;
	DrawImage(dst);
	//DrawImage();
}


void CImageOpenDlg::OnBnClickedButtonImageReduction()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CClientDC dc(this);
	Mat dst;
	CClientDC dc(this);

	CPen pen;
	pen.CreatePen(PS_DOT, 0, RGB(255, 255, 255));    // 펜을 생성
	CPen* oldPen = dc.SelectObject(&pen);
	dc.Rectangle(0, 0, 551, 551);
	
	count = count * 0.5;

	//resize(m_matImage, dst1, Size(), 4, 4, INTER_NEAREST);
	resize(m_matImage, dst, Size(m_matImage.cols*count , m_matImage.rows*count ), 0,0, INTER_CUBIC);
	CreateBitmapInfo(dst.cols, dst.rows, dst.channels() * 8);
	c_matImage = dst;
	DrawImage(dst);
	//DrawImage();
	dc.SelectObject(oldPen);
	pen.DeleteObject();
}


void CImageOpenDlg::OnBnClickedButtonImageOriginal()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	count = 1;
	resize(m_matImage, c_matImage, Size(m_matImage.cols / count, m_matImage.rows / count), 0, 0, INTER_CUBIC);
	CreateBitmapInfo(c_matImage.cols, c_matImage.rows, c_matImage.channels() * 8);
	CString str;
	str.Format(_T(""));
	LPCTSTR lpszTemp = str;
	while(1)
	{
	//m_List.InsertString(listcount-1,str);
	m_List.DeleteString(listcount);
	m_List.InsertString(listcount, str);
	if (listcount < 1)
		break;
	listcount--;
	}
	//UpdateData();
	//

	DrawImage(c_matImage);
	//DrawImage();
}


void CImageOpenDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	//m_VScroll.SetScrollRange(0, 572);
	//m_VScroll.SetScrollPos(572);
	
	if (count > 1)
	{
		if (pScrollBar->GetSafeHwnd() == m_VScroll.GetSafeHwnd()) {
			int pos;
			//double rate = 1800 / 553;
			pos = m_VScroll.GetScrollPos()*count;

			if (nSBCode == SB_LINEDOWN) 
				m_VScroll.SetScrollPos(pos + 10);
			else if (nSBCode == SB_LINEUP)
				m_VScroll.SetScrollPos(pos - 10);
			else if (nSBCode == SB_PAGEUP) 
				m_VScroll.SetScrollPos(pos - 50);
			else if (nSBCode == SB_PAGEDOWN) 
				m_VScroll.SetScrollPos(pos + 50);
			else if (nSBCode == SB_THUMBTRACK) 
				m_VScroll.SetScrollPos(nPos);

			c.y = m_VScroll.GetScrollPos();
			DrawImage(c_matImage);   // 윈도우 다시 그리기

			
		}
		CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
	}


}


void CImageOpenDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//m_HScroll.SetScrollRange(0, 651);
	//m_HScroll.SetScrollPos(0);
	if(count>1)
	{
	if (pScrollBar->GetSafeHwnd() == m_HScroll.GetSafeHwnd()) {
		// Scroll Bar ID 확인 (복수개의 Scroll Bar 가 있을수 있기 때문)
		double pos;
		//double rate = 2592 / 620;
		
		pos = m_HScroll.GetScrollPos();   // 현재 위치 가져옴


		if (nSBCode == SB_LINEDOWN) 
			m_HScroll.SetScrollPos(pos + 10);    // Line 단위 이동 event
		else if (nSBCode == SB_LINEUP) 
			m_HScroll.SetScrollPos(pos - 10);
		else if (nSBCode == SB_PAGEUP) 
			m_HScroll.SetScrollPos(pos - 50);    // Page 단위 이동 event
		else if (nSBCode == SB_PAGEDOWN) 
			m_HScroll.SetScrollPos(pos + 50);
		else if (nSBCode == SB_THUMBTRACK) m_HScroll.SetScrollPos(nPos);   // 스크롤 헤더를 마우스로 끌때 event


		c.x = m_HScroll.GetScrollPos();  // 현재 위치를 Point 에 저장
		DrawImage(c_matImage);   // 윈도우 다시 그리기

	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
	}
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

	UpdateData();
	m_List.InsertString(listcount, _T("Sobel"));

	listcount++;
	DrawImage(c_matImage);
}


void CImageOpenDlg::OnBnClickedButtonCannyedge()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Mat dst1;
	Canny(c_matImage, dst1, 50, 100);

	c_matImage = dst1;
	UpdateData();
	m_List.InsertString(listcount, _T("canny"));
	listcount++;
	DrawImage(c_matImage);

}











void CImageOpenDlg::OnBnClickedButtonBlur()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Mat blur;
	// 가우시안 필터
	// GaussianBlur(c_matImage, blur, Size(3, 3), 0);
	// 평균값 필터
	cv::blur(c_matImage, blur, Size(3, 3));
	c_matImage = blur;

	DrawImage(c_matImage);
}


void CImageOpenDlg::OnDestroy()
{
	CDialogEx::OnDestroy();


	if (m_pBitmapInfo != NULL)
	{
		delete m_pBitmapInfo;
		m_pBitmapInfo = NULL;
	}

	//capCaptureStop();
	//capDriverDisconnect(hWindow);
}

void CImageOpenDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	capture->read(mat_frame);


	//이곳에 OpenCV 함수들을 적용합니다.
	//여기에서는 그레이스케일 이미지로 변환합니다.
	//cvtColor(mat_frame, mat_frame, COLOR_BGR2GRAY);



	//화면에 보여주기 위한 처리입니다.
	int bpp = 8 * mat_frame.elemSize();
	assert((bpp == 8 || bpp == 24 || bpp == 32));

	int padding = 0;
	//32 bit image is always DWORD aligned because each pixel requires 4 bytes
	if (bpp < 32)
		padding = 4 - (mat_frame.cols % 4);

	if (padding == 4)
		padding = 0;

	int border = 0;
	//32 bit image is always DWORD aligned because each pixel requires 4 bytes
	if (bpp < 32)
	{
		border = 4 - (mat_frame.cols % 4);
	}



	Mat mat_temp;
	if (border > 0 || mat_frame.isContinuous() == false)
	{
		// Adding needed columns on the right (max 3 px)
		cv::copyMakeBorder(mat_frame, mat_temp, 0, 0, 0, border, cv::BORDER_CONSTANT, 0);
	}
	else
	{
		mat_temp = mat_frame;
	}


	//RECT r;

	m_picture.GetClientRect(&rect);
	cv::Size winSize(rect.right, rect.bottom);

	cimage_mfc.Create(winSize.width, winSize.height, 24);


	BITMAPINFO* bitInfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFO) + 256 * sizeof(RGBQUAD));
	bitInfo->bmiHeader.biBitCount = bpp;
	bitInfo->bmiHeader.biWidth = mat_temp.cols;
	bitInfo->bmiHeader.biHeight = -mat_temp.rows;
	bitInfo->bmiHeader.biPlanes = 1;
	bitInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitInfo->bmiHeader.biCompression = BI_RGB;
	bitInfo->bmiHeader.biClrImportant = 0;
	bitInfo->bmiHeader.biClrUsed = 0;
	bitInfo->bmiHeader.biSizeImage = 0;
	bitInfo->bmiHeader.biXPelsPerMeter = 0;
	bitInfo->bmiHeader.biYPelsPerMeter = 0;


	//그레이스케일 인경우 팔레트가 필요
	if (bpp == 8)
	{
		RGBQUAD* palette = bitInfo->bmiColors;
		for (int i = 0; i < 256; i++)
		{
			palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed = (BYTE)i;
			palette[i].rgbReserved = 0;
		}
	}


	// Image is bigger or smaller than into destination rectangle
	// we use stretch in full rect

	if (mat_temp.cols == winSize.width && mat_temp.rows == winSize.height)
	{
		// source and destination have same size
		// transfer memory block
		// NOTE: the padding border will be shown here. Anyway it will be max 3px width

		SetDIBitsToDevice(cimage_mfc.GetDC(),
			//destination rectangle
			0, 0, winSize.width, winSize.height,
			0, 0, 0, mat_temp.rows,
			mat_temp.data, bitInfo, DIB_RGB_COLORS);
	}
	else
	{
		// destination rectangle
		int destx = 0, desty = 0;
		int destw = winSize.width;
		int desth = winSize.height;

		// rectangle defined on source bitmap
		// using imgWidth instead of mat_temp.cols will ignore the padding border
		int imgx = 0, imgy = 0;
		int imgWidth = mat_temp.cols - border;
		int imgHeight = mat_temp.rows;

		StretchDIBits(cimage_mfc.GetDC(),
			destx, desty, destw, desth,
			imgx, imgy, imgWidth, imgHeight,
			mat_temp.data, bitInfo, DIB_RGB_COLORS, SRCCOPY);
	}

	m_matImage = mat_temp;
	c_matImage = mat_temp;

	HDC dc = ::GetDC(m_picture.m_hWnd);
	cimage_mfc.BitBlt(dc, 0, 0);


	::ReleaseDC(m_picture.m_hWnd, dc);

	cimage_mfc.ReleaseDC();
	cimage_mfc.Destroy();
	count = 1;
}


void CImageOpenDlg::OnBnClickedButtonCamera()
{
	capture->read(mat_frame);


	//이곳에 OpenCV 함수들을 적용합니다.
	//여기에서는 그레이스케일 이미지로 변환합니다.
	cvtColor(mat_frame, mat_frame, COLOR_BGR2GRAY);



	//화면에 보여주기 위한 처리입니다.
	int bpp = 8 * mat_frame.elemSize();
	assert((bpp == 8 || bpp == 24 || bpp == 32));

	int padding = 0;
	//32 bit image is always DWORD aligned because each pixel requires 4 bytes
	if (bpp < 32)
		padding = 4 - (mat_frame.cols % 4);

	if (padding == 4)
		padding = 0;

	int border = 0;
	//32 bit image is always DWORD aligned because each pixel requires 4 bytes
	if (bpp < 32)
	{
		border = 4 - (mat_frame.cols % 4);
	}



	Mat mat_temp;
	if (border > 0 || mat_frame.isContinuous() == false)
	{
		// Adding needed columns on the right (max 3 px)
		cv::copyMakeBorder(mat_frame, mat_temp, 0, 0, 0, border, cv::BORDER_CONSTANT, 0);
	}
	else
	{
		mat_temp = mat_frame;
	}


	RECT r;
	m_picture.GetClientRect(&r);
	cv::Size winSize(r.right, r.bottom);

	cimage_mfc.Create(winSize.width, winSize.height, 24);


	BITMAPINFO* bitInfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFO) + 256 * sizeof(RGBQUAD));
	bitInfo->bmiHeader.biBitCount = bpp;
	bitInfo->bmiHeader.biWidth = mat_temp.cols;
	bitInfo->bmiHeader.biHeight = -mat_temp.rows;
	bitInfo->bmiHeader.biPlanes = 1;
	bitInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitInfo->bmiHeader.biCompression = BI_RGB;
	bitInfo->bmiHeader.biClrImportant = 0;
	bitInfo->bmiHeader.biClrUsed = 0;
	bitInfo->bmiHeader.biSizeImage = 0;
	bitInfo->bmiHeader.biXPelsPerMeter = 0;
	bitInfo->bmiHeader.biYPelsPerMeter = 0;


	//그레이스케일 인경우 팔레트가 필요
	if (bpp == 8)
	{
		RGBQUAD* palette = bitInfo->bmiColors;
		for (int i = 0; i < 256; i++)
		{
			palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed = (BYTE)i;
			palette[i].rgbReserved = 0;
		}
	}


	// Image is bigger or smaller than into destination rectangle
	// we use stretch in full rect

	if (mat_temp.cols == winSize.width && mat_temp.rows == winSize.height)
	{
		// source and destination have same size
		// transfer memory block
		// NOTE: the padding border will be shown here. Anyway it will be max 3px width

		SetDIBitsToDevice(cimage_mfc.GetDC(),
			//destination rectangle
			0, 0, winSize.width, winSize.height,
			0, 0, 0, mat_temp.rows,
			mat_temp.data, bitInfo, DIB_RGB_COLORS);
	}
	else
	{
		// destination rectangle
		int destx = 0, desty = 0;
		int destw = winSize.width;
		int desth = winSize.height;

		// rectangle defined on source bitmap
		// using imgWidth instead of mat_temp.cols will ignore the padding border
		int imgx = 0, imgy = 0;
		int imgWidth = mat_temp.cols - border;
		int imgHeight = mat_temp.rows;

		StretchDIBits(cimage_mfc.GetDC(),
			destx, desty, destw, desth,
			imgx, imgy, imgWidth, imgHeight,
			mat_temp.data, bitInfo, DIB_RGB_COLORS, SRCCOPY);
	}

	m_matImage = mat_temp;
	c_matImage = mat_temp;

	HDC dc = ::GetDC(m_picture.m_hWnd);
	cimage_mfc.BitBlt(dc, 0, 0);


	::ReleaseDC(m_picture.m_hWnd, dc);

	cimage_mfc.ReleaseDC();
	cimage_mfc.Destroy();
	count = 1;

	resize(m_matImage, c_matImage, Size(m_matImage.cols / count, m_matImage.rows / count), 0, 0, INTER_CUBIC);
	CreateBitmapInfo(c_matImage.cols, c_matImage.rows, c_matImage.channels() * 8);

	DrawImage(c_matImage);
	
	CWnd* pWnd1 = (CWnd*)GetDlgItem(IDC_BUTTON_IMAGE_ENLARGEMENT);
	CWnd* pWnd2 = (CWnd*)GetDlgItem(IDC_BUTTON_IMAGE_REDUCTION);
	CWnd* pWnd3 = (CWnd*)GetDlgItem(IDC_BUTTON_IMAGE_ORIGINAL);
	CWnd* pWnd4 = (CWnd*)GetDlgItem(IDC_BUTTON_SOBEL);
	CWnd* pWnd5 = (CWnd*)GetDlgItem(IDC_BUTTON_CANNYEDGE);
	CWnd* pWnd6 = (CWnd*)GetDlgItem(IDC_BUTTON_BLUR);
	CWnd* pWnd7 = (CWnd*)GetDlgItem(IDC_BUTTON_EMBOSSING);
	CWnd* pWnd8 = (CWnd*)GetDlgItem(IDC_BUTTON_UNSHARP);
	CWnd* pWnd9 = (CWnd*)GetDlgItem(IDC_BUTTON_BINARY);
	pWnd1->EnableWindow(TRUE);
	pWnd2->EnableWindow(TRUE);
	pWnd3->EnableWindow(TRUE);
	pWnd4->EnableWindow(TRUE);
	pWnd5->EnableWindow(TRUE);
	pWnd6->EnableWindow(TRUE);
	pWnd7->EnableWindow(TRUE);
	pWnd8->EnableWindow(TRUE);
	pWnd9->EnableWindow(TRUE);

	//OnTimer(nIDEvent);
}


void CImageOpenDlg::OnBnClickedButtonCamera2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//VideoCapture cap(0);

	////double fps = cap.get(CAP_PROP_FPS);
	////int delyay = cvRound(1000 / fps);

	//Mat frame, inversed;
	//while (true)
	//{
	//	cap >> frame;
	//	if (frame.empty())
	//		break;

	//	inversed = ~frame;
	//	imshow("frame", frame);
	//	
	//	if (waitKey(10) == 27) // ESC Key
	//		break;
	//}
	//destroyAllWindows();
}


void CImageOpenDlg::OnBnClickedButtonEmbossing()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	

	/*Canny(c_matImage, dst1, 50, 100);

	c_matImage = dst1;
	UpdateData();
	m_List.InsertString(listcount, _T("canny"));
	listcount++;
	*/
	float data[] = { -1,-1,0,-1,0,1,0,1,1 };
	
	Mat emboss(3, 3, CV_32FC1, data);
	Mat dst;
	filter2D(c_matImage, dst, -1, emboss, Point(-1, -1), 128);

	c_matImage = dst;

	DrawImage(c_matImage);
}


void CImageOpenDlg::OnBnClickedButtonUnsharp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Mat blurred;
	GaussianBlur(c_matImage, blurred, Size(), 5);

	float alpha = 1.f;
	Mat dst = (1 + alpha) * c_matImage - alpha * blurred;

	c_matImage = dst;

	DrawImage(c_matImage);
}


void CImageOpenDlg::OnBnClickedButtonBinary()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Mat dst;
	threshold(c_matImage, dst, 127, 255, THRESH_BINARY);

	c_matImage = dst;

	DrawImage(c_matImage);
}


void CImageOpenDlg::OnBnClickedButtonHoughline()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Mat edge;
	Canny(c_matImage, edge, 50, 150);

	vector<Vec2f>lines;
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

}