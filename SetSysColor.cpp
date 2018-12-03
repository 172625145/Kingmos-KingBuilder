// SetSysColor.cpp : implementation file
//

#include "stdafx.h"
#include "SpecialView.h"
#include "SetSysColor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetSysColor dialog


CSetSysColor::CSetSysColor(CWnd* pParent /*=NULL*/)
	: CDialog(CSetSysColor::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetSysColor)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSetSysColor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetSysColor)
	DDX_Control(pDX, IDC_COLORITEMCOMBO, m_ColorItem);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetSysColor, CDialog)
	//{{AFX_MSG_MAP(CSetSysColor)
	ON_BN_CLICKED(IDC_SelColor, OnSelColor)
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_COLORITEMCOMBO, OnSelchangeColoritemcombo)
	ON_BN_CLICKED(IDC_DEFAULTCOLOR, OnDefaultcolor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetSysColor message handlers

void CSetSysColor::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CSetSysColor::OnSelColor() 
{
	// TODO: Add your control notification handler code here

	COLORREF color;
	color = GetCurColor();

	CColorDialog dlg(color);

	if (dlg.DoModal() == IDOK)
	{
	   	color = dlg.GetColor();

		SetCurColor(color);
		
		CDC *pdc;
		pdc = GetDC();
		DrawCurColor(pdc);
		ReleaseDC(pdc);	
	}	
}


BOOL CSetSysColor::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitialItemCombo();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetSysColor::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	DrawCurColor(&dc);
	// Do not call CDialog::OnPaint() for painting messages
}

void CSetSysColor::DrawCurColor(CDC *pdc)
{
	RECT rect={28,80,180,110};
	CBrush Brush(GetSysColor(COLOR_WINDOW));
	pdc->DrawEdge(&rect,BDR_SUNKENOUTER|BDR_SUNKENINNER,BF_RECT);
	InflateRect(&rect,-2,-2);
	pdc->FillRect(&rect,&Brush);

	COLORREF color ;//= RGB(128,128,255);
	
	color = GetCurColor();

	InflateRect(&rect,-2,-2);

	pdc->FillSolidRect(&rect,color);
	pdc->DrawFocusRect(&rect);
}

void CSetSysColor::OnSelchangeColoritemcombo() 
{
	// TODO: Add your control notification handler code here
	CDC *pdc;

	pdc = GetDC();
	DrawCurColor(pdc);
	ReleaseDC(pdc);	
}

void CSetSysColor::SetSysColorItem(LPTSTR lpItemString, COLORREF colorItem, COLORREF defaultcolorItem)
{
	ColotItemInfo ColotItemInfo;
	
	ColotItemInfo.stringItem.Insert(0,lpItemString);
	ColotItemInfo.colorItem = colorItem;
	ColotItemInfo.defaultcolorItem = defaultcolorItem;
	m_aItemData.Add(ColotItemInfo);
}

void CSetSysColor::InitialItemCombo()
{
	int nItemNum;
	ColotItemInfo ColorItemInfo;
	int i;

		nItemNum = m_aItemData.GetSize();
		for (i=0;i<nItemNum;i++)
		{
			ColorItemInfo = m_aItemData.GetAt(i);
			m_ColorItem.AddString(ColorItemInfo.stringItem);
		}
		m_ColorItem.SetCurSel(0);

}

COLORREF CSetSysColor::GetCurColor()
{
	COLORREF color ;//= RGB(128,128,255);
	int nIndex;
	ColotItemInfo ColorItemInfo;

	nIndex = m_ColorItem.GetCurSel();
	ColorItemInfo = m_aItemData.GetAt(nIndex);
	color = ColorItemInfo.colorItem;

	return color;
}

void CSetSysColor::SetCurColor(COLORREF color)
{
		int nIndex;
		ColotItemInfo ColorItemInfo;

		nIndex = m_ColorItem.GetCurSel();
		ColorItemInfo = m_aItemData.GetAt(nIndex);
		ColorItemInfo.colorItem = color ;

		m_aItemData.SetAt(nIndex,ColorItemInfo);
}

COLORREF CSetSysColor::GetItemColor(LPTSTR lpItemString)
{
	int nIndex,nIndexMax;
	ColotItemInfo ColorItemInfo;


	nIndexMax = m_aItemData.GetSize();
	for (nIndex =0; nIndex < nIndexMax; nIndex ++)
	{
		ColorItemInfo = m_aItemData.GetAt(nIndex);
		if (ColorItemInfo.stringItem.Compare(lpItemString) ==0)
		{
			return ColorItemInfo.colorItem;
		}
	}

	return RGB(0,0,0);
}

void CSetSysColor::OnDefaultcolor() 
{
	// TODO: Add your control notification handler code here
/*	int nItemNum;
	ColotItemInfo ColorItemInfo;
	int nIndex;

		nItemNum = m_aItemData.GetSize();
		for (nIndex=0;nIndex<nItemNum;nIndex++)
		{
			ColorItemInfo = m_aItemData.GetAt(nIndex);
			ColorItemInfo.colorItem = ColorItemInfo.defaultcolorItem;
			m_aItemData.SetAt(nIndex,ColorItemInfo);
		}
*/
//	COLORREF color ;//= RGB(128,128,255);
	int nIndex;
	ColotItemInfo ColorItemInfo;

		nIndex = m_ColorItem.GetCurSel();
		ColorItemInfo = m_aItemData.GetAt(nIndex);
		ColorItemInfo.colorItem = ColorItemInfo.defaultcolorItem;
		m_aItemData.SetAt(nIndex,ColorItemInfo);

		CDC *pdc;
		pdc = GetDC();
		DrawCurColor(pdc);
		ReleaseDC(pdc);	
}
