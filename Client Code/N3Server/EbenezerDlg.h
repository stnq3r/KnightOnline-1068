// EbenezerDlg.h : header file
//

#if !defined(AFX_EBENEZERDLG_H__655A21EF_E029_42C0_890A_68DA7F542428__INCLUDED_)
#define AFX_EBENEZERDLG_H__655A21EF_E029_42C0_890A_68DA7F542428__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Iocport.h"
#include "Map.h"
/////////////////////////////////////////////////////////////////////////////
// CEbenezerDlg dialog

struct _POSITION
{
	float initx;
	float initz;
};

class CUser;
class CEbenezerDlg : public CDialog
{
// Construction
public:
	void UserInOutForMe( CUser* pUser );
	int GetRegionCount( int x, int z );
	void Send_Region( char* pBuf, int len, int x, int z, BOOL tome = FALSE );
	void Send_All( char* pBuf, int len, BOOL tome = FALSE );
	static CUser* GetUserPtr( char* userid );
	CEbenezerDlg(CWnd* pParent = NULL);	// standard constructor

	static CIOCPort	m_Iocport;
	C3DMap	m_Map;

	int	m_nCount;

	_POSITION	m_pInitPos[5];
// Dialog Data
	//{{AFX_DATA(CEbenezerDlg)
	enum { IDD = IDD_EBENEZER_DIALOG };
	CEdit	m_CountCtrl;
	CListBox	m_StatusList;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEbenezerDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CEbenezerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EBENEZERDLG_H__655A21EF_E029_42C0_890A_68DA7F542428__INCLUDED_)
