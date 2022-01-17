// CRotateAngleDlg.cpp: 구현 파일
//

#include "pch.h"
#include "MFCOpenCV.h"
#include "afxdialogex.h"
#include "CRotateAngleDlg.h"


// CRotateAngleDlg 대화 상자

IMPLEMENT_DYNAMIC(CRotateAngleDlg, CDialog)

CRotateAngleDlg::CRotateAngleDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_ROTATEANGLE, pParent)
	, m_nRotateAngle(0)
	, m_nXpoint(0)
	, m_nYpoint(0)
{

}

CRotateAngleDlg::~CRotateAngleDlg()
{
}

void CRotateAngleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_ROTATEANGLE, m_sliderRotateAngle);
	DDX_Text(pDX, IDC_EDIT_ROTATEANGLE, m_nRotateAngle);
	DDV_MinMaxDouble(pDX, m_nRotateAngle, 0, 360);
	DDX_Text(pDX, IDC_EDIT_XPOINT, m_nXpoint);
	DDX_Text(pDX, IDC_EDIT_YPOINT, m_nYpoint);
}


BEGIN_MESSAGE_MAP(CRotateAngleDlg, CDialog)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_EDIT_ROTATEANGLE, &CRotateAngleDlg::OnEnChangeEditRotateangle)
END_MESSAGE_MAP()


// CRotateAngleDlg 메시지 처리기


BOOL CRotateAngleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_sliderRotateAngle.SetRange(0, 360); // 슬라이더 범위
	m_sliderRotateAngle.SetTicFreq(10);	  // 눈금의 간격
	m_sliderRotateAngle.SetTicFreq(10);	  // 눈금의 간격
	m_sliderRotateAngle.SetPageSize(10);  // 엄지단추 이외의 부분 클릭시 이동거리


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CRotateAngleDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if (m_sliderRotateAngle.GetSafeHwnd() == pScrollBar->GetSafeHwnd())
	{
		m_nRotateAngle = m_sliderRotateAngle.GetPos();
		UpdateData(FALSE);
	}


	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CRotateAngleDlg::OnEnChangeEditRotateangle()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
