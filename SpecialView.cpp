
// CSpecialView.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "Specialedit.h"
#include "SpecialView.h"
#include "FindText.h"
#include "SetSysColor.h"
#include "ReplaceText.h"

#include "Ambo.h"
#include "MainFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpecialView

IMPLEMENT_DYNCREATE(CSpecialView, CView)

CSpecialView::CSpecialView()
{
	h_Edit=new CSpecialEdit;
//	nMaxSize=64*1024l;
	nMaxSize=10*1024*1024l;
	m_pShadowBuffer = NULL;
	m_nShadowSize = 0;

	m_pPrintFont = NULL;
	m_pnPages = NULL;
	m_nPrintPages = 0;
	m_nPrintLineHeight = 0;
	m_bPrintHeader=TRUE;
	m_bPrintFooter=TRUE;

}

CSpecialView::~CSpecialView()
{
	if (m_pShadowBuffer != NULL)
		delete[] m_pShadowBuffer;
	delete h_Edit;

}


BEGIN_MESSAGE_MAP(CSpecialView, CView)
	//{{AFX_MSG_MAP(CSpecialView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_EDIT_TOGGLE_BOOKMARK, OnEditToggleBookmark)
	ON_COMMAND(ID_EDIT_GOTO_NEXT_BOOKMARK, OnEditGotoNextBookmark)
	ON_UPDATE_COMMAND_UI(ID_EDIT_GOTO_NEXT_BOOKMARK, OnUpdateEditGotoNextBookmark)
	ON_COMMAND(ID_EDIT_GOTO_PREV_BOOKMARK, OnEditGotoPrevBookmark)
	ON_UPDATE_COMMAND_UI(ID_EDIT_GOTO_PREV_BOOKMARK, OnUpdateEditGotoPrevBookmark)
	ON_COMMAND(ID_EDIT_CLEAR_ALL_BOOKMARKS, OnEditClearAllBookmarks)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR_ALL_BOOKMARKS, OnUpdateEditClearAllBookmarks)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_SETSYSCOLOR, OnSetsyscolor)
	ON_COMMAND(ID_EDIT_REPLACE, OnEditReplace)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REPLACE, OnUpdateEditReplace)
	ON_WM_ERASEBKGND()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpecialView drawing

void CSpecialView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
}

/////////////////////////////////////////////////////////////////////////////
// CSpecialView diagnostics

#ifdef _DEBUG
void CSpecialView::AssertValid() const
{
	CView::AssertValid();
}

void CSpecialView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

void CSpecialView::WriteToArchive(CArchive& ar)
	// Write just the text to an archive, no length prefix.
{
	ASSERT_VALID(this);
	LPCTSTR lpszText = LockBuffer();
	ASSERT(lpszText != NULL);
	UINT nLen = GetBufferLength();
	TRY
	{
		ar.Write(lpszText, nLen*sizeof(TCHAR));
	}
	CATCH_ALL(e)
	{
		UnlockBuffer();
		THROW_LAST();
	}
	END_CATCH_ALL
	UnlockBuffer();

	ASSERT_VALID(this);
}
void CSpecialView::ReadFromArchive(CArchive& ar, UINT nLen)
	// Read certain amount of text from the file, assume at least nLen
	// characters (not bytes) are in the file.
{
	ASSERT_VALID(this);

	LPVOID hText = LocalAlloc(LMEM_MOVEABLE, (nLen+1)*sizeof(TCHAR));
	if (hText == NULL)
		AfxThrowMemoryException();

	LPTSTR lpszText = (LPTSTR)LocalLock(hText);
	ASSERT(lpszText != NULL);
	if (ar.Read(lpszText, nLen*sizeof(TCHAR)) != nLen*sizeof(TCHAR))
	{
		LocalUnlock(hText);
		LocalFree(hText);
		AfxThrowArchiveException(CArchiveException::endOfFile);
	}
	// Replace the editing edit buffer with the newly loaded data
	lpszText[nLen] = '\0';
#ifndef _UNICODE
//	if (afxData.bWin95)
	{
		// set the text with SetWindowText, then free
//		BOOL bResult = ::SetWindowText(m_hWnd, lpszText);
		if (strlen(lpszText) < nLen)
			h_Edit->SetEditText("");
		else
			h_Edit->SetEditText(lpszText);
		LocalUnlock(hText);
		LocalFree(hText);

		// make sure that SetWindowText was successful
		if (h_Edit->GetEditTextLength() < (int)nLen)
		{
			AfxThrowMemoryException();
//			AfxMessageBox("打开文件失败");
		}

		// remove old shadow buffer
		delete[] m_pShadowBuffer;
		m_pShadowBuffer = NULL;
		m_nShadowSize = 0;

		ASSERT_VALID(this);
		return;
	}
#endif
	LocalUnlock(hText);
//	HLOCAL hOldText = GetEditCtrl().GetHandle();
//	ASSERT(hOldText != NULL);
//	LocalFree(hOldText);
//	GetEditCtrl().SetHandle((HLOCAL)(UINT)(DWORD)hText);
//	Invalidate();

	ASSERT_VALID(this);
}

void CSpecialView::SerializeRaw(CArchive& ar)
	// Read/Write object as stand-alone file.
{
	ASSERT_VALID(this);
	if (ar.IsStoring())
	{
		WriteToArchive(ar);
	}
	else
	{
		CFile* pFile = ar.GetFile();
		ASSERT(pFile->GetPosition() == 0);
		DWORD nFileSize = pFile->GetLength();
		if (nFileSize/sizeof(TCHAR) > nMaxSize)
		{
//			AfxMessageBox(AFX_IDP_FILE_TOO_LARGE);
//			return ;
			AfxThrowUserException();
		}

		// ReadFromArchive takes the number of characters as argument
		ReadFromArchive(ar, (UINT)nFileSize/sizeof(TCHAR));
	}
	ASSERT_VALID(this);
}
/////////////////////////////////////////////////////////////////////////////
// CSpecialView message handlers

BOOL CSpecialView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

int CSpecialView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	RECT rectClient;
	GetClientRect(&rectClient);

	h_Edit->Create( ES_AUTOHSCROLL|ES_AUTOVSCROLL|ES_MULTILINE|ES_WANTRETURN|WS_CHILD|WS_VISIBLE|WS_HSCROLL|WS_VSCROLL,
					rectClient,
					this,
					101);
	// TODO: Add your specialized creation code here
	CDocument* pDoc = GetDocument( );

	CString lpszPathName = pDoc->GetPathName();
	if (lpszPathName.IsEmpty()==FALSE)
	{
		pDoc->OnOpenDocument( lpszPathName );
	}
	
	return 0;
}

void CSpecialView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	h_Edit->SetWindowPos(NULL,0,0,cx,cy,SWP_NOMOVE|SWP_NOZORDER);
	
}

/////////////////////////////////////////////////////////////////////////////
// CCSpecialView attributes

LPCTSTR CSpecialView::LockBuffer() const
{
	ASSERT_VALID(this);
	ASSERT(m_hWnd != NULL);
#ifndef _UNICODE
//	if (afxData.bWin95)
	{
		// under Win32s, it is necessary to maintain a shadow buffer
		//  it is only updated when the control contents have been changed.
		if (m_pShadowBuffer == NULL || h_Edit->GetModify())
		{
			ASSERT(m_pShadowBuffer != NULL || m_nShadowSize == 0);
			UINT nSize = h_Edit->GetEditTextLength()+1;
			if (nSize > m_nShadowSize)
			{
				// need more room for shadow buffer
				CSpecialView* pThis = (CSpecialView*)this;
				delete[] m_pShadowBuffer;
				pThis->m_pShadowBuffer = NULL;
				pThis->m_nShadowSize = 0;
				pThis->m_pShadowBuffer = new TCHAR[nSize];
				pThis->m_nShadowSize = nSize;
			}

			// update the shadow buffer with GetWindowText
			ASSERT(m_nShadowSize >= nSize);
			ASSERT(m_pShadowBuffer != NULL);
			h_Edit->GetEditText(m_pShadowBuffer, nSize);

			// turn off edit control's modify bit
			h_Edit->SetModify(FALSE);
		}
		return m_pShadowBuffer;
	}
#endif
	// else -- running under non-subset Win32 system
//	HLOCAL hLocal = GetEditCtrl().GetHandle();
//	ASSERT(hLocal != NULL);
//	LPCTSTR lpszText = (LPCTSTR)LocalLock(hLocal);
//	ASSERT(lpszText != NULL);
//	ASSERT_VALID(this);
//	return lpszText;
	return NULL;
}

void CSpecialView::UnlockBuffer() const
{
	ASSERT_VALID(this);
	ASSERT(m_hWnd != NULL);
#ifndef _UNICODE
//	if (afxData.bWin95)
		return;
#endif
//	HLOCAL hLocal = GetEditCtrl().GetHandle();
//	ASSERT(hLocal != NULL);
//	LocalUnlock(hLocal);
}
UINT CSpecialView::GetBufferLength() const
{
	ASSERT_VALID(this);
	ASSERT(m_hWnd != NULL);
	LPCTSTR lpszText = LockBuffer();
	UINT nLen = lstrlen(lpszText);
	UnlockBuffer();
	return nLen;
}


/*
void CSpecialView::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
//	pCmdUI->Enable(IsClipboardFormatAvailable(CF_TEXT));
//	pCmdUI->Enable(TRUE);
	
}
*/

void CSpecialView::OnEditToggleBookmark() 
{
	// TODO: Add your command handler code here
	if (h_Edit->GetMarkFlag(-1,FLAG_BOOKMARKS) == FALSE)
		h_Edit->SetMarkFlag(-1,FLAG_BOOKMARKS);
	else
		h_Edit->ClearMarkFlag(-1,FLAG_BOOKMARKS);
}

void CSpecialView::OnEditGotoNextBookmark() 
{
	// TODO: Add your command handler code here
	h_Edit->ToNextMarkFlag(-1,FLAG_BOOKMARKS);
}

void CSpecialView::OnUpdateEditGotoNextBookmark(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	int nIndex = -1;
	
		nIndex = h_Edit->GetNextMarkFlag(-1,FLAG_BOOKMARKS);
		pCmdUI->Enable(nIndex != -1);
}

void CSpecialView::OnEditGotoPrevBookmark() 
{
	// TODO: Add your command handler code here
	h_Edit->ToPrevMarkFlag(-1,FLAG_BOOKMARKS);
}

void CSpecialView::OnUpdateEditGotoPrevBookmark(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	int nIndex = -1;
	
		nIndex = h_Edit->GetPrevMarkFlag(-1,FLAG_BOOKMARKS);
		pCmdUI->Enable(nIndex != -1);
}

void CSpecialView::OnEditClearAllBookmarks() 
{
	// TODO: Add your command handler code here
	h_Edit->ClearAllMarkFlag(FLAG_BOOKMARKS);
}

void CSpecialView::OnUpdateEditClearAllBookmarks(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	int nIndex = -1;
	
		nIndex = h_Edit->GetNextMarkFlag(-1,FLAG_BOOKMARKS);
		pCmdUI->Enable(nIndex != -1);
}

void CSpecialView::OnEditFind() 
{
	// TODO: Add your command handler code here

	CFindText dlg;
	CString strSearch;

	CMainFrame* pMainFrame ;
//	CComboBox *pSearchInputComboBox;
	pMainFrame = (CMainFrame*)((CAmboApp *)AfxGetApp())->m_pMainWnd;

	h_Edit->GetCursorWord(strSearch);
//	h_Edit->SetSearchString(strSearch);

	((CAmboApp *)AfxGetApp())->SetSearchString(strSearch);

//	pSearchInputComboBox = pMainFrame->m_wndEditBar.GetComboBox(ID_SEARCHINPUT);

//	dlg.m_pSearchComboBox = pSearchInputComboBox;
	dlg.m_strText = strSearch;//h_Edit->m_strSearch;
	dlg.m_bMatchAllWordOnly = h_Edit->m_bMatchAllWordOnly;
	dlg.m_bMatchCase = h_Edit->m_bMatchCase;
	if (h_Edit->iDirection == DIRECTION_UP)
		dlg.m_iDir = 0;
	else
		dlg.m_iDir = 1;

	dlg.DoModal();

	if (dlg.nFindStyle == -1 )
	{
		return ;
	}
	
	h_Edit->m_strSearch = dlg.m_strText;
	h_Edit->m_bMatchAllWordOnly = dlg.m_bMatchAllWordOnly;
	h_Edit->m_bMatchCase = dlg.m_bMatchCase;
	if (dlg.m_iDir == 0)
		h_Edit->iDirection = DIRECTION_UP;
	else
		h_Edit->iDirection = DIRECTION_DOWN;

	if (dlg.nFindStyle == FIND_FINDNEXT)
	{
		((CAmboApp *)AfxGetApp())->SetSearchString(dlg.m_strText);
		 h_Edit->SearchString(CPoint(-1,-1), dlg.m_strText,dlg.m_bMatchAllWordOnly,dlg.m_bMatchCase,h_Edit->iDirection);
	}
	else
	{
		((CAmboApp *)AfxGetApp())->SetSearchString(dlg.m_strText);
		 h_Edit->MarkAllString(dlg.m_strText,dlg.m_bMatchAllWordOnly,dlg.m_bMatchCase);
	}
//	h_Edit->SetSearchString(dlg.m_strText);
//	((CAmboApp *)AfxGetApp())->SetSearchString(dlg.m_strText);
	
	::SetFocus(h_Edit->m_hWnd);
}
/*
void CSpecialView::OnUpdateEditFind(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(::GetFocus() == h_Edit->m_hWnd);	
}
*/
void CSpecialView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	
//	CView::OnBeginPrinting(pDC, pInfo);
	ASSERT(m_pnPages == NULL);
	ASSERT(m_pPrintFont == NULL);
	CFont *pDisplayFont = GetFont();

	LOGFONT lf;
	pDisplayFont->GetLogFont(&lf);

	CDC *pDisplayDC = GetDC();
	lf.lfHeight = MulDiv(lf.lfHeight, pDC->GetDeviceCaps(LOGPIXELSY), pDisplayDC->GetDeviceCaps(LOGPIXELSY) * 2);
	lf.lfWidth = MulDiv(lf.lfWidth, pDC->GetDeviceCaps(LOGPIXELSX), pDisplayDC->GetDeviceCaps(LOGPIXELSX) * 2);
	ReleaseDC(pDisplayDC);

	m_pPrintFont = new CFont;
	if (! m_pPrintFont->CreateFontIndirect(&lf))
	{
		delete m_pPrintFont;
		m_pPrintFont = NULL;
		return;
	}

	pDC->SelectObject(m_pPrintFont);
}

void CSpecialView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	
//	CView::OnEndPrinting(pDC, pInfo);
	if (m_pPrintFont != NULL)
	{
		delete m_pPrintFont;
		m_pPrintFont = NULL;
	}
	if (m_pnPages != NULL)
	{
		delete m_pnPages;
		m_pnPages = NULL;
	}
	m_nPrintPages = 0;
	m_nPrintLineHeight = 0;
	
}

BOOL CSpecialView::OnPreparePrinting(CPrintInfo* pInfo) 
{
	// TODO: call DoPreparePrinting to invoke the Print dialog box
	
	return CView::OnPreparePrinting(pInfo);
}

void CSpecialView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	
//	CView::OnPrint(pDC, pInfo);

	if (m_pnPages == NULL)
	{
		RecalcPageLayouts(pDC, pInfo);
		ASSERT(m_pnPages != NULL);
	}

	ASSERT(pInfo->m_nCurPage >= 1 && (int) pInfo->m_nCurPage <= m_nPrintPages);
	
	int nLine = m_pnPages[pInfo->m_nCurPage - 1];
	int nEndLine = h_Edit->GetLineCount();
	if ((int) pInfo->m_nCurPage < m_nPrintPages)
		nEndLine = m_pnPages[pInfo->m_nCurPage];
	TRACE(_T("Printing page %d of %d, lines %d - %d\n"), pInfo->m_nCurPage, m_nPrintPages,
						nLine, nEndLine - 1);

	if (m_bPrintHeader)
		PrintHeader(pDC, pInfo->m_nCurPage);
	if (m_bPrintFooter)
		PrintFooter(pDC, pInfo->m_nCurPage);

	int y = m_rcPrintArea.top;
	for (; nLine < nEndLine; nLine ++)
	{
		h_Edit->PrintLine(pDC,nLine,y);
		y += m_nPrintLineHeight;
	}
}

void CSpecialView::RecalcPageLayouts(CDC *pdc, CPrintInfo *pInfo)
{
	m_ptPageArea = pInfo->m_rectDraw;
	m_ptPageArea.NormalizeRect();

	m_nPrintLineHeight = pdc->GetTextExtent(_T("X")).cy;

	m_rcPrintArea = m_ptPageArea;
	CSize szTopLeft, szBottomRight;
	CWinApp *pApp = AfxGetApp();
	ASSERT(pApp != NULL);
	szTopLeft.cx = 40;//pApp->GetProfileInt(REG_PAGE_SUBKEY, REG_MARGIN_LEFT, DEFAULT_PRINT_MARGIN);
	szBottomRight.cx = 40;//pApp->GetProfileInt(REG_PAGE_SUBKEY, REG_MARGIN_RIGHT, DEFAULT_PRINT_MARGIN);
	szTopLeft.cy = 10;//pApp->GetProfileInt(REG_PAGE_SUBKEY, REG_MARGIN_TOP, DEFAULT_PRINT_MARGIN);
	szBottomRight.cy = 10;//pApp->GetProfileInt(REG_PAGE_SUBKEY, REG_MARGIN_BOTTOM, DEFAULT_PRINT_MARGIN);
	pdc->HIMETRICtoLP(&szTopLeft);
	pdc->HIMETRICtoLP(&szBottomRight);
	m_rcPrintArea.left += szTopLeft.cx;
	m_rcPrintArea.right -= szBottomRight.cx;
	m_rcPrintArea.top += szTopLeft.cy;
	m_rcPrintArea.bottom -= szBottomRight.cy;
	if (m_bPrintHeader)
		m_rcPrintArea.top += m_nPrintLineHeight + m_nPrintLineHeight / 2;
	if (m_bPrintFooter)
		m_rcPrintArea.bottom -= m_nPrintLineHeight + m_nPrintLineHeight / 2;
//		m_rcPrintArea.bottom += m_nPrintLineHeight + m_nPrintLineHeight / 2;

	int nLimit = 32;
	m_nPrintPages = 1;
	m_pnPages = new int[nLimit];
	m_pnPages[0] = 0;

	int nLineCount = h_Edit->GetLineCount();
	int nLine = 1;
	int y = m_rcPrintArea.top ;//+ PrintLineHeight(pdc, 0);
	int nHeight = m_nPrintLineHeight;//40;//PrintLineHeight(pdc, nLine);
	while (nLine < nLineCount)
	{
//		int nHeight = m_nPrintLineHeight;//40;//PrintLineHeight(pdc, nLine);
		if (y + nHeight <= m_rcPrintArea.bottom)
		{
			y += nHeight;
		}
		else
		{
			ASSERT(nLimit >= m_nPrintPages);
			if (nLimit <= m_nPrintPages)
			{
				nLimit += 32;
				int *pnNewPages = new int[nLimit];
				memcpy(pnNewPages, m_pnPages, sizeof(int) * m_nPrintPages);
				delete m_pnPages;
				m_pnPages = pnNewPages;
			}
			ASSERT(nLimit > m_nPrintPages);
			m_pnPages[m_nPrintPages ++] = nLine;
			y = m_rcPrintArea.top + nHeight;
		}
		nLine ++;
	}
}



void CSpecialView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CView::OnPrepareDC(pDC, pInfo);
	if (pInfo != NULL)
	{
		pInfo->m_bContinuePrinting = TRUE;
		if (m_pnPages != NULL && (int) pInfo->m_nCurPage > m_nPrintPages)
			pInfo->m_bContinuePrinting = FALSE;
	}
}


/*
void CCrystalTextView::GetPrintHeaderText(int nPageNum, CString &text)
{
	ASSERT(m_bPrintHeader);
	text = _T("");
}

void CCrystalTextView::GetPrintFooterText(int nPageNum, CString &text)
{
	ASSERT(m_bPrintFooter);
	text.Format(_T("Page %d/%d"), nPageNum, m_nPrintPages);
}

void CCrystalTextView::PrintHeader(CDC *pdc, int nPageNum)
{
	CRect rcHeader = m_rcPrintArea;
	rcHeader.bottom = rcHeader.top;
	rcHeader.top -= (m_nPrintLineHeight + m_nPrintLineHeight / 2);

	CString text;
	GetPrintHeaderText(nPageNum, text);
	if (! text.IsEmpty())
		pdc->DrawText(text, &rcHeader, DT_CENTER | DT_NOPREFIX | DT_TOP | DT_SINGLELINE);
}

void CCrystalTextView::PrintFooter(CDC *pdc, int nPageNum)
{
	CRect rcFooter = m_rcPrintArea;
	rcFooter.top = rcFooter.bottom;
	rcFooter.bottom += (m_nPrintLineHeight + m_nPrintLineHeight / 2);

	CString text;
	GetPrintFooterText(nPageNum, text);
	if (! text.IsEmpty())
		pdc->DrawText(text, &rcFooter, DT_CENTER | DT_NOPREFIX | DT_BOTTOM | DT_SINGLELINE);
}
*/

void CSpecialView::GetPrintHeaderText(int nPageNum, CString &text)
{
	ASSERT(m_bPrintHeader);
	text = _T(" Jami chen Test");
}

void CSpecialView::GetPrintFooterText(int nPageNum, CString &text)
{
	ASSERT(m_bPrintFooter);
	text.Format(_T("Page %d/%d"), nPageNum, m_nPrintPages);
}

void CSpecialView::PrintHeader(CDC *pdc, int nPageNum)
{
	CRect rcHeader = m_rcPrintArea;
	rcHeader.bottom = rcHeader.top;
	rcHeader.top -= (m_nPrintLineHeight + m_nPrintLineHeight / 2);

	CString text;
	GetPrintHeaderText(nPageNum, text);
	if (! text.IsEmpty())
		pdc->DrawText(text, &rcHeader, DT_CENTER | DT_NOPREFIX | DT_TOP | DT_SINGLELINE);

}

void CSpecialView::PrintFooter(CDC *pdc, int nPageNum)
{
	CRect rcFooter = m_rcPrintArea;
	rcFooter.top = rcFooter.bottom;
	rcFooter.bottom += (m_nPrintLineHeight + m_nPrintLineHeight / 2);

	CString text;
	GetPrintFooterText(nPageNum, text);
	if (! text.IsEmpty())
		pdc->DrawText(text, &rcFooter, DT_CENTER | DT_NOPREFIX | DT_BOTTOM | DT_SINGLELINE);

}

void CSpecialView::OnEditRedo() 
{
	// TODO: Add your command handler code here
	h_Edit->Redo();
}

void CSpecialView::OnUpdateEditRedo(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(h_Edit->CanRedo());	
}

void CSpecialView::OnEditUndo() 
{
	// TODO: Add your command handler code here
	h_Edit->Undo();
}

void CSpecialView::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(h_Edit->CanUndo());
}

void CSpecialView::OnSetsyscolor() 
{
	// TODO: Add your command handler code here
	CSetSysColor dlg;
	int nIndex = 0,nIndexMax;
	COLORREF color,colorDefault;

	struct structColorItem{
		LPTSTR lpItemString;
		int idColor;
	}ColorItem[]={
		{"Normal Text",COLOR_NORMALTEXT},
		{"Key Word",COLOR_KEYWORDTEXT},
		{"Quote Text",COLOR_QUOTETEXT},
		{"Mask Text",COLOR_MASKTEXT},
		{"Define Text",COLOR_DEFINE},
		{"Selection Text",COLOR_SELTEXTCOLOR},
		{"Selection BKColor",COLOR_SELBKCOLOR},
		{"BreakPoint",COLOR_BREAKPOINT},
		{"BookMarks",COLOR_BOOKMARKS},
		{"Mark Area",COLOR_MARKAREA},
	};

	nIndexMax =  sizeof(ColorItem) / sizeof(struct structColorItem);

	for (nIndex =0; nIndex < nIndexMax; nIndex ++)
	{
		color = h_Edit->GetEditTextColor(ColorItem[nIndex].idColor);
		colorDefault = h_Edit->GetEditTextColor(ColorItem[nIndex].idColor,TRUE);
		dlg.SetSysColorItem(ColorItem[nIndex].lpItemString,color,colorDefault);
	}
	if (dlg.DoModal() == IDOK)
	{
		for (nIndex =0; nIndex < nIndexMax; nIndex ++)
		{
			color = dlg.GetItemColor(ColorItem[nIndex].lpItemString);
			h_Edit->SetEditTextColor(ColorItem[nIndex].idColor,color);
		}
		h_Edit->InvalidateRect(NULL,TRUE);
	}

	
}

void CSpecialView::SaveOldFile()
{

}

void CSpecialView::SetModify(BOOL bModified)
{
	GetDocument()->SetModifiedFlag(bModified);
}
BOOL CSpecialView::GetModify()
{
	return GetDocument()->IsModified();
}
/*
void CSpecialView::OnPrebuild() 
{
	// TODO: Add your command handler code here
	//h_Edit->PreBuild();
	CDocument* pDoc = GetDocument();
	CString strFileName="";
	PROCESS_INFORMATION processInfo;
	STARTUPINFO startInfo;
	BOOL bResult;
	CString strCommand= "\"f:\\work\\testMyEdit\\prebuild\\debug\\PreBuild.exe\" ";

	memset(&startInfo,0,sizeof(STARTUPINFO));

	startInfo.cb = sizeof(STARTUPINFO);

	strFileName = pDoc->GetPathName();

	if (strFileName.IsEmpty() == TRUE)
		return;

	strCommand += strFileName;

	strCommand += " abcdef werwee";

//	bResult = CreateProcess("f:\\work\\testMyEdit\\prebuild\\debug\\PreBuild.exe",strFileName.LockBuffer(),NULL,NULL,NULL,CREATE_NO_WINDOW,NULL,NULL,&startInfo,&processInfo);
	bResult = CreateProcess(NULL,strCommand.LockBuffer(),NULL,NULL,NULL,CREATE_NO_WINDOW,NULL,NULL,&startInfo,&processInfo);
//	bResult = CreateProcess(strCommand.LockBuffer(),NULL,NULL,NULL,NULL,CREATE_NO_WINDOW,NULL,NULL,&startInfo,&processInfo);

	if (bResult == FALSE)
	{
		DWORD err=GetLastError();
	}
//	CreateProcess("c:\\winnt\\notepad.exe","",NULL,NULL,NULL,CREATE_NO_WINDOW,NULL,NULL,&startInfo,&processInfo);
}
*/

void CSpecialView::OnEditReplace() 
{
	// TODO: Add your command handler code here
	CReplaceText dlgReplaceText;
	CString strSearch;

	CMainFrame* pMainFrame ;
//	CComboBox *pSearchInputComboBox;
	pMainFrame = (CMainFrame*)((CAmboApp *)AfxGetApp())->m_pMainWnd;

	h_Edit->GetCursorWord(strSearch);
//	h_Edit->SetSearchString(strSearch);

	((CAmboApp *)AfxGetApp())->SetSearchString(strSearch);

	dlgReplaceText.m_pEdit = h_Edit;
	if (dlgReplaceText.DoModal() == IDCANCEL)
		return ;

	h_Edit->m_strSearch = dlgReplaceText.m_strSearch;
	h_Edit->m_bMatchAllWordOnly = dlgReplaceText.m_bMatchAllWordOnly;
	h_Edit->m_bMatchCase = dlgReplaceText.m_bMatchCase;
	h_Edit->iDirection = DIRECTION_DOWN;

	h_Edit->OnReplaceAll(dlgReplaceText.m_strReplaceText) ;
	/*
	if (dlg.nFindStyle == -1 )
	{
		return ;
	}
	
	h_Edit->m_strSearch = dlg.m_strText;
	h_Edit->m_bMatchAllWordOnly = dlg.m_bMatchAllWordOnly;
	h_Edit->m_bMatchCase = dlg.m_bMatchCase;
	if (dlg.m_iDir == 0)
		h_Edit->iDirection = DIRECTION_UP;
	else
		h_Edit->iDirection = DIRECTION_DOWN;

	if (dlg.nFindStyle == FIND_FINDNEXT)
	{
		((CAmboApp *)AfxGetApp())->SetSearchString(dlg.m_strText);
		 h_Edit->SearchString(CPoint(-1,-1), dlg.m_strText,dlg.m_bMatchAllWordOnly,dlg.m_bMatchCase,h_Edit->iDirection);
	}
	else
	{
		((CAmboApp *)AfxGetApp())->SetSearchString(dlg.m_strText);
		 h_Edit->MarkAllString(dlg.m_strText,dlg.m_bMatchAllWordOnly,dlg.m_bMatchCase);
	}
//	h_Edit->SetSearchString(dlg.m_strText);
//	((CAmboApp *)AfxGetApp())->SetSearchString(dlg.m_strText);
*/	
	::SetFocus(h_Edit->m_hWnd);
	
}

void CSpecialView::OnUpdateEditReplace(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(TRUE);	
}

BOOL CSpecialView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
//	return CView::OnEraseBkgnd(pDC);
	return TRUE;
}

void CSpecialView::OnSetFocus(CWnd* pOldWnd) 
{
	CView::OnSetFocus(pOldWnd);
	
//	((CAmboApp *)AfxGetApp())->ShowMessageAtDesktop(10,70,"SpecialView:OnSetFocus");
	// TODO: Add your message handler code here
	h_Edit->SetFocus();
}
