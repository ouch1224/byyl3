
// byyl3Dlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "byyl3.h"
#include "byyl3Dlg.h"
#include "afxdialogex.h"
#include "CMyDialog1.h"
#include "CSHOWTREE.h"
#include <atlbase.h> // 包含 ATL 的基本头文件
#include <atlstr.h> 
#include <fstream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Cbyyl3Dlg 对话框



Cbyyl3Dlg::Cbyyl3Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BYYL3_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cbyyl3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Cbyyl3Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OPEN, &Cbyyl3Dlg::OnBnClickedOpen)
	ON_BN_CLICKED(IDC_SAVE, &Cbyyl3Dlg::OnBnClickedSave)
	ON_BN_CLICKED(IDC_CREATE, &Cbyyl3Dlg::OnBnClickedCreate)
	
	ON_BN_CLICKED(IDC_LEXICAL, &Cbyyl3Dlg::OnBnClickedLexical)
	ON_BN_CLICKED(IDC_SHOWTREE, &Cbyyl3Dlg::OnBnClickedShowtree)
	
END_MESSAGE_MAP()


// Cbyyl3Dlg 消息处理程序

BOOL Cbyyl3Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void Cbyyl3Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void Cbyyl3Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR Cbyyl3Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}





void Cbyyl3Dlg::OnBnClickedOpen()
{
	// TODO: 在此添加控件通知处理程序代码
	CString path;
	CFileDialog dlg(TRUE);
	if (dlg.DoModal() == IDOK)
	{
		path = dlg.GetPathName();
		CStdioFile file;
		CString text, line;
		file.Open(path, CFile::modeRead);
		while (file.ReadString(line))
		{
			text += line;
			text += L"\r\n";
		}
		file.Close();
		SetDlgItemText(IDC_CODE, text);
	}
	else
	{
		MessageBox(L"未选择文件");
		return;
	}

}


void Cbyyl3Dlg::OnBnClickedSave()
{
	// TODO: 在此添加控件通知处理程序代码
	CString text, path, fileName = L"temp.txt";
	CFileDialog dlg(FALSE);
	dlg.m_ofn.lpstrTitle = L"我的文件保存对话框";
	dlg.m_ofn.lpstrFilter = L"Text Files(*.txt)\0*.txt\0All Files(*.*)\0*.*\0\0";
	dlg.m_ofn.lpstrDefExt = L"txt";
	if (IDOK == dlg.DoModal())
	{
		path = dlg.GetPathName();
		fileName = dlg.GetFileName();
		GetDlgItemText(IDC_SHOW, text);
		//创建文件
		CStdioFile file;
		file.Open(path, CFile::modeCreate | CFile::modeWrite);
		file.SeekToEnd();
		file.WriteString(text);
		file.Flush();
		//关闭文件
		file.Close();
	}

}


vector<string> Cbyyl3Dlg::splitString(const string& str, char delimiter) {
	vector<string> tokens;
	string token;
	istringstream tokenStream(str);

	while (getline(tokenStream, token, delimiter)) {
		tokens.push_back(token);
	}

	return tokens;
}

void Cbyyl3Dlg::OnBnClickedCreate()
{
	// TODO: 在此添加控件通知处理程序代码
	CString ctext;
	GetDlgItemText(IDC_CODE, ctext); 
	if (ctext.IsEmpty())
	{
		MessageBox(L"请选择文件或直接输入源代码！", L"错误", MB_OK | MB_ICONWARNING);
		return;
	}
	else {
		string text;
		text = CT2A(ctext);
		SyntaxTree st;
		char delimiter = '\n';
		vector<string> tvector = splitString(text, delimiter);
		st.parse(tvector);
		MessageBox(L"已成功创建语法树！");

		CString cerror = st.getAllError();
		SetDlgItemTextW(IDC_ERROR, cerror);
		st.deleteTree();

	}
}




void Cbyyl3Dlg::OnBnClickedLexical()
{
	// TODO: 在此添加控件通知处理程序代码
	CMyDialog1 dlg;  // 创建新视图对话框实例  
	CString ctext;
	GetDlgItemText(IDC_CODE, ctext);
	string text;
	text = CT2A(ctext);
	SyntaxTree st;
	char delimiter = '\n';
	vector<string> tvector = splitString(text, delimiter);
	st.parse(tvector);
	dlg.m_OutputText = CString(st.getText().c_str());
	dlg.DoModal();  // 模态显示对话框
	st.deleteTree();
	
}


string str = "./syntaxtree.out";

void Cbyyl3Dlg::OnBnClickedShowtree()
{
	// TODO: 在此添加控件通知处理程序代码
	CSHOWTREE dlg;  // 创建新视图对话框实例  
	CString ctext;
	GetDlgItemText(IDC_CODE, ctext);
	string text;
	text = CT2A(ctext);
	SyntaxTree st;
	char delimiter = '\n';
	vector<string> tvector = splitString(text, delimiter);
	TreeNode* node= st.parse(tvector);
	st.printTree(node);


	
	m_output = st.getTreeText().c_str();

	dlg.m_OutputText = m_output;
	dlg.DoModal();
	st.deleteTree();
}

