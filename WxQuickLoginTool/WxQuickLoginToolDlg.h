
// WxQuickLoginToolDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "WxAllUserConfig.h"

// CWxQuickLoginToolDlg �Ի���
class CWxQuickLoginToolDlg : public CDialogEx
{
// ����
public:
	CWxQuickLoginToolDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_WXQUICKLOGINTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	/*
	 *	��ʼ������
	 */
	void InitUI();


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
