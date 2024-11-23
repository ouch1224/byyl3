#pragma once
#include "afxdialogex.h"


// CSHOWTREE 对话框

class CSHOWTREE : public CDialogEx
{
	DECLARE_DYNAMIC(CSHOWTREE)

public:
	CSHOWTREE(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSHOWTREE();
	CString m_OutputText;
	virtual BOOL OnInitDialog();
	CEdit m_EditOutput;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeShowtree();
};
