#pragma once
#include "afxdialogex.h"


// CMyDialog1 对话框

class CMyDialog1 : public CDialogEx
{
	DECLARE_DYNAMIC(CMyDialog1)

public:
	CMyDialog1(CWnd* pParent = nullptr);   // 标准构造函数
	CString m_OutputText;
	virtual ~CMyDialog1();
	virtual BOOL OnInitDialog();
	CEdit m_EditOutput;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit1();
};
