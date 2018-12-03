#if !defined(AFX_SPECIALEDIT_H__4A4B9D5F_D8F2_456F_B9C9_0162D27874FB__INCLUDED_)
#define AFX_SPECIALEDIT_H__4A4B9D5F_D8F2_456F_B9C9_0162D27874FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SpecialEdit.h : header file
//

#ifndef __AFXTEMPL_H__
#pragma message("Include <afxtempl.h> in your stdafx.h to avoid this message")
#include <afxtempl.h>
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpecialEdit window


#define FLAG_BOOKMARKS  0x00000001
#define FLAG_BREAKPOINT 0x00000002

#define DIRECTION_UP	1
#define DIRECTION_DOWN	2

// !!!!!!!! Undo Process

#define NOPROCESS       0x0000
#define DELETEPROCESS   0x0001
#define INSERTPROCESS   0x0002
#define REPLACEPROCESS  0x0003

#define TYPING          0x0100


#define COLOR_NORMALTEXT	0X0003
#define COLOR_KEYWORDTEXT	0X0004
#define COLOR_MASKTEXT		0X0005
#define COLOR_QUOTETEXT		0X0006
#define COLOR_DEFINE		0X0007
#define COLOR_MARKAREA		0X0008
#define COLOR_SELBKCOLOR	0X0009
#define COLOR_SELTEXTCOLOR	0x000A
#define COLOR_BREAKPOINT	0x000B
#define COLOR_BOOKMARKS		0x000C
#define COLOR_NORMALBKCOLOR	0x000D
#define CL_WINDOW			-1

#define DEFAULTCOLOR_NORMALBKCOLOR	CL_WINDOW

struct UndoLinkStruct {
	int iProcessStyle;  // the process style : Insert ,Delete ,Typeing , Replace, and ...
	CString *strDelete;  // the delete char 
	CPoint startDelete,endDelete;  // will to be Delete ,must know the process position
	CString *strInsert;  // the insert char
	CPoint startInsert,endInsert;  // will to be Delete ,must know the process position
	CPoint startCursor,endCursor;    // the cursor position in process
	struct UndoLinkStruct *prevLink;  // will link the prev undo process
	struct UndoLinkStruct *nextLink;  // will link the next undo process
};

typedef struct UndoLinkStruct UNDOLINK, *LPUNDOLINK;

class CSpecialEdit : public CWnd
{
// Construction
public:
	CSpecialEdit();

protected:

// The private variable
private:
	#define  FLAG_MULTIMASK			0x00000001
	#define  FLAG_SINGLEMASK		0x00000002
	#define  FLAG_DOUBLEQUOTATION	0X00000004
	#define  FLAG_SINGLEQUOTATION   0X00000008

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

		LPTSTR crFlag;

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

		int m_nTabSize;

		BOOL m_bSingleMask;
		BOOL m_bMultiMask;
		BOOL m_bDQuotation;
		BOOL m_bSingleQuotation;

		int m_nTopLine;
		int m_nOffsetChar;
		int m_nMarkAreaWidth;

		LPUNDOLINK curUndoLink;
		LPUNDOLINK rootUndoLink;

		int m_nCharWidth;
		int m_nCharHeight;

		BOOL m_bFocus;
		CFont m_FixEditFont;
		static DWORD dwCharSet;
// The Private function
private:
	BOOL ProcessTab();
	BOOL IsSpace(CString strWord);
	void ClearMaskContent(LineInfo LineInfo , CString &strLine);
	void GetCLangLine(int &iIndex,CString &strLine);
	void ReadSysColor();
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
	BOOL MergeNewLinkToCurrentLink(LPUNDOLINK newUndoLink);
	void ReleaseUndoLink(LPUNDOLINK lpUndoLink);
	void DeleteUndoLink(LPUNDOLINK lpUndoLink);
	void InitUndoLinkStruct(LPUNDOLINK lpUndoLink);
	void InsertNewUndoLink(LPUNDOLINK newUndoLink);
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
	void DeleteChar(LPUNDOLINK lpUndoLink);
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
	void BackSpace(CPoint m_ptCursor,LPUNDOLINK lpUndoLink = NULL);
	int InsertCR(CPoint m_ptPos,BOOL bAlignLeft = FALSE,LPUNDOLINK lpUndoLink = NULL);
	void HideEditCaret(void);
	void CursorPos2CaretPos(void);
	void ShowEditCaret();
	void SetTextEditFont(CDC *pdc);
	void ScrollToLine(int nNewTopLine);
	int GetTextLineCount(void);
	void RecalcVertScrollBar(BOOL bPositionOnly = FALSE);
	void ClearData();
	BOOL GetLineSingleQuotationFlag(LineInfo LineInfo);
	BOOL GetLineDoubleQuotationFlag(LineInfo LineInfo);
	BOOL GetLineSingleMaskFlag(LineInfo LineInfo);
	void SetLineSingleMaskFlag(LineInfo &LineInfo,BOOL bSingleMask);
	void SetLineSingleQuoatationFlag(LineInfo &LineInfo, BOOL bSingleQuoatation);
	void SetLineDoubleQuoatationFlag(LineInfo &LineInfo, BOOL bDoubleQuoatation);
	BOOL IsSingleQuotation(LPTSTR lpLine);
	BOOL IsDoubleQuotation(LPTSTR lpLine);
	BOOL IsDefineWord(CString strWord);
	BOOL IsSingleMaskStart(LPTSTR lpLine);
	void SetLineMultiMaskFlag(LineInfo &LineInfo, BOOL bMultiMask);
	void ParseText(int iIndex,BOOL bPraseAll);
	BOOL IsMultiMskStart(LPCTSTR lpLine);
	BOOL GetLineMultiMaskFlag(struct LineInfo LineInfo);
	BOOL IsMultiMskEnd(LPCTSTR lpLine);
	int GetTabSize();
//	BOOL IsTab(CString strWord);
	int GetWordWidth(CDC *pdc ,CString strWord,int nCount = -1);
	BOOL IsKeyword(CString strWord);
	int GetLineHeight(void);
	BOOL IsCR(LPCTSTR lpszChar);

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

		// 删除选择区间
		//void DeleteSelection ( void );
		void DeleteSelection ( BOOL bShow = TRUE,LPUNDOLINK lpUndoLink = NULL);

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

		void SetPosToStatusBar(int line, int col);
// Attributes
public:
		CString m_strSearch;

// Operations
public:


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpecialEdit)
	public:
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, int nID, CCreateContext* pContext = NULL);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	void GetEditCaretPos(CPoint &ptNewCaret);
	void OnReplaceAll(CString text);
	void OnReplace(CString text);
	void ParseLine(int &iIndex, CString &strLine);
	void PreBuild();
	void SaveSysColor();
	COLORREF GetEditTextColor(int iTextType,BOOL bDefaultColor = FALSE);
	void SetEditTextColor(int iTextType,COLORREF color);
	void ResetAdjustIndent();
	void AutoIndent();
	void AutoMatchBrace();
	void Redo();
	void Undo();
	BOOL CanRedo();
	int GetnCharExtern(LPCTSTR pszString,int nLength = -1);
	int GetCursorVisiblePos();
	int GetExternCharLength(CPoint ptPos);
	void PrintLine (CDC *pDC, int nLine,int y);
	int iDirection;
	BOOL IsWholeWord(CString line,CString find,int nPos);
	BOOL m_bMatchCase;
	BOOL m_bMatchAllWordOnly;
	void MarkAllString(CString strWord,BOOL bMatchAllWordOnly,BOOL bMatchCase);
	BOOL SearchString(CPoint ptCur,CString m_strSearch, BOOL bMatchAllWordOnly, BOOL bMatchCase, int iDirection);
	int GetWordAtPos(CPoint ptPos,CString &strWord);
	BOOL ExistSelection();
	BOOL GetMarkFlag(int nIndex, DWORD dwFlag);
	void ToNextMarkFlag(int nIndex, DWORD dwFlag);
	int GetNextMarkFlag(int nIndex, DWORD dwFlag);
	void ToPrevMarkFlag(int nIndex, DWORD dwFlag);
	int GetPrevMarkFlag(int nIndex, DWORD dwFlag);
	void ClearAllMarkFlag(DWORD dwFlag);
	void ClearMarkFlag(int nIndex, DWORD dwFlag);
	void SetMarkFlag(int nIndex, DWORD dwFlag);
	void ShowMarkFlag(CDC *pdc ,int nIndex);
	// 得到第一个可看到的行索引
	int GetFirstVisibleLine ( void );
	int GetLine( int nIndex, LPTSTR lpszBuffer, int nMaxLength );
	DWORD GetMargins( );
	void SetMargins( UINT nLeft, UINT nRight );
	int GetLineCount ();
	BOOL CanUndo();
	void Paste();
	void Cut();
	void Copy();
	void Clear();
	void OnSelAll();
	void GetEditText(LPTSTR lpBuffer,int nSize);
	int GetEditTextLength(void);
	void SetEditText(LPCTSTR lpszText);
	void SetModify( BOOL bModify );
	BOOL GetModify( void );
	virtual ~CSpecialEdit();
	void SetEditCaretPos(CPoint ptNewCaret);
	void GetSearchString(CString &strSearch);
//	void SetSearchString(CString strSearch);
	void SearchInFile(CString strSearch);
	void ResetColor(BOOL bDefault);
	void GetCursorWord(CString &strWord);

// Generated message map functions
protected:
	void WarningCanntCompleteAction();
	BOOL CanMergeNextLine(int nLineIndex);
	void ExternWord(CString strSrc,CString &strWord);
	int WordInSelection(CPoint pt_Point,CString strWord);
	COLORREF GetEditBkColor(void);
	void InsertAdditionalMessage(int nChar);
//	int GetnCharHead2Visible(int nIndex);
	//{{AFX_MSG(CSpecialEdit)
	afx_msg void OnChar(int nChar, int nRepCnt, int nFlags);
	afx_msg void OnKeyDown(int nChar, int nRepCnt, int nFlags);
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
	afx_msg void OnCut();
	afx_msg void OnPaste();
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.



#endif // !defined(AFX_SPECIALEDIT_H__4A4B9D5F_D8F2_456F_B9C9_0162D27874FB__INCLUDED_)


