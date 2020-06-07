
// WxQuickLoginToolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WxQuickLoginTool.h"
#include "WxQuickLoginToolDlg.h"
#include "afxdialogex.h"
#include "WxConfigHelper.h"
#include "WxLoginUserConfig.h"
#include "WxConfigManager.h"
#include "FileHelp.h"
#include "WxConfigManager.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CWxQuickLoginToolDlg 对话框



CWxQuickLoginToolDlg::CWxQuickLoginToolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWxQuickLoginToolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWxQuickLoginToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_LOGIN_LIST, m_login_user_list);
}

void CWxQuickLoginToolDlg::InitUI()
{
	auto ret = m_config.Open();
	if(ret){
		for(auto item : m_config.GetAllLoginUserInfo()){
			//CString info;
			//info.Format(L"微信号:%s 微信昵称:%s 签名:%s", item.wx_number.c_str(), item.nick_name.c_str(), item.signature.c_str());
			CString info;
			info.Format(L"%s(%s)", item.nick_name.c_str(), item.wx_number.c_str());
			OutputDebugString(info.GetBuffer());
			int index = m_login_user_list.AddString(info);
			auto userinfo = new WxUserInfo();	//主窗口，没有写释放函数
			*userinfo = item;
			m_login_user_list.SetItemData(index, (DWORD_PTR)userinfo);
		}
	}
}

BEGIN_MESSAGE_MAP(CWxQuickLoginToolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CWxQuickLoginToolDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CWxQuickLoginToolDlg 消息处理程序

BOOL CWxQuickLoginToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	InitUI();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CWxQuickLoginToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CWxQuickLoginToolDlg::OnPaint()
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
HCURSOR CWxQuickLoginToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CWxQuickLoginToolDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
	int index = m_login_user_list.GetCurSel();
	WxUserInfo* p_info = (WxUserInfo*)m_login_user_list.GetItemData(index);
	if(p_info != NULL && (int)p_info != CB_ERR){
		m_config.ModifyLoginWx(p_info->wx_number);
	}
	WxConfigManager::GetInstance().StartWeChat();
}
