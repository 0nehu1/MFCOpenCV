#pragma once
#include "afxdialogex.h"


// CRotateAngleDlg 대화 상자

class CRotateAngleDlg : public CDialog
{
	DECLARE_DYNAMIC(CRotateAngleDlg)

public:
	CRotateAngleDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CRotateAngleDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ROTATEANGLE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl m_sliderRotateAngle;
	double m_nRotateAngle;
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEnChangeEditRotateangle();
	double m_nXpoint;
	double m_nYpoint;
};
