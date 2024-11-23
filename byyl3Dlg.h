
// byyl3Dlg.h: 头文件
//

#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "SyntaxTree.h"
using namespace std;

// Cbyyl3Dlg 对话框
class Cbyyl3Dlg : public CDialogEx
{
// 构造
public:
	Cbyyl3Dlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BYYL3_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOpen();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedCreate();
	
	CEdit program;  // 源程序编辑框
	CString m_file_path; // 路径
	CString m_input;    // 输入程序
	CString m_output;   // 输出
	

	vector<string> splitString(const string& str, char delimiter);
	
	afx_msg void OnBnClickedLexical();
	afx_msg void OnBnClickedShowtree();
	
};
