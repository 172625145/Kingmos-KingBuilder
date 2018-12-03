#if !defined(AFX_OUTPUTMESSAGEEDIT_H__F25816BC_9E0C_4377_AD2F_7F34A1413065__INCLUDED_)
#define AFX_OUTPUTMESSAGEEDIT_H__F25816BC_9E0C_4377_AD2F_7F34A1413065__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OutputMessageEdit.h : header file
//
#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////
// COutputMessageEdit window
/*
#ifndef DEFAULTCOLOR_NORMALTEXT
#define DEFAULTCOLOR_NORMALTEXT		RGB(0x00,0x00,0x00)
#endif

#ifndef DEFAULTCOLOR_SELBKCOLOR
#define DEFAULTCOLOR_SELBKCOLOR		RGB(0x00,0x00,0xff)
#endif

#ifndef DEFAULTCOLOR_SELTEXTCOLOR
#define DEFAULTCOLOR_SELTEXTCOLOR	RGB(0xff,0xff,0xff)
#endif
*/

#define DIRECTION_UP	1
#define DIRECTION_DOWN	2

#define MAX_LINELENGTH    1024
//#define CHARWIDTH		  8

#define WHOLEINSELECTION	1
#define WHOLENOTINSELECTION	2
#define HALFINSELECTION		3

#define SCREEN_CHARWIDTH  80
#define MAX_CHARWIDTH     2048

class COutputMessageEdit : public CWnd
{
// Construction
public:
	COutputMessageEdit();

// Attributes
public:
		CString m_strSearch;
private:
	struct LineInfo{
			CString *m_strLine;  // 用于存放当前行的数据
			DWORD m_dwStatus;    // 用于设置当前行的状态，如是否被修改，是否需要显示...
			DWORD m_dwMarkFlag ; // 用于查看当前行是否存在标志，如BOOKMARKS ...

			LineInfo() { memset(this, 0, sizeof(LineInfo)); };  // 创建时初始化为0
		};
	    CArray < LineInfo, LineInfo& > m_aDataLine;  //存放每一行的数据

		CPoint m_ptSelStart,m_ptSelEnd;

		BOOL bCaretCaret;
		CPoint m_ptCursor;
		CPoint m_ptCaretPos;

		COLORREF color_NormalText;
		COLORREF color_SelBkColor ;
		COLORREF color_SelTextColor ;

		int m_nTopLine;
		int m_nOffsetChar;

		LPTSTR crFlag[3];

		int m_nTabSize;

		int m_nCharWidth;
		int m_nCharHeight;

		CRITICAL_SECTION m_CriticalSection;

		int m_iHide;
		BOOL m_bFocus;

// Operations
public:

private:
	BOOL m_bWindowsFileInfo;
private:
	BOOL IsSpace(CString strWord);
	void ClearMaskContent(LineInfo LineInfo , CString &strLine);
	void GetCLangLine(int &iIndex,CString &strLine);
	void GetFirstValidWord(LPTSTR lpLine , CString &strWord,BOOL bIgnoreBrace = FALSE);
	CPoint GetMatchCasePos(CPoint ptCursor);
	int GetMatchCaseIndent(CPoint ptCursor, CString &strIndent);
	void ClearInvalidContent(LineInfo LineInfo,CString &strNew);
	CPoint GetMatchBracePos(CPoint ptCursor);
	int GetMatchBraceIndent(CPoint ptCursor,CString &strIndent);
	BOOL IsFirstValidChar(CString *line,int iIndex);
	void AdjustIndent(int nChar);
	int GetLastValidChar(LPTSTR lpStr);
	BOOL IsIndentWord(CString strWord);
	BOOL NeedIncIndent(LPTSTR lpStr);
	int GetLineIndent(int iLineIndex,CString &strIndent,BOOL bAutoIndent = TRUE);
	BOOL IsChinese(LPTSTR lpCurChar);
	void ReleaseTheOldestUndoLink();
	void TestError();
	void ProcessF3();
	void ProcessF2();
	BOOL bModified;
	int GetScreenCharCount();
	void ClearMarkArea(CDC *pdc,int nLineIndex);
	void ScrollToHorz(int nNewLeftChar);
	void RecalcHorzScrollBar(BOOL bPositionOnly = FALSE);
	void GetStringText(const CString &stringSource,int nStart,int nEnd,CString &text);
	void InsertText(const CPoint &ptPosition,CString &text);
	void GetText(const CPoint &ptStart, const CPoint &ptEnd, CString &text);
	void InitializeEditStatus();
	TCHAR ChineseChar[8];
	BOOL bIsChinese;
	int GetLineHeadSpaceLen(LineInfo LineInfo);
	BOOL PutToClipboard(LPCTSTR pszText);
	BOOL GetFromClipboard(CString &strText);
	void DefaultKeyProcess(int nChar);
	CPoint TextToClient(CPoint &point);
	void MoveDown();
	void MoveRight();
	void MoveUp();
	void MoveLeft();
	void MoveHome();
	void MoveEnd();
	void MovePageDown();
	void MovePageUp();
	void InvalidateLines(int nIndexStart,int nIndexEnd = -1);
	CPoint ClientToText(const CPoint &point);
	void HideEditCaret(void);
	void CursorPos2CaretPos(void);
	void ShowEditCaret();
	void SetTextEditFont(CDC *pdc);
	void ScrollToLine(int nNewTopLine);
	int GetTextLineCount(void);
	void RecalcVertScrollBar(BOOL bPositionOnly = FALSE);
	void ClearData();
	int GetTabSize();
//	BOOL IsTab(CString strWord);
	int GetWordWidth(CDC *pdc ,CString strWord,int nCount = -1);
	BOOL IsKeyword(CString strWord);
	int GetLineHeight(void);
	DWORD IsCR(LPCTSTR lpszChar);

		// 插入一个新行
		void InsertNewLine( int nLineIndex, LPCTSTR pszChars = NULL, int nLength = -1 );

		// 删除指定行
		void DeleteLine( int nLineIndex );

		// 在指定行中插入多个字符
		int InsertCharInLine( int nLineIndex, LPCTSTR pszChars, int nLength = -1, int nPosition = 0 );
		
		//在指定行中删除多个字符
		void DeleteCharInLine( int nLineIndex, int nLength, int nPosition = 0 );

		// 重绘指定行的指定区域
		void DrawLine( CDC *pdc, int nLineIndex); 

		// 得到要显示的指定位置字符的颜色
		COLORREF GetCharColor( CPoint ptPos );

		// 得到选择区间
		void GetSelection ( CPoint &ptSelStart, CPoint &ptSelEnd );

		// 设置选择区间
		void SetSelection ( CPoint ptSelStart, CPoint ptSelEnd );


		// 指定位置字符是否在选择区间内
		BOOL IsSelection ( CPoint ptPos );

		// 得到当前屏幕可显示的行数
		int GetScreenCount ( void );

		// 确保指定位置字符可见
		void EnsureVisible ( void );

		// 得到所有行中的最多字符的那一行的字符数
		int GetMaxCharCount ( void );

		CString GetWord(LPCTSTR lpChar);
		COLORREF GetWordColor(CString strWord);

public:

	void GetCursorWord(CString &strWord);
	int GetnCharExtern(LPCTSTR pszString,int nLength = -1);
	int GetCursorVisiblePos();
	int GetExternCharLength(CPoint ptPos);
//	void PrintLine (CDC *pDC, int nLine,int y);
	int iDirection;
	BOOL IsWholeWord(CString line,CString find,int nPos);
	BOOL m_bMatchCase;
	BOOL m_bMatchAllWordOnly;
//	void MarkAllString(CString strWord,BOOL bMatchAllWordOnly,BOOL bMatchCase);
	void SearchString(CPoint ptCur,CString m_strSearch, BOOL bMatchAllWordOnly, BOOL bMatchCase, int iDirection);
	int GetWordAtPos(CPoint ptPos,CString &strWord);
	BOOL ExistSelection();
	// 得到第一个可看到的行索引
	int GetFirstVisibleLine ( void );
	int GetLine( int nIndex, LPTSTR lpszBuffer, int nMaxLength );
	int GetLineCount ();
	void Copy();
	void Clear();
	void OnSelAll();
	void GetEditText(LPTSTR lpBuffer,int nSize);
	int GetEditTextLength(void);
	void SetEditText(LPCTSTR lpszText);
	void GetSearchString(CString &strSearch);
//	void SetSearchString(CString strSearch);
	void SearchInFile(CString strSearch);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COutputMessageEdit)
	public:
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, int nID, CCreateContext* pContext = NULL);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnKeyDown(int nChar, int nRepCnt, int nFlags);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL GetMessageInfo(CString strLine,CString& strFileName,int& iLineIndex);
	BOOL GetWindowsMessageInfo(CString strLine, CString &strFileName, int &iLineIndex);
	void SetWindowsFileInfo( BOOL bWindowsFileInfo = FALSE);

	BOOL GetBuildFilePath(int nIndex,CString &strPath);
	BOOL AddString(LPCTSTR lpMessage);
	virtual ~COutputMessageEdit();

	// Generated message map functions
protected:
	void ExternWord(CString strSrc,CString &strWord);
	int WordInSelection(CPoint pt_Point,CString strWord);
	//{{AFX_MSG(COutputMessageEdit)
	afx_msg void OnKeyUp(int nChar, int nRepCnt, int nFlags);
	afx_msg void OnLButtonDown(int nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnVScroll(int nSBCode, int nPos, CScrollBar* pScrollBar);
	afx_msg void OnShowWindow(BOOL bShow, int nStatus);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnRButtonUp(int nFlags, CPoint point);
	afx_msg void OnRButtonDown(int nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(int nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(int nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(int nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonUp(int nFlags, CPoint point);
	afx_msg void OnHScroll(int nSBCode, int nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDblClk(int nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnCopy();
	afx_msg void OnEditCopy();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSize(UINT nType, int cx, int cy);
//	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OUTPUTMESSAGEEDIT_H__F25816BC_9E0C_4377_AD2F_7F34A1413065__INCLUDED_)
