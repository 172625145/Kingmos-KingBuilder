// SpecialEdit.cpp : implementation file
//

#include "stdafx.h"
//#include "TestMyEdit.h"
#include "SpecialEdit.h"
#include "SpecialView.h"
#include "Ambo.h"
#include "MainFrame.h"
#include "Infomation.h"
#include <atlbase.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#define CHARWIDTH		  8
#define SCREEN_CHARWIDTH  80
#define MAX_CHARWIDTH     2048

#define MAX_LINELENGTH    1024

#define WHOLEINSELECTION	1
#define WHOLENOTINSELECTION	2
#define HALFINSELECTION		3


static LPCTSTR Sys_KeyWord[]={
	_T("__asm"),
	_T("enum"),
	_T("__multiple_inheritance"),
	_T("template"),
	_T("auto"),
	_T("__except"),
	_T("__single_inheritance"),
	_T("this"),
	_T("__based"),
	_T("explicit"),
	_T("__virtual_inheritance"),
	_T("thread"),
	_T("bool"),
	_T("extern"),
	_T("mutable"),
	_T("throw"),
	_T("break"),
	_T("false"),
	_T("naked"),
	_T("true"),
	_T("case"),
	_T("__fastcall"),
	_T("namespace"),
	_T("try"),
	_T("catch"),
	_T("__finally"),
	_T("new"),
	_T("__try"),
	_T("__cdecl"),
	_T("float"),
	_T("operator"),
	_T("typedef"),
	_T("char"),
	_T("for"),
	_T("private"),
	_T("typeid"),
	_T("class"),
	_T("friend"),
	_T("protected"),
	_T("typename"),
	_T("const"),
	_T("goto"),
	_T("public"),
	_T("union"),
	_T("const_cast"),
	_T("if"),
	_T("register"),
	_T("unsigned"),
	_T("continue"),
	_T("inline"),
	_T("reinterpret_cast"),
	_T("using"),
	_T("__declspec"),
	_T("__inline"),
	_T("return"),
	_T("uuid"),
	_T("default"),
	_T("int"),
	_T("short"),
	_T("__uuidof"),
	_T("delete"),
	_T("__int8"),
	_T("signed"),
	_T("virtual"),
	_T("dllexport"),
	_T("__int16"),
	_T("sizeof"),
	_T("void"),
	_T("dllimport"),
	_T("__int32"),
	_T("static"),
	_T("volatile"),
	_T("do"),
	_T("__int64"),
	_T("static_cast"),
	_T("wmain"),
	_T("double"),
	_T("__leave"),
	_T("__stdcall"),
	_T("while"),
	_T("dynamic_cast"),
	_T("long"),
	_T("struct"),
	_T("xalloc"),
	_T("else"),
	_T("main"),
	_T("switch"),
	_T("interface"),
	_T("persistent"),
	_T("_persistent"),
	_T("transient"),
	_T("depend"),
	_T("ondemand"),
	_T("transient"),
	_T("cset"),
	_T("useindex"),
	_T("indexdef"),
	NULL
};

/////////////////////////////////////////////////////////////////////////////
// CSpecialEdit
DWORD CSpecialEdit::dwCharSet = -1;

CSpecialEdit::CSpecialEdit()
{
	crFlag="\x0d\x0a";

/*
	color_NormalText = DEFAULTCOLOR_NORMALTEXT;
	color_KeyWord =DEFAULTCOLOR_KEYWORDTEXT;
	color_Mask = DEFAULTCOLOR_MASKTEXT;
	color_Quotation = DEFAULTCOLOR_QUOTETEXT;//RGB(0xf0,0x00,0x0f);
	color_Define = DEFAULTCOLOR_DEFINE;//RGB(0x00,0x7f,0x00);
	color_MarkArea = DEFAULTCOLOR_MARKAREA;
	color_SelBkColor = DEFAULTCOLOR_SELBKCOLOR;
	color_SelTextColor = DEFAULTCOLOR_SELTEXTCOLOR;
	color_BreakPoint = DEFAULTCOLOR_BREAKPOINT;
	color_BookMarks = DEFAULTCOLOR_BOOKMARKS;
*/
	ReadSysColor();

	m_nMarkAreaWidth = 24;
	m_bMatchAllWordOnly = FALSE;
	m_bMatchCase = FALSE;
	iDirection = DIRECTION_DOWN;

	m_bFocus = FALSE;

	InitializeEditStatus();

	rootUndoLink = new UNDOLINK;

	InitUndoLinkStruct( rootUndoLink );
	curUndoLink = rootUndoLink;

	m_nCharWidth = 8;
	m_nCharHeight = 16;

// 2005-08-22
	if( dwCharSet == -1 )
	{
		TCHAR szCharset[64];
		if( LoadString( GetModuleHandle(NULL), IDS_CHARSET, szCharset, sizeof(szCharset) ) )
		{
			if( stricmp( szCharset, "GB2312_CHARSET" ) == 0 )
			{
				dwCharSet = GB2312_CHARSET;
			}
			else if( stricmp( szCharset, "CHINESEBIG5_CHARSET" ) == 0 )
			{
				dwCharSet = CHINESEBIG5_CHARSET;
			}
			else if( stricmp( szCharset, "ANSI_CHARSET" ) == 0 )
			{
				dwCharSet = ANSI_CHARSET;
			}
			else
			{
				dwCharSet = DEFAULT_CHARSET;
			}			
		}
		else
		{
			dwCharSet = DEFAULT_CHARSET;
		}
	}

	LOGFONT lfFont;
//	TEXTMETRIC textmetric;
	memset(&lfFont, 0, sizeof(lfFont));
	lstrcpy(lfFont.lfFaceName, _T("FixedSys"));
	lfFont.lfHeight = 16;
	lfFont.lfWidth = 8;
	lfFont.lfWeight = FW_NORMAL;
	lfFont.lfItalic = FALSE;
	lfFont.lfCharSet = (BYTE)dwCharSet;//GB2312_CHARSET;//DEFAULT_CHARSET;
	lfFont.lfOutPrecision = OUT_RASTER_PRECIS;//OUT_DEFAULT_PRECIS;
	lfFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lfFont.lfQuality = DEFAULT_QUALITY;
	lfFont.lfPitchAndFamily = DEFAULT_PITCH;

	m_FixEditFont.CreateFontIndirect(&lfFont);

	
}

CSpecialEdit::~CSpecialEdit()
{
	SaveSysColor();
	ClearData();
	DeleteUndoLink(rootUndoLink);
	rootUndoLink = curUndoLink = NULL;

	CMainFrame* pMainFrame ;
	pMainFrame = (CMainFrame*)((CAmboApp *)AfxGetApp())->m_pMainWnd;
	pMainFrame->SetActiveView(NULL,FALSE);
}


BEGIN_MESSAGE_MAP(CSpecialEdit, CWnd)
	//{{AFX_MSG_MAP(CSpecialEdit)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_VSCROLL()
	ON_WM_SHOWWINDOW()
	ON_WM_SETFOCUS()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONUP()
	ON_WM_HSCROLL()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KILLFOCUS()
	ON_WM_SETCURSOR()
	ON_WM_SIZE()
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSpecialEdit private function

// 插入一个新行
void CSpecialEdit::InsertNewLine( int nLineIndex, LPCTSTR pszChars, int nLength )
{
	LineInfo LineInfo;

	if (pszChars != NULL)
	{
		if (nLength == -1)
			LineInfo.m_strLine = new CString(pszChars);
		else
			LineInfo.m_strLine = new CString(pszChars,nLength);
	}
	else
	{
		LineInfo.m_strLine = new CString;
	}
	m_aDataLine.InsertAt(nLineIndex,LineInfo);
}

// 删除指定行
void CSpecialEdit::DeleteLine( int nLineIndex )
{
}

// 在指定行中插入多个字符
// return : 实际插入的字符个数
int CSpecialEdit::InsertCharInLine( int nLineIndex, LPCTSTR pszChars, int nLength , int nPosition  )
{
	LineInfo LineInfo;
	int nLineCount=GetTextLineCount();
	int nLineLength = 0, nNeedMaxLength = 0;
	int nInsertCharNum = nLength;

//	TestError();
	if ( nLength == 0 )
		return 0;
	if (nLineIndex > nLineCount)
		return 0;
	if (nLineIndex == nLineCount)
	{
		InsertNewLine(nLineIndex);
		RecalcVertScrollBar();
	}

	LineInfo=m_aDataLine.GetAt(nLineIndex);
	
//	nLineLength = LineInfo.m_strLine->GetLength();
	nLineLength = GetnCharExtern(LineInfo.m_strLine->LockBuffer());
	int nInsertLen = GetnCharExtern(pszChars,nLength);

//	if (nLineLength + nLength > MAX_LINELENGTH)
	if (nLineLength + nInsertLen > MAX_LINELENGTH)
	{
		nNeedMaxLength = MAX_LINELENGTH - nLineLength;
		nInsertCharNum = 0;
		LPTSTR lpCurPos = (LPTSTR)pszChars;
		int nCharLen = 0;
		nInsertLen = 0;
		while(*lpCurPos)
		{
			if ( *lpCurPos == '\t' )
			{
				nInsertLen += m_nTabSize;
				nCharLen = 1;
			}
//			else if ( *lpCurPos < 0 || *lpCurPos >0x7f )
			else if (IsChinese(lpCurPos))
			{
				nInsertLen += 2;
				nCharLen = 2;
			}
			else
			{
				nInsertLen ++;
				nCharLen = 1;
			}

			if (nInsertLen > nNeedMaxLength)
				break;
//			if (nLength + nCharLen > nNeedMaxLength)
//				break;
			nInsertCharNum += nCharLen;
			lpCurPos += nCharLen;
			if (nInsertCharNum >= nLength)
			{
				MessageBox("Had Some Error occur");
			}
		}
	}

	if (nInsertCharNum > 0)
	{
		CString InsertString(pszChars,nInsertCharNum);

		LineInfo.m_strLine->Insert(nPosition,InsertString.LockBuffer());
		InsertString.UnlockBuffer();
	}
	if (nInsertCharNum < nLength)
	{
		//MessageBox (" 到达最大长度 ");
		WarningCanntCompleteAction();
		SetFocus();
	}
//	TestError();
	return nInsertCharNum;
}

//在指定行中删除多个字符
void CSpecialEdit::DeleteCharInLine( int nLineIndex, int nLength, int nPosition  )
{
		LineInfo LineInfo = m_aDataLine.GetAt(nLineIndex);
	
		LineInfo.m_strLine->Delete(nPosition,nLength );
}

// 重绘指定行的指定区域
void CSpecialEdit::DrawLine( CDC *pdc, int nLineIndex)
{
	LineInfo LineInfo;
	LPTSTR lpLine;
	CString strWord;
	int x,y;
	CPoint pt_CurPos;
	COLORREF color_OrgWord,color_OrgBackColor;
	int nNeedShowpos = 0;
	int nScreenWidth = 0;

	RECT rect;
	int bDefineStart = FALSE;

		HideEditCaret();
		GetClientRect(&rect);

		nScreenWidth = rect.right - rect.left;
		if (m_aDataLine.GetSize()<=nLineIndex)
		{// This is have no data , But Must Clear it

			rect.top=(nLineIndex-GetFirstVisibleLine()) * GetLineHeight();
			rect.bottom=rect.top+GetLineHeight();
			rect.left=m_nMarkAreaWidth;
//			pdc->FillSolidRect(&rect,GetSysColor(COLOR_WINDOW));
			pdc->FillSolidRect(&rect,GetEditBkColor());
			ShowEditCaret();
			return;
		}
		int nScreenCount = GetScreenCount();
		if (nLineIndex < m_nTopLine || nLineIndex > m_nTopLine + nScreenCount)
		{
			// Is Not In the Screen , not display
			ShowEditCaret();
			return ;
		}
		//pdc->SetBkColor(GetSysColor(COLOR_WINDOW));
//		color_OrgBackColor = GetSysColor(COLOR_WINDOW);
		color_OrgBackColor = GetEditBkColor();
		SetTextEditFont(pdc);
		LineInfo=m_aDataLine.GetAt(nLineIndex);

		m_bSingleMask = GetLineSingleMaskFlag(LineInfo);
		m_bMultiMask =  GetLineMultiMaskFlag(LineInfo);
		m_bDQuotation = GetLineDoubleQuotationFlag(LineInfo);
		m_bSingleQuotation = GetLineSingleQuotationFlag(LineInfo);


//		x=m_nMarkAreaWidth;
		x=m_nMarkAreaWidth - m_nOffsetChar * m_nCharWidth;
//		x=m_nMarkAreaWidth - GetnCharHead2Visible(nLineIndex) * m_nCharWidth;
		y=(nLineIndex-GetFirstVisibleLine()) * GetLineHeight();
		pt_CurPos.y = nLineIndex;
		pt_CurPos.x = 0;
		lpLine=LineInfo.m_strLine->LockBuffer();
		while(TRUE)
		{

			if (x > nScreenWidth)
				break;
			strWord=GetWord(lpLine);
			if (strWord.GetLength() == 0)
				break;
			nNeedShowpos = 0;
			// Normal Text
			//pdc->SetTextColor(GetWordColor(strWord));
			if (bDefineStart == FALSE)
			{
				if (m_bDQuotation == FALSE && m_bSingleQuotation == FALSE &&
					m_bMultiMask == FALSE && m_bSingleMask == FALSE )
				{
					if (IsDefineWord(strWord) == TRUE)
						bDefineStart  = TRUE;
				}
				color_OrgWord = GetWordColor(strWord);
			}
			else
			{
				color_OrgWord = color_NormalText;//GetWordColor(strWord);
			}

			if (*lpLine == '/')
			{
				if (m_bMultiMask == FALSE)  //  '/*  */' Mask
				{
					if (IsMultiMskStart(lpLine))  // x != 0 is because the end must not the first char
					{
						m_bMultiMask = TRUE;
						strWord += "*";
					}
//					pdc->SetTextColor(color_Mask);
				}
				if (m_bSingleMask == FALSE && m_bMultiMask == FALSE)  //  '//' Mask
				{
					if (IsSingleMaskStart(lpLine))  // x != 0 is because the end must not the first char
					{
						m_bSingleMask = TRUE;
						strWord += "/";
					}
				}
			}
			else if (*lpLine == '*')
			{
				if (m_bMultiMask == TRUE)  //  '/*  */' Mask
				{
					color_OrgWord = color_Mask ;
					if (IsMultiMskEnd(lpLine))  
					{
						m_bMultiMask = FALSE;
						strWord += "/";
//						lpCurPos +=2;  // '*/*' ,then the second * is not Start Mask
//						continue;
					}
				}
			}
			else if (bDefineStart == TRUE)
			{ // if the Define mode ,then nothing do ,will show normal
			}
			else if (IsDoubleQuotation(lpLine) ==TRUE )
			{
				if (m_bDQuotation == TRUE)
				{
					//pdc->SetTextColor(color_Quotation);
					color_OrgWord = color_Quotation;
					m_bDQuotation = FALSE;
				}
				else
					m_bDQuotation = TRUE;
			}
			else if (IsSingleQuotation(lpLine) == TRUE)
			{
				if (m_bSingleQuotation == TRUE)
				{
					//pdc->SetTextColor(color_Quotation);
					color_OrgWord = color_Quotation;
					m_bSingleQuotation = FALSE;
				}
				else
					m_bSingleQuotation = TRUE;
			}
			if (m_bMultiMask == TRUE)  //  '/*  */' Mask
			{
				//pdc->SetTextColor(color_Mask);
				color_OrgWord = color_Mask;
			}
			else if (m_bSingleMask == TRUE)  //  '//' Mask
			{
				//pdc->SetTextColor(color_Mask);
				color_OrgWord = color_Mask;
			}
			else if (m_bDQuotation == TRUE)  //  "   "  
			{
				//pdc->SetTextColor(color_Quotation);
				color_OrgWord = color_Quotation;
			}
			else if (m_bSingleQuotation == TRUE)  // '   '
			{
				//pdc->SetTextColor(color_Quotation);
				color_OrgWord = color_Quotation;
			}

			if (IsSpace(strWord))
			{
				int nWordLen=strWord.GetLength();
				int nCount = 0;
				int nWordinSel = 0;
				LPTSTR lpShow = lpLine;

					nWordinSel = WordInSelection(pt_CurPos,strWord);
					if (nWordinSel == HALFINSELECTION )
					{
						while (1)
						{
							if (x > nScreenWidth)
								break;
							if (nWordLen == 0)
								break;
							if (*lpLine == '\t') // Is Chinese
							{// Is tab code
								nCount = m_nTabSize;
								lpShow = "    ";
							}
							else
							{
								nCount = 1;
								lpShow = lpLine;
							}
							if (IsSelection(pt_CurPos)==TRUE)
							{
								pdc->SetBkColor(color_SelBkColor);
								pdc->SetTextColor(color_SelTextColor);
							}
							else
							{
								pdc->SetBkColor(color_OrgBackColor);
								pdc->SetTextColor(color_OrgWord);
							}
							if (x + nCount * m_nCharWidth > m_nMarkAreaWidth)
								pdc->TextOut(x,y,lpShow,nCount);
							lpLine ++;
							nWordLen --;
							x += nCount * m_nCharWidth ;
							pt_CurPos.x ++;
						}
					}
					else
					{
						lpLine +=strWord.GetLength();  // the '\t' only is a byte
						pt_CurPos.x += strWord.GetLength();
						ExternWord(strWord,strWord);
						if (IsSelection(pt_CurPos)==TRUE)
						{
							pdc->SetBkColor(color_SelBkColor);
							pdc->SetTextColor(color_SelTextColor);
						}
						else
						{
							pdc->SetBkColor(color_OrgBackColor);
							pdc->SetTextColor(color_OrgWord);
						}				
						// The Word Need Display
						if (x<m_nMarkAreaWidth)
						{
							if ((x + GetWordWidth(pdc,strWord)) < m_nMarkAreaWidth )
							{
								// Is Not Show This word
								nNeedShowpos = -1;
							}
							else
							{
								nNeedShowpos = (m_nMarkAreaWidth - x) / m_nCharWidth;
							}
						}
						if (nNeedShowpos >= 0 )
						{
							pdc->TextOut(x + nNeedShowpos * m_nCharWidth,y,(strWord.LockBuffer() + nNeedShowpos));
							strWord.UnlockBuffer();
						}
						x += GetWordWidth(pdc,strWord);
					}
			}
			else
			{
				int nWordLen=strWord.GetLength();
				int nCount = 0;
				int nWordinSel = 0;

					nWordinSel = WordInSelection(pt_CurPos,strWord);
					if (nWordinSel == HALFINSELECTION )
					{
						while (1)
						{
							if (x > nScreenWidth)
								break;
							if (nWordLen == 0)
								break;
//							if (*lpLine<0 || *lpLine >0x7f) // Is Chinese
							if (IsChinese(lpLine))
								nCount = 2;
							else
								nCount = 1;
							if (IsSelection(pt_CurPos)==TRUE)
							{
								pdc->SetBkColor(color_SelBkColor);
								pdc->SetTextColor(color_SelTextColor);
							}
							else
							{
								pdc->SetBkColor(color_OrgBackColor);
								pdc->SetTextColor(color_OrgWord);
							}
							if (x + nCount * m_nCharWidth > m_nMarkAreaWidth)
								pdc->TextOut(x,y,lpLine,nCount);
							lpLine += nCount;
							nWordLen -=nCount;
							x += nCount * m_nCharWidth ;
							pt_CurPos.x += nCount;
						}
					}
					else
					{
						if (nWordinSel == WHOLEINSELECTION )
						{
								pdc->SetBkColor(color_SelBkColor);
								pdc->SetTextColor(color_SelTextColor);
						}
						else
						{
								pdc->SetBkColor(color_OrgBackColor);
								pdc->SetTextColor(color_OrgWord);
						}
//						if (x>=m_nMarkAreaWidth)
//							pdc->TextOut(x,y,strWord,nWordLen);
						if (x<m_nMarkAreaWidth)
						{
							if ((x + GetWordWidth(pdc,strWord)) < m_nMarkAreaWidth )
							{
								// Is Not Show This word
								nNeedShowpos = -1;
							}
							else
							{
								int nHideLen= (m_nMarkAreaWidth - x) / m_nCharWidth;
								//nNeedShowpos = (m_nMarkAreaWidth - x) / m_nCharWidth;
								nNeedShowpos =0;
								LPTSTR lpWord = strWord.LockBuffer();
								while(*lpWord)
								{
//									if (*lpWord <0 || *lpWord >0x7f)
									if (IsChinese(lpWord))
									{
										nNeedShowpos +=2;
										lpWord += 2;
									}
									else
									{
										nNeedShowpos ++;
										lpWord ++ ;
									}
									if (nNeedShowpos >= nHideLen -2)
										break;
								}
							}
						}
						if (nNeedShowpos >= 0 )
						{
							pdc->TextOut(x + nNeedShowpos * m_nCharWidth,y,(strWord.LockBuffer() + nNeedShowpos));
							strWord.UnlockBuffer();
						}
						pt_CurPos.x += nWordLen;
						lpLine += nWordLen;
						x += GetWordWidth(pdc,strWord);
					}
			}
		}
		LineInfo.m_strLine->UnlockBuffer();

		if (x < m_nMarkAreaWidth)
			x = m_nMarkAreaWidth;
//		RECT rect;
//		GetClientRect(&rect);
		rect.top=y;
		rect.bottom=y+GetLineHeight();
		rect.left=x;
//		pdc->FillSolidRect(&rect,GetSysColor(COLOR_WINDOW));
		pdc->FillSolidRect(&rect,GetEditBkColor());
		ShowMarkFlag(pdc,nLineIndex);
		ShowEditCaret();
		//DrawMarkArea(pdc,nLineIndex);
//	pdc->TextOut(0,nLineIndex*16,*(m_aDataLine.GetAt(nLineIndex)).m_strLine);
}

// 得到要显示的指定位置字符的颜色
COLORREF CSpecialEdit::GetCharColor( CPoint ptPos )
{
	return RGB(0,0,0);
}

// 得到选择区间
void CSpecialEdit::GetSelection ( CPoint &ptSelStart, CPoint &ptSelEnd )
{
	ptSelStart = m_ptSelStart;
	ptSelEnd = m_ptSelEnd;
}

// 设置选择区间
void CSpecialEdit::SetSelection ( CPoint ptSelStart, CPoint ptSelEnd )
{
//	ASSERT_VALIDTEXTPOS(ptStart);
//	ASSERT_VALIDTEXTPOS(ptEnd);
	CPoint pt_OldSelStart,pt_OldSelEnd;

	pt_OldSelStart = m_ptSelStart;
	pt_OldSelEnd = m_ptSelEnd;

	m_ptSelStart = ptSelStart;
	m_ptSelEnd = ptSelEnd;
	if (pt_OldSelStart == ptSelStart)
	{
		if (pt_OldSelEnd != ptSelEnd)
		{
//			HideEditCaret();
			InvalidateLines(ptSelEnd.y, pt_OldSelEnd.y);
//			ShowEditCaret();
		}
	}
	else
	{
//		HideEditCaret();
		InvalidateLines(ptSelStart.y, ptSelEnd.y);
		InvalidateLines(pt_OldSelStart.y, pt_OldSelEnd.y);
//		ShowEditCaret();
	}
}

// 删除选择区间
//void CSpecialEdit::DeleteSelection ( void )
void CSpecialEdit::DeleteSelection (BOOL bShow, LPUNDOLINK lpUndoLink )
{
	CPoint pt_SelStart,pt_SelEnd;

	if (m_ptSelStart == m_ptSelEnd)
		return;
	
	if (lpUndoLink)
	{
		lpUndoLink->iProcessStyle |= DELETEPROCESS;
		lpUndoLink->startDelete = m_ptSelStart;
		lpUndoLink->endDelete = m_ptSelEnd;
		GetText(m_ptSelStart, m_ptSelEnd, *lpUndoLink->strDelete);
	}
//	TestError();
	// Adjust the Selection Start and End Position
	if (m_ptSelStart.y > m_ptSelEnd.y)
	{
		pt_SelStart = m_ptSelEnd;
		pt_SelEnd = m_ptSelStart;
	}
	else if (m_ptSelStart.y < m_ptSelEnd.y)
	{
		pt_SelStart = m_ptSelStart;
		pt_SelEnd = m_ptSelEnd;
	}
	else
	{
		if (m_ptSelStart.x > m_ptSelEnd.x)
		{
			pt_SelStart = m_ptSelEnd;
			pt_SelEnd = m_ptSelStart;
		}
		else
		{
			pt_SelStart = m_ptSelStart;
			pt_SelEnd = m_ptSelEnd;
		}
	}

	if (pt_SelStart.y == pt_SelEnd.y)
	{
//		LineInfo LineInfo = m_aDataLine.GetAt(pt_SelStart.y);
	
//		LineInfo.m_strLine->Delete(pt_SelStart.x,pt_SelEnd.x - pt_SelStart.x );
//		lpUndoLink->strDelete.Insert(0,
		DeleteCharInLine(pt_SelStart.y,pt_SelEnd.x - pt_SelStart.x,pt_SelStart.x);
		m_ptSelStart = m_ptSelEnd =pt_SelStart;
		m_ptCursor=m_ptSelStart;
//		InvalidateLines(pt_SelStart.y,pt_SelStart.y);
//		HideEditCaret();
//		CursorPos2CaretPos();
//		ShowEditCaret();
	}
	else
	{
		LineInfo LineInfo1,LineInfo2;
		LineInfo1=m_aDataLine.GetAt(pt_SelStart.y);
		LineInfo2=m_aDataLine.GetAt(pt_SelEnd.y);

		int nDelCount=LineInfo1.m_strLine->GetLength() - pt_SelStart.x;
		LineInfo1.m_strLine->Delete(pt_SelStart.x,nDelCount);  // delete the first line Sel
		LineInfo2.m_strLine->Delete(0,pt_SelEnd.x); // Delete the last Line Sel

		
//		for (int i=pt_SelStart.y+1; i<=pt_SelEnd.y;i++)
		for (int i=pt_SelStart.y+1; i<pt_SelEnd.y;i++)
		{
			LineInfo2=m_aDataLine.GetAt(i);
			delete LineInfo2.m_strLine;
		}
		nDelCount=pt_SelEnd.y-pt_SelStart.y-1;
		m_aDataLine.RemoveAt(pt_SelStart.y + 1, nDelCount);
		if (CanMergeNextLine(pt_SelStart.y)==TRUE)
		{
			LineInfo2=m_aDataLine.GetAt(pt_SelStart.y + 1);
			LineInfo1.m_strLine->Insert(pt_SelStart.x,LineInfo2.m_strLine->LockBuffer());  // Linkage the last Sel Line the Remain chars
			LineInfo2.m_strLine->UnlockBuffer();
			delete LineInfo2.m_strLine;
			m_aDataLine.RemoveAt(pt_SelStart.y+1);
		}
		else
		{
			WarningCanntCompleteAction();
		}
		
		m_ptSelStart = m_ptSelEnd = pt_SelStart;
		m_ptCursor.y=pt_SelStart.y;
		m_ptCursor.x=pt_SelStart.x;
		RecalcVertScrollBar();
//		InvalidateLines(pt_SelStart.y);
//		HideEditCaret();
//		CursorPos2CaretPos();
//		ShowEditCaret();
//		EnsureVisible();
	}
	ParseText(m_ptCursor.y,TRUE);
	if (bShow)
	{
		InvalidateLines(pt_SelStart.y);
		CursorPos2CaretPos();
		EnsureVisible();
	}
	//bModified = TRUE;
	SetModify(TRUE);
//	TestError();
}

// 指定位置字符是否在选择区间内
BOOL CSpecialEdit::IsSelection ( CPoint ptPos )
{
	CPoint pt_SelStart,pt_SelEnd;

	// Adjust the Selection Start and End Position
	if (m_ptSelStart.y > m_ptSelEnd.y)
	{
		pt_SelStart = m_ptSelEnd;
		pt_SelEnd = m_ptSelStart;
	}
	else if (m_ptSelStart.y < m_ptSelEnd.y)
	{
		pt_SelStart = m_ptSelStart;
		pt_SelEnd = m_ptSelEnd;
	}
	else
	{
		if (m_ptSelStart.x > m_ptSelEnd.x)
		{
			pt_SelStart = m_ptSelEnd;
			pt_SelEnd = m_ptSelStart;
		}
		else
		{
			pt_SelStart = m_ptSelStart;
			pt_SelEnd = m_ptSelEnd;
		}
	}
	if (ptPos.y>=pt_SelStart.y && ptPos.y <= pt_SelEnd.y)
	{
		if (ptPos.y==pt_SelStart.y)
		{
			if (ptPos.x >= pt_SelStart.x)
			{
				if (ptPos.y==pt_SelEnd.y)
				{
					if (ptPos.x < pt_SelEnd.x)
					{
						return TRUE;
					}
					else
					{
						return FALSE;
					}
				}
				else
				{
					return TRUE;
				}
			}
			else
			{
				return FALSE;
			}
		}
		else if (ptPos.y==pt_SelEnd.y)
		{
			if (ptPos.x < pt_SelEnd.x)
			{
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
		else
		{
			return TRUE;
		}
	}
	return FALSE;
}

// 得到当前屏幕可显示的行数
int CSpecialEdit::GetScreenCount ( void )
{
	int nLineHeight,nScreenHeight;
	RECT rect;

	nLineHeight=GetLineHeight();
	GetClientRect(&rect);
	nScreenHeight=rect.bottom-rect.top;
	return (nScreenHeight+nLineHeight-1)/nLineHeight;
}

// 得到第一个可看到的行索引
int CSpecialEdit::GetFirstVisibleLine ( void )
{
	return m_nTopLine;
}

// 确保光标位置字符可见
void CSpecialEdit::EnsureVisible ()
{
	// the vert 
	if (m_ptCursor.y < m_nTopLine)
	{
		ScrollToLine(m_ptCursor.y);
		//return ;
	}
	int nScreenCount = GetScreenCount();
	if (m_ptCursor.y >= m_nTopLine + nScreenCount - 1)
	{
		int nNewTopLine = m_ptCursor.y - nScreenCount + 2;
		ScrollToLine(nNewTopLine);
	}

	// the horz
	int nCursorPos = GetExternCharLength(m_ptCursor);
//	if (m_ptCursor.x < m_nOffsetChar)
	if (nCursorPos < m_nOffsetChar)
	{
//		ScrollToHorz(m_ptCursor.x);
		ScrollToHorz(nCursorPos);
	}

	int nScreenCharCount = GetScreenCharCount();
	// the tab code is width with four char, so the tab key must calc 4 char
//	GetExternCharLength(m_ptCursor.y);
//	if (m_ptCursor.x >= m_nOffsetChar + nScreenCharCount - 1)
	if (nCursorPos >= m_nOffsetChar + nScreenCharCount - 1 )
	{
//		int nNewLeftChar = m_ptCursor.x - nScreenCharCount + 2;
		int nNewLeftChar = GetCursorVisiblePos();
		ScrollToHorz(nNewLeftChar);
	}
}
// 得到所有行中的最多字符的那一行的字符数
int CSpecialEdit::GetMaxCharCount ( void )
{
	int nLineCount = GetTextLineCount();
	int i=0;
	int nMaxLineLength=0,nLineLength;

		for (i=0;i<nLineCount;i++)
		{
			LineInfo LineInfo = m_aDataLine.GetAt(i);

			nLineLength = LineInfo.m_strLine->GetLength();
			if (nLineLength > nMaxLineLength)
				nMaxLineLength = nLineLength;
		}
		return nMaxLineLength;
}


/////////////////////////////////////////////////////////////////////////////
// CSpecialEdit message handlers

#define BACKSPACE  0x08
void CSpecialEdit::OnChar(int nChar, int nRepCnt, int nFlags) 
{
	// TODO: Add your message handler code here and/or call default
//	CDC *pdc;
	LPUNDOLINK newUndoLink = (LPUNDOLINK)malloc(sizeof(UNDOLINK));

	InitUndoLinkStruct( newUndoLink );

	if (nChar=='\r')  // Is CR
	{
		//m_ptCursor.y++;
		//InsertNewLine(m_ptCursor.y);
		DeleteSelection(TRUE,newUndoLink);

		newUndoLink->startInsert = m_ptCursor;
		newUndoLink->iProcessStyle |= INSERTPROCESS;
		int xPos = InsertCR(m_ptCursor,TRUE,newUndoLink);
		ParseText(m_ptCursor.y,TRUE);
		m_ptCursor.y++;
		m_ptCursor.x=xPos;

		newUndoLink->endInsert = m_ptCursor;
		//		InvalidateRect(NULL);
		RecalcVertScrollBar();
		InvalidateLines(m_ptCursor.y-1, -1);
//		HideEditCaret();
		CursorPos2CaretPos();
//		ShowEditCaret();
		EnsureVisible();
		//bModified = TRUE;
		SetModify(TRUE);
	
		{
			LPTSTR lpLine;
			CString strWord;
//			int nChar;

			LineInfo LineInfo2 = m_aDataLine.GetAt(m_ptCursor.y);

			lpLine =LineInfo2.m_strLine->LockBuffer();
			GetFirstValidWord(lpLine,strWord);
			LineInfo2.m_strLine->UnlockBuffer();
			if (strWord.Compare("{")==0 || strWord.Compare("}")==0)
			{
				nChar = strWord.GetAt(0);
			}
			if (strWord.Compare("case")==0 || strWord.Compare("default")==0)
			{
				nChar = ':';
			}
		}
	}
	else if(nChar == BACKSPACE) 
	{
		BackSpace(m_ptCursor,newUndoLink);
	}
	else
	{
//		TestError();
		if(nChar == '\t' && ProcessTab())
		{
			// 已经将需要处理的TAB功能处理完成。
			SetModify(TRUE);
		}
		else if (nChar >= 0x20 || nChar == '\t')  // The Char Must is visible char or Tab 
		{
			if (nChar >= 0x80 || bIsChinese == TRUE)  // the char is Chinese
			{
				if (bIsChinese== FALSE)
					bIsChinese = TRUE;
				else
					bIsChinese = FALSE;
				if (bIsChinese == TRUE)
				{
					ChineseChar[0] = nChar;
					ChineseChar[1] = 0;
				}
				else
				{
					ChineseChar[1] = nChar;
					ChineseChar[2] = 0;
					// Insert a chinese
//  					InsertCharInLine(m_ptCursor.y,(const char *)ChineseChar,2,m_ptCursor.x); 
					//ParseText();
//					m_ptCursor.x+=2;
					DeleteSelection(TRUE,newUndoLink);
					newUndoLink->iProcessStyle |= INSERTPROCESS | TYPING;
					newUndoLink->startInsert = m_ptCursor;
					newUndoLink->strInsert->Insert(0,ChineseChar);
					m_ptCursor.x+=InsertCharInLine(m_ptCursor.y,(const char *)ChineseChar,2,m_ptCursor.x); 
					newUndoLink->endInsert = m_ptCursor;
				}
			}
			else
			{
				if (bIsChinese == TRUE)
				{
					bIsChinese = FALSE;
					ChineseChar[0] = 0;
				}
//				InsertCharInLine(m_ptCursor.y,(const char *)&nChar,1,m_ptCursor.x);
				//ParseText();
//				m_ptCursor.x++;
				DeleteSelection(TRUE,newUndoLink);
				newUndoLink->iProcessStyle |= INSERTPROCESS | TYPING;
				newUndoLink->startInsert = m_ptCursor;
				newUndoLink->strInsert->Insert(0,nChar);
				m_ptCursor.x += InsertCharInLine(m_ptCursor.y,(const char *)&nChar,1,m_ptCursor.x);
				newUndoLink->endInsert = m_ptCursor;

			}
			if (bIsChinese == FALSE)
			{ // had half chinese char , must wait the next half char
				CursorPos2CaretPos();
				//InvalidateRect(NULL);
//				HideEditCaret();
//				pdc=GetDC();
				if (nChar == '/' || nChar == '*'|| nChar == '\\')
				{
					ParseText(m_ptCursor.y,FALSE);
					InvalidateLines(m_ptCursor.y);
					//DrawLine(pdc,m_ptCursor.y);
				}
				else
				{
//					InvalidateLines(m_ptCursor.y,1);
					InvalidateLines(m_ptCursor.y,m_ptCursor.y);
				}
//				DrawLine(pdc,m_ptCursor.y);
//				ReleaseDC(pdc);
//				ShowEditCaret();
				EnsureVisible();
				//bModified = TRUE;
				SetModify(TRUE);
			}
		}
	}
//	TestError();
	if (newUndoLink->iProcessStyle == NOPROCESS)
	{
		//delete newUndoLink;
		ReleaseUndoLink(newUndoLink);
	}
	else
	{
		newUndoLink->endCursor = m_ptCursor;
		InsertNewUndoLink(newUndoLink);
	}
	if (nChar == '{' || nChar == '}' || nChar == ':' || nChar == '\r')
	{
		AdjustIndent(nChar);
	}
	CWnd::OnChar(nChar, nRepCnt, nFlags);
}

void CSpecialEdit::OnKeyDown(int nChar, int nRepCnt, int nFlags) 
{
	// TODO: Add your message handler code here and/or call default
		switch(nChar)
		{
			case VK_PRIOR:// page up
				MovePageUp();
				break;
			case VK_NEXT:// page down
				MovePageDown();
				break;
			case VK_END://  to the text end
				MoveEnd();
				break;
			case VK_HOME:// to the text start
				MoveHome();
				break;
			case VK_LEFT:// to the left Char
				MoveLeft();
				break;
			case VK_UP:// to the up line
				MoveUp();
				break;
			case VK_RIGHT://  to the right Char
				MoveRight();
				break;
			case VK_DOWN:// to the line down
				MoveDown();
				break;
			case VK_DELETE: // delete char
				{
					LPUNDOLINK newUndoLink = (LPUNDOLINK)malloc(sizeof(UNDOLINK));

					InitUndoLinkStruct( newUndoLink );
					DeleteChar(newUndoLink);
					if (newUndoLink->iProcessStyle == NOPROCESS)
					{
						delete newUndoLink;
					}
					else
					{
						newUndoLink->endCursor = m_ptCursor;
						InsertNewUndoLink(newUndoLink);
					}
				}
				break;
			case VK_F2:
				ProcessF2();
				break;
			case VK_F3:
				ProcessF3();
				break;
			case 0xdd:
				AutoMatchBrace();
				break;
			case VK_F12:
				ResetAdjustIndent();
				break;
			default:
				DefaultKeyProcess(nChar);
				break;
		}
		CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CSpecialEdit::OnKeyUp(int nChar, int nRepCnt, int nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CWnd::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CSpecialEdit::OnLButtonDown(int nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (this !=GetCapture())
	{
		SetCapture();
		SetFocus();
	}
	//	HideEditCaret();
	m_ptCursor=ClientToText(point);
	CursorPos2CaretPos();
	//	m_ptSelStart=m_ptSelEnd=m_ptCursor;
	SetSelection(m_ptCursor,m_ptCursor);
	//	ShowEditCaret();
	EnsureVisible();

	CWnd::OnLButtonDown(nFlags, point);
}

BOOL CSpecialEdit::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	RECT rect;

//	FillRect(pdc->SetBkColor(GetSysColor(COLOR_WINDOW));
	GetClientRect(&rect);
	rect.left=m_nMarkAreaWidth;
//	pDC->FillSolidRect(&rect,GetSysColor(COLOR_WINDOW));
	pDC->FillSolidRect(&rect,GetEditBkColor());
	

	rect.left=0;
	rect.right=m_nMarkAreaWidth;
	pDC->FillSolidRect(&rect,color_MarkArea);

	return CWnd::OnEraseBkgnd(pDC);
}

void CSpecialEdit::OnVScroll(int nSBCode, int nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);

	//	Note we cannot use nPos because of its 16-bit nature
	SCROLLINFO si;
	si.cbSize = sizeof(si);
	si.fMask = SIF_ALL;
	VERIFY(GetScrollInfo(SB_VERT, &si));

	int nPageLines = GetScreenCount();
	int nLineCount = GetTextLineCount();

	int nNewTopLine;
//	BOOL bDisableSmooth = TRUE;
	switch (nSBCode)
	{
	case SB_TOP:
		nNewTopLine = 0;
//		bDisableSmooth = FALSE;
		break;
	case SB_BOTTOM:
		nNewTopLine = nLineCount - nPageLines + 1;
//		bDisableSmooth = FALSE;
		break;
	case SB_LINEUP:
		nNewTopLine = m_nTopLine - 1;
		break;
	case SB_LINEDOWN:
		nNewTopLine = m_nTopLine + 1;
		break;
	case SB_PAGEUP:
		nNewTopLine = m_nTopLine - si.nPage + 1;
//		bDisableSmooth = FALSE;
		break;
	case SB_PAGEDOWN:
		nNewTopLine = m_nTopLine + si.nPage - 1;
//		bDisableSmooth = FALSE;
		break;
	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		nNewTopLine = si.nTrackPos;
		break;
	default:
		return;
	}

	if (nNewTopLine < 0)
		nNewTopLine = 0;
	if (nNewTopLine >= nLineCount)
		nNewTopLine = nLineCount - 1;
	ScrollToLine(nNewTopLine);
}

void CSpecialEdit::OnShowWindow(BOOL bShow, int nStatus) 
{
	CWnd::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if (GetTextLineCount() ==0)
		InsertNewLine(0);
	RecalcVertScrollBar();
	RecalcHorzScrollBar();
}

void CSpecialEdit::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);
	// TODO: Add your message handler code here

	m_bFocus = TRUE;
	
//	((CAmboApp *)AfxGetApp())->ShowMessageAtDesktop(10,40,"OnSetFocus");
	ShowEditCaret();
	SetPosToStatusBar(m_ptCursor.y,m_ptCursor.x);
	
	CSpecialView * pView = (CSpecialView *)GetParent();

	CMainFrame* pMainFrame ;
	pMainFrame = (CMainFrame*)((CAmboApp *)AfxGetApp())->m_pMainWnd;
	pMainFrame->SetActiveView(pView);
}

void CSpecialEdit::OnRButtonUp(int nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CWnd::OnRButtonUp(nFlags, point);
}

void CSpecialEdit::OnRButtonDown(int nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CMenu TailMenu;
	CMenu *popupMenu;
	CRect rectMenu(0,0,100,80);

	
	ClientToScreen(&point);
	TailMenu.LoadMenu(IDR_EDITPOPUPMENU);
	popupMenu = TailMenu.GetSubMenu(0);    
	popupMenu->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON ,point.x,point.y,((CAmboApp *)AfxGetApp())->m_pMainWnd,&rectMenu);

	CWnd::OnRButtonDown(nFlags, point);
}

void CSpecialEdit::OnRButtonDblClk(int nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CWnd::OnRButtonDblClk(nFlags, point);
}

void CSpecialEdit::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here

	int nCount,nVisibleLine;
	int nLineHeight;
	int y;

		//nCount=m_aDataLine.GetSize();	
		nCount = GetScreenCount();
		if (nCount == 0)
			return ;

		nVisibleLine=GetFirstVisibleLine();
		//HideEditCaret();
		nLineHeight = GetLineHeight();
		for (int i=0; i<nCount ; i++)
		{
			y=i * nLineHeight;
			if (y>dc.m_ps.rcPaint.bottom)
				continue;
			if (y + nLineHeight < dc.m_ps.rcPaint.top)
				continue;
			DrawLine(&dc,i+nVisibleLine);
		}
		//if (bCaretCaret)
		//ShowEditCaret();
	// Do not call CWnd::OnPaint() for painting messages
}

void CSpecialEdit::OnMouseMove(int nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
//	::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_IBEAM)));
	if (this !=GetCapture())
	{
		return;
	}

//	HideEditCaret();
	m_ptCursor=ClientToText(point);
	CursorPos2CaretPos();
	SetSelection(m_ptSelStart,m_ptCursor);
//	m_ptSelEnd=m_ptCursor;
//	InvalidateRect(NULL);
//	ShowEditCaret();
	EnsureVisible();
	
	CWnd::OnMouseMove(nFlags, point);
}

BOOL CSpecialEdit::OnMouseWheel(int nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	SCROLLINFO si;
	si.cbSize = sizeof(si);
	si.fMask = SIF_ALL;
	VERIFY(GetScrollInfo(SB_VERT, &si));

	int nScrollLines;
	int nLineCount = GetTextLineCount();

	int nNewTopLine;

	nScrollLines = zDelta / GetLineHeight();

	nNewTopLine = si.nTrackPos - nScrollLines;

	if (nNewTopLine < 0)
		nNewTopLine = 0;
	if (nNewTopLine >= nLineCount)
		nNewTopLine = nLineCount - 1;
	ScrollToLine(nNewTopLine);
	
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void CSpecialEdit::OnLButtonUp(int nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (this ==GetFocus())
	{
		ReleaseCapture();
	}
	
	CWnd::OnLButtonUp(nFlags, point);
}

void CSpecialEdit::OnHScroll(int nSBCode, int nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);

	//	Note we cannot use nPos because of its 16-bit nature
	SCROLLINFO si;
	si.cbSize = sizeof(si);
	si.fMask = SIF_ALL;
	VERIFY(GetScrollInfo(SB_HORZ, &si));

//	int nPageLines = SCREEN_CHARWIDTH;
	int nLineCount = MAX_CHARWIDTH;

	int nNewLeftChar;
//	BOOL bDisableSmooth = TRUE;
	switch (nSBCode)
	{
	case SB_TOP:
		nNewLeftChar = 0;
//		bDisableSmooth = FALSE;
		break;
	case SB_BOTTOM:
		nNewLeftChar = nLineCount - si.nPage + 1;
//		bDisableSmooth = FALSE;
		break;
	case SB_LINEUP:    
		nNewLeftChar = m_nOffsetChar - 2;
		break;
	case SB_LINEDOWN:
		nNewLeftChar = m_nOffsetChar + 2;
		break;
	case SB_PAGEUP:
		nNewLeftChar = m_nOffsetChar - si.nPage + 1;
//		bDisableSmooth = FALSE;
		break;
	case SB_PAGEDOWN:
		nNewLeftChar = m_nOffsetChar + si.nPage - 1;
//		bDisableSmooth = FALSE;
		break;
	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		nNewLeftChar = si.nTrackPos;
		break;
	default:
		return;
	}

	if (nNewLeftChar < 0)
		nNewLeftChar = 0;
	if (nNewLeftChar >= nLineCount)
		nNewLeftChar = nLineCount - 1;
	ScrollToHorz(nNewLeftChar);
}

void CSpecialEdit::OnLButtonDblClk(int nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	int nCount;
	CPoint ptText;

	nCount = GetTextLineCount();

	ptText = ClientToText(point);
	if (ptText.y >= nCount)
		return ;
/*
	LineInfo LineInfo = m_aDataLine.GetAt(ptText.y);

	if (LineInfo.m_strLine->GetLength() < ptText.x)
		return ;

	LPTSTR lpLine = LineInfo.m_strLine->LockBuffer();

	CString strWord="";
	int nWordLen =0,nCharPos = 0;

	while(1)
	{
		if ( *lpLine == NULL)
			break;
		strWord = GetWord(lpLine);
		nWordLen =strWord.GetLength();

		if (nCharPos + nWordLen >= m_ptCursor.x)
		{
			// Will Selection the word
			break;
		}
		lpLine +=nWordLen;
		nCharPos += nWordLen;
	}

	LineInfo.m_strLine->UnlockBuffer();
*/
	CString strWord="";
	int nWordLen =0,nCharPos = 0;
	nCharPos = GetWordAtPos(ptText,strWord);
	nWordLen = strWord.GetLength();

	if (nCharPos == -1)
		return ;
	m_ptCursor.y = ptText.y;
	m_ptCursor.x = nCharPos + nWordLen;

	CursorPos2CaretPos();
	
	CPoint ptStart,ptEnd;

	ptStart = ptEnd = m_ptCursor;
	ptStart.x = nCharPos;

	SetSelection(ptStart,ptEnd);

	
	CWnd::OnLButtonDblClk(nFlags, point);
}

void CSpecialEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CWnd::OnKillFocus(pNewWnd);
	
//	((CAmboApp *)AfxGetApp())->ShowMessageAtDesktop(40,40,"OnKillFocus");
	// TODO: Add your message handler code here
//	((CAmboApp *)AfxGetApp())->ShowMessageAtDesktop(10,10,"DestroyCaret");
	::DestroyCaret();
	bCaretCaret = FALSE;

	m_bFocus = FALSE;
	
}


BOOL CSpecialEdit::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, int nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWnd::Create(NULL, NULL, dwStyle, rect, pParentWnd, nID, pContext);
}

BOOL CSpecialEdit::GetModify()
{
//	return bModified;
	return ((CSpecialView *)GetParent())->GetModify();

}

void CSpecialEdit::SetModify(BOOL bModify)
{
	//if (bModified == bModify)
	{
//		bModified = bModify;
		((CSpecialView *)GetParent())->SetModify(bModify);
	}
}

BOOL CSpecialEdit::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWnd::PreCreateWindow(cs);
}


void CSpecialEdit::SetEditText(LPCTSTR lpszText)
{
	LPTSTR lpLine=NULL;
	LPTSTR lpCurChar;
	int   nLineLength = 0 ;
	int iInsertLen;
	int nLineIndex = 0;
	int iCRlen;

	ClearData();



	lpLine = lpCurChar = (LPTSTR)lpszText;
	InsertNewLine(nLineIndex);

	while(1)
	{
		if (*lpCurChar == 0)
		{
			InsertCharInLine(nLineIndex,lpLine,nLineLength);
			break;
		}
		if ((iCRlen = IsCR(lpCurChar)) != 0)
		{
			lpCurChar += iCRlen;//strlen(crFlag);
//			nLineLength+=strlen(crFlag);
			iInsertLen = InsertCharInLine(nLineIndex,lpLine,nLineLength);
			if (iInsertLen != nLineLength)
				break;
			nLineIndex++;
			InsertNewLine(nLineIndex);
			lpLine = lpCurChar;
			nLineLength = 0;
			continue;
		}
		lpCurChar++;
		nLineLength++;
	}
	ParseText(0,TRUE);
	RecalcVertScrollBar();
	
	UINT nCount = m_aDataLine.GetSize();
//	TestError();
//	ShowEditCaret();
}

int CSpecialEdit::IsCR(LPCTSTR lpszChar)
{
	if (strncmp(lpszChar,crFlag,strlen(crFlag)) == 0)
		return strlen(crFlag);
// !!! Add By Jami chen for open anomalism File
	if (strncmp(lpszChar,"\r",strlen("\r")) == 0)
		return strlen("\r");
	if (strncmp(lpszChar,"\n",strlen("\n")) == 0)
		return strlen("\n");
// !!! Add End By Jami chen for open anomalism File
	return 0;
}

int CSpecialEdit::GetEditTextLength()
{
	int nCount;
	LineInfo LineInfo;
	int nTextLength=0;

		nCount=m_aDataLine.GetSize();	
		if (nCount == 0)
			return 0;

		for (int i=0; i<nCount ; i++)
		{
			LineInfo=m_aDataLine.GetAt(i);
			nTextLength += LineInfo.m_strLine->GetLength();
		}
		nTextLength += (nCount-1)*strlen(crFlag);

		return nTextLength;
}

int CSpecialEdit::GetLineHeight()
{
	return m_nCharHeight;
}

COLORREF CSpecialEdit::GetWordColor(CString strWord)
{
	// The Word Is Keyword
	if (IsKeyword(strWord)==TRUE)
		return color_KeyWord;
	if (IsDefineWord(strWord) == TRUE)
		return color_Define;
	return color_NormalText;
}

// 从当前字串中提取一个词
CString CSpecialEdit::GetWord(LPCTSTR lpChar)
{
	int nWordLength=0;
	LPTSTR lpCurChar;
	BOOL bSpace = FALSE;

		lpCurChar = (LPTSTR)lpChar;
		if ( *lpCurChar == ' ' || *lpCurChar == '\t')
			bSpace = TRUE;
		while(1)
		{
			if (*lpCurChar == 0)
				break;
			if (bSpace == TRUE)
			{
				if ( *lpCurChar == ' ' || *lpCurChar == '\t')
				{
					nWordLength++;
					lpCurChar++;
					continue;
				}
				else
				{
					break;
				}
			}
			if (*lpCurChar>='0' && *lpCurChar<='9')
			{
				nWordLength++;
				lpCurChar++;
				continue;
			}
			if (*lpCurChar>='a' && *lpCurChar<='z')
			{
				nWordLength++;
				lpCurChar++;
				continue;
			}
			if (*lpCurChar>='A' && *lpCurChar<='Z')
			{
				nWordLength++;
				lpCurChar++;
				continue;
			}
			if (*lpCurChar=='#' || *lpCurChar=='_' )
			{
				nWordLength++;
				lpCurChar++;
				continue;
			}
//			if ( *lpCurChar<0 || *lpCurChar>0x7f )
			if (IsChinese(lpCurChar))
			{
				nWordLength+=2;
				lpCurChar+=2;
				continue;
			}
			if (nWordLength==0)
				nWordLength++;
			break;
		}

		CString string(lpChar,nWordLength);
		return string;
}


BOOL CSpecialEdit::IsKeyword(CString strWord)
{

	int i=0;

	while (TRUE)
	{
		if (Sys_KeyWord[i]==NULL)
			break;
		if (strWord.Compare(Sys_KeyWord[i]) ==0)	
			return TRUE;
		i++;
	}
	return FALSE;
}

int CSpecialEdit::GetWordWidth(CDC *pdc,CString strWord,int nCount)
{
//	INT lpWidth[128];
	INT nWordWidth=0;
	LPTSTR lpWord;
	int nCountAdd=0;
	
//		pdc->GetCharWidth(0,0x7f,lpWidth);

		lpWord=strWord.LockBuffer();

		while(*lpWord)
		{
			if (nCount!=-1 && nCountAdd>=nCount)
				break;
//			if (*lpWord<0 || *lpWord>0x7f)
			if (IsChinese(lpWord))
			{
				nWordWidth+=m_nCharWidth * 2;//16;
				lpWord += 2;
				nCountAdd+=2;
			}
			else
			{
				if (*lpWord=='\t')  // Tab
				{
					nWordWidth+=m_nCharWidth * m_nTabSize;//(lpWidth[*lpWord] * m_nTabSize);
				}
				else
					nWordWidth+=m_nCharWidth;//lpWidth[*lpWord];
				lpWord++;
				nCountAdd++;
			}
		}
		strWord.UnlockBuffer();
		return nWordWidth;
		

}

BOOL CSpecialEdit::IsSpace(CString strWord)
{
	int nChar = strWord.GetAt(0);
//	if (strWord.Compare("\t") == 0)
	if (nChar == '\t' || nChar == ' ')
		return TRUE;
	return FALSE;
}

int CSpecialEdit::GetTabSize()
{
	return m_nTabSize;
}

BOOL CSpecialEdit::IsMultiMskEnd(LPCTSTR lpLine)
{
/*	if (*lpLine == '/')
	{
		if (*(lpLine-1) == '*' )
			return TRUE;
	}
	return FALSE;
*/
	if (*lpLine == '*')
	{
		if (*(lpLine+1) == '/' )
			return TRUE;
	}
	return FALSE;

}

BOOL CSpecialEdit::GetLineMultiMaskFlag(LineInfo LineInfo)
{

	if (LineInfo.m_dwStatus & FLAG_MULTIMASK)
		return TRUE;

	return FALSE;
}

BOOL CSpecialEdit::IsMultiMskStart(LPCTSTR lpLine)
{
	if (m_bSingleMask == TRUE ||
		m_bMultiMask  == TRUE ||
		m_bDQuotation == TRUE ||
		m_bSingleQuotation == TRUE
		)
	{
		return FALSE;
	}
	if (*lpLine == '/')
	{
		if (*(lpLine+1) == '*' )
			return TRUE;
	}

	return FALSE;
}

void CSpecialEdit::ParseText(int iIndex,BOOL bPraseAll)
{
	LineInfo LineInfo;
	LPTSTR lpLine,lpCurPos;
	int nLineIndex,nCount;

		nCount=m_aDataLine.GetSize();	
		if (nCount == 0)
			return ;
/*		m_bSingleMask = FALSE;
		m_bDQuotation = FALSE;
		m_bSingleQuotation = FALSE;
		m_bMultiMask = FALSE ;
*/
		LineInfo=m_aDataLine.GetAt(iIndex);

		m_bSingleMask = GetLineSingleMaskFlag(LineInfo);
		m_bMultiMask =  GetLineMultiMaskFlag(LineInfo);
		m_bDQuotation = GetLineDoubleQuotationFlag(LineInfo);
		m_bSingleQuotation = GetLineSingleQuotationFlag(LineInfo);

//		for (nLineIndex=0; nLineIndex<nCount ; nLineIndex++)
		for (nLineIndex=iIndex; nLineIndex<nCount ; nLineIndex++)
		{
			LineInfo=m_aDataLine.GetAt(nLineIndex);
			lpLine=LineInfo.m_strLine->LockBuffer();
			lpCurPos=lpLine;

			if (bPraseAll == FALSE && nLineIndex != iIndex)
			{
				BOOL bSingleMask, bMultiMask, bDQuotation, bSingleQuotation ;

				bSingleMask = GetLineSingleMaskFlag(LineInfo);
				bMultiMask =  GetLineMultiMaskFlag(LineInfo);
				bDQuotation = GetLineDoubleQuotationFlag(LineInfo);
				bSingleQuotation = GetLineSingleQuotationFlag(LineInfo);

				if (bSingleMask == m_bSingleMask &&
					bMultiMask == m_bMultiMask &&
					bDQuotation == m_bDQuotation &&
					bSingleQuotation == m_bSingleQuotation)
				{
					break;
				}

			}

			if (m_bMultiMask)
				SetLineMultiMaskFlag(LineInfo,TRUE);
			else
				SetLineMultiMaskFlag(LineInfo,FALSE);

			if (m_bSingleMask)
				SetLineSingleMaskFlag(LineInfo,TRUE);
			else
				SetLineSingleMaskFlag(LineInfo,FALSE);

			if (m_bDQuotation)
				SetLineDoubleQuoatationFlag(LineInfo,TRUE);
			else
				SetLineDoubleQuoatationFlag(LineInfo,FALSE);

			if (m_bSingleQuotation)
				SetLineSingleQuoatationFlag(LineInfo,TRUE);
			else
				SetLineSingleQuoatationFlag(LineInfo,FALSE);

			while(TRUE)
			{
				if (*lpCurPos == 0)
				{
					if (lpCurPos!=lpLine)
					{
						if (*(lpCurPos-1) == '\\')  // Is Linkage next line code
						{
							// then the Flag will be contine;
						}
						else
						{
							// clear the line flag
							m_bSingleMask = FALSE;
							m_bDQuotation = FALSE;
							m_bSingleQuotation = FALSE;
						}
					}
					else
					{
						// clear the line flag
						m_bSingleMask = FALSE;
						m_bDQuotation = FALSE;
						m_bSingleQuotation = FALSE;
					}
					break;
				}
				if (*lpCurPos == '/')
				{
					if (m_bMultiMask == FALSE)  //  '/*  */' Mask
					{
						if (IsMultiMskStart(lpCurPos) == TRUE )
						{
							m_bMultiMask = TRUE;
							lpCurPos += 3;   // '/*/' ,then the second / is not End Mask
							continue;
						}
					}
//					if (m_bMultiMask == TRUE)  //  '/*  */' Mask
//					{
//						if (IsMultiMskEnd(lpCurPos) &&  lpCurPos != lpLine)  // lpCurPos != lpLine is because the end must not the first char
//						{
//							m_bMultiMask = FALSE;
//							lpCurPos ++;  // '*/*' ,then the second * is not Start Mask
//							continue;
//						}
//					}
					if (m_bSingleMask == FALSE)  //  '//' Mask
					{
						if (IsSingleMaskStart(lpCurPos))  // x != 0 is because the end must not the first char
						{
							m_bSingleMask = TRUE;
						}
					}
				}
				else if (*lpCurPos == '*')
				{
					if (m_bMultiMask == TRUE)  //  '/*  */' Mask
					{
						if (IsMultiMskEnd(lpCurPos))  
						{
							m_bMultiMask = FALSE;
							lpCurPos +=2;  // '*/*' ,then the second * is not Start Mask
							continue;
						}
					}
				}
				else if (IsDoubleQuotation(lpCurPos) ==TRUE )
				{
					if (m_bDQuotation == TRUE)
					{
						m_bDQuotation = FALSE;
					}
					else
						m_bDQuotation = TRUE;
				}
				else if (IsSingleQuotation(lpCurPos) == TRUE)
				{
					if (m_bSingleQuotation == TRUE)
					{
						m_bSingleQuotation = FALSE;
					}
					else
						m_bSingleQuotation = TRUE;
				}

				lpCurPos ++;
			}
			m_aDataLine.SetAt(nLineIndex,LineInfo);
			LineInfo.m_strLine->UnlockBuffer();
		}
}

void CSpecialEdit::SetLineMultiMaskFlag(LineInfo &LineInfo, BOOL bMultiMask)
{
	if (bMultiMask)
	{
		LineInfo.m_dwStatus |= FLAG_MULTIMASK;
	}
	else
	{
		LineInfo.m_dwStatus &= ~FLAG_MULTIMASK;
	}
}

BOOL CSpecialEdit::IsSingleMaskStart(LPTSTR lpLine)
{
	if (m_bSingleMask == TRUE ||
		m_bMultiMask  == TRUE ||
		m_bDQuotation == TRUE ||
		m_bSingleQuotation == TRUE
		)
	{
		return FALSE;
	}
	if (*lpLine == '/')
	{
		if (*(lpLine+1) == '/' )
			return TRUE;
	}
	return FALSE;
}

BOOL CSpecialEdit::IsDefineWord(CString strWord)
{
	if (strWord.GetAt(0) == '#')
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CSpecialEdit::IsDoubleQuotation(LPTSTR lpLine)
{
	if (m_bSingleMask == TRUE ||
		m_bMultiMask  == TRUE ||
		m_bSingleQuotation == TRUE
		)
	{
		return FALSE;
	}
	if (*lpLine == '\"')
		return TRUE;
	return FALSE;
}

BOOL CSpecialEdit::IsSingleQuotation(LPTSTR lpLine)
{
	if (m_bSingleMask == TRUE ||
		m_bMultiMask  == TRUE ||
		m_bDQuotation == TRUE
		)
	{
		return FALSE;
	}
	if (*lpLine == '\'')
		return TRUE;

	return FALSE;
}

void CSpecialEdit::SetLineSingleMaskFlag(LineInfo &LineInfo, BOOL bSingleMask)
{
	if (bSingleMask)
	{
		LineInfo.m_dwStatus |= FLAG_SINGLEMASK;
	}
	else
	{
		LineInfo.m_dwStatus &= ~FLAG_SINGLEMASK;
	}
}

void CSpecialEdit::SetLineDoubleQuoatationFlag(LineInfo &LineInfo, BOOL bDoubleQuoatation)
{
	if (bDoubleQuoatation)
	{
		LineInfo.m_dwStatus |= FLAG_DOUBLEQUOTATION;
	}
	else
	{
		LineInfo.m_dwStatus &= ~FLAG_DOUBLEQUOTATION;
	}
}

void CSpecialEdit::SetLineSingleQuoatationFlag(LineInfo &LineInfo, BOOL bSingleQuoatation)
{
	if (bSingleQuoatation)
	{
		LineInfo.m_dwStatus |= FLAG_SINGLEQUOTATION;
	}
	else
	{
		LineInfo.m_dwStatus &= ~FLAG_SINGLEQUOTATION;
	}

}

BOOL CSpecialEdit::GetLineSingleMaskFlag(LineInfo LineInfo)
{
	if (LineInfo.m_dwStatus & FLAG_SINGLEMASK)
		return TRUE;

	return FALSE;
}

BOOL CSpecialEdit::GetLineDoubleQuotationFlag(LineInfo LineInfo)
{
	if (LineInfo.m_dwStatus & FLAG_DOUBLEQUOTATION)
		return TRUE;

	return FALSE;
}

BOOL CSpecialEdit::GetLineSingleQuotationFlag(LineInfo LineInfo)
{
	if (LineInfo.m_dwStatus & FLAG_SINGLEQUOTATION)
		return TRUE;

	return FALSE;
}

void CSpecialEdit::ClearData()
{
	LineInfo LineInfo;
//	LPTSTR lpLine;//,lpCurPos;
	int nLineIndex,nCount;

		nCount=m_aDataLine.GetSize();	
		if (nCount == 0)
			return ;

		for (nLineIndex=0; nLineIndex<nCount ; nLineIndex++)
		{
			LineInfo=m_aDataLine.GetAt(nLineIndex);
			delete LineInfo.m_strLine;
		}
		m_aDataLine.RemoveAll();
	
		InitializeEditStatus();

		if (rootUndoLink)
		{
			DeleteUndoLink(rootUndoLink->nextLink);
			rootUndoLink->nextLink = NULL;
//			InitUndoLinkStruct( rootUndoLink );
			curUndoLink = rootUndoLink ;
		}

}

void CSpecialEdit::RecalcVertScrollBar(BOOL bPositionOnly)
{
	SCROLLINFO si;
	si.cbSize = sizeof(si);
	if (bPositionOnly)
	{
		si.fMask = SIF_POS;
		si.nPos = m_nTopLine;
	}
	else
	{
		if (GetScreenCount() >= GetTextLineCount() && m_nTopLine > 0)
		{
			m_nTopLine = 0;
			Invalidate();
//			UpdateCaret();
		}
		si.fMask = SIF_DISABLENOSCROLL | SIF_PAGE | SIF_POS | SIF_RANGE;
		si.nMin = 0;
		si.nMax = GetTextLineCount() - 1;
		si.nPage = GetScreenCount();
		si.nPos = m_nTopLine;
	}
	VERIFY(SetScrollInfo(SB_VERT, &si));
}

int CSpecialEdit::GetTextLineCount()
{
	return m_aDataLine.GetSize();
}

void CSpecialEdit::ScrollToLine(int nNewTopLine)
{
	if (m_nTopLine != nNewTopLine)
	{
		int nScrollLines = m_nTopLine - nNewTopLine;
		m_nTopLine = nNewTopLine;
		ScrollWindow(0, nScrollLines * GetLineHeight());
		//UpdateWindow();
//		if (bTrackScrollBar)
		RecalcVertScrollBar(TRUE);
//		HideEditCaret();
		CursorPos2CaretPos();
//		ShowEditCaret();
	}
}

void CSpecialEdit::SetTextEditFont(CDC *pdc)
{
	TEXTMETRIC textmetric;
	/* 2005-08-22, put to init 
	CFont font;
	LOGFONT lfFont;
	TEXTMETRIC textmetric;
	memset(&lfFont, 0, sizeof(lfFont));
	lstrcpy(lfFont.lfFaceName, _T("FixedSys"));
	lfFont.lfHeight = 16;
	lfFont.lfWidth = 8;
	lfFont.lfWeight = FW_NORMAL;
	lfFont.lfItalic = FALSE;
	lfFont.lfCharSet = GB2312_CHARSET;//DEFAULT_CHARSET;
	lfFont.lfOutPrecision = OUT_RASTER_PRECIS;//OUT_DEFAULT_PRECIS;
	lfFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lfFont.lfQuality = DEFAULT_QUALITY;
	lfFont.lfPitchAndFamily = DEFAULT_PITCH;
//	lfFont.lfEscapement=0;

	font.CreateFontIndirect(&lfFont);
	*/
	pdc->SelectObject(m_FixEditFont);

//	pdc->GetCharWidth('a','a',&m_nCharWidth);
	pdc->GetTextMetrics(&textmetric);
	m_nCharWidth = textmetric.tmAveCharWidth;
	m_nCharHeight = textmetric.tmHeight;

}

void CSpecialEdit::ShowEditCaret()
{
//	if (GetFocus() != this)
//		return ; // this window have not get focus , not need caret
	if (m_bFocus == FALSE)
		return ; // this window have not get focus , not need caret
	if (bCaretCaret == FALSE)
	{
//		((CAmboApp *)AfxGetApp())->ShowMessageAtDesktop(10,10,"CreateCaret");
		::CreateCaret(m_hWnd, NULL, 2, GetLineHeight());
		bCaretCaret = TRUE;
	}
	if (m_ptCaretPos.x < m_nMarkAreaWidth)
	{
		m_ptCaretPos.x = -10; // Not Show caret
	}
	SetCaretPos(m_ptCaretPos);
	ShowCaret();
}

void CSpecialEdit::CursorPos2CaretPos()
{
//	m_ptCursor.x=0;
//	m_ptCursor.y=0;	
	CDC *pdc;
	LineInfo LineInfo;

	HideEditCaret();
	if (GetTextLineCount()!=0)
	{
		pdc=GetDC();
		SetTextEditFont(pdc);
		if (m_ptCursor.y <0 || m_ptCursor.y >GetTextLineCount())
		{
			return;
		}
		LineInfo=m_aDataLine.GetAt(m_ptCursor.y);
//		m_ptCaretPos.x=GetWordWidth(pdc,*LineInfo.m_strLine,m_ptCursor.x)+m_nMarkAreaWidth-m_nOffsetChar * m_nCharWidth;
		m_ptCaretPos.x=m_nMarkAreaWidth+ (GetExternCharLength(m_ptCursor)-m_nOffsetChar)* m_nCharWidth;
		m_ptCaretPos.y=(m_ptCursor.y-GetFirstVisibleLine())*GetLineHeight();
		ReleaseDC(pdc);
		SetPosToStatusBar(m_ptCursor.y,m_ptCursor.x);
	}
	else
	{
		m_ptCaretPos.x=m_nMarkAreaWidth;
		m_ptCaretPos.y=m_ptCursor.y;
		SetPosToStatusBar(0,0);
	}
	ShowEditCaret();
}

void CSpecialEdit::HideEditCaret()
{
	if (bCaretCaret == FALSE)
	{
		return ; // have not create caret, so not hide caret
	}
	HideCaret();
}

int CSpecialEdit::InsertCR(CPoint m_ptPos,BOOL bAlignLeft,LPUNDOLINK lpUndoLink)
{
	LineInfo LineInfo1,LineInfo2;
	int nCount,nStart=0;
	LPTSTR lpLine1;

//	TestError();
	if (lpUndoLink)
	{
		lpUndoLink->strInsert->Insert(0,"\r\n");
	}
	InsertNewLine(m_ptPos.y+1);
	LineInfo1=m_aDataLine.GetAt(m_ptPos.y);
	LineInfo2=m_aDataLine.GetAt(m_ptPos.y+1);

	lpLine1=LineInfo1.m_strLine->LockBuffer();
	LineInfo2.m_strLine->Insert(0,lpLine1+m_ptPos.x);
	LineInfo1.m_strLine->UnlockBuffer();

	// !!! Add The space in the new line head
//	if (bAlignLeft)
//		nStart = GetLineHeadSpaceLen(LineInfo1);

	lpLine1=LineInfo1.m_strLine->LockBuffer();

//	if (bAlignLeft)
//	{
//		CString InsertString(lpLine1,nStart);
//		LineInfo2.m_strLine->Insert(0,InsertString);
//	}

	if (strlen(lpLine1) <= (unsigned int )m_ptPos.x)
		nCount = 0;
	else
		nCount=strlen(lpLine1+m_ptPos.x);
	LineInfo1.m_strLine->UnlockBuffer();

	LineInfo1.m_strLine->Delete(m_ptPos.x,nCount);

//	if (bAlignLeft)
//		nStart = GetLineHeadSpaceLen(LineInfo1);

//	lpLine1=LineInfo1.m_strLine->LockBuffer();
/*
	if (bAlignLeft)
	{
//		CString InsertString(lpLine1,nStart);
		int nSpaceLen;
		nSpaceLen = GetLineHeadSpaceLen(LineInfo2);
		LineInfo2.m_strLine->Delete(0,nSpaceLen);

		CString InsertString;
		nStart = GetLineIndent(m_ptPos.y,InsertString);
		LineInfo2.m_strLine->Insert(0,InsertString);
		
	}
*/
//	LineInfo1.m_strLine->UnlockBuffer();
//	TestError();

	return nStart;
}

void CSpecialEdit::BackSpace(CPoint m_ptPos,LPUNDOLINK lpUndoLink)
{
	LineInfo LineInfo1,LineInfo2;
//	TestError();
	if (ExistSelection())
	{
		DeleteSelection(TRUE,lpUndoLink);
//		DeleteSelection();
		return;
	}

	if (m_ptPos.x==0)
	{  // will merge the m_ptPos.y-1 and m_ptPos.y
		if (m_ptPos.y ==0 )
			return;

		if (CanMergeNextLine(m_ptPos.y-1) == TRUE)
		{
			LineInfo LineInfo1,LineInfo2;
			LineInfo1=m_aDataLine.GetAt(m_ptPos.y-1);
			LineInfo2=m_aDataLine.GetAt(m_ptPos.y);

			if (lpUndoLink)
			{
				lpUndoLink->iProcessStyle |= DELETEPROCESS;
				lpUndoLink->startDelete = m_ptCursor;
				lpUndoLink->strDelete->Insert(0,"\r\n");
			}
			m_ptCursor.y--;
			m_ptCursor.x=LineInfo1.m_strLine->GetLength();

			LineInfo1.m_strLine->Insert(m_ptCursor.x,LineInfo2.m_strLine->LockBuffer());
			
			delete LineInfo2.m_strLine;
			m_aDataLine.RemoveAt(m_ptPos.y);


			if (lpUndoLink)
			{
				lpUndoLink->endDelete = m_ptCursor;
			}
			CursorPos2CaretPos();
	//		HideEditCaret();
			RecalcVertScrollBar();
			InvalidateLines(m_ptCursor.y);
	//		ShowEditCaret();
	//		InvalidateRect(NULL);
		}
		else
		{
			WarningCanntCompleteAction();
		}
	}
	else
	{
		LineInfo LineInfo;
//		CDC *pdc;
		int nDeleteCount = 1;
		int nChar;
		LineInfo=m_aDataLine.GetAt(m_ptPos.y);
/*
		nChar=LineInfo.m_strLine->GetAt(m_ptPos.x-1);
		if (nChar < 0 || nChar >0x7f)
			nDeleteCount = 2;  // This char is chinese
*/
		{
			int iPos=0;
			LPTSTR lpCurPos;

				lpCurPos = LineInfo.m_strLine->LockBuffer();
				while(*lpCurPos)
				{
//					if ( *lpCurPos < 0)
					if ( IsChinese(lpCurPos))
					{
						lpCurPos +=2;
						iPos += 2;
						nDeleteCount = 2;
						nChar = 0; // for test '*' , '\\' , '/', so the chinese is Invalid
					}
					else
					{
						nChar = *lpCurPos ;
						lpCurPos ++;
						iPos ++;
						nDeleteCount = 1;
					}
					if (iPos >= m_ptPos.x)
						break;
				}
				LineInfo.m_strLine->UnlockBuffer();
		}
		if (lpUndoLink)
		{
			int i;
			lpUndoLink->iProcessStyle |= DELETEPROCESS;
			lpUndoLink->startDelete = m_ptCursor;
			for (i = 0; i < nDeleteCount; i++)
				lpUndoLink->strDelete->Insert(i,LineInfo.m_strLine->GetAt(m_ptPos.x-nDeleteCount + i));
		}
		LineInfo.m_strLine->Delete(m_ptPos.x-nDeleteCount,nDeleteCount);
//		m_ptCursor.y++;
		m_ptCursor.x-=nDeleteCount;
		//pdc=GetDC();
		//DrawLine(pdc,m_ptCursor.y);
		//ReleaseDC(pdc);
		if (lpUndoLink)
		{
			lpUndoLink->endDelete = m_ptCursor;
		}
		if (nChar == '/' || nChar == '*' || nChar == '\\')
		{
			ParseText(m_ptCursor.y, FALSE);
			InvalidateLines(m_ptCursor.y);
		}
		else
		{
//			InvalidateLines(m_ptCursor.y,1);
			InvalidateLines(m_ptCursor.y,m_ptCursor.y);
		}
//		HideEditCaret();
		CursorPos2CaretPos();
//		ShowEditCaret();
	}
	EnsureVisible();
	//bModified = TRUE;
	SetModify(TRUE);
//	TestError();

}

CPoint CSpecialEdit::ClientToText(const CPoint &point)
{
	int nLineCount = GetTextLineCount();

	CPoint pt;
	pt.y = m_nTopLine + point.y / (int)GetLineHeight();
	if (pt.y >= nLineCount)
		pt.y = nLineCount - 1;
	if (pt.y < 0)
		pt.y = 0;

	int nLength = 0;
	LPCTSTR pszLine = NULL;
	LineInfo LineInfo;
	if (pt.y >= 0 && pt.y < nLineCount)
	{
		LineInfo=m_aDataLine.GetAt(pt.y);
		nLength = LineInfo.m_strLine->GetLength();
		pszLine = LineInfo.m_strLine->LockBuffer();
	}

	int nPos = m_nOffsetChar + (point.x - m_nMarkAreaWidth) / m_nCharWidth;//GetCharWidth();
	if (nPos < 0)
		nPos = 0;

	int nIndex = 0, nCurPos = 0;
	int nCharWidth=0;
	int nTabSize = GetTabSize();
	while (nIndex < nLength)
	{
		nCharWidth = 1;
		if (pszLine[nIndex] == _T('\t'))
			nCurPos += nTabSize;
//			nCurPos += (nTabSize - nCurPos % nTabSize);
		else
		{
//			if (pszLine[nIndex] & 0x80)
			if (IsChinese((LPTSTR)(pszLine + nIndex)))
			{
				nCurPos += 2;
				nCharWidth = 2;
			}
			else
				nCurPos ++;
		}

		if (nCurPos > nPos)
			break;

		nIndex += nCharWidth;
	}

	ASSERT(nIndex >= 0 && nIndex <= nLength);
	pt.x = nIndex ;
	return pt;
}

void CSpecialEdit::InvalidateLines(int nIndexStart, int nIndexEnd)
{
	int nStart,nEnd,nScreenCount;
	CDC *pdc;

		nScreenCount = GetScreenCount();
		if (nIndexEnd ==-1)
		{
			nIndexEnd = m_nTopLine + nScreenCount +1;
		}

		pdc=GetDC();
		if (nIndexStart > nIndexEnd)
		{
			nStart = nIndexEnd;
			nEnd = nIndexStart;
		}
		else
		{
			nStart = nIndexStart;
			nEnd = nIndexEnd;
		}

		if (nStart < m_nTopLine)
			nStart = m_nTopLine;

		if (nEnd >  m_nTopLine + nScreenCount +1 )
			nEnd = m_nTopLine + nScreenCount +1 ;
		for (int i = nStart ; i<=nEnd;i++)
		{
			DrawLine(pdc,i);
		}
		ReleaseDC(pdc);
}



void CSpecialEdit::MovePageUp()
{
	CPoint point;
	int nScreenCount;

	SHORT nShift;
	SHORT nCtrl;

		nCtrl = GetAsyncKeyState(VK_CONTROL); 
		nShift=GetAsyncKeyState(VK_SHIFT);
		if (nCtrl)
			return ; // Ctrl + pageup is invalidate
		if (nShift)
		{
			if (m_ptCursor != m_ptSelEnd)
			{
				SetSelection(m_ptCursor,m_ptCursor);
			}
		}
		else
		{
			if (ExistSelection())
			{
				SetSelection(m_ptCursor,m_ptCursor);
			}
		}
		if (m_ptCursor.y > 0)
		{
			//m_ptCursor.y --;
			// 首先向上滚动一屏
			OnVScroll(SB_PAGEUP, 0, NULL) ;

			nScreenCount= GetScreenCount();
			nScreenCount --;
			if (m_ptCursor.y < nScreenCount)
				m_ptCursor.y =0;
			else
				m_ptCursor.y -= nScreenCount;
			point = TextToClient(m_ptCursor);
			m_ptCursor=ClientToText(point);
//			HideEditCaret();
			CursorPos2CaretPos();
//			ShowEditCaret();
		}
		if (nShift)
		{
			SetSelection(m_ptSelStart,m_ptCursor);
		}
		EnsureVisible();
}

void CSpecialEdit::MovePageDown()
{
	CPoint point;
	int nCount=GetTextLineCount();
	int nScreenCount = GetScreenCount();

	SHORT nShift;
	SHORT nCtrl;

		nCtrl = GetAsyncKeyState(VK_CONTROL); 
		nShift=GetAsyncKeyState(VK_SHIFT);
		if (nCtrl)
			return ; // Ctrl + pagedown is invalidate
		if (nShift)
		{
			if (m_ptCursor != m_ptSelEnd)
			{
				SetSelection(m_ptCursor,m_ptCursor);
			}
		}
		else
		{
			if (ExistSelection())
			{
				SetSelection(m_ptCursor,m_ptCursor);
			}
		}
		if (nCount <= m_ptCursor.y + 1)  // 
			return;  // Had The last line

		OnVScroll(SB_PAGEDOWN, 0, NULL) ;
		nScreenCount --;

		m_ptCursor.y +=nScreenCount;
		if (m_ptCursor.y > nCount -1)
			m_ptCursor.y = nCount -1;
		point = TextToClient(m_ptCursor);
		m_ptCursor=ClientToText(point);
//		HideEditCaret();
		CursorPos2CaretPos();
//		ShowEditCaret();
		if (nShift)
		{
			SetSelection(m_ptSelStart,m_ptCursor);
		}
		EnsureVisible();
}

void CSpecialEdit::MoveEnd()
{
	SHORT nShift;
	SHORT nCtrl;

		nCtrl = GetAsyncKeyState(VK_CONTROL); 
		nShift=GetAsyncKeyState(VK_SHIFT);
		if (nShift)
		{
			if (m_ptCursor != m_ptSelEnd)
			{
				SetSelection(m_ptCursor,m_ptCursor);
			}
		}
		else
		{
			if (ExistSelection())
			{
				SetSelection(m_ptCursor,m_ptCursor);
			}
		}
		if (nCtrl)
		{
			int nCount = GetTextLineCount();
			if (nCount == 0)
				return ;
			m_ptCursor.y = nCount -1;
		}
		LineInfo LineInfo = m_aDataLine.GetAt(m_ptCursor.y);
		m_ptCursor.x=LineInfo.m_strLine->GetLength();
//		HideEditCaret();
		CursorPos2CaretPos();
//		ShowEditCaret();
		if (nShift)
		{
			SetSelection(m_ptSelStart,m_ptCursor);
		}
		EnsureVisible();
}

void CSpecialEdit::MoveHome()
{
	SHORT nShift;
	SHORT nCtrl;

		nCtrl = GetAsyncKeyState(VK_CONTROL); 
		nShift=GetAsyncKeyState(VK_SHIFT);
		if (nShift)
		{
			if (m_ptCursor != m_ptSelEnd)
			{
				SetSelection(m_ptCursor,m_ptCursor);
			}
		}
		else
		{
			if (ExistSelection())
			{
				SetSelection(m_ptCursor,m_ptCursor);
			}
		}
		if (nCtrl)
		{
			m_ptCursor.y=0;
			m_ptCursor.x=0;
		}
		else
		{
			m_ptCursor.x=0;
		}
//		HideEditCaret();
		CursorPos2CaretPos();
//		ShowEditCaret();
		if (nShift)
		{
			SetSelection(m_ptSelStart,m_ptCursor);
		}
		EnsureVisible();
}

void CSpecialEdit::MoveLeft()
{
	SHORT nShift;
	SHORT nCtrl;

		nCtrl = GetAsyncKeyState(VK_CONTROL); 
		nShift=GetAsyncKeyState(VK_SHIFT);
		if (nShift)
		{
			if (m_ptCursor != m_ptSelEnd)
			{
				SetSelection(m_ptCursor,m_ptCursor);
			}
		}
		else
		{
			if (ExistSelection())
			{
				SetSelection(m_ptCursor,m_ptCursor);
			}
		}
		if (m_ptCursor.x == 0)
		{
			// the head of the line, will to the perior line tail
			if (m_ptCursor.y > 0)
			{
				m_ptCursor.y --;
				LineInfo LineInfo = m_aDataLine.GetAt(m_ptCursor.y);
				m_ptCursor.x=LineInfo.m_strLine->GetLength();
//				HideEditCaret();
				CursorPos2CaretPos();
//				ShowEditCaret();
			}

		}
		else
		{
			if (nCtrl)
			{
				LineInfo LineInfo = m_aDataLine.GetAt(m_ptCursor.y);
				LPTSTR lpLine;
				int nCharPos =0;
				lpLine = LineInfo.m_strLine->LockBuffer();
				//lpLine += m_ptCursor.x;
				while(1)
				{
					if ( *lpLine == NULL)
						break;
					CString strWord = GetWord(lpLine);
					int nWordLen =strWord.GetLength();

					if (nCharPos + nWordLen >= m_ptCursor.x)
						break;
					lpLine +=nWordLen;
					nCharPos += nWordLen;
				}
				//m_ptCursor.x += strWord.GetLength();
				m_ptCursor.x = nCharPos;
				LineInfo.m_strLine->UnlockBuffer();
			}
			else
			{
//				m_ptCursor.x --;
				LineInfo LineInfo = m_aDataLine.GetAt(m_ptCursor.y);
				LPTSTR lpLine;
				int nCharPos =0;
				lpLine = LineInfo.m_strLine->LockBuffer();
				//lpLine += m_ptCursor.x;
				while(1)
				{
					int nWordLen = 1;
					if ( *lpLine == NULL)
						break;
//					if (*lpLine<0 || *lpLine >=0x80)
					if (IsChinese(lpLine))
					{
						nWordLen = 2;
					}

					if (nCharPos + nWordLen >= m_ptCursor.x)
						break;
					lpLine +=nWordLen;
					nCharPos += nWordLen;
				}
				//m_ptCursor.x += strWord.GetLength();
				m_ptCursor.x = nCharPos;
				LineInfo.m_strLine->UnlockBuffer();
			}
//			HideEditCaret();
			CursorPos2CaretPos();
//			ShowEditCaret();
		}
		if (nShift)
		{
			SetSelection(m_ptSelStart,m_ptCursor);
		}
		EnsureVisible();

}

void CSpecialEdit::MoveUp()
{
	CPoint point;
	SHORT nShift;
	SHORT nCtrl;

		nCtrl = GetAsyncKeyState(VK_CONTROL); 
		nShift=GetAsyncKeyState(VK_SHIFT);
		if (nShift)
		{
		if (nShift && nCtrl)
				return ; // Ctrl + Shift + up is invalidate
			if (m_ptCursor != m_ptSelEnd)
			{
				SetSelection(m_ptCursor,m_ptCursor);
			}
		}
		else
		{
			if (ExistSelection())
			{
				SetSelection(m_ptCursor,m_ptCursor);
			}
		}
		if (nCtrl)
		{
			// Ctrl + up  == Scroll Line up
			if (m_nTopLine > 0)
				ScrollToLine(m_nTopLine - 1);
		}
		else
		{
			if (m_ptCursor.y > 0)
			{
				m_ptCursor.y --;
				point = TextToClient(m_ptCursor);
				m_ptCursor=ClientToText(point);
//				HideEditCaret();
				CursorPos2CaretPos();
//				ShowEditCaret();
			}
			EnsureVisible();
		}
		if (nShift)
		{
			SetSelection(m_ptSelStart,m_ptCursor);
		}
}

void CSpecialEdit::MoveRight()
{
	LineInfo LineInfo = m_aDataLine.GetAt(m_ptCursor.y);

	SHORT nShift;
	SHORT nCtrl;

		nCtrl = GetAsyncKeyState(VK_CONTROL); 
		nShift=GetAsyncKeyState(VK_SHIFT);
		if (nShift)
		{
			if (m_ptCursor != m_ptSelEnd)
			{
				SetSelection(m_ptCursor,m_ptCursor);
			}
		}
		else
		{
			if (ExistSelection())
			{
				SetSelection(m_ptCursor,m_ptCursor);
			}
		}

		if (m_ptCursor.x == LineInfo.m_strLine->GetLength())
		{// Had the end of this line ,will to the head of the next line
			int nCount=GetTextLineCount();

				if (nCount>m_ptCursor.y + 1)
				{
					m_ptCursor.y ++;
					m_ptCursor.x = 0;
//					HideEditCaret();
					CursorPos2CaretPos();
//					ShowEditCaret();
				}
		}
		else
		{  // To The next Position
			if (nCtrl)
			{
				LPTSTR lpLine;
				lpLine = LineInfo.m_strLine->LockBuffer();
				lpLine += m_ptCursor.x;
				CString strWord = GetWord(lpLine);
				m_ptCursor.x += strWord.GetLength();
				LineInfo.m_strLine->UnlockBuffer();
			}
			else
			{
				LPTSTR lpLine;
				lpLine = LineInfo.m_strLine->LockBuffer();
//				if (*(lpLine+m_ptCursor.x)<0 || *(lpLine+m_ptCursor.x) >0x7f) // Is Chinese
				if (IsChinese(lpLine + m_ptCursor.x))
					m_ptCursor.x += 2;
				else
					m_ptCursor.x ++;
				LineInfo.m_strLine->UnlockBuffer();
			}
//			HideEditCaret();
			CursorPos2CaretPos();
//			ShowEditCaret();
		}
		if (nShift)
		{
			SetSelection(m_ptSelStart,m_ptCursor);
		}
		EnsureVisible();
}

void CSpecialEdit::MoveDown()
{
	CPoint point;

	int nCount=GetTextLineCount();
	SHORT nShift;
	short nCtrl;

		nCtrl = GetAsyncKeyState(VK_CONTROL);
		nShift=GetAsyncKeyState(VK_SHIFT);

		if (nShift)
		{
			if (nShift && nCtrl)
					return ; // Ctrl + Shift + up is invalidate
			if (m_ptCursor != m_ptSelEnd)
			{
				SetSelection(m_ptCursor,m_ptCursor);
			}
		}
		else
		{
			if (ExistSelection())
			{
				SetSelection(m_ptCursor,m_ptCursor);
			}
		}

		if (nCtrl)
		{
			// Ctrl + up  == Scroll Line down
			int nNewTopLine = m_nTopLine +1;
			if (nNewTopLine >= nCount)
				nNewTopLine = nCount - 1;
			ScrollToLine(nNewTopLine);
		}
		else
		{
			if (nCount>m_ptCursor.y + 1)
			{
				m_ptCursor.y ++;
				point = TextToClient(m_ptCursor);
				m_ptCursor=ClientToText(point);
//				HideEditCaret();
				CursorPos2CaretPos();
//				ShowEditCaret();
			}
			EnsureVisible();
		}
		if (nShift)
		{
			SetSelection(m_ptSelStart,m_ptCursor);
		}
}

void CSpecialEdit::DeleteChar(LPUNDOLINK lpUndoLink)
{
//	LineInfo NextLineInfo;
	if (ExistSelection())
	{
		DeleteSelection(TRUE,lpUndoLink);
		return;
	}
	LineInfo LineInfo1 = m_aDataLine.GetAt(m_ptCursor.y);
//	TestError();

	if (m_ptCursor.x == LineInfo1.m_strLine->GetLength())
	{
		// The Cursor At the End of the Line, will delete the CR ,and Linkage the next Line
		if (m_ptCursor.y == (GetTextLineCount()-1))
		{
			//Had in the Last line
			return ;
		}

		LineInfo NextLineInfo= m_aDataLine.GetAt(m_ptCursor.y+1);
		
		if (CanMergeNextLine(m_ptCursor.y) == TRUE)
		{
			LineInfo1.m_strLine->Insert(m_ptCursor.x,NextLineInfo.m_strLine->LockBuffer());

			if (lpUndoLink)
			{
				lpUndoLink->iProcessStyle |= DELETEPROCESS;
				lpUndoLink->startDelete = m_ptCursor;
				lpUndoLink->strDelete->Insert(0,"\r\n");
			}

			NextLineInfo.m_strLine->UnlockBuffer();
			delete NextLineInfo.m_strLine; 
			m_aDataLine.RemoveAt(m_ptCursor.y+1);

			if (lpUndoLink)
			{
				lpUndoLink->endDelete = m_ptCursor;
			}

			RecalcVertScrollBar();
			InvalidateLines(m_ptCursor.y);
		}
		else
		{
			WarningCanntCompleteAction();
		}
		
	}
	else
	{
		// Only Will Delete the cursor char
		LPTSTR lpLine;
		int nDelCount = 1;
		TCHAR nChar;

		lpLine = LineInfo1.m_strLine->LockBuffer();
//		nChar =*(lpLine+m_ptCursor.x);
//		if ( nChar <0 || nChar >0x7f) // Is Chinese
		if (IsChinese(lpLine + m_ptCursor.x))
		{
			nDelCount = 2;
			nChar = 0;
		}
		else
		{
			nChar = *(lpLine + m_ptCursor.x);
			nDelCount = 1;
		}
		LineInfo1.m_strLine->UnlockBuffer();

		if (lpUndoLink)
		{
			int i;
			lpUndoLink->iProcessStyle |= DELETEPROCESS;
			lpUndoLink->startDelete = m_ptCursor;
			lpUndoLink->startDelete.x += nDelCount ;
			for (i = 0; i < nDelCount; i++)
				lpUndoLink->strDelete->Insert(i,LineInfo1.m_strLine->GetAt(m_ptCursor.x + i));
		}

		DeleteCharInLine( m_ptCursor.y, nDelCount, m_ptCursor.x );

		if (lpUndoLink)
		{
			lpUndoLink->endDelete = m_ptCursor;
		}
		//		HideEditCaret();
		if ( nChar == '*' || nChar == '/' || nChar == '\\')
		{
			ParseText(m_ptCursor.y,FALSE);
			InvalidateLines(m_ptCursor.y);
		}
		else
		{
//			InvalidateLines(m_ptCursor.y,1);
			InvalidateLines(m_ptCursor.y,m_ptCursor.y);
		}
//		ShowEditCaret();
	}
	EnsureVisible();
	//bModified = TRUE;
	SetModify(TRUE);
//	TestError();
}

BOOL CSpecialEdit::ExistSelection()
{
	if (m_ptSelStart == m_ptSelEnd)
		return FALSE;
	return TRUE;
}

CPoint CSpecialEdit::TextToClient(CPoint &point)
{
//	ASSERT_VALIDTEXTPOS(point);
	LineInfo LineInfo = m_aDataLine.GetAt(point.y);

	int nLength = LineInfo.m_strLine->GetLength();
	LPCTSTR pszLine = LineInfo.m_strLine->LockBuffer();;

	CPoint pt;
	pt.y = (point.y - m_nTopLine) * GetLineHeight();
	pt.x = 0;
	int nTabSize = GetTabSize();
	for (int nIndex = 0; nIndex < point.x; nIndex ++)
	{
		if (pszLine[nIndex] == _T('\t'))
			pt.x += (nTabSize - pt.x % nTabSize);
		else
			pt.x ++;
	}

	pt.x = (pt.x - m_nOffsetChar) * m_nCharWidth + m_nMarkAreaWidth;
	return pt;
}

void CSpecialEdit::GetEditText(LPTSTR lpBuffer, int nSize)
{
	int nCount = GetTextLineCount();
	LPTSTR lpLinePos;
	int nReadCount = 0 , nLineLength;
	int nIndexLine = 0;

		lpLinePos = lpBuffer;
		//for (int i=0; i<nCount ;i++)
		while(1)
		{
			LineInfo LineInfo = m_aDataLine.GetAt(nIndexLine);

			nLineLength = LineInfo.m_strLine->GetLength();
			if ((nReadCount + nLineLength) > nSize )
				break;

			strcpy(lpLinePos,LineInfo.m_strLine->LockBuffer());
			LineInfo.m_strLine->UnlockBuffer();
			lpLinePos += nLineLength;
			nReadCount += nLineLength;
			nIndexLine++;
			if (nIndexLine >= nCount)
				break;
			if ((nReadCount + 2) > nSize )  // need add the CR
				break;
			strcpy(lpLinePos,"\r\n");
			lpLinePos += 2;
			nReadCount += 2;
		}
}


void CSpecialEdit::DefaultKeyProcess(int nChar)
{
	SHORT nShift;
	SHORT nCtrl;

		nCtrl = GetAsyncKeyState(VK_CONTROL); 
		nShift=GetAsyncKeyState(VK_SHIFT);

		if (nCtrl && nShift)
		{
			  // Ctrl + Shift + Key
			return ;
		}

		if (nShift)
		{
			// Shift + Key
			switch(nChar)
			{
				case VK_INSERT:
					OnPaste();
					break;
				case VK_F5:  // 插入当前时间
				case VK_F9:  // 插入C文件头信息
				case VK_F8:  // 插入H文件头信息
				case VK_F7:  // 插入函数定义头信息
					InsertAdditionalMessage(nChar);
					break;
			}
			return ;
		}

		if (nCtrl)
		{
			// Ctrl + Key
			switch(nChar)
			{
				case 'V':
					OnPaste();
					break;
				case 'X':
					OnCut();
					break;
				case 'C':
					OnCopy();
					break;
				case 'A':
					OnSelAll();
					break;
				case VK_INSERT:
					OnCopy();
					break;
			}
			return ;
		}
		return ;
}
BOOL CSpecialEdit::GetFromClipboard(CString &strText)
{
	BOOL bSuccess = FALSE;
	if (OpenClipboard())
	{
		HGLOBAL hData = GetClipboardData(CF_TEXT);
		if (hData != NULL)
		{
			LPSTR pszData = (LPSTR) GlobalLock(hData);
			if (pszData != NULL)
			{
				strText = pszData;
				GlobalUnlock(hData);
				bSuccess = TRUE;
			}
		}
		CloseClipboard();
	}
	return bSuccess;
}

BOOL CSpecialEdit::PutToClipboard(LPCTSTR pszText)
{
	if (pszText == NULL || lstrlen(pszText) == 0)
		return FALSE;

	CWaitCursor wc;
	BOOL bOK = FALSE;
	if (OpenClipboard())
	{
		EmptyClipboard();
		HGLOBAL hData = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, lstrlen(pszText) + 1);
		if (hData != NULL)
		{
			LPSTR pszData = (LPSTR) ::GlobalLock(hData);
//			USES_CONVERSION;
			strcpy(pszData,  pszText);
			GlobalUnlock(hData);
			bOK = SetClipboardData(CF_TEXT, hData) != NULL;
		}
		CloseClipboard();
	}
	return bOK;
}

void CSpecialEdit::OnCopy() 
{
	// TODO: Add your command handler code here
	if (ExistSelection())
	{
		CPoint ptSelStart,ptSelEnd;
		CString text;
//		CopySelectionToClipboard();
		GetSelection(ptSelStart,ptSelEnd);
		GetText(ptSelStart,ptSelEnd,text);
		PutToClipboard(text);
		return;
	}	
}

void CSpecialEdit::OnCut() 
{
	// TODO: Add your command handler code here
	if (ExistSelection())
	{
		CPoint ptSelStart,ptSelEnd;
		CString text;

		LPUNDOLINK newUndoLink = (LPUNDOLINK)malloc(sizeof(UNDOLINK));

		InitUndoLinkStruct( newUndoLink );

		GetSelection(ptSelStart,ptSelEnd);
		GetText(ptSelStart,ptSelEnd,text);
		PutToClipboard(text);
		DeleteSelection(TRUE,newUndoLink);

		if (newUndoLink->iProcessStyle == NOPROCESS)
		{
			delete newUndoLink;
		}
		else
		{
			newUndoLink->endCursor = m_ptCursor;
			InsertNewUndoLink(newUndoLink);
		}
		return;
	}	
}

void CSpecialEdit::OnPaste() 
{
	// TODO: Add your command handler code here
	CString text;
	LPUNDOLINK newUndoLink = (LPUNDOLINK)malloc(sizeof(UNDOLINK));

	InitUndoLinkStruct( newUndoLink );

	DeleteSelection(TRUE,newUndoLink);

	GetFromClipboard(text);

	if (newUndoLink)
	{
		newUndoLink->startInsert = m_ptCursor;
		newUndoLink->iProcessStyle |= INSERTPROCESS;
		newUndoLink->strInsert->Insert(0,text);
	}
	InsertText(m_ptCursor,text);
	if (newUndoLink)
	{
		newUndoLink->endInsert = m_ptCursor;
	}

	if (newUndoLink->iProcessStyle == NOPROCESS)
	{
		delete newUndoLink;
	}
	else
	{
		newUndoLink->endCursor = m_ptCursor;
		InsertNewUndoLink(newUndoLink);
	}
}

void CSpecialEdit::OnEditCopy() 
{
	// TODO: Add your command handler code here
	OnCopy();
}

void CSpecialEdit::OnEditCut() 
{
	// TODO: Add your command handler code here
	OnCut();
}

void CSpecialEdit::OnEditPaste() 
{
	// TODO: Add your command handler code here
	OnPaste();
}

int CSpecialEdit::GetLineHeadSpaceLen(LineInfo LineInfo)
{
	LPTSTR lpStr;
	int nCount = 0;

		lpStr = LineInfo.m_strLine->LockBuffer();

		while(1)
		{
			if (*lpStr == 0)
			{
				nCount =0;
				break;
			}
			if (*lpStr != ' ' && *lpStr != '\t')
				break;
			lpStr ++ ;
			nCount ++;
		}

		LineInfo.m_strLine->UnlockBuffer();
		return nCount;
}

void CSpecialEdit::InitializeEditStatus()
{
	m_nTabSize = 4;

	m_bSingleMask = FALSE;
	m_bMultiMask = FALSE;
	m_bDQuotation = FALSE;
	m_bSingleQuotation = FALSE;

	m_nTopLine = 0;
	m_nOffsetChar = 0;

	m_ptCursor.x=0;
	m_ptCursor.y=0;

	m_ptSelStart = m_ptSelEnd = m_ptCursor;

	bCaretCaret=FALSE;
	CursorPos2CaretPos();

//	m_nOffsetChar = 0;

	bIsChinese = FALSE;
	//bModified = FALSE;
//	SetModify(FALSE);
}

void CSpecialEdit::GetText(const CPoint &ptStart, const CPoint &ptEnd, CString &text)
{
	CPoint pt_SelStart,pt_SelEnd;

	if (ptStart == ptEnd)
		return;
	
	// Adjust the Selection Start and End Position
	if (ptStart.y > ptEnd.y)
	{
		pt_SelStart = ptEnd;
		pt_SelEnd = ptStart;
	}
	else if (ptStart.y < ptEnd.y)
	{
		pt_SelStart = ptStart;
		pt_SelEnd = ptEnd;
	}
	else
	{
		if (ptStart.x > ptEnd.x)
		{
			pt_SelStart = ptEnd;
			pt_SelEnd = ptStart;
		}
		else
		{
			pt_SelStart = ptStart;
			pt_SelEnd = ptEnd;
		}
	}

	if (pt_SelStart.y == pt_SelEnd.y)
	{
//		int i=0;
		LineInfo LineInfo1;
//		TCHAR ch;

		LineInfo1=m_aDataLine.GetAt(pt_SelStart.y);
		GetStringText(*LineInfo1.m_strLine,pt_SelStart.x,pt_SelEnd.x,text);
	}
	else
	{
/*		LineInfo LineInfo1,LineInfo2;
		LineInfo1=m_aDataLine.GetAt(pt_SelStart.y);
		LineInfo2=m_aDataLine.GetAt(pt_SelEnd.y);

		GetStringText(*LineInfo1.m_strLine,pt_SelStart.x,LineInfo1.m_strLine->GetLength(),text);
		
		for (int i=pt_SelStart.y+1; i<pt_SelEnd.y;i++)
		{
			text.Insert(text.GetLength(),"\r\n");
			LineInfo2=m_aDataLine.GetAt(i);
			text.Insert(text.GetLength(),*LineInfo2.m_strLine);
		}
		text.Insert(text.GetLength(),"\r\n");
		CString textEndLine;
		LineInfo2=m_aDataLine.GetAt(pt_SelEnd.y);
		GetStringText(*LineInfo2.m_strLine,0,pt_SelEnd.x,textEndLine);
		text.Insert(text.GetLength(),textEndLine);
*/
		LineInfo LineInfo1,LineInfo2;
		int nCRLFLength = lstrlen(crFlag);
		ASSERT(nCRLFLength > 0);

		int nBufSize = 0;
		for (int L = pt_SelStart.y; L <= pt_SelEnd.y; L ++)
		{
			LineInfo1 = m_aDataLine.GetAt(L);
			nBufSize += LineInfo1.m_strLine->GetLength();
			nBufSize += nCRLFLength;
		}

		LPTSTR pszBuf = text.GetBuffer(nBufSize);
		LPTSTR pszCurPos = pszBuf;

		LineInfo1 = m_aDataLine.GetAt(pt_SelStart.y);
		int nCount = LineInfo1.m_strLine->GetLength() - pt_SelStart.x;
		if (nCount > 0)
		{
			LineInfo1 = m_aDataLine.GetAt(pt_SelStart.y);
			memcpy(pszBuf, LineInfo1.m_strLine->LockBuffer() + pt_SelStart.x, sizeof(TCHAR) * nCount);
			LineInfo1.m_strLine->UnlockBuffer();
			pszBuf += nCount;
		}
		memcpy(pszBuf, crFlag, sizeof(TCHAR) * nCRLFLength);
		pszBuf += nCRLFLength;
		for (int I = pt_SelStart.y + 1; I < pt_SelEnd.y; I ++)
		{
			LineInfo1 = m_aDataLine.GetAt(I);
			nCount = LineInfo1.m_strLine->GetLength();
			if (nCount > 0)
			{
				memcpy(pszBuf, LineInfo1.m_strLine->LockBuffer(), sizeof(TCHAR) * nCount);
				LineInfo1.m_strLine->UnlockBuffer();
				pszBuf += nCount;
			}
			memcpy(pszBuf, crFlag, sizeof(TCHAR) * nCRLFLength);
			pszBuf += nCRLFLength;
		}
		if (pt_SelEnd.x > 0)
		{
			LineInfo1 = m_aDataLine.GetAt(pt_SelEnd.y);
			memcpy(pszBuf, LineInfo1.m_strLine->LockBuffer(), sizeof(TCHAR) * pt_SelEnd.x);
			LineInfo1.m_strLine->UnlockBuffer();
			pszBuf += pt_SelEnd.x;
		}
		pszBuf[0] = 0;
		text.ReleaseBuffer();
		text.FreeExtra();
	}
}

void CSpecialEdit::InsertText(const CPoint &ptPosition, CString &text)
{
	LPTSTR lpLine=NULL;
	LPTSTR lpCurChar;
	WORD   nLineLength = 0 ;
	int nLineIndex = 0;
	CPoint ptCurPos;
	int iCRlen;

	lpLine = lpCurChar = text.LockBuffer();
	ptCurPos = ptPosition;

	while(1)
	{
		if (*lpCurChar == 0)
		{
//			InsertCharInLine(ptCurPos.y,lpLine,nLineLength,ptCurPos.x);
//			ptCurPos.x +=nLineLength;
			ptCurPos.x += InsertCharInLine(ptCurPos.y,lpLine,nLineLength,ptCurPos.x);
			break;
		}
		if ((iCRlen = IsCR(lpCurChar)) != 0)
		{
			//InsertCharInLine(ptCurPos.y,lpLine,nLineLength,ptCurPos.x);
			//ptCurPos.x +=nLineLength;
			ptCurPos.x += InsertCharInLine(ptCurPos.y,lpLine,nLineLength,ptCurPos.x);
			InsertCR(ptCurPos);
			ptCurPos.x = 0;
			ptCurPos.y ++;
			lpCurChar += iCRlen;//strlen(crFlag);
			lpLine = lpCurChar;
			nLineLength = 0;
			continue;
		}
		lpCurChar++;
		nLineLength++;
	}
	ParseText(ptPosition.y, TRUE);
//	HideEditCaret();
	RecalcVertScrollBar();
	InvalidateLines(ptPosition.y, -1);
	m_ptCursor=ptCurPos;
	CursorPos2CaretPos();
//	ShowEditCaret();
	EnsureVisible();
	//bModified = TRUE;
	SetModify(TRUE);
}

void CSpecialEdit::GetStringText(const CString &stringSource, int nStart, int nEnd, CString &text)
{
	TCHAR ch;
	int i=0;
	int nLength;

		if (nStart == nEnd)
			return;
		nLength = stringSource.GetLength();
		while(1)
		{
			if (nStart + i >= nLength)
				break;
			ch = stringSource.GetAt(nStart + i);
			text.Insert(i,ch);
			i++;
			if (nStart + i >= nEnd)
				break;
		}
}

void CSpecialEdit::OnSelAll()
{
	CPoint ptHead,ptTail;

//	HideEditCaret();
	ptHead.x=0;
	ptHead.y=0;
	m_ptCursor = ptHead ;  // Set Cursor to head

	ptTail.y = GetTextLineCount() - 1;

	LineInfo lineTail;

	lineTail=m_aDataLine.GetAt(ptTail.y);
	ptTail.x = lineTail.m_strLine->GetLength();
	CursorPos2CaretPos();
	SetSelection(ptHead,ptTail);
//	ShowEditCaret();
	EnsureVisible();
}



void CSpecialEdit::RecalcHorzScrollBar(BOOL bPositionOnly)
{
	SCROLLINFO si;
	si.cbSize = sizeof(si);
	if (bPositionOnly)
	{
		si.fMask = SIF_POS;
		si.nPos = m_nOffsetChar;
	}
	else
	{
		si.fMask = SIF_DISABLENOSCROLL | SIF_PAGE | SIF_POS | SIF_RANGE;
		si.nMin = 0;
		si.nMax = MAX_CHARWIDTH;
		si.nPage = SCREEN_CHARWIDTH;
		si.nPos = m_nOffsetChar;
	}
	VERIFY(SetScrollInfo(SB_HORZ, &si));
}

void CSpecialEdit::ScrollToHorz(int nNewLeftChar)
{
	if (m_nOffsetChar != nNewLeftChar)
	{
		int nScrollWidth = m_nOffsetChar - nNewLeftChar;
		m_nOffsetChar = nNewLeftChar;
//		ScrollWindow(nScrollWidth * CHARWIDTH, 0);
//		UpdateWindow();
		RecalcHorzScrollBar(TRUE);
		InvalidateLines(GetFirstVisibleLine(), -1);

//		HideEditCaret();
		CursorPos2CaretPos();
//		ShowEditCaret();
//		InvalidateRect(NULL,TRUE);
	}
}

void CSpecialEdit::ClearMarkArea(CDC *pdc,int nLineIndex)
{
	RECT rect;

//	GetClientRect(&rect);
	rect.left=0;
	rect.right=m_nMarkAreaWidth;
	rect.top=(nLineIndex-GetFirstVisibleLine()) * GetLineHeight();
	rect.bottom=rect.top + GetLineHeight();
	pdc->FillSolidRect(&rect,color_MarkArea);
}

int CSpecialEdit::GetScreenCharCount()
{
	int nScreenWidth;
	RECT rect;

//	nLineHeight=GetLineHeight();
	GetClientRect(&rect);
	nScreenWidth=rect.right-rect.left;
	return (nScreenWidth-m_nMarkAreaWidth)/m_nCharWidth;
}


BOOL CSpecialEdit::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	if (nHitTest == HTCLIENT)
	{
		POINT point;
		GetCursorPos(&point);
		ScreenToClient(&point);
// Jami chen delete for can build ,will to be open
		if (point.x < m_nMarkAreaWidth)
			::SetCursor(::LoadCursor(NULL, IDC_ARROW));
		else
			::SetCursor(::LoadCursor(NULL, IDC_IBEAM));
		return TRUE;
	}
	else
		return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CSpecialEdit::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here

	RecalcVertScrollBar();
	RecalcHorzScrollBar();

}


void CSpecialEdit::Clear()
{
	if (ExistSelection())
	{
		DeleteSelection();
		return;
	}
}

void CSpecialEdit::Copy()
{
	OnCopy();
}

void CSpecialEdit::Cut()
{
	OnCut();
}

void CSpecialEdit::Paste()
{
	OnPaste();
}

BOOL CSpecialEdit::CanUndo()
{
	if (curUndoLink -> prevLink == NULL)
		return FALSE;
	else
		return TRUE;
}

int CSpecialEdit::GetLineCount()
{
	return GetTextLineCount();
}

void CSpecialEdit::SetMargins(UINT nLeft, UINT nRight)
{
	if ((int)nLeft != m_nMarkAreaWidth)
	{
		m_nMarkAreaWidth = nLeft;
		InvalidateRect(NULL,TRUE);
	}
}

DWORD CSpecialEdit::GetMargins()
{
	DWORD dwMargins;

		dwMargins = MAKELONG(m_nMarkAreaWidth , 0);
		return dwMargins;
}

int CSpecialEdit::GetLine(int nIndex, LPTSTR lpszBuffer, int nMaxLength)
{

	LineInfo LineInfo1;
	

		if (nIndex < 0 || nIndex > GetLineCount())
			return 0;
		LineInfo1 = m_aDataLine.GetAt(nIndex);
		int nCopyCount;
			
		if (nMaxLength > LineInfo1.m_strLine->GetLength())
			nCopyCount=LineInfo1.m_strLine->GetLength();
		else
			nCopyCount=nMaxLength-1;

		LPTSTR lpLine;

		lpLine = LineInfo1.m_strLine->LockBuffer();
		strncpy(lpszBuffer,lpLine,nCopyCount);
		lpszBuffer[nCopyCount] = 0;

		return nCopyCount;
}

#define BOOKMARKS   0x00000001
#define BREAKPOINT	0x00000002

void CSpecialEdit::ShowMarkFlag(CDC *pdc ,int nIndex)
{
	LineInfo LineInfo1;
	RECT rect;

//	GetClientRect(&rect);
		rect.left=0;
		rect.right=m_nMarkAreaWidth;
		rect.top=(nIndex-GetFirstVisibleLine()) * GetLineHeight();
		rect.bottom=rect.top + GetLineHeight();
		pdc->FillSolidRect(&rect,color_MarkArea);

		LineInfo1 = m_aDataLine.GetAt(nIndex);
		if (LineInfo1.m_dwMarkFlag & BOOKMARKS)
		{
			CPen hPen(PS_SOLID,1,RGB(0,0,0));
			CBrush hBrush(color_BookMarks);
			CPen *oldPen;
			CBrush *oldBrush;
			
			oldPen = pdc->SelectObject(&hPen);
			oldBrush = pdc ->SelectObject(&hBrush);
			pdc->RoundRect(rect.left + 1 ,rect.top  ,rect.right - 1, rect.bottom , 5 , 5 );
			pdc->SelectObject(oldPen);
			pdc ->SelectObject(oldBrush);
		}
		if (LineInfo1.m_dwMarkFlag & BREAKPOINT)
		{
			int nOffset;
			int oldMode;
			COLORREF oldColor;
				
				oldMode = pdc->SetBkMode(TRANSPARENT);
				oldColor = pdc->SetTextColor(color_BreakPoint);
				nOffset = (m_nMarkAreaWidth - 16 ) /2;
				pdc->TextOut(rect.left + nOffset,rect.top, "●");
				pdc->SetBkMode(oldMode);
				pdc->SetTextColor(oldColor);
		}
	
}

void CSpecialEdit::SetMarkFlag(int nIndex, DWORD dwFlag)
{
	DWORD dwNewMarkFlag = 0;
	LineInfo LineInfo1;

		if ( nIndex == -1 )
		{
			nIndex = m_ptCursor.y;
		}
		LineInfo1 = m_aDataLine.GetAt(nIndex);
		if (dwFlag & FLAG_BOOKMARKS)
		{
			dwNewMarkFlag |= BOOKMARKS ;
		}
		if (dwFlag & FLAG_BREAKPOINT)
		{
			dwNewMarkFlag |= BREAKPOINT ;
		}

		int nScreenCount = GetScreenCount();
		if (dwNewMarkFlag != LineInfo1.m_dwMarkFlag)
		{
			LineInfo1.m_dwMarkFlag = dwNewMarkFlag;
			m_aDataLine.SetAt(nIndex,LineInfo1);
			if (nIndex < m_nTopLine || nIndex > m_nTopLine + nScreenCount)
			{
				// Is Not In the Screen , not display
				return ;
			}
			CDC *pdc;
			pdc = GetDC();
			DrawLine(pdc,nIndex);
			ReleaseDC(pdc);
		}
}

void CSpecialEdit::ClearMarkFlag(int nIndex, DWORD dwFlag)
{
	DWORD dwNewMarkFlag = 0;
	LineInfo LineInfo1;
		if ( nIndex == -1 )
		{
			nIndex = m_ptCursor.y;
		}
		LineInfo1 = m_aDataLine.GetAt(nIndex);
		if (dwFlag & FLAG_BOOKMARKS)
		{
			dwNewMarkFlag &= ~BOOKMARKS ;
		}
		if (dwFlag & FLAG_BREAKPOINT)
		{
			dwNewMarkFlag &= ~BREAKPOINT ;
		}
		if (dwNewMarkFlag != LineInfo1.m_dwMarkFlag)
		{
			LineInfo1.m_dwMarkFlag = dwNewMarkFlag;
			m_aDataLine.SetAt(nIndex,LineInfo1);
			CDC *pdc;
			pdc = GetDC();
			DrawLine(pdc,nIndex);
			ReleaseDC(pdc);
		}
}

void CSpecialEdit::ClearAllMarkFlag(DWORD dwFlag)
{
	int nIndex = GetFirstVisibleLine();

	if (GetMarkFlag(nIndex,dwFlag) ==TRUE)
		ClearMarkFlag(nIndex,dwFlag);

	while(1)
	{
		nIndex = GetNextMarkFlag(nIndex, dwFlag) ;
		if (nIndex == -1)
			break;
		ClearMarkFlag(nIndex,dwFlag);
	}
}

void CSpecialEdit::ToNextMarkFlag(int nIndex, DWORD dwFlag)
{
	int nCaretIndex;
	nCaretIndex=GetNextMarkFlag(nIndex,dwFlag);
	if (nCaretIndex != -1)
	{
		m_ptCursor.y = nCaretIndex;
		m_ptCursor.x = 0;
		CursorPos2CaretPos();
		EnsureVisible();
	}
}
int CSpecialEdit::GetNextMarkFlag(int nIndex, DWORD dwFlag)
{
	if ( nIndex == -1 )
	{
		nIndex = m_ptCursor.y;
	}
	nIndex ++ ; // Will Search Next Mark
	int nCount = GetLineCount();

	while(nIndex < nCount)
	{
		if (GetMarkFlag(nIndex,dwFlag) ==TRUE)
			return nIndex;
		nIndex ++;
	}

	nIndex = 0;
	while(nIndex < nCount)
	{
		if (GetMarkFlag(nIndex,dwFlag) ==TRUE)
			return nIndex;
		nIndex ++;
	}
	return -1;
}

void CSpecialEdit::ToPrevMarkFlag(int nIndex, DWORD dwFlag)
{
	int nCaretIndex;
	nCaretIndex=GetPrevMarkFlag(nIndex,FLAG_BOOKMARKS);
	if (nCaretIndex != -1)
	{
		m_ptCursor.y = nCaretIndex;
		m_ptCursor.x = 0;
		CursorPos2CaretPos();
		EnsureVisible();
	}
}
int CSpecialEdit::GetPrevMarkFlag(int nIndex, DWORD dwFlag)
{
	if ( nIndex == -1 )
	{
		nIndex = m_ptCursor.y;
	}
	nIndex -- ; // Will Search Next Mark
	int nCount = GetLineCount();

	while(nIndex >= 0)
	{
		if (GetMarkFlag(nIndex,dwFlag) ==TRUE)
			return nIndex;
		nIndex --;
	}

	nIndex = nCount-1;
	while(nIndex >= 0)
	{
		if (GetMarkFlag(nIndex,dwFlag) ==TRUE)
			return nIndex;
		nIndex --;
	}
	return -1;
}

void CSpecialEdit::ProcessF2()
{

	SHORT nShift;
	short nCtrl;
	int nCaretIndex;

		nCtrl = GetAsyncKeyState(VK_CONTROL);
		nShift=GetAsyncKeyState(VK_SHIFT);

		nCaretIndex = m_ptCursor.y;

		if (nCtrl)
		{
			if (nShift)
			{
				ClearAllMarkFlag(FLAG_BOOKMARKS);
			}
			else
			{
				if (GetMarkFlag(nCaretIndex,FLAG_BOOKMARKS)==FALSE)
					SetMarkFlag(nCaretIndex,FLAG_BOOKMARKS);
				else
					ClearMarkFlag(nCaretIndex,FLAG_BOOKMARKS);
			}
		}
		else
		{
			ToNextMarkFlag(nCaretIndex,FLAG_BOOKMARKS);
		}
}

BOOL CSpecialEdit::GetMarkFlag(int nIndex, DWORD dwFlag)
{
	LineInfo LineInfo1;
		if ( nIndex == -1 )
		{
			nIndex = m_ptCursor.y;
		}
		LineInfo1 = m_aDataLine.GetAt(nIndex);
		if (dwFlag & FLAG_BOOKMARKS)
		{
			return LineInfo1.m_dwMarkFlag & BOOKMARKS;
		}
		if (dwFlag & FLAG_BREAKPOINT)
		{
			return LineInfo1.m_dwMarkFlag & FLAG_BREAKPOINT;
		}
		return FALSE;
}

void CSpecialEdit::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here	
	pCmdUI->Enable(ExistSelection());
}

void CSpecialEdit::ProcessF3()
{
	short nCtrl;
//	int nCaretIndex;

		nCtrl = GetAsyncKeyState(VK_CONTROL);

		if (nCtrl)
		{
			// Get Current word , and Search this Word
			CString strWord;
			int nCharPos;
			nCharPos = GetWordAtPos(m_ptCursor,strWord);
			if (nCharPos == -1)
				return ;
			m_strSearch = strWord;
//			SetSearchString(m_strSearch);
			((CAmboApp *)AfxGetApp())->SetSearchString(m_strSearch);
		}
		else
		{
			GetSearchString(m_strSearch);
		}
		CPoint ptFind;
		// Search the SearchString
		SearchString(CPoint(-1,-1),m_strSearch,m_bMatchAllWordOnly, m_bMatchCase,DIRECTION_DOWN);
/*
		if (ptFind.x == -1 || ptFind.y == -1)
		{
			// Can.t Find the word
			MessageBox("找不到字符串");
		}
		m_ptCursor.y = ptFind.y;
		m_ptCursor.x = ptFind.x + m_strSearch.GetLength();

		CursorPos2CaretPos();
		
		CPoint ptStart,ptEnd;

		ptStart = ptFind;
		ptEnd = m_ptCursor;

		SetSelection(ptStart,ptEnd);
		EnsureVisible();
*/
}

int CSpecialEdit::GetWordAtPos(CPoint ptPos, CString &strWord)
{
	LineInfo LineInfo = m_aDataLine.GetAt(ptPos.y);

	if (LineInfo.m_strLine->GetLength() < ptPos.x)
		return -1;

	LPTSTR lpLine = LineInfo.m_strLine->LockBuffer();

	int nWordLen =0,nCharPos = 0;

	while(1)
	{
		if ( *lpLine == NULL)
			break;
		strWord = GetWord(lpLine);
		nWordLen =strWord.GetLength();

		if (nCharPos + nWordLen >= ptPos.x)
		{
			// Will Selection the word
			break;
		}
		lpLine +=nWordLen;
		nCharPos += nWordLen;
	}

	LineInfo.m_strLine->UnlockBuffer();
	return nCharPos;
}

BOOL CSpecialEdit::SearchString(CPoint ptStart, CString m_strSearch, BOOL bMatchAllWordOnly, BOOL bMatchCase, int iDirection)
{
	int iPos;
	int nCount = GetLineCount();
	CString line;
	CString find;
	LineInfo LineInfo;
	
	find = m_strSearch;
	if (bMatchCase == FALSE)
	{
		find.MakeUpper();
	}

	// Can Find in the current Line ?
	if (ptStart.x == -1 || ptStart.y == -1)
	{
		ptStart = m_ptCursor;
	}

	CPoint ptCur = ptStart;

	if (iDirection== DIRECTION_UP)
	{
		while(ptCur.y >= 0 )
		{
			LineInfo = m_aDataLine.GetAt(ptCur.y);
		
			line = *LineInfo.m_strLine;
			if (bMatchCase == FALSE)
			{
				line.MakeUpper();
			}

			while(1)
			{
				iPos = line.Find(find,ptCur.x);
		//		iPos = LineInfo.m_strLine->Find(m_strSearch,ptCur.x);
				if (iPos != -1)
				{
					if (bMatchAllWordOnly == TRUE)
					{
						if (IsWholeWord(line,find,iPos) == FALSE)
						{
							ptCur.x = iPos + 1;
							continue;
						}
					}
					// Had find
					ptCur.x = iPos;
		//			return ptCur;
					goto FindResult;
				}
				else
				{
					break;
				}
			}
			ptCur.y --;
			ptCur.x = 0;
		}
		// Had Find to the End , Will from the Text Start
		ptCur.y = nCount -1;
		while(ptCur.y >= ptStart.y )
		{
			LineInfo = m_aDataLine.GetAt(ptCur.y);

			line = *LineInfo.m_strLine;
			if (bMatchCase == FALSE)
			{
				line.MakeUpper();
			}

			while(1)
			{
				iPos = line.Find(find,ptCur.x);
		//		iPos = LineInfo.m_strLine->Find(m_strSearch,ptCur.x);
				if (iPos != -1)
				{
					if (bMatchAllWordOnly == TRUE)
					{
						if (IsWholeWord(line,find,iPos) == FALSE)
						{
							ptCur.x = iPos + 1;
							continue;
						}
					}
					// Had find
					ptCur.x = iPos;
					goto FindResult;
		//			return ptCur;
				}
				else
				{
					break;
				}
			}
			ptCur.y --;
			ptCur.x = 0;
		}
	}
	else
	{
		while(ptCur.y < nCount )
		{
			LineInfo = m_aDataLine.GetAt(ptCur.y);
		
			line = *LineInfo.m_strLine;
			if (bMatchCase == FALSE)
			{
				line.MakeUpper();
			}

			while(1)
			{
				iPos = line.Find(find,ptCur.x);
		//		iPos = LineInfo.m_strLine->Find(m_strSearch,ptCur.x);
				if (iPos != -1)
				{
					if (bMatchAllWordOnly == TRUE)
					{
						if (IsWholeWord(line,find,iPos) == FALSE)
						{
							ptCur.x = iPos + 1;
							continue;
						}
					}
					// Had find
					ptCur.x = iPos;
		//			return ptCur;
					goto FindResult;
				}
				else
				{
					break;
				}
			}
			ptCur.y ++;
			ptCur.x = 0;
		}
		// Had Find to the End , Will from the Text Start
		ptCur.y = 0;
		while(ptCur.y <= ptStart.y )
		{
			LineInfo = m_aDataLine.GetAt(ptCur.y);

			line = *LineInfo.m_strLine;
			if (bMatchCase == FALSE)
			{
				line.MakeUpper();
			}

			while(1)
			{
				iPos = line.Find(find,ptCur.x);
		//		iPos = LineInfo.m_strLine->Find(m_strSearch,ptCur.x);
				if (iPos != -1)
				{
					if (bMatchAllWordOnly == TRUE)
					{
						if (IsWholeWord(line,find,iPos) == FALSE)
						{
							ptCur.x = iPos + 1;
							continue;
						}
					}
					// Had find
					ptCur.x = iPos;
					goto FindResult;
		//			return ptCur;
				}
				else
				{
					break;
				}
			}
			ptCur.y ++;
			ptCur.x = 0;
		}

	}
	// Had not find
	ptCur.y = -1;
	ptCur.x = -1;

FindResult:
	
	if (ptCur.x == -1 || ptCur.y == -1)
	{
		// Can.t Find the word
		MessageBox("找不到字符串");
		return FALSE;
	}
	m_ptCursor.y = ptCur.y;
	m_ptCursor.x = ptCur.x + m_strSearch.GetLength();

	CursorPos2CaretPos();
	
	CPoint r_ptStart,r_ptEnd;

	r_ptStart = ptCur;
	r_ptEnd = m_ptCursor;

	SetSelection(r_ptStart,r_ptEnd);
	EnsureVisible();
	return TRUE;
}

void CSpecialEdit::MarkAllString(CString strWord, BOOL bMatchAllWordOnly, BOOL bMatchCase)
{
	int iPos;
	int nCount = GetLineCount();

	CPoint ptCur(0,0);

	LineInfo LineInfo = m_aDataLine.GetAt(ptCur.y);
	
	while(ptCur.y < nCount )
	{
		LineInfo = m_aDataLine.GetAt(ptCur.y);
		CString line;
		CString find;
		
		line = *LineInfo.m_strLine;
		find = m_strSearch;
		if (bMatchCase == FALSE)
		{
			line.MakeUpper();
			find.MakeUpper();
		}
		while (1)
		{
			iPos = line.Find(find,ptCur.x);
			if (iPos != -1)
			{
				if (bMatchAllWordOnly == TRUE)
				{
					if (IsWholeWord(line,find,iPos) == FALSE)
					{
						{
							ptCur.x = iPos + 1;
							continue;
						}
					}
				}
				SetMarkFlag(ptCur.y,FLAG_BOOKMARKS);
			}
			break;
		}
		ptCur.y ++;
		ptCur.x = 0;
	}

	return ;
}

BOOL CSpecialEdit::IsWholeWord(CString line, CString find, int iPos)
{
	int nlenFind = find.GetLength();
	int ch;
	// Then must the the word head and the tail must is space or NULL;
	if (iPos !=0)
	{
		ch=line.GetAt(iPos-1);
		if ( ch<0x80 && (isalnum(ch) !=0  || ch =='_' ))  // cheak the head
		{
			return FALSE;
		}
	}
	if (iPos+nlenFind != line.GetLength())
	{
		ch=line.GetAt(iPos+nlenFind);
		if ( ch < 0x80 && (isalnum( ch ) !=0  || ch =='_' ))  // cheak the tail
		{
			return FALSE;
		}
	}
	return TRUE;
}

void CSpecialEdit::PrintLine(CDC *pDC, int nLine,int y )
{
	LineInfo LineInfo;
	LPTSTR lpLine;
	CString strWord;
	int x;//,y;
	CPoint pt_CurPos;
	COLORREF color_OrgWord;//,color_OrgBackColor;
	int nNeedShowpos = 0;
	int nScreenWidth = 0;
	int bDefineStart = FALSE;

//	RECT rect;

		if (m_aDataLine.GetSize()<=nLine)
		{// This is have no data , But Must Clear it
			return;
		}
		LineInfo=m_aDataLine.GetAt(nLine);
		lpLine=LineInfo.m_strLine->LockBuffer();

		m_bSingleMask = GetLineSingleMaskFlag(LineInfo);
		m_bMultiMask =  GetLineMultiMaskFlag(LineInfo);
		m_bDQuotation = GetLineDoubleQuotationFlag(LineInfo);
		m_bSingleQuotation = GetLineSingleQuotationFlag(LineInfo);


//		x=m_nMarkAreaWidth;
		x=m_nMarkAreaWidth - m_nOffsetChar * m_nCharWidth;
//		y=(nLine-GetFirstVisibleLine()) * GetLineHeight();
		pt_CurPos.y = nLine;
		pt_CurPos.x = 0;
		while(TRUE)
		{

//			if (x > nScreenWidth)
//				break;
			strWord=GetWord(lpLine);
			if (strWord.GetLength() == 0)
				break;
			nNeedShowpos = 0;
			// Normal Text
			//pdc->SetTextColor(GetWordColor(strWord));
//			color_OrgWord = GetWordColor(strWord);
			if (bDefineStart == FALSE)
			{
				if (IsDefineWord(strWord) == TRUE)
					bDefineStart  = TRUE;
				color_OrgWord = GetWordColor(strWord);
			}
			else
			{
				color_OrgWord = color_NormalText;//GetWordColor(strWord);
			}

			if (*lpLine == '/')
			{
				if (m_bMultiMask == FALSE)  //  '/*  */' Mask
				{
					if (IsMultiMskStart(lpLine))  // x != 0 is because the end must not the first char
					{
						m_bMultiMask = TRUE;
						strWord += "*";
					}
//					pdc->SetTextColor(color_Mask);
				}
				if (m_bSingleMask == FALSE && m_bMultiMask == FALSE)  //  '//' Mask
				{
					if (IsSingleMaskStart(lpLine))  // x != 0 is because the end must not the first char
					{
						m_bSingleMask = TRUE;
						strWord += "/";
					}
				}
			}
			else if (*lpLine == '*')
			{
				if (m_bMultiMask == TRUE)  //  '/*  */' Mask
				{
					color_OrgWord = color_Mask ;
					if (IsMultiMskEnd(lpLine))  
					{
						m_bMultiMask = FALSE;
						strWord += "/";
//						lpCurPos +=2;  // '*/*' ,then the second * is not Start Mask
//						continue;
					}
				}
			}
			else if (bDefineStart == TRUE)
			{ // if the Define mode ,then nothing do ,will show normal
			}
			else if (IsDoubleQuotation(lpLine) ==TRUE )
			{
				if (m_bDQuotation == TRUE)
				{
					//pdc->SetTextColor(color_Quotation);
					color_OrgWord = color_Quotation;
					m_bDQuotation = FALSE;
				}
				else
					m_bDQuotation = TRUE;
			}
			else if (IsSingleQuotation(lpLine) == TRUE)
			{
				if (m_bSingleQuotation == TRUE)
				{
					//pdc->SetTextColor(color_Quotation);
					color_OrgWord = color_Quotation;
					m_bSingleQuotation = FALSE;
				}
				else
					m_bSingleQuotation = TRUE;
			}
			if (m_bMultiMask == TRUE)  //  '/*  */' Mask
			{
				//pdc->SetTextColor(color_Mask);
				color_OrgWord = color_Mask;
			}
			else if (m_bSingleMask == TRUE)  //  '//' Mask
			{
				//pdc->SetTextColor(color_Mask);
				color_OrgWord = color_Mask;
			}
			else if (m_bDQuotation == TRUE)  //  "   "  
			{
				//pdc->SetTextColor(color_Quotation);
				color_OrgWord = color_Quotation;
			}
			else if (m_bSingleQuotation == TRUE)  // '   '
			{
				//pdc->SetTextColor(color_Quotation);
				color_OrgWord = color_Quotation;
			}

			if (IsSpace(strWord))
			{
				lpLine += strWord.GetLength();
				pt_CurPos.x += strWord.GetLength();
//				strWord.Empty();
//				int nTabSize=GetTabSize();
//				for (int i=0; i<nTabSize;i++)
//					strWord.Insert(0,' ');
				ExternWord(strWord,strWord);
				pDC->SetTextColor(color_OrgWord);
				// The Word Need Display
				if (x<m_nMarkAreaWidth)
				{
					if ((x + GetWordWidth(pDC,strWord)) < m_nMarkAreaWidth )
					{
						// Is Not Show This word
						nNeedShowpos = -1;
					}
					else
					{
						nNeedShowpos = (m_nMarkAreaWidth - x) / m_nCharWidth;
					}
				}
				if (nNeedShowpos >= 0 )
				{
					pDC->TextOut(x + nNeedShowpos * m_nCharWidth,y,(strWord.LockBuffer() + nNeedShowpos));
					strWord.UnlockBuffer();
				}
				x += GetWordWidth(pDC,strWord);
			}
			else
			{
				pDC->SetTextColor(color_OrgWord);
				pDC->TextOut(x,y,strWord);
				lpLine += strWord.GetLength();
				x += GetWordWidth(pDC,strWord);
			}
		}
//		LineInfo.m_strLine->UnlockBuffer();
}



int CSpecialEdit::GetExternCharLength(CPoint ptPos)
{
	LineInfo LineInfo;
	LPTSTR lpLine;
	int nCharCount = 0;
	int nPos;
//	CString strExtern;
	
		LineInfo=m_aDataLine.GetAt(ptPos.y);
//		ExternWord(*LineInfo.m_strLine,strExtern);
//		lpLine = strExtern.LockBuffer();
		lpLine = LineInfo.m_strLine->LockBuffer();
//		lpLine += m_nOffsetChar;
//		nPos = m_nOffsetChar;
		nPos = 0;
		while(1)
		{
//			if (nPos >= m_ptCursor.x)
			if (nPos >= ptPos.x)
				break;
			if (*lpLine == '\t')
				nCharCount += m_nTabSize;
			else
				nCharCount ++;
			nPos ++;
			lpLine ++;
		}
//		strExtern.UnlockBuffer();
		LineInfo.m_strLine->UnlockBuffer();
		return nCharCount;
}

int CSpecialEdit::GetCursorVisiblePos()
{
	LineInfo LineInfo;
//	LPTSTR lpLine;
	int nCharCount = 0;
	int nPos = 0;
	int nScreenCharCount = GetScreenCharCount();
	int nCurPos = GetExternCharLength(m_ptCursor);
	
		nPos = nCurPos - nScreenCharCount + 32;
		/*
		LineInfo=m_aDataLine.GetAt(m_ptCursor.y);
		lpLine = LineInfo.m_strLine->LockBuffer();
		lpLine += m_ptCursor.x;
		nPos = m_ptCursor.x;
		while(1)
		{
			if (nCharCount >= nScreenCharCount - 32)
				break;
			if (*lpLine == '\t')
			{
				nCharCount += m_nTabSize;
				nPos --;
				lpLine --;
			}
			else
			{
				if (*lpLine <0 || *lpLine >=0x80)
				{
					nCharCount +=2;
					nPos -=2;
					lpLine -=2;
				}
				else
				{
					nCharCount ++;
					nPos --;
					lpLine --;
				}
			}
		}
		LineInfo.m_strLine->UnlockBuffer();
		*/
		return nPos;
}
/*
int CSpecialEdit::GetnCharHead2Visible(int nIndex)
{
	LineInfo LineInfo;
	LPTSTR lpLine;
	int nCharCount = 0;
	int nPos;
	
		LineInfo=m_aDataLine.GetAt(nIndex);
		lpLine = LineInfo.m_strLine->LockBuffer();
//		lpLine += m_nOffsetChar;
		nPos = 0;
		while(1)
		{
			if (nPos >= m_nOffsetChar)
				break;
			if (*lpLine == '\t')
				nCharCount += m_nTabSize;
			else
				nCharCount ++;
			nPos ++;
			lpLine ++;
		}
		LineInfo.m_strLine->UnlockBuffer();
		return nCharCount;
}
*/
int CSpecialEdit::WordInSelection(CPoint pt_Point, CString strWord)
{
	CPoint pt_Start, pt_End;
//	BOOL bStartInSel, bEndInSel;
	CPoint pt_SelStart ,pt_SelEnd;

		pt_Start = pt_End = pt_Point;
		pt_End.x = pt_Start.x + strWord.GetLength();
//		bStartInSel = IsSelection(pt_Start);
//		bEndInSel = IsSelection(pt_End);

		// Adjust the Selection Start and End Position
		if (m_ptSelStart.y > m_ptSelEnd.y)
		{
			pt_SelStart = m_ptSelEnd;
			pt_SelEnd = m_ptSelStart;
		}
		else if (m_ptSelStart.y < m_ptSelEnd.y)
		{
			pt_SelStart = m_ptSelStart;
			pt_SelEnd = m_ptSelEnd;
		}
		else
		{
			if (m_ptSelStart.x > m_ptSelEnd.x)
			{
				pt_SelStart = m_ptSelEnd;
				pt_SelEnd = m_ptSelStart;
			}
			else
			{
				pt_SelStart = m_ptSelStart;
				pt_SelEnd = m_ptSelEnd;
			}
		}
		
		// Get The Special line Selection
		if ((pt_SelStart.y > pt_Start.y) || (pt_SelEnd.y < pt_Start.y))
		{ // the Special line has not selection
			return WHOLENOTINSELECTION;
		}

		if ((pt_SelStart.y < pt_Start.y) && (pt_SelEnd.y > pt_Start.y))
		{ // the special line has in the selection
			return WHOLEINSELECTION;
		}

		if ((pt_SelStart.y == pt_Start.y)  && (pt_SelEnd.y > pt_Start.y))
		{
			if (pt_SelStart.x > pt_End.x)
			{
				return WHOLENOTINSELECTION;
			}
			else if (pt_SelStart.x < pt_Start.x)
			{
				return WHOLEINSELECTION;
			}
			else
			{
				return HALFINSELECTION;
			}
		}
		else if ((pt_SelStart.y < pt_Start.y)  && (pt_SelEnd.y == pt_Start.y))
		{
			if (pt_SelEnd.x > pt_End.x)
			{
				return WHOLEINSELECTION;
			}
			else if (pt_SelEnd.x < pt_Start.x)
			{
				return WHOLENOTINSELECTION;
			}
			else
			{
				return HALFINSELECTION;
			}
		}
		else
		{
			if (pt_SelStart.x > pt_End.x)
			{
				return WHOLENOTINSELECTION;
			}
			else if (pt_SelEnd.x < pt_Start.x)
			{
				return WHOLENOTINSELECTION;
			}
			else if ((pt_SelStart.x < pt_Start.x) && (pt_SelEnd.x > pt_End.x) )
			{
				return WHOLEINSELECTION;
			}
			else
			{
				return HALFINSELECTION;
			}
		}


}

int CSpecialEdit::GetnCharExtern(LPCTSTR pszChars,int nLength)
{
	int nCharCount = 0;
	int nPos = 0;
		while(*pszChars)
		{
			if (*pszChars == '\t')
				nCharCount += m_nTabSize;
			else
				nCharCount ++;
			pszChars ++;
			if (nLength != -1)
			{
				nPos ++;
				if (nPos >=nLength)
					break;
			}
		}
		return nCharCount;
}

void CSpecialEdit::ExternWord(CString strSrc,CString &strWord)
{
	CString strExtern;
	LPTSTR lpLine;

		lpLine = strSrc.LockBuffer();
		while(*lpLine)
		{
			if ( *lpLine == '\t')
				strExtern.Insert(strExtern.GetLength(),"    ");
			else
				strExtern.Insert(strExtern.GetLength(),*lpLine);
			lpLine ++;
		}
		strWord = strExtern;
}

BOOL CSpecialEdit::CanMergeNextLine(int nLineIndex)
{
	int nLineLength,nNextLineLength;
	LineInfo LineInfo;

	LineInfo=m_aDataLine.GetAt(nLineIndex);	
	nLineLength = GetnCharExtern(LineInfo.m_strLine->LockBuffer());
	LineInfo.m_strLine->UnlockBuffer();

	LineInfo=m_aDataLine.GetAt(nLineIndex+1);	
	nNextLineLength = GetnCharExtern(LineInfo.m_strLine->LockBuffer());
	LineInfo.m_strLine->UnlockBuffer();

	if (nLineLength + nNextLineLength > MAX_LINELENGTH)
		return FALSE;
	else
		return TRUE;
}

void CSpecialEdit::TestError()
{
	int iPos;
	int nCount = GetLineCount();
	CString line;
	CString find("\xd");
	LineInfo LineInfo;
	CPoint ptStart(0,0);
	

	// Can Find in the current Line ?
	CPoint ptCur = ptStart;

	while(ptCur.y < nCount )
	{
		LineInfo = m_aDataLine.GetAt(ptCur.y);
	
		line = *LineInfo.m_strLine;

		while(1)
		{
			iPos = line.Find(find,ptCur.x);
	//		iPos = LineInfo.m_strLine->Find(m_strSearch,ptCur.x);
			if (iPos != -1)
			{
				// Had find
				ptCur.x = iPos;
	//			return ptCur;
				goto FindResult;
			}
			else
			{
				break;
			}
		}
		ptCur.y ++;
		ptCur.x = 0;
	}
	// Had Find to the End , Will from the Text Start
	ptCur.y = 0;
	while(ptCur.y <= ptStart.y )
	{
		LineInfo = m_aDataLine.GetAt(ptCur.y);

		line = *LineInfo.m_strLine;

		while(1)
		{
			iPos = line.Find(find,ptCur.x);
	//		iPos = LineInfo.m_strLine->Find(m_strSearch,ptCur.x);
			if (iPos != -1)
			{
				// Had find
				ptCur.x = iPos;
				goto FindResult;
	//			return ptCur;
			}
			else
			{
				break;
			}
		}
		ptCur.y ++;
		ptCur.x = 0;
	}
	// Had not find
	ptCur.y = -1;
	ptCur.x = -1;

FindResult:
	
	if (ptCur.x == -1 || ptCur.y == -1)
	{
		// Can.t Find the word
		return ;
	}
	MessageBox("找到非法字符");
	return ;
}


void CSpecialEdit::WarningCanntCompleteAction()
{
	Beep(1800,100);
}


// -------------------------------------------------------------------//
// process Undo
// -------------------------------------------------------------------//


void CSpecialEdit::InsertNewUndoLink(LPUNDOLINK newUndoLink)
{
	if (curUndoLink ==NULL || newUndoLink == NULL)
		return ;
	if (MergeNewLinkToCurrentLink(newUndoLink) == TRUE)
	{
			DeleteUndoLink(newUndoLink);
			return;
	}

	if (curUndoLink->nextLink)
	{
		DeleteUndoLink(curUndoLink->nextLink);
		curUndoLink->nextLink = NULL;
	}
	curUndoLink->nextLink = newUndoLink;
	newUndoLink->prevLink = curUndoLink;
	curUndoLink = newUndoLink;
	
	ReleaseTheOldestUndoLink();
}

BOOL CSpecialEdit::MergeNewLinkToCurrentLink(LPUNDOLINK newUndoLink)
{
	if ((newUndoLink->iProcessStyle & TYPING) == FALSE)  // Only can Merge the Typing process
		return FALSE;
	if ((curUndoLink->iProcessStyle & TYPING) == FALSE)  // Only can Merge the Typing process
		return FALSE;
	if (newUndoLink->strDelete->IsEmpty() == FALSE)  // must have not Replace process
		return FALSE; 
	if (newUndoLink->startCursor != curUndoLink->endCursor)  // must continue the prev process
		return FALSE;
	if (newUndoLink->strInsert->GetLength() > 1)
		return FALSE;

	int typeCurLink, typeNewLink;

	typeCurLink = isalnum(curUndoLink->strInsert->GetAt(0));
	typeNewLink = isalnum(newUndoLink->strInsert->GetAt(0));

	if (typeNewLink != 0 && typeCurLink == 0)
		return FALSE;
	if (typeNewLink == 0 && typeCurLink != 0)
		return FALSE;

	// now merge the newUndoLink to Current Undo Link

	curUndoLink->strInsert->Insert(curUndoLink->strInsert->GetLength(),newUndoLink->strInsert->LockBuffer());
	curUndoLink->endCursor = newUndoLink->endCursor;
	curUndoLink->endInsert = newUndoLink->endInsert;

	return TRUE;
}


void CSpecialEdit::InitUndoLinkStruct(LPUNDOLINK lpUndoLink)
{
	lpUndoLink->iProcessStyle = NOPROCESS;
	lpUndoLink->strDelete = new CString("");
	lpUndoLink->strInsert = new CString("");
	lpUndoLink->nextLink = NULL;
	lpUndoLink->prevLink = NULL;
	lpUndoLink->startInsert = lpUndoLink->endInsert = CPoint(0,0);
	lpUndoLink->startDelete = lpUndoLink->endDelete = CPoint(0,0);
	lpUndoLink->startCursor = m_ptCursor;
	lpUndoLink->endCursor = CPoint(0,0);
}

void CSpecialEdit::DeleteUndoLink(LPUNDOLINK lpUndoLink)
{
	LPUNDOLINK curLink,nextLink;

		curLink = lpUndoLink;
		while(curLink)
		{
			nextLink = curLink->nextLink;
			ReleaseUndoLink(curLink);
			curLink = nextLink;
		}
}

void CSpecialEdit::ReleaseUndoLink(LPUNDOLINK lpUndoLink)
{
	if (lpUndoLink == NULL)
		return;

	if (lpUndoLink->strDelete)
		delete lpUndoLink->strDelete;

	if (lpUndoLink->strInsert)
		delete lpUndoLink->strInsert;

	delete lpUndoLink;
}

BOOL CSpecialEdit::CanRedo()
{
	if (curUndoLink -> nextLink == NULL)
		return FALSE;
	else
		return TRUE;
}

void CSpecialEdit::Undo()
{
	if (curUndoLink -> prevLink == NULL)
		return ;

	// !!! restore the cursor position
	m_ptCursor = curUndoLink->endCursor ;
	// !!! Fisrt Delete the Insert Part

	SetSelection(m_ptCursor,m_ptCursor); 
	if (curUndoLink->strInsert->IsEmpty() == FALSE)
	{
		if (curUndoLink->startInsert == curUndoLink->endInsert)
		{
			MessageBox("Have some error occur");
		}
		else
		{
			m_ptSelStart = curUndoLink->startInsert;
			m_ptSelEnd = curUndoLink->endInsert;
			DeleteSelection();
		}
	}
	// !!! next Insert the Delete Part
	if (curUndoLink->strDelete->IsEmpty() == FALSE)
	{
		InsertText(m_ptCursor,*curUndoLink->strDelete);
	}

	if (curUndoLink->prevLink)
		curUndoLink = curUndoLink->prevLink;
}

void CSpecialEdit::Redo()
{
	if (curUndoLink -> nextLink == NULL)
		return ;
	if (curUndoLink->nextLink)
		curUndoLink = curUndoLink->nextLink;

	// !!! restore the cursor position
	m_ptCursor = curUndoLink->startCursor ;
	// !!! Fisrt Delete the Delete Part
	if (curUndoLink->strDelete->IsEmpty() == FALSE)
	{
		if (curUndoLink->startDelete == curUndoLink->endDelete)
		{
			MessageBox("Have some error occur");
		}
		else
		{
			m_ptSelStart = curUndoLink->startDelete;
			m_ptSelEnd = curUndoLink->endDelete;
			DeleteSelection();
		}
	}
	// !!! next Insert the Delete Part
	if (curUndoLink->strInsert->IsEmpty() == FALSE)
	{
		InsertText(m_ptCursor,*curUndoLink->strInsert);
	}
}

#define  MAXUNDOSTEP  1024

void CSpecialEdit::ReleaseTheOldestUndoLink()
{
	int numLink = 0;
	LPUNDOLINK lpcurLink;

		if (rootUndoLink == NULL)
			return;
		lpcurLink = rootUndoLink->nextLink;

		while(lpcurLink)
		{
			numLink ++;
			lpcurLink = lpcurLink->nextLink;
		}

		if (numLink > MAXUNDOSTEP)
		{
			lpcurLink = rootUndoLink->nextLink ;
			rootUndoLink->nextLink = (rootUndoLink->nextLink) ->nextLink;
			(rootUndoLink->nextLink) -> prevLink = rootUndoLink;
			ReleaseUndoLink(lpcurLink);
		}
}

BOOL CSpecialEdit::IsChinese(LPTSTR lpCurChar)
{
	if (*lpCurChar < 0)
		return TRUE;
	else
		return FALSE;
}


void CSpecialEdit::AutoMatchBrace()
{
	short nCtrl;
//	int nCaretIndex;

		nCtrl = GetAsyncKeyState(VK_CONTROL);
		static int iNum=0;
		if (nCtrl)
		{
			iNum++;
			if (iNum==3)
				iNum=0;
			m_ptCursor = GetMatchBracePos(m_ptCursor);
			SetSelection(m_ptCursor,m_ptCursor);
			CursorPos2CaretPos();
			EnsureVisible();
		}
}


void CSpecialEdit::ResetAdjustIndent()
{
	AutoIndent();
	m_ptCursor.x = 0;
	m_ptCursor.y = 0;
	SetSelection(m_ptCursor,m_ptCursor);
	CursorPos2CaretPos();
	EnsureVisible();
	InvalidateRect(NULL,TRUE);

	if (rootUndoLink)
	{
		DeleteUndoLink(rootUndoLink->nextLink);
		rootUndoLink->nextLink = NULL;
//			InitUndoLinkStruct( rootUndoLink );
		curUndoLink = rootUndoLink ;
	}
}


void CSpecialEdit::SetEditTextColor(int iTextType, COLORREF color)
{
	switch(iTextType)
	{
		case COLOR_NORMALTEXT:
			color_NormalText = color;
			break;
		case COLOR_NORMALBKCOLOR:
			color_NormalBKColor = color;
			break;
		case COLOR_KEYWORDTEXT:
			color_KeyWord =color;
			break;
		case COLOR_MASKTEXT:
			color_Mask = color;
			break;
		case COLOR_QUOTETEXT:
			color_Quotation = color;
			break;
		case COLOR_DEFINE:
			color_Define = color;
			break;
		case COLOR_MARKAREA:
			color_MarkArea = color;
			break;
		case COLOR_SELBKCOLOR:
			color_SelBkColor = color;
			break;
		case COLOR_SELTEXTCOLOR:
			color_SelTextColor = color;
			break;
		case COLOR_BREAKPOINT:
			color_BreakPoint = color;
			break;
		case COLOR_BOOKMARKS:
			color_BookMarks = color;
			break;
		default:
			break;
	}

}

COLORREF CSpecialEdit::GetEditTextColor(int iTextType,BOOL bDefaultColor)
{
	COLORREF color;
	if (bDefaultColor == FALSE)
	{
		switch(iTextType)
		{
			case COLOR_NORMALTEXT:
				color = color_NormalText;
				break;
			case COLOR_NORMALBKCOLOR:
				color = color_NormalBKColor;
				break;
			case COLOR_KEYWORDTEXT:
				color = color_KeyWord ;
				break;
			case COLOR_MASKTEXT:
				color = color_Mask ;
				break;
			case COLOR_QUOTETEXT:
				color = color_Quotation ;
				break;
			case COLOR_DEFINE:
				color = color_Define ;
				break;
			case COLOR_MARKAREA:
				color = color_MarkArea;
				break;
			case COLOR_SELBKCOLOR:
				color = color_SelBkColor;
				break;
			case COLOR_SELTEXTCOLOR:
				color = color_SelTextColor ;
				break;
			case COLOR_BREAKPOINT:
				color = color_BreakPoint;
				break;
			case COLOR_BOOKMARKS:
				color = color_BookMarks;
				break;
			default:
				color = RGB(0,0,0);
				break;
		}
	}
	else
	{
		switch(iTextType)
		{
			case COLOR_NORMALTEXT:
				color = DEFAULTCOLOR_NORMALTEXT;
				break;
			case COLOR_NORMALBKCOLOR:
				color = DEFAULTCOLOR_NORMALBKCOLOR;
				break;
			case COLOR_KEYWORDTEXT:
				color = DEFAULTCOLOR_KEYWORDTEXT;
				break;
			case COLOR_MASKTEXT:
				color = DEFAULTCOLOR_MASKTEXT;
				break;
			case COLOR_QUOTETEXT:
				color = DEFAULTCOLOR_QUOTETEXT;
				break;
			case COLOR_DEFINE:
				color = DEFAULTCOLOR_DEFINE;
				break;
			case COLOR_MARKAREA:
				color = DEFAULTCOLOR_MARKAREA;
				break;
			case COLOR_SELBKCOLOR:
				color = DEFAULTCOLOR_SELBKCOLOR;
				break;
			case COLOR_SELTEXTCOLOR:
				color = DEFAULTCOLOR_SELTEXTCOLOR;
				break;
			case COLOR_BREAKPOINT:
				color = DEFAULTCOLOR_BREAKPOINT;
				break;
			case COLOR_BOOKMARKS:
				color = DEFAULTCOLOR_BOOKMARKS;
				break;
			default:
				color = RGB(0,0,0);
				break;
		}
	}
	return color;
}


void CSpecialEdit::ReadSysColor()
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

void CSpecialEdit::SaveSysColor()
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


void CSpecialEdit::SetEditCaretPos(CPoint ptNewCaret)
{
	if (ptNewCaret.y<0 || ptNewCaret.y >= GetTextLineCount() )
		return ;
	m_ptCursor=ptNewCaret;
	CursorPos2CaretPos();
	EnsureVisible();
	SetFocus();
}
void CSpecialEdit::GetSearchString(CString &strSearch)
{
	CMainFrame* pMainFrame ;
	CComboBox *pSearchInputComboBox;
	pMainFrame = (CMainFrame*)((CAmboApp *)AfxGetApp())->m_pMainWnd;

	pSearchInputComboBox = pMainFrame->m_wndEditBar.GetComboBox(ID_SEARCHINPUT);

	pSearchInputComboBox->GetWindowText(strSearch);
}
/*
void CSpecialEdit::SetSearchString(CString strSearch)
{
	CMainFrame* pMainFrame ;
	CComboBox *pSearchInputComboBox;
	pMainFrame = (CMainFrame*)((CAmboApp *)AfxGetApp())->m_pMainWnd;

	pSearchInputComboBox = pMainFrame->m_wndEditBar.GetComboBox(ID_SEARCHINPUT);

//	pSearchInputComboBox->GetWindowText(strSearch);
	//如果有查询到指定的字符串，则设定为当前串，否则插入该字符串
	if (pSearchInputComboBox->SelectString(0, strSearch) == CB_ERR )
	{
//		int iIndex = pSearchInputComboBox->AddString(strSearch);
		int iIndex = pSearchInputComboBox->InsertString(0,strSearch);
		pSearchInputComboBox->SetCurSel(iIndex);
	}
	((CAmboApp *)AfxGetApp())->m_pRecentFindTextList->Add(strSearch);
}
*/
void CSpecialEdit::SearchInFile(CString strSearch)
{
//	CPoint ptFind;

		m_strSearch = strSearch;
		// Search the SearchString
		SearchString(CPoint(-1,-1),m_strSearch,m_bMatchAllWordOnly, m_bMatchCase,DIRECTION_DOWN);
}

COLORREF CSpecialEdit::GetEditBkColor()
{
	COLORREF color;

		color = color_NormalBKColor;

		if (color == CL_WINDOW)
			color = GetSysColor(COLOR_WINDOW);
		return color;
}

void CSpecialEdit::ResetColor(BOOL bDefault)
{
	if (bDefault == TRUE)
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
	}
	else
	{
		ReadSysColor();
	}
}

#define ID_INDICATOR_POSITION  1000

void CSpecialEdit::SetPosToStatusBar(int line, int col)
{
	CMainFrame* pMainFrame = (CMainFrame*)((CAmboApp *)AfxGetApp())->m_pMainWnd;
	CString strPos;
//	int iWidth;
	CDC *dc;
	CSize size;

	if (pMainFrame)
	{
		int iIndex = pMainFrame->m_wndStatusBar.CommandToIndex( ID_INDICATOR_POSITION );
		if (iIndex >=0)
		{
			dc = pMainFrame->m_wndStatusBar.GetDC();
			strPos.Format("ln %d, Col %d",line +1 ,col +1 );
			size = dc->GetTextExtent(strPos);
			pMainFrame->m_wndStatusBar.ReleaseDC(dc);
			//iWidth = strPos.GetLength() * 8;
			pMainFrame->m_wndStatusBar.SetPaneInfo( 1, ID_INDICATOR_POSITION, SBPS_NORMAL, size.cx);
			pMainFrame->m_wndStatusBar.SetPaneText( iIndex, strPos);
		}
	}
}

void CSpecialEdit::GetCursorWord(CString &strWord)
{
	GetWordAtPos(m_ptCursor,strWord);
}

void CSpecialEdit::InsertAdditionalMessage(int nChar)
{
	CString strInsertMessage = "",strVersion = "",strCurTime = "";
	SYSTEMTIME timeLocal;
	CString strFileNameDef = "",strFileTitle = "",strFileExt = "";

		switch(nChar)
		{
			case VK_F5:  // 插入当前时间
				GetLocalTime(&timeLocal);
				strInsertMessage.Format("%04d-%02d-%02d",timeLocal.wYear,timeLocal.wMonth,timeLocal.wDay);
				break;
			case VK_F9:  // 插入C文件头信息
				((CAmboApp *)AfxGetApp())->GetVersion(strVersion);
				GetLocalTime(&timeLocal);
				strCurTime.Format("%04d-%02d-%02d",timeLocal.wYear,timeLocal.wMonth,timeLocal.wDay);
				strInsertMessage = "/***************************************************\r\nCopyright(c) 版权所有，1998-2003微逻辑。保留所有权利。\r\n***************************************************/\r\n";
				strInsertMessage += "/**************************************************\r\n文件说明：\r\n版本号：";
				strInsertMessage += strVersion;
				strInsertMessage += "\r\n开发时期：";
				strInsertMessage += strCurTime;
				strInsertMessage += "\r\n作者：\r\n修改记录：\r\n**************************************************/\r\n";
				break;
			case VK_F8:  // 插入H文件头信息

				((CAmboApp *)AfxGetApp())->GetActiveFileName(strFileNameDef);

				_splitpath( strFileNameDef, NULL, NULL, strFileTitle.GetBuffer(MAX_PATH), strFileExt.GetBuffer(MAX_PATH));
				
				strFileTitle.ReleaseBuffer();
				strFileExt.ReleaseBuffer();
				strFileTitle.MakeUpper();
				strFileExt.MakeUpper();
				strFileExt.Replace(".","_");
				strFileNameDef = "__";
				strFileNameDef += strFileTitle;
				strFileNameDef += strFileExt;
				
				strInsertMessage = "/***************************************************\r\nCopyright(c) 版权所有，1998-2003微逻辑。保留所有权利。\r\n***************************************************/\r\n";
				strInsertMessage += "#ifndef ";
				strInsertMessage += strFileNameDef;
				strInsertMessage += "\r\n#define ";
				strInsertMessage += strFileNameDef;
				strInsertMessage += "\r\n#ifdef __cplusplus \r\nextern \"C\" {\r\n#endif  /* __cplusplus */    \r\n// 这里是你的有关声明部分\r\n\r\n\r\n#ifdef __cplusplus\r\n}\r\n#endif  /* __cplusplus */\r\n#endif  //";
				strInsertMessage += strFileNameDef;
				strInsertMessage += "\r\n";
				break;
			case VK_F7:  // 插入函数定义头信息
				strInsertMessage += "/**************************************************\r\n声明：\r\n参数：\r\n\tIN\r\n\tOUT\r\n\tIN/OUT\r\n\r\n返回值：\r\n功能描述：\r\n引用: \r\n************************************************/\r\n";
				break;
		}

		LPUNDOLINK newUndoLink = (LPUNDOLINK)malloc(sizeof(UNDOLINK));

		InitUndoLinkStruct( newUndoLink );

		SetSelection(m_ptCursor,m_ptCursor);

		if (newUndoLink)
		{
			newUndoLink->startInsert = m_ptCursor;
			newUndoLink->iProcessStyle |= INSERTPROCESS;
			newUndoLink->strInsert->Insert(0,strInsertMessage);
		}
		InsertText(m_ptCursor,strInsertMessage);
		if (newUndoLink)
		{
			newUndoLink->endInsert = m_ptCursor;
		}

		if (newUndoLink->iProcessStyle == NOPROCESS)
		{
			delete newUndoLink;
		}
		else
		{
			newUndoLink->endCursor = m_ptCursor;
			InsertNewUndoLink(newUndoLink);
		}
}

void CSpecialEdit::OnReplace(CString text) 
{
	// TODO: Add your command handler code here
	LPUNDOLINK newUndoLink = (LPUNDOLINK)malloc(sizeof(UNDOLINK));

	InitUndoLinkStruct( newUndoLink );

	DeleteSelection(TRUE,newUndoLink);

	if (newUndoLink)
	{
		newUndoLink->startInsert = m_ptCursor;
		newUndoLink->iProcessStyle |= INSERTPROCESS;
		newUndoLink->strInsert->Insert(0,text);
	}
	InsertText(m_ptCursor,text);
	if (newUndoLink)
	{
		newUndoLink->endInsert = m_ptCursor;
	}

	if (newUndoLink->iProcessStyle == NOPROCESS)
	{
		delete newUndoLink;
	}
	else
	{
		newUndoLink->endCursor = m_ptCursor;
		InsertNewUndoLink(newUndoLink);
	}
}
void CSpecialEdit::OnReplaceAll(CString text) 
{
	CPoint ptSaveCur(0,0);
	while(1)
	{
		if (SearchString(ptSaveCur,m_strSearch,m_bMatchAllWordOnly, m_bMatchCase,DIRECTION_DOWN) == TRUE)
		{ //查找到一个，将它替换掉
			if ((m_ptCursor.y < ptSaveCur.y) || (m_ptCursor.y == ptSaveCur.y && m_ptCursor.x < ptSaveCur.x))
			{
				// 已经查询到尾，开始从头查询
				break;
			}
			OnReplace(text);
			ptSaveCur = m_ptCursor;
			continue; // 继续下一个查询
		}
		break;
	}
}

BOOL CSpecialEdit::ProcessTab()
{
	SHORT nShift;
	SHORT nCtrl;

	nCtrl = GetAsyncKeyState(VK_CONTROL); 
	nShift=GetAsyncKeyState(VK_SHIFT);

	if (nCtrl)
	{
		  // Ctrl + Shift + Key
		return FALSE;
	}
	if (ExistSelection())
	{
		CPoint ptSelStart,ptSelEnd;
		GetSelection(ptSelStart,ptSelEnd);
		if (ptSelStart.y != ptSelEnd.y)
		{
			//反白有跨行，将处理多行缩进处理
			int iLineStart,iLineEnd;
			LineInfo LineInfo;

			//  得到要处理的行的位置
			if (ptSelStart.y > ptSelEnd.y)
			{
				iLineStart = ptSelEnd.y;
				if (ptSelStart.x == 0)
					iLineEnd = ptSelStart.y -1;
				else
					iLineEnd = ptSelStart.y;
			}
			else
			{
				iLineStart = ptSelStart.y;
				if (ptSelEnd.x == 0)
					iLineEnd = ptSelEnd.y -1;
				else
					iLineEnd = ptSelEnd.y;
			}
			// 开始处理
			if (nShift)
			{
				// dent process
				int iIndex;
				for (iIndex = iLineStart; iIndex <= iLineEnd; iIndex ++)
				{
					LineInfo=m_aDataLine.GetAt(iIndex);
					if (LineInfo.m_strLine->IsEmpty() == 0)
					{
						// 将在头上去掉一个"TAB" or 四个"SPACE"
						if (LineInfo.m_strLine->GetAt(0) == '\t')
						{//第一个字符是"TAB",删除该"TAB"就OK
							LineInfo.m_strLine->Delete(0);
						}
						else if (LineInfo.m_strLine->GetAt(0) == ' ')
						{
							int iDeleteNum = 0;
							int nTabSize = GetTabSize();
							while(1)
							{
								LineInfo.m_strLine->Delete(0);  // 删除一个空格
								iDeleteNum ++;
								if(iDeleteNum == nTabSize)
									break; //已经删除了足够多的空格，可以退出
								if (LineInfo.m_strLine->IsEmpty())
									break; // 已经没有数据了，可以退出
								if (LineInfo.m_strLine->GetAt(0) == ' ')
									continue;
								if (LineInfo.m_strLine->GetAt(0) == '\t')
								{
									LineInfo.m_strLine->Delete(0);  // 删除该TAB
									for (int i = 0; i < iDeleteNum; i++)
										LineInfo.m_strLine->Insert(0,' '); // 补回已经删除的空格
									break; // 已经完成，可以退出
								}
								break; //已经没有空格或TAB，可以退出	
							}
						}
						else
						{
							//不是SPACE OR TAB ，不需要做任何事
						}

					}
				}

			}
			else
			{
				// Indent process
				int iIndex;
				for (iIndex = iLineStart; iIndex <= iLineEnd; iIndex ++)
				{
					LineInfo=m_aDataLine.GetAt(iIndex);
					//在每一行的头上添加一个TAB就可以了
					LineInfo.m_strLine->Insert(0,'\t');
				}

			}
			ptSelStart.x = 0;
			ptSelStart.y = iLineStart;
			
			int nLineCount=GetTextLineCount();
			if (nLineCount <= iLineEnd + 1)
			{
				LineInfo = m_aDataLine.GetAt(iLineEnd);
				ptSelEnd.x = LineInfo.m_strLine->GetLength();
				ptSelEnd.y = iLineEnd;
			}
			else
			{
				ptSelEnd.x = 0;
				ptSelEnd.y = iLineEnd + 1;
			}
			SetSelection ( ptSelStart, ptSelEnd );

			m_ptCursor = ptSelEnd;

			CursorPos2CaretPos();

			InvalidateLines(iLineStart,iLineEnd);
			return TRUE;
		}
	}	
	return FALSE;
}

void CSpecialEdit::GetEditCaretPos(CPoint &ptNewCaret)
{
	ptNewCaret = m_ptCursor;
}
