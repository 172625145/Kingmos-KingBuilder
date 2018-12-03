// SysOption.cpp : implementation file
//

#include "stdafx.h"
#include "ambo.h"
#include "SysOption.h"
#include <atlbase.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define CL_BLACK        RGB( 0x00, 0x00, 0x00 )  //0
#define CL_DARKRED      RGB( 0x80, 0x00, 0x00 )  //1
#define CL_DARKGREEN    RGB( 0x00, 0x80, 0x00 )  //2
#define CL_DARKYELLOW   RGB( 0x80, 0x80, 0x00 )  //3
#define CL_DARKBLUE     RGB( 0x00, 0x00, 0x80 )  //4
#define CL_DARKVIOLET   RGB( 0x80, 0x00, 0x80 )  //5
#define CL_DARKCYAN     RGB( 0x00, 0x80, 0x80 )  //6
#define CL_DARKGRAY     RGB( 0xdf, 0xdf, 0xdf )  //7
#define CL_LIGHTGRAY    RGB( 0xc0, 0xc0, 0xc0 )  //8
#define CL_RED          RGB( 0xff, 0x00, 0x00 )  //9
#define CL_GREEN        RGB( 0x00, 0x7f, 0x00 )  //10
#define CL_YELLOW       RGB( 0xff, 0xff, 0x00 )  //11
#define CL_BLUE         RGB( 0x00, 0x00, 0xff )  //12
#define CL_VIOLET       RGB( 0xff, 0x00, 0xff )  //13
#define CL_CYAN         RGB( 0x00, 0xff, 0xff )  //14
#define CL_WHITE        RGB( 0xff, 0xff, 0xff )  //15

#define CL_WINDOW		-1

#define DEFAULTCOLOR_NORMALTEXT		CL_BLACK
#define DEFAULTCOLOR_KEYWORDTEXT	CL_BLUE
#define DEFAULTCOLOR_MASKTEXT		CL_GREEN
#define DEFAULTCOLOR_QUOTETEXT		CL_BLACK
#define DEFAULTCOLOR_DEFINE			CL_BLUE
#define DEFAULTCOLOR_MARKAREA		CL_DARKGRAY
#define DEFAULTCOLOR_SELBKCOLOR		CL_DARKBLUE
#define DEFAULTCOLOR_SELTEXTCOLOR	CL_WHITE
#define DEFAULTCOLOR_BREAKPOINT		CL_DARKRED
#define DEFAULTCOLOR_BOOKMARKS		CL_CYAN
#define DEFAULTCOLOR_NORMALBKCOLOR	CL_WINDOW

/////////////////////////////////////////////////////////////////////////////
// CSysOption dialog



CSysOption::CSysOption(CWnd* pParent /*=NULL*/)
	: CDialog(CSysOption::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSysOption)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSysOption::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSysOption)
	DDX_Control(pDX, IDC_EFFECT, m_Effect);
	DDX_Control(pDX, IDC_TEXTLIST, m_lbxTextList);
	DDX_Control(pDX, IDC_FOREGROUND, m_cbxForeGround);
	DDX_Control(pDX, IDC_BACKGROUND, m_cbxBackGround);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSysOption, CDialog)
	//{{AFX_MSG_MAP(CSysOption)
	ON_WM_DRAWITEM()
	ON_LBN_SELCHANGE(IDC_TEXTLIST, OnSelchangeTextlist)
	ON_CBN_SELCHANGE(IDC_FOREGROUND, OnSelchangeForeground)
	ON_CBN_SELCHANGE(IDC_BACKGROUND, OnSelchangeBackground)
	ON_BN_CLICKED(IDC_RESET, OnReset)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSysOption message handlers

void CSysOption::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your message handler code here and/or call default

	if (nIDCtl == IDC_EFFECT)
	{
			DrawEffect(lpDrawItemStruct->hDC,lpDrawItemStruct->rcItem);
			return;
	}
	COLORREF selColor[]={
				CL_BLACK      ,     
				CL_DARKRED    ,    
				CL_DARKGREEN  ,  
				CL_DARKYELLOW , 
				CL_DARKBLUE   ,
				CL_DARKVIOLET ,
				CL_DARKCYAN   ,
				CL_DARKGRAY   ,
				CL_LIGHTGRAY  ,
				CL_RED        ,
				CL_GREEN      ,
				CL_YELLOW     ,
				CL_BLUE       ,
				CL_VIOLET     ,
				CL_CYAN       ,
				CL_WHITE      ,
				CL_WINDOW
						};
	COLORREF clColor = selColor[lpDrawItemStruct->itemID];
	BOOL bUseWindowColor = FALSE;

	if (clColor == CL_WINDOW)
	{
		bUseWindowColor = TRUE;
		clColor = GetSysColor(COLOR_WINDOW);
	}
	HBRUSH hBrush = CreateSolidBrush(clColor);
	RECT rect = lpDrawItemStruct->rcItem;

	InflateRect(&rect,-2,-2);

	hBrush = (HBRUSH)SelectObject(lpDrawItemStruct->hDC,hBrush);
	Rectangle(lpDrawItemStruct->hDC,rect.left,rect.top,rect.right,rect.bottom);
	if (bUseWindowColor == TRUE)
	{
		int iOldMode = SetBkMode( lpDrawItemStruct->hDC, TRANSPARENT ) ;
		DrawText(lpDrawItemStruct->hDC,"windows color",13,&rect,DT_CENTER);
		SetBkMode( lpDrawItemStruct->hDC, iOldMode ) ;
	}
	hBrush = (HBRUSH)SelectObject(lpDrawItemStruct->hDC,hBrush);

	DeleteObject(hBrush);
	
//	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

BOOL CSysOption::OnInitDialog() 
{
	CDialog::OnInitDialog();

	ReadSysColor();

	// TODO: Add extra initialization here
	m_lbxTextList.AddString("Text");
	m_lbxTextList.AddString("KeyWord");
	m_lbxTextList.AddString("Mask");
	m_lbxTextList.AddString("Quotation");
	m_lbxTextList.AddString("Define");
	m_lbxTextList.AddString("MarkArea");
	m_lbxTextList.AddString("Selection");
	m_lbxTextList.AddString("BreakPoint");
	m_lbxTextList.AddString("BookMarks");

	m_lbxTextList.SetCurSel(0);
	OnSelchangeTextlist();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSysOption::ReadSysColor()
{

	color_NormalText = DEFAULTCOLOR_NORMALTEXT;
	color_NormalBKColor = DEFAULTCOLOR_NORMALBKCOLOR;
	color_KeyWord =DEFAULTCOLOR_KEYWORDTEXT;
	color_Mask = DEFAULTCOLOR_MASKTEXT;
	color_Quotation = DEFAULTCOLOR_QUOTETEXT;//RGB(0xf0,0x00,0x0f);
	color_Define = DEFAULTCOLOR_DEFINE;//RGB(0x00,0x7f,0x00);
	color_MarkArea = DEFAULTCOLOR_MARKAREA;
	color_SelBkColor = DEFAULTCOLOR_SELBKCOLOR;
	color_SelTextColor = DEFAULTCOLOR_SELTEXTCOLOR;
	color_BreakPoint = DEFAULTCOLOR_BREAKPOINT;
	color_BookMarks = DEFAULTCOLOR_BOOKMARKS;

//	SaveSysColor();

	CRegKey regColor;
	COLORREF color;

	regColor.Create(HKEY_CURRENT_USER,"SoftWare\\SpecilaEdit\\Color");
	if (regColor.QueryValue(color,"NormalText") == ERROR_SUCCESS)
		color_NormalText = color;
	if (regColor.QueryValue(color,"NormalBKColor") == ERROR_SUCCESS)
		color_NormalBKColor = color;
	if (regColor.QueryValue(color,"KeyWord") == ERROR_SUCCESS)
		color_KeyWord = color;
	if (regColor.QueryValue(color,"Mask") == ERROR_SUCCESS)
		color_Mask = color;
	if (regColor.QueryValue(color,"Quotation") == ERROR_SUCCESS)
		color_Quotation = color;
	if (regColor.QueryValue(color,"Define") == ERROR_SUCCESS)
		color_Define = color;
	if (regColor.QueryValue(color,"MarkArea") == ERROR_SUCCESS)
		color_MarkArea = color;
	if (regColor.QueryValue(color,"SelBkColor") == ERROR_SUCCESS)
		color_SelBkColor = color;
	if (regColor.QueryValue(color,"SelTextColor") == ERROR_SUCCESS)
		color_SelTextColor = color;
	if (regColor.QueryValue(color,"BreakPoint") == ERROR_SUCCESS)
		color_BreakPoint = color;
	if (regColor.QueryValue(color,"BookMarks") == ERROR_SUCCESS)
		color_BookMarks = color;
	regColor.Close();
}

void CSysOption::SaveSysColor()
{
	CRegKey regColor;

	regColor.Create(HKEY_CURRENT_USER,"SoftWare\\SpecilaEdit\\Color");
	regColor.SetValue(color_NormalText,"NormalText");
	regColor.SetValue(color_NormalBKColor,"NormalBKColor");
	regColor.SetValue(color_KeyWord,"KeyWord");
	regColor.SetValue(color_Mask,"Mask");
	regColor.SetValue(color_Quotation,"Quotation");
	regColor.SetValue(color_Define,"Define");
	regColor.SetValue(color_MarkArea,"MarkArea");
	regColor.SetValue(color_SelBkColor,"SelBkColor");
	regColor.SetValue(color_SelTextColor,"SelTextColor");
	regColor.SetValue(color_BreakPoint,"BreakPoint");
	regColor.SetValue(color_BookMarks,"BookMarks");
	regColor.Close();


}

void CSysOption::OnOK() 
{
	// TODO: Add extra validation here
	SaveSysColor();	
	CDialog::OnOK();
}

void CSysOption::OnSelchangeTextlist() 
{
	// TODO: Add your control notification handler code here
	int nCurSel = m_lbxTextList.GetCurSel();
	COLORREF clForeGround,clBackGround;
	switch(nCurSel)
	{
		case 0:
			clForeGround = color_NormalText;
			clBackGround = color_NormalBKColor;
			break;
		case 1:
			clForeGround = color_KeyWord;
			clBackGround = color_NormalBKColor;
			break;
		case 2:
			clForeGround = color_Mask;
			clBackGround = color_NormalBKColor;
			break;
		case 3:
			clForeGround = color_Quotation;
			clBackGround = color_NormalBKColor;
			break;
		case 4:
			clForeGround = color_Define;
			clBackGround = color_NormalBKColor;
			break;
		case 5:
			clForeGround = color_MarkArea;
			clBackGround = color_NormalBKColor;
			break;
		case 6:
			clForeGround = color_SelTextColor;
			clBackGround = color_SelBkColor;
			break;
		case 7:
			clForeGround = color_BreakPoint;
			clBackGround = color_NormalBKColor;
			break;
		case 8:
			clForeGround = color_BookMarks;
			clBackGround = color_NormalBKColor;
			break;
		default:
			return;
	}
	int nID = GetColorID(clForeGround);

	m_cbxForeGround.SetCurSel(nID);
	color_CurForeGround = clForeGround;

	nID = GetColorID(clBackGround);

	m_cbxBackGround.SetCurSel(nID);

	color_CurBackGround = clBackGround;
//	DrawEffect();
	m_Effect.InvalidateRect(NULL);
}

int CSysOption::GetColorID(COLORREF clColor)
{
	COLORREF selColor[]={
				CL_BLACK      ,     
				CL_DARKRED    ,    
				CL_DARKGREEN  ,  
				CL_DARKYELLOW , 
				CL_DARKBLUE   ,
				CL_DARKVIOLET ,
				CL_DARKCYAN   ,
				CL_DARKGRAY   ,
				CL_LIGHTGRAY  ,
				CL_RED        ,
				CL_GREEN      ,
				CL_YELLOW     ,
				CL_BLUE       ,
				CL_VIOLET     ,
				CL_CYAN       ,
				CL_WHITE      ,
				CL_WINDOW
						};

	for (int i =0; i< 16; i++ )
	{
		if (clColor == selColor[i])
			break;
	}
	return i;
}

void CSysOption::OnSelchangeForeground() 
{
	// TODO: Add your control notification handler code here
	COLORREF selColor[]={
				CL_BLACK      ,     
				CL_DARKRED    ,    
				CL_DARKGREEN  ,  
				CL_DARKYELLOW , 
				CL_DARKBLUE   ,
				CL_DARKVIOLET ,
				CL_DARKCYAN   ,
				CL_DARKGRAY   ,
				CL_LIGHTGRAY  ,
				CL_RED        ,
				CL_GREEN      ,
				CL_YELLOW     ,
				CL_BLUE       ,
				CL_VIOLET     ,
				CL_CYAN       ,
				CL_WHITE      ,
				CL_WINDOW
						};
	
	int nID = m_cbxForeGround.GetCurSel();
	COLORREF clNewForeGround = selColor[nID];

	int nCurSel = m_lbxTextList.GetCurSel();
	switch(nCurSel)
	{
		case 0:
			color_NormalText = clNewForeGround;
			break;
		case 1:
			color_KeyWord = clNewForeGround;
			break;
		case 2:
			color_Mask = clNewForeGround;
			break;
		case 3:
			color_Quotation = clNewForeGround;
			break;
		case 4:
			color_Define = clNewForeGround;
			break;
		case 5:
			color_MarkArea = clNewForeGround;
			break;
		case 6:
			color_SelTextColor = clNewForeGround;
			break;
		case 7:
			color_BreakPoint = clNewForeGround;
			break;
		case 8:
			color_BookMarks = clNewForeGround;
			break;
		default:
			return;
	}
	color_CurForeGround = clNewForeGround;
//	DrawEffect();
	m_Effect.InvalidateRect(NULL);
}

void CSysOption::OnSelchangeBackground() 
{
	// TODO: Add your control notification handler code here
	COLORREF selColor[]={
				CL_BLACK      ,     
				CL_DARKRED    ,    
				CL_DARKGREEN  ,  
				CL_DARKYELLOW , 
				CL_DARKBLUE   ,
				CL_DARKVIOLET ,
				CL_DARKCYAN   ,
				CL_DARKGRAY   ,
				CL_LIGHTGRAY  ,
				CL_RED        ,
				CL_GREEN      ,
				CL_YELLOW     ,
				CL_BLUE       ,
				CL_VIOLET     ,
				CL_CYAN       ,
				CL_WHITE      ,
				CL_WINDOW
						};
	
	int nID = m_cbxBackGround.GetCurSel();
	COLORREF clNewBackGround = selColor[nID];

	int nCurSel = m_lbxTextList.GetCurSel();
	switch(nCurSel)
	{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 7:
		case 8:
			color_NormalBKColor = clNewBackGround;
			break;
		case 6:
			color_SelBkColor = clNewBackGround;
			break;
		default:
			return;
	}	
	color_CurBackGround = clNewBackGround;
//	DrawEffect();
	m_Effect.InvalidateRect(NULL);
}

void CSysOption::OnReset() 
{
	// TODO: Add your control notification handler code here
		color_NormalText = DEFAULTCOLOR_NORMALTEXT;
		color_NormalBKColor = DEFAULTCOLOR_NORMALBKCOLOR;
		color_KeyWord =DEFAULTCOLOR_KEYWORDTEXT;
		color_Mask = DEFAULTCOLOR_MASKTEXT;
		color_Quotation = DEFAULTCOLOR_QUOTETEXT;//RGB(0xf0,0x00,0x0f);
		color_Define = DEFAULTCOLOR_DEFINE;//RGB(0x00,0x7f,0x00);
		color_MarkArea = DEFAULTCOLOR_MARKAREA;
		color_SelBkColor = DEFAULTCOLOR_SELBKCOLOR;
		color_SelTextColor = DEFAULTCOLOR_SELTEXTCOLOR;
		color_BreakPoint = DEFAULTCOLOR_BREAKPOINT;
		color_BookMarks = DEFAULTCOLOR_BOOKMARKS;	

		m_lbxTextList.SetCurSel(0);
		OnSelchangeTextlist();
//		DrawEffect();
		m_Effect.InvalidateRect(NULL);
}

void CSysOption::DrawEffect(HDC hdc,RECT rect)
{
	COLORREF clFore,clBack;

	clBack = color_CurBackGround;
	if (clBack == CL_WINDOW)
		clBack = GetSysColor(COLOR_WINDOW);
	clFore = color_CurForeGround;
	if (clFore == CL_WINDOW)
		clFore = GetSysColor(COLOR_WINDOWTEXT);

	HBRUSH hBrush = CreateSolidBrush(clBack);

	hBrush = (HBRUSH)SelectObject(hdc,hBrush);
	Rectangle(hdc,rect.left,rect.top,rect.right,rect.bottom);

	InflateRect(&rect,-10,-10);

	int iOldMode = SetBkMode(hdc, TRANSPARENT ) ;
	clFore = SetTextColor(hdc,clFore);
	DrawText(hdc,"Kingmos",7,&rect,DT_CENTER);
	SetTextColor(hdc,clFore);
	SetBkMode(hdc, iOldMode ) ;

	hBrush = (HBRUSH)SelectObject(hdc,hBrush);

	DeleteObject(hBrush);
//	ReleaseDC(pdc);
}

void CSysOption::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here	
//	DrawEffect();
	// Do not call CDialog::OnPaint() for painting messages
}
