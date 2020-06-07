
// WxQuickLoginToolDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "WxAllUserConfig.h"

// CWxQuickLoginToolDlg 对话框
class CWxQuickLoginToolDlg : public CDialogEx
{
// 构造
public:
	CWxQuickLoginToolDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_WXQUICKLOGINTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	/*
	 *	初始化界面
	 */
	void InitUI();


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
	afx_msg void OnBnClickedOk();
	CComboBox m_login_user_list;
	WxAllUserConfig m_config;
};
