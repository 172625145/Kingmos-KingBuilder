#if !defined(AFX_SETSYSCOLOR_H__06312794_B75C_4B4C_90D8_676B8BDC66CA__INCLUDED_)
#define AFX_SETSYSCOLOR_H__06312794_B75C_4B4C_90D8_676B8BDC66CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetSysColor.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetSysColor dialog

class CSetSysColor : public CDialog
{
// Construction
public:
	COLORREF GetItemColor(LPTSTR lpItemString);
	void SetSysColorItem(LPTSTR lpItemString,COLORREF colorItem,COLORREF defaultcolorItem);
	CSetSysColor(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetSysColor)
	enum { IDD = IDD_SETSYSCOLOR };
	CComboBox	m_ColorItem;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetSysColor)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetSysColor)
	virtual void OnOK();
	afx_msg void OnSelColor();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSelchangeColoritemcombo();
	afx_msg void OnDefaultcolor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void SetCurColor(COLORREF color);
	COLORREF GetCurColor();
	void InitialItemCombo();

	struct ColotItemInfo{
			CString stringItem;  // 用于存放当前行的数据
			COLORREF colorItem;
			COLORREF defaultcolorItem;
			//ColotItemInfo() {};  // 创建时初始化为0
		};
	    CArray < ColotItemInfo, ColotItemInfo& > m_aItemData;  //存放每一行的数据

		void DrawCurColor(CDC *pdc);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETSYSCOLOR_H__06312794_B75C_4B4C_90D8_676B8BDC66CA__INCLUDED_)
