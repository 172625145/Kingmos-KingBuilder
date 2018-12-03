// OutputMessageEdit.cpp : implementation file
//

#include "stdafx.h"
#include "ambo.h"
#include "OutputMessageEdit.h"
#include "MainFrame.h"
#include "Infomation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputMessageEdit


/////////////////////////////////////////////////////////////////////////////
// COutputMessageEdit message handlers
COutputMessageEdit::COutputMessageEdit()
{
	color_NormalText = DEFAULTCOLOR_NORMALTEXT;
	color_SelBkColor = DEFAULTCOLOR_SELBKCOLOR;
	color_SelTextColor = DEFAULTCOLOR_SELTEXTCOLOR;

	m_bMatchAllWordOnly = FALSE;
	m_bMatchCase = FALSE;
	iDirection = DIRECTION_DOWN;

	crFlag[0]="\x0d\x0a";
	crFlag[1]="\x0d";
	crFlag[2]="\x0a";

	m_bFocus = FALSE;

	InitializeEditStatus();

	m_bWindowsFileInfo = FALSE;


	InitializeCriticalSection( &m_CriticalSection ); 	
}

COutputMessageEdit::~COutputMessageEdit()
{
	ClearData();
}


BEGIN_MESSAGE_MAP(COutputMessageEdit, CWnd)
	//{{AFX_MSG_MAP(COutputMessageEdit)
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
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COutputMessageEdit private function

// 插入一个新行
void COutputMessageEdit::InsertNewLine( int nLineIndex, LPCTSTR pszChars, int nLength )
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
void COutputMessageEdit::DeleteLine( int nLineIndex )
{
}

// 在指定行中插入多个字符
// return : 实际插入的字符个数
int COutputMessageEdit::InsertCharInLine( int nLineIndex, LPCTSTR pszChars, int nLength , int nPosition  )
{
	LineInfo LineInfo;
	int nLineCount=GetTextLineCount();
	int nLineLength = 0, nNeedMaxLength = 0;
	int nInsertCharNum = nLength;

//	TestError();
	if (nLineIndex > nLineCount)
		return 0;
	if (nLineIndex == nLineCount)
	{
		InsertNewLine(nLineIndex);
		RecalcVertScrollBar();
	}
	if ( nLength == 0 )
		return 0;

	LineInfo=m_aDataLine.GetAt(nLineIndex);
	/********************************/
	// Add By Jami for Insert char will must put the tail
	nPosition = LineInfo.m_strLine->GetLength();

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
				nInsertLen += 4;
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
		SetFocus();
	}
//	TestError();
	return nInsertCharNum;
}

//在指定行中删除多个字符
void COutputMessageEdit::DeleteCharInLine( int nLineIndex, int nLength, int nPosition  )
{
		LineInfo LineInfo = m_aDataLine.GetAt(nLineIndex);
	
		LineInfo.m_strLine->Delete(nPosition,nLength );
}

// 重绘指定行的指定区域
void COutputMessageEdit::DrawLine( CDC *pdc, int nLineIndex)
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
			rect.left=0;
			pdc->FillSolidRect(&rect,GetSysColor(COLOR_WINDOW));
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
		color_OrgBackColor = GetSysColor(COLOR_WINDOW);
		SetTextEditFont(pdc);
		LineInfo=m_aDataLine.GetAt(nLineIndex);

		x=0 - m_nOffsetChar * m_nCharWidth;
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
			color_OrgWord = color_NormalText;//GetWordColor(strWord);

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
								nCount = 4;
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
							if (x + nCount * m_nCharWidth > 0)
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
						if (x<0)
						{
							if ((x + GetWordWidth(pdc,strWord)) < 0 )
							{
								// Is Not Show This word
								nNeedShowpos = -1;
							}
							else
							{
								nNeedShowpos = (0 - x) / m_nCharWidth;
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
							if (x + nCount * m_nCharWidth > 0)
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
						if (x<0)
						{
							if ((x + GetWordWidth(pdc,strWord)) < 0 )
							{
								// Is Not Show This word
								nNeedShowpos = -1;
							}
							else
							{
								int nHideLen= (0 - x) / m_nCharWidth;
								//nNeedShowpos = (0 - x) / m_nCharWidth;
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

		if (x < 0)
			x = 0;
//		RECT rect;
//		GetClientRect(&rect);
		rect.top=y;
		rect.bottom=y+GetLineHeight();
		rect.left=x;
		pdc->FillSolidRect(&rect,GetSysColor(COLOR_WINDOW));
		ShowEditCaret();
		//DrawMarkArea(pdc,nLineIndex);
//	pdc->TextOut(0,nLineIndex*16,*(m_aDataLine.GetAt(nLineIndex)).m_strLine);
}

// 得到要显示的指定位置字符的颜色
COLORREF COutputMessageEdit::GetCharColor( CPoint ptPos )
{
	return RGB(0,0,0);
}

// 得到选择区间
void COutputMessageEdit::GetSelection ( CPoint &ptSelStart, CPoint &ptSelEnd )
{
	ptSelStart = m_ptSelStart;
	ptSelEnd = m_ptSelEnd;
}

// 设置选择区间
void COutputMessageEdit::SetSelection ( CPoint ptSelStart, CPoint ptSelEnd )
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


// 指定位置字符是否在选择区间内
BOOL COutputMessageEdit::IsSelection ( CPoint ptPos )
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
int COutputMessageEdit::GetScreenCount ( void )
{
	int nLineHeight,nScreenHeight;
	RECT rect;

	nLineHeight=GetLineHeight();
	GetClientRect(&rect);
	nScreenHeight=rect.bottom-rect.top;
	return (nScreenHeight+nLineHeight-1)/nLineHeight;
}

// 得到第一个可看到的行索引
int COutputMessageEdit::GetFirstVisibleLine ( void )
{
	return m_nTopLine;
}

// 确保光标位置字符可见
void COutputMessageEdit::EnsureVisible ()
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
int COutputMessageEdit::GetMaxCharCount ( void )
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
// COutputMessageEdit message handlers

void COutputMessageEdit::OnKeyDown(int nChar, int nRepCnt, int nFlags) 
{
	// TODO: Add your message handler code here and/or call default
		EnterCriticalSection( &m_CriticalSection ); 
//		((CAmboApp *)AfxGetApp())->ShowMessageAtDesktop(40,10,"Enter Open Document");
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
			case VK_F3:
				ProcessF3();
				break;
			default:
				DefaultKeyProcess(nChar);
				break;
		}
		LeaveCriticalSection( &m_CriticalSection ); 
//		TestError();
//		CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void COutputMessageEdit::OnKeyUp(int nChar, int nRepCnt, int nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CWnd::OnKeyUp(nChar, nRepCnt, nFlags);
}

void COutputMessageEdit::OnLButtonDown(int nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	EnterCriticalSection( &m_CriticalSection ); 
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

	LeaveCriticalSection( &m_CriticalSection ); 
	CWnd::OnLButtonDown(nFlags, point);
}

BOOL COutputMessageEdit::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	RECT rect;

//	FillRect(pdc->SetBkColor(GetSysColor(COLOR_WINDOW));
	GetClientRect(&rect);
	rect.left=0;
	pDC->FillSolidRect(&rect,GetSysColor(COLOR_WINDOW));

	return CWnd::OnEraseBkgnd(pDC);
}

void COutputMessageEdit::OnVScroll(int nSBCode, int nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);

	EnterCriticalSection( &m_CriticalSection ); 
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
		LeaveCriticalSection( &m_CriticalSection ); 
		return;
	}

	if (nNewTopLine >= nLineCount)
		nNewTopLine = nLineCount - 1;
	if (nNewTopLine < 0)
		nNewTopLine = 0;
	ScrollToLine(nNewTopLine);
	LeaveCriticalSection( &m_CriticalSection ); 
}

void COutputMessageEdit::OnShowWindow(BOOL bShow, int nStatus) 
{
	CWnd::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	EnterCriticalSection( &m_CriticalSection ); 
	if (GetTextLineCount() ==0)
		InsertNewLine(0);
	RecalcVertScrollBar();
	RecalcHorzScrollBar();
	LeaveCriticalSection( &m_CriticalSection ); 
}

void COutputMessageEdit::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);
	// TODO: Add your message handler code here
	EnterCriticalSection( &m_CriticalSection ); 
	m_bFocus = TRUE;
	ShowEditCaret();

	CView * pView = (CView *)GetParent();

	CMainFrame* pMainFrame ;
	pMainFrame = (CMainFrame*)((CAmboApp *)AfxGetApp())->m_pMainWnd;
	pMainFrame->SetActiveView(pView);

	LeaveCriticalSection( &m_CriticalSection ); 
	
}

void COutputMessageEdit::OnRButtonUp(int nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CWnd::OnRButtonUp(nFlags, point);
}

void COutputMessageEdit::OnRButtonDown(int nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
/*
	  CMenu menu;

	  if (menu.LoadMenu(IDR_POPUPEDITMENU))
	  {
		  CMenu *subMenu = menu.GetSubMenu(0);
		 ClientToScreen(&point);
		 subMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON,
			point.x, point.y,
			this); // use main window for cmds
	  }
*/
	CWnd::OnRButtonDown(nFlags, point);
}

void COutputMessageEdit::OnRButtonDblClk(int nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CWnd::OnRButtonDblClk(nFlags, point);
}

void COutputMessageEdit::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here

	int nCount,nVisibleLine;
	int nLineHeight;
	int y;

		EnterCriticalSection( &m_CriticalSection ); 
		//nCount=m_aDataLine.GetSize();	
		nCount = GetScreenCount();
		if (nCount == 0)
		{
			LeaveCriticalSection( &m_CriticalSection ); 
			return ;
		}

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
		LeaveCriticalSection( &m_CriticalSection ); 
		//if (bCaretCaret)
		//ShowEditCaret();
	// Do not call CWnd::OnPaint() for painting messages
}

void COutputMessageEdit::OnMouseMove(int nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
//	::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_IBEAM)));
	if (this !=GetCapture())
	{
		return;
	}

	EnterCriticalSection( &m_CriticalSection ); 
//	HideEditCaret();
	m_ptCursor=ClientToText(point);
	CursorPos2CaretPos();
	SetSelection(m_ptSelStart,m_ptCursor);
//	m_ptSelEnd=m_ptCursor;
//	InvalidateRect(NULL);
//	ShowEditCaret();
	EnsureVisible();
	
	LeaveCriticalSection( &m_CriticalSection ); 
	CWnd::OnMouseMove(nFlags, point);
}

BOOL COutputMessageEdit::OnMouseWheel(int nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	SCROLLINFO si;
	si.cbSize = sizeof(si);
	si.fMask = SIF_ALL;
	VERIFY(GetScrollInfo(SB_VERT, &si));

	EnterCriticalSection( &m_CriticalSection ); 
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
	
	LeaveCriticalSection( &m_CriticalSection ); 
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void COutputMessageEdit::OnLButtonUp(int nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	EnterCriticalSection( &m_CriticalSection ); 
	if (this ==GetFocus())
	{
		ReleaseCapture();
	}
	
	LeaveCriticalSection( &m_CriticalSection ); 
	CWnd::OnLButtonUp(nFlags, point);
}

void COutputMessageEdit::OnHScroll(int nSBCode, int nPos, CScrollBar* pScrollBar) 
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

	EnterCriticalSection( &m_CriticalSection ); 
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
		LeaveCriticalSection( &m_CriticalSection ); 
		return;
	}

	if (nNewLeftChar < 0)
		nNewLeftChar = 0;
	if (nNewLeftChar >= nLineCount)
		nNewLeftChar = nLineCount - 1;
	ScrollToHorz(nNewLeftChar);
	LeaveCriticalSection( &m_CriticalSection ); 
}

void COutputMessageEdit::OnLButtonDblClk(int nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	int nCount;
	CPoint ptText;
	int nIndex;

	EnterCriticalSection( &m_CriticalSection ); 
	nCount = GetTextLineCount();

	ptText = ClientToText(point);
	nIndex  = ptText.y;
	if (ptText.y >= nCount)
	{
		LeaveCriticalSection( &m_CriticalSection ); 
		return ;
	}


	CString strLine="";
	LineInfo LineInfo1;
	
	LineInfo1 = m_aDataLine.GetAt(nIndex);
	strLine = *LineInfo1.m_strLine; 
	
	int iLineIndex;
	BOOL bRet;
	CString strFileName("");

	if (m_bWindowsFileInfo == FALSE)
	{
		bRet = GetMessageInfo(strLine,strFileName,iLineIndex);
		if ( bRet == TRUE)
		{
			// Get Current path
			CString strPath("");
			CPoint pt;

			if (strFileName.GetAt(0) == '/')
			{
				// had include directory
				strPath = strFileName;
			}
			else
			{
				if (GetBuildFilePath(nIndex,strPath) == TRUE)
				{
					strPath += "\\";
					strPath += strFileName;
				}
			}
			if (strPath.IsEmpty() == FALSE)
			{
				// Trun The Microsoft file path
				strPath.Replace( "/cygdrive/","");
				strPath.Insert(1,':');
				strPath.Replace( "/", "\\");
				
				// 打开文件，并将光标设置到指定行
				if (AfxGetApp()->OpenDocumentFile(strPath) != NULL)
				{
					pt.x = 0;
					pt.y = iLineIndex -1;
					((CAmboApp *)AfxGetApp())->SetCaretPos(pt);
				}
			}
		}
	}
	else
	{
		bRet = GetWindowsMessageInfo(strLine,strFileName,iLineIndex);
		if ( bRet == TRUE)
		{
			CPoint pt;
			// 打开文件，并将光标设置到指定行
			if (AfxGetApp()->OpenDocumentFile(strFileName) != NULL)
			{
				pt.x = 0;
				pt.y = iLineIndex -1;
				((CAmboApp *)AfxGetApp())->SetCaretPos(pt);
			}
		}
	}


	m_ptCursor = ptText;

	CursorPos2CaretPos();
	LeaveCriticalSection( &m_CriticalSection ); 
	CWnd::OnLButtonDblClk(nFlags, point);
}

void COutputMessageEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CWnd::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here
	EnterCriticalSection( &m_CriticalSection ); 
	::DestroyCaret();
	bCaretCaret = FALSE;
	m_iHide = 0;
	m_bFocus = FALSE;
	LeaveCriticalSection( &m_CriticalSection ); 
	
}


BOOL COutputMessageEdit::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, int nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWnd::Create(NULL, NULL, dwStyle, rect, pParentWnd, nID, pContext);
}

BOOL COutputMessageEdit::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWnd::PreCreateWindow(cs);
}


void COutputMessageEdit::SetEditText(LPCTSTR lpszText)
{
	EnterCriticalSection( &m_CriticalSection ); 
	LPTSTR lpLine=NULL;
	LPTSTR lpCurChar;
	WORD   nLineLength = 0 ;

	int nLineIndex = 0;
	DWORD dwCRLen;

//	ClearData();

	nLineIndex = GetTextLineCount()-1;
	if (nLineIndex < 0)
		nLineIndex =0;



	lpLine = lpCurChar = (LPTSTR)lpszText;
//	InsertNewLine(nLineIndex);

	while(1)
	{
		if (*lpCurChar == 0)
		{
			InsertCharInLine(nLineIndex,lpLine,nLineLength);
			break;
		}
		if ((dwCRLen = IsCR(lpCurChar)) !=0)
		{
			lpCurChar += dwCRLen;//strlen(crFlag);
//			nLineLength+=strlen(crFlag);
			InsertCharInLine(nLineIndex,lpLine,nLineLength);
			nLineIndex++;
			InsertNewLine(nLineIndex);
			lpLine = lpCurChar;
			nLineLength = 0;
			continue;
		}
		lpCurChar++;
		nLineLength++;
	}
	RecalcVertScrollBar();

	LeaveCriticalSection( &m_CriticalSection ); 
	
//	UINT nCount = m_aDataLine.GetSize();
//	TestError();
//	ShowEditCaret();
}

DWORD COutputMessageEdit::IsCR(LPCTSTR lpszChar)
{
	int i;
	for (i=0;i<3;i++)
	{
		if (strncmp(lpszChar,crFlag[i],strlen(crFlag[i])) == 0)
			return strlen(crFlag[i]);
	}
	return 0;
}

int COutputMessageEdit::GetEditTextLength()
{
	EnterCriticalSection( &m_CriticalSection ); 
	int nCount;
	LineInfo LineInfo;
	int nTextLength=0;

		nCount=m_aDataLine.GetSize();	
		if (nCount == 0)
		{
			LeaveCriticalSection( &m_CriticalSection ); 
			return 0;
		}

		for (int i=0; i<nCount ; i++)
		{
			LineInfo=m_aDataLine.GetAt(i);
			nTextLength += LineInfo.m_strLine->GetLength();
		}
		nTextLength += (nCount-1)*strlen(crFlag[0]);

		LeaveCriticalSection( &m_CriticalSection ); 
		return nTextLength;
}

int COutputMessageEdit::GetLineHeight()
{
	return m_nCharHeight;
}

// 从当前字串中提取一个词
CString COutputMessageEdit::GetWord(LPCTSTR lpChar)
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


int COutputMessageEdit::GetWordWidth(CDC *pdc,CString strWord,int nCount)
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

BOOL COutputMessageEdit::IsSpace(CString strWord)
{
	int nChar = strWord.GetAt(0);
//	if (strWord.Compare("\t") == 0)
	if (nChar == '\t' || nChar == ' ')
		return TRUE;
	return FALSE;
}

int COutputMessageEdit::GetTabSize()
{
	return m_nTabSize;
}


void COutputMessageEdit::ClearData()
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

}

void COutputMessageEdit::RecalcVertScrollBar(BOOL bPositionOnly)
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

int COutputMessageEdit::GetTextLineCount()
{
	return m_aDataLine.GetSize();
}

void COutputMessageEdit::ScrollToLine(int nNewTopLine)
{
	if (m_nTopLine != nNewTopLine)
	{
		int nScrollLines = m_nTopLine - nNewTopLine;
		m_nTopLine = nNewTopLine;
		//HideEditCaret();
		ScrollWindow(0, nScrollLines * GetLineHeight());
		//ShowEditCaret();
		//UpdateWindow();
//		if (bTrackScrollBar)
		RecalcVertScrollBar(TRUE);
//		HideEditCaret();
//		CursorPos2CaretPos();
	}
}

void COutputMessageEdit::SetTextEditFont(CDC *pdc)
{
	CFont font;
	LOGFONT lfFont;
	TEXTMETRIC textmetric;

	memset(&lfFont, 0, sizeof(lfFont));
	lstrcpy(lfFont.lfFaceName, _T("FixedSys"));
	lfFont.lfHeight = 16;
	lfFont.lfWidth = 8;
	lfFont.lfWeight = FW_NORMAL;
	lfFont.lfItalic = FALSE;
	lfFont.lfCharSet = DEFAULT_CHARSET;
	lfFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lfFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lfFont.lfQuality = DEFAULT_QUALITY;
	lfFont.lfPitchAndFamily = DEFAULT_PITCH;
//	lfFont.lfEscapement=0;

	font.CreateFontIndirect(&lfFont);

	pdc->SelectObject(font);

	pdc->GetTextMetrics(&textmetric);
	m_nCharWidth = textmetric.tmAveCharWidth;
	m_nCharHeight = textmetric.tmHeight;

}

void COutputMessageEdit::ShowEditCaret()
{
//	if (GetFocus()== NULL || GetFocus()->m_hWnd != m_hWnd)
	if (m_bFocus == FALSE)
		return ; // this window have not get focus , not need caret
	if (bCaretCaret == FALSE)
	{
		::CreateCaret(m_hWnd, NULL, 2, GetLineHeight());
		bCaretCaret = TRUE;
		m_iHide = 1;  // 当前还没有打开光标了
	}
	if (m_ptCaretPos.x < 0)
	{
		m_ptCaretPos.x = -10; // Not Show caret
	}
	SetCaretPos(m_ptCaretPos);
	m_iHide--;

	if (m_iHide == 0)
		ShowCaret();
}

void COutputMessageEdit::CursorPos2CaretPos()
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
		LineInfo=m_aDataLine.GetAt(m_ptCursor.y);
		m_ptCaretPos.x=0+ (GetExternCharLength(m_ptCursor)-m_nOffsetChar)* m_nCharWidth;
		m_ptCaretPos.y=(m_ptCursor.y-GetFirstVisibleLine())*GetLineHeight();
		ReleaseDC(pdc);
	}
	else
	{
		m_ptCaretPos.x=0;
		m_ptCaretPos.y=m_ptCursor.y;
	}
	ShowEditCaret();
}

void COutputMessageEdit::HideEditCaret()
{
	if (bCaretCaret == FALSE)
	{
		return ; // have not create caret, so not hide caret
	}

	if (m_iHide == 0)
		HideCaret();
	m_iHide++;
}

CPoint COutputMessageEdit::ClientToText(const CPoint &point)
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

	int nPos = m_nOffsetChar + (point.x - 0) / m_nCharWidth;//GetCharWidth();
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

void COutputMessageEdit::InvalidateLines(int nIndexStart, int nIndexEnd)
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



void COutputMessageEdit::MovePageUp()
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
			nScreenCount= GetScreenCount();
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

void COutputMessageEdit::MovePageDown()
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

void COutputMessageEdit::MoveEnd()
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

void COutputMessageEdit::MoveHome()
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

void COutputMessageEdit::MoveLeft()
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

void COutputMessageEdit::MoveUp()
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

void COutputMessageEdit::MoveRight()
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

void COutputMessageEdit::MoveDown()
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


BOOL COutputMessageEdit::ExistSelection()
{
	if (m_ptSelStart == m_ptSelEnd)
		return FALSE;
	return TRUE;
}

CPoint COutputMessageEdit::TextToClient(CPoint &point)
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

	pt.x = (pt.x - m_nOffsetChar) * m_nCharWidth + 0;
	return pt;
}

void COutputMessageEdit::GetEditText(LPTSTR lpBuffer, int nSize)
{
	EnterCriticalSection( &m_CriticalSection ); 
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
		LeaveCriticalSection( &m_CriticalSection ); 
}


void COutputMessageEdit::DefaultKeyProcess(int nChar)
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
			return ;
		}

		if (nCtrl)
		{
			// Ctrl + Key
			switch(nChar)
			{
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
BOOL COutputMessageEdit::PutToClipboard(LPCTSTR pszText)
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

void COutputMessageEdit::OnCopy() 
{
	// TODO: Add your command handler code here
	EnterCriticalSection( &m_CriticalSection ); 
	if (ExistSelection())
	{
		CPoint ptSelStart,ptSelEnd;
		CString text;
//		CopySelectionToClipboard();
		GetSelection(ptSelStart,ptSelEnd);
		GetText(ptSelStart,ptSelEnd,text);
		PutToClipboard(text);
//		return;
	}	
	LeaveCriticalSection( &m_CriticalSection ); 
}

void COutputMessageEdit::OnEditCopy() 
{
	// TODO: Add your command handler code here
	EnterCriticalSection( &m_CriticalSection ); 
	OnCopy();
	LeaveCriticalSection( &m_CriticalSection ); 
}

int COutputMessageEdit::GetLineHeadSpaceLen(LineInfo LineInfo)
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

void COutputMessageEdit::InitializeEditStatus()
{
	m_nTabSize = 4;


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
	m_nCharWidth = 8;
	m_nCharHeight = 16;

}

void COutputMessageEdit::GetText(const CPoint &ptStart, const CPoint &ptEnd, CString &text)
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
		int nCRLFLength = lstrlen(crFlag[0]);
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
		memcpy(pszBuf, crFlag[0], sizeof(TCHAR) * nCRLFLength);
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
			memcpy(pszBuf, crFlag[0], sizeof(TCHAR) * nCRLFLength);
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


void COutputMessageEdit::GetStringText(const CString &stringSource, int nStart, int nEnd, CString &text)
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

void COutputMessageEdit::OnSelAll()
{
	EnterCriticalSection( &m_CriticalSection ); 
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
	LeaveCriticalSection( &m_CriticalSection ); 
}



void COutputMessageEdit::RecalcHorzScrollBar(BOOL bPositionOnly)
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

void COutputMessageEdit::ScrollToHorz(int nNewLeftChar)
{
	if (m_nOffsetChar != nNewLeftChar)
	{
		int nScrollWidth = m_nOffsetChar - nNewLeftChar;
		m_nOffsetChar = nNewLeftChar;
//		ScrollWindow(nScrollWidth * m_nCharWidth, 0);
//		UpdateWindow();
		RecalcHorzScrollBar(TRUE);
		InvalidateLines(GetFirstVisibleLine(), -1);

//		HideEditCaret();
		CursorPos2CaretPos();
//		ShowEditCaret();
//		InvalidateRect(NULL,TRUE);
	}
}

int COutputMessageEdit::GetScreenCharCount()
{
	int nScreenWidth;
	RECT rect;

//	nLineHeight=GetLineHeight();
	GetClientRect(&rect);
	nScreenWidth=rect.right-rect.left;
	return (nScreenWidth-0)/m_nCharWidth;
}


BOOL COutputMessageEdit::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	if (nHitTest == HTCLIENT)
	{
		POINT point;
		EnterCriticalSection( &m_CriticalSection ); 
		GetCursorPos(&point);
		ScreenToClient(&point);
		LeaveCriticalSection( &m_CriticalSection ); 
		::SetCursor(::LoadCursor(NULL, IDC_IBEAM));
		return TRUE;
	}
	else
		return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void COutputMessageEdit::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here

	EnterCriticalSection( &m_CriticalSection ); 
	RecalcVertScrollBar();
	RecalcHorzScrollBar();
	LeaveCriticalSection( &m_CriticalSection ); 

}


void COutputMessageEdit::Clear()
{
	EnterCriticalSection( &m_CriticalSection ); 
	ClearData();
	LeaveCriticalSection( &m_CriticalSection ); 
}

void COutputMessageEdit::Copy()
{
	EnterCriticalSection( &m_CriticalSection ); 
	OnCopy();
	LeaveCriticalSection( &m_CriticalSection ); 
}


int COutputMessageEdit::GetLineCount()
{
	int nCount;
	
		EnterCriticalSection( &m_CriticalSection ); 
		nCount = GetTextLineCount();
		LeaveCriticalSection( &m_CriticalSection ); 
		return nCount;
}

int COutputMessageEdit::GetLine(int nIndex, LPTSTR lpszBuffer, int nMaxLength)
{

	EnterCriticalSection( &m_CriticalSection ); 
	LineInfo LineInfo1;
	

		if (nIndex < 0 || nIndex > GetLineCount())
		{
			LeaveCriticalSection( &m_CriticalSection ); 
			return 0;
		}
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

		LeaveCriticalSection( &m_CriticalSection ); 
		return nCopyCount;
}


void COutputMessageEdit::ProcessF3()
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

int COutputMessageEdit::GetWordAtPos(CPoint ptPos, CString &strWord)
{
	EnterCriticalSection( &m_CriticalSection ); 
	LineInfo LineInfo = m_aDataLine.GetAt(ptPos.y);

	if (LineInfo.m_strLine->GetLength() < ptPos.x)
	{
		LeaveCriticalSection( &m_CriticalSection ); 
		return -1;
	}

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
	LeaveCriticalSection( &m_CriticalSection ); 
	return nCharPos;
}

void COutputMessageEdit::SearchString(CPoint ptStart, CString m_strSearch, BOOL bMatchAllWordOnly, BOOL bMatchCase, int iDirection)
{
	int iPos;
	int nCount = GetLineCount();
	CString line;
	CString find;
	LineInfo LineInfo;
	
	EnterCriticalSection( &m_CriticalSection ); 
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
		return ;
	}
	m_ptCursor.y = ptCur.y;
	m_ptCursor.x = ptCur.x + m_strSearch.GetLength();

	CursorPos2CaretPos();
	
	CPoint r_ptStart,r_ptEnd;

	r_ptStart = ptCur;
	r_ptEnd = m_ptCursor;

	SetSelection(r_ptStart,r_ptEnd);
	EnsureVisible();
	LeaveCriticalSection( &m_CriticalSection ); 
	return ;
}

BOOL COutputMessageEdit::IsWholeWord(CString line, CString find, int iPos)
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

int COutputMessageEdit::GetExternCharLength(CPoint ptPos)
{
	EnterCriticalSection( &m_CriticalSection ); 
	LineInfo LineInfo;
	LPTSTR lpLine;
	int nCharCount = 0;
	int nPos,nCount;
//	CString strExtern;
	
		nCount=m_aDataLine.GetSize();	
		if (nCount == 0)
		{
			LeaveCriticalSection( &m_CriticalSection ); 
			return 0;
		}
		LineInfo=m_aDataLine.GetAt(ptPos.y);
//		ExternWord(*LineInfo.m_strLine,strExtern);
//		lpLine = strExtern.LockBuffer();
		lpLine = LineInfo.m_strLine->LockBuffer();
//		lpLine += m_nOffsetChar;
//		nPos = m_nOffsetChar;
		nPos = 0;
		while(*lpLine)
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
		LeaveCriticalSection( &m_CriticalSection ); 
		return nCharCount;
}

int COutputMessageEdit::GetCursorVisiblePos()
{
	EnterCriticalSection( &m_CriticalSection ); 
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
		LeaveCriticalSection( &m_CriticalSection ); 
		return nPos;
}
/*
int COutputMessageEdit::GetnCharHead2Visible(int nIndex)
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
int COutputMessageEdit::WordInSelection(CPoint pt_Point, CString strWord)
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

int COutputMessageEdit::GetnCharExtern(LPCTSTR pszChars,int nLength)
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

void COutputMessageEdit::ExternWord(CString strSrc,CString &strWord)
{
	CString strExtern;
	LPTSTR lpLine;

		EnterCriticalSection( &m_CriticalSection ); 
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
		LeaveCriticalSection( &m_CriticalSection ); 
}



BOOL COutputMessageEdit::IsChinese(LPTSTR lpCurChar)
{
	if (*lpCurChar < 0)
		return TRUE;
	else
		return FALSE;
}



BOOL COutputMessageEdit::AddString(LPCTSTR lpMessage)
{
	EnterCriticalSection(&m_CriticalSection);
	int nLineCount=GetTextLineCount();
//	InsertNewLine(nLineCount,lpMessage);
	SetEditText(lpMessage);
	//m_ptCursor=ClientToText(point);
	if (nLineCount)
		m_ptCursor.y = nLineCount-1;
	RecalcVertScrollBar();
	CursorPos2CaretPos();
//	InvalidateRect();
	InvalidateLines(nLineCount-1, -1);
	EnsureVisible();
	LeaveCriticalSection(&m_CriticalSection);
	return TRUE;
}

// for Microsoft File Type Message
BOOL COutputMessageEdit::GetWindowsMessageInfo(CString strLine, CString &strFileName, int &iLineIndex)
{
	LPCTSTR lpLine;
	TCHAR chDriver;
	DWORD dwlen=0;

//		lpLine = strLine.LockBuffer();
		lpLine = (LPCTSTR)strLine;

		chDriver = *lpLine++;
		if (!((chDriver>='a' && chDriver <='z')||(chDriver>='A' && chDriver <='Z')))
		{
			return FALSE;
		}
		if (*lpLine++!= ':')
		{
			return FALSE;
		}
		dwlen = 2;
		while(1)
		{
			if (*lpLine == 0)
				return FALSE;
			if (*lpLine++ == '(')
				break;
			dwlen ++;
		}

		strFileName=strLine.Left( dwlen ) ;
		iLineIndex = atoi(lpLine);
		if (iLineIndex == 0)
			return FALSE;
//		strLine.UnlockBuffer();
		return TRUE;
}

// for cygwin Build Message
BOOL COutputMessageEdit::GetMessageInfo(CString strLine, CString &strFileName, int &iLineIndex)
{
	LPTSTR lpLine;
//	TCHAR chDriver;
	DWORD dwlen=0;

		EnterCriticalSection( &m_CriticalSection ); 
		lpLine = strLine.LockBuffer();
//		chDriver = *lpLine++;
		while(1)
		{
			if (*lpLine == 0)
			{
				LeaveCriticalSection( &m_CriticalSection ); 
				return FALSE;
			}
			if (*lpLine == ' ')
			{
				LeaveCriticalSection( &m_CriticalSection ); 
				return FALSE;
			}
			if (*lpLine++ == ':')
				break;
			dwlen ++;
		}

		if (*lpLine<'0' || *lpLine > '9')
		{
			LeaveCriticalSection( &m_CriticalSection ); 
			return FALSE;
		}
		strFileName=strLine.Left( dwlen ) ;
		iLineIndex = atoi(lpLine);
		strLine.UnlockBuffer();

		// Get Current path

		LeaveCriticalSection( &m_CriticalSection ); 
		return TRUE;
}
BOOL COutputMessageEdit::GetBuildFilePath(int nIndex,CString &strPath)
{
	LineInfo LineInfo1;
	CString strLine="";

	EnterCriticalSection( &m_CriticalSection ); 
	while(nIndex)
	{
		nIndex --;
		LineInfo1 = m_aDataLine.GetAt(nIndex);
		strLine = *LineInfo1.m_strLine; 
		
		int pos = strLine.Find("Entering directory");
		if ( pos != -1)
		{
			pos = strLine.Find( '/', pos ); 
			LPTSTR lpPos =strLine.LockBuffer() + pos;
			int iCount = 0;
			while(1)
			{
				if (*lpPos == '\'' || *lpPos == 0)
					break;
				iCount ++;
				lpPos ++;
			}
			strLine.UnlockBuffer();
			if (iCount == 0)
			{
				LeaveCriticalSection( &m_CriticalSection ); 
				return FALSE;
			}
			strPath = strLine.Mid(pos,iCount);
			
			LeaveCriticalSection( &m_CriticalSection ); 
			return TRUE;
		}
	}
	LeaveCriticalSection( &m_CriticalSection ); 
	return FALSE;
}
void COutputMessageEdit::GetSearchString(CString &strSearch)
{
	CMainFrame* pMainFrame ;
	CComboBox *pSearchInputComboBox;
	pMainFrame = (CMainFrame*)((CAmboApp *)AfxGetApp())->m_pMainWnd;

	pSearchInputComboBox = pMainFrame->m_wndEditBar.GetComboBox(ID_SEARCHINPUT);

	pSearchInputComboBox->GetWindowText(strSearch);
}
/*
void COutputMessageEdit::SetSearchString(CString strSearch)
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
void COutputMessageEdit::SearchInFile(CString strSearch)
{
//	CPoint ptFind;

		EnterCriticalSection( &m_CriticalSection ); 
		m_strSearch = strSearch;
		// Search the SearchString
		SearchString(CPoint(-1,-1),m_strSearch,m_bMatchAllWordOnly, m_bMatchCase,DIRECTION_DOWN);
		LeaveCriticalSection( &m_CriticalSection ); 
}

void COutputMessageEdit::SetWindowsFileInfo( BOOL bWindowsFileInfo)
{
	m_bWindowsFileInfo = bWindowsFileInfo;
}

void COutputMessageEdit::GetCursorWord(CString &strWord)
{
	EnterCriticalSection( &m_CriticalSection ); 
	GetWordAtPos(m_ptCursor,strWord);
	LeaveCriticalSection( &m_CriticalSection ); 
}

BOOL COutputMessageEdit::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
//	if (pMsg->message == WM_KEYDOWN)
//	{
//
//		return 0;
//	}
	return CWnd::PreTranslateMessage(pMsg);
}
