// CImageOpenDlg.cpp: 구현 파일
//

#include "pch.h"
#include "MFCOpenCV.h"
#include "afxdialogex.h"
#include "CImageOpenDlg.h"


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

void CImageOpenDlg::DrawImage()
{
	CClientDC dc(GetDlgItem(IDC_PICTURE));

	CRect rect;
	GetDlgItem(IDC_PICTURE)->GetClientRect(&rect);

	SetStretchBltMode(dc.GetSafeHdc(), COLORONCOLOR);
	StretchDIBits(dc.GetSafeHdc(), 0, 0, rect.Width(), rect.Height(), 0, 0, 
		m_matImage.cols, m_matImage.rows, m_matImage.data, m_pBitmapInfo, DIB_RGB_COLORS, SRCCOPY);
}

void CImageOpenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TEXT_SIZE, m_TextSize);
	DDX_Control(pDX, IDC_TEXT_SIZE2, m_TextSize2);
	DDX_Control(pDX, IDC_TEXT_SIZE3, m_TextSize3);
}


BEGIN_MESSAGE_MAP(CImageOpenDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_IMAGE, &CImageOpenDlg::OnBnClickedButtonImage)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_NCMOUSEMOVE()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CImageOpenDlg 메시지 처리기


void CImageOpenDlg::OnBnClickedButtonImage()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_READONLY, _T("image file(*.jpg;*.bmp;*.png;)|*.jpg;*.bmp;*.png;|All Files(*.*)|*.*||"));
	if (fileDlg.DoModal() == IDOK)
	{
		CString path = fileDlg.GetPathName();

		CT2CA pszString(path);
		std::string strPath(pszString);

		m_matImage = imread(strPath, IMREAD_UNCHANGED);



		CreateBitmapInfo(m_matImage.cols, m_matImage.rows, m_matImage.channels() * 8);

		DrawImage();
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

	trans_count_w = static_cast<float>(651) / m_matImage.cols;	// 이미지 가로 비율 
	trans_count_h = static_cast<float>(572) / m_matImage.rows;  // 이미지 세로 비율

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
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
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
	if (point.x > 651)
	{
		point.x = 651;
		
	}
	if (point.y > 572)
	{
		point.y = 572;
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
