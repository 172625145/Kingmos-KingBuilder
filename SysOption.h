#if !defined(AFX_SYSOPTION_H__A8F3CA93_E45A_4E43_A952_C24D3F66FB05__INCLUDED_)
#define AFX_SYSOPTION_H__A8F3CA93_E45A_4E43_A952_C24D3F66FB05__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SysOption.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSysOption dialog

class CSysOption : public CDialog
{
// Construction
public:
	void DrawEffect(HDC hdc,RECT rect);
	int GetColorID(COLORREF clColor);
	CSysOption(CWnd* pParent = NULL);   // standard constructor
	void ReadSysColor(void);
	void SaveSysColor(void);

// Dialog Data
	//{{AFX_DATA(CSysOption)
	enum { IDD = IDD_OPTION };
	CButton	m_Effect;
	CListBox	m_lbxTextList;
	CComboBox	m_cbxForeGround;
	CComboBox	m_cbxBackGround;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSysOption)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
		COLORREF color_NormalText;
		COLORREF color_NormalBKColor;
		COLORREF color_KeyWord;
		COLORREF color_Mask;
		COLORREF color_Quotation;
		COLORREF color_Define;
		COLORREF color_MarkArea;
		COLORREF color_SelTextColor;
		COLORREF color_SelBkColor;
		COLORREF color_BreakPoint ;
		COLORREF color_BookMarks ;

		COLORREF color_CurForeGround;
		COLORREF color_CurBackGround;


	// Generated message map functions
	//{{AFX_MSG(CSysOption)
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeTextlist();
	afx_msg void OnSelchangeForeground();
	afx_msg void OnSelchangeBackground();
	afx_msg void OnReset();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSOPTION_H__A8F3CA93_E45A_4E43_A952_C24D3F66FB05__INCLUDED_)
