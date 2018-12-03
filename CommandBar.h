#if !defined(AFX_COMMANDBAR_H__4B0C5A20_2DE1_4285_8696_FD0DE3935322__INCLUDED_)
#define AFX_COMMANDBAR_H__4B0C5A20_2DE1_4285_8696_FD0DE3935322__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CommandBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCommandBar window

typedef struct ComboBoxList{
	CComboBox *pComboBox;
	UINT uID;
	struct ComboBoxList * pNext;
}COMBOBOXLIST,* LPCOMBOBOXLIST;

class CCommandBar : public CToolBar
{
// Construction
public:
	CCommandBar();
	~CCommandBar();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommandBar)
	//}}AFX_VIRTUAL

// Implementation
public:
//	BOOL Create (CWnd *pParent, BOOL bTips);
	BOOL CreateEx(CWnd *pParent, DWORD dwCtrlStyle ,DWORD dwStyle,UINT nID);
	BOOL AddComboBox(UINT uID,int iWidth,DWORD dwStyle);
	CComboBox * GetComboBox(UINT uID);
	UINT GetComboBoxID(CWnd *pItem);
	BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	// Generated message map functions
protected:
	//{{AFX_MSG(CCommandBar)
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
//	CComboBox m_Search;
	LPCOMBOBOXLIST m_ComboList;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMANDBAR_H__4B0C5A20_2DE1_4285_8696_FD0DE3935322__INCLUDED_)
