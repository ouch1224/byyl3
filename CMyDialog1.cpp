﻿// CMyDialog1.cpp: 实现文件
//

#include "pch.h"
#include "byyl3.h"
#include "afxdialogex.h"
#include "CMyDialog1.h"


// CMyDialog1 对话框

IMPLEMENT_DYNAMIC(CMyDialog1, CDialogEx)

CMyDialog1::CMyDialog1(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

CMyDialog1::~CMyDialog1()
{
}

void CMyDialog1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMyDialog1, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT1, &CMyDialog1::OnEnChangeEdit1)
END_MESSAGE_MAP()


// CMyDialog1 消息处理程序


BOOL CMyDialog1::OnInitDialog() {
	CDialogEx::OnInitDialog();

	m_EditOutput.SubclassDlgItem(IDC_EDIT1, this);
	m_EditOutput.SetWindowText(m_OutputText); // 设置文本框内容  

	return TRUE;  // 返回 TRUE 以设置焦点到默认的控件  
}

void CMyDialog1::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	// 清空编辑框的内容
	


}