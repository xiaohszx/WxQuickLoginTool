
// WxQuickLoginToolDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CWxQuickLoginToolDlg �Ի���



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
			//info.Format(L"΢�ź�:%s ΢���ǳ�:%s ǩ��:%s", item.wx_number.c_str(), item.nick_name.c_str(), item.signature.c_str());
			CString info;
			info.Format(L"%s(%s)", item.nick_name.c_str(), item.wx_number.c_str());
			OutputDebugString(info.GetBuffer());
			int index = m_login_user_list.AddString(info);
			auto userinfo = new WxUserInfo();	//�����ڣ�û��д�ͷź���
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


// CWxQuickLoginToolDlg ��Ϣ�������

BOOL CWxQuickLoginToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	InitUI();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CWxQuickLoginToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CWxQuickLoginToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CWxQuickLoginToolDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnOK();
	int index = m_login_user_list.GetCurSel();
	WxUserInfo* p_info = (WxUserInfo*)m_login_user_list.GetItemData(index);
	if(p_info != NULL && (int)p_info != CB_ERR){
		m_config.ModifyLoginWx(p_info->wx_number);
	}
	WxConfigManager::GetInstance().StartWeChat();
}
