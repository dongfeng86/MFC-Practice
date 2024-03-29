﻿
// CCustomLstCtrlDlg.h: 头文件
//

#pragma once
#include "CCustomListCtrl.h"
#include "CDlgAreaCondition.h"


// CCCustomLstCtrlDlg 对话框
class CCCustomLstCtrlDlg : public CDialog
{
// 构造
public:
	CCCustomLstCtrlDlg(CWnd* pParent = nullptr);	// 标准构造函数
	enum { IDD = IDD_CCUSTOMLSTCTRL_DIALOG };


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
	CCustomListCtrl m_wndLst;
	CComboBox m_wndCmbTemp;
	CCustomHeader m_wndCustomHeadCtrl;
	afx_msg void OnCbnDropdownCombo1();

	CDlgAreaCondition m_wndPopDialog;
};
