// SpecialEdit.cpp : implementation file
//

#include "stdafx.h"
#include "specialview.h"
#include "SpecialEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CSpecialEdit


int CSpecialEdit::GetLineIndent(int iLineIndex, CString &strIndent,BOOL bAutoIndent)
{
	LPTSTR lpStr,lpLine;
	int nCount = 0;
	int iCurLine;
	LineInfo LineInfo;
//	int i;
//	BOOL bMask;
	CString strNew;

		if (iLineIndex < 0)
		{
			strIndent = "";
			return 0;
		}
		iCurLine = iLineIndex;
		while(1)
		{
		
			LineInfo = m_aDataLine.GetAt(iCurLine);

			strNew = *LineInfo.m_strLine;
//			bMask =  GetLineMultiMaskFlag(LineInfo);	
//			ClearInvalidContent(bMask,strNew);
			ClearInvalidContent(LineInfo,strNew);
//			lpLine = lpStr = LineInfo.m_strLine->LockBuffer();
			lpLine = lpStr = strNew.LockBuffer();
			nCount = 0;

			while(1)
			{
				if (*lpStr == 0)
				{
					nCount = -1;
					break;
				}
				if (*lpStr != ' ' && *lpStr != '\t')
					break;  // the line indent to be reach
				lpStr ++ ;
				nCount ++;
			}
//			LineInfo.m_strLine->UnlockBuffer();
			if (nCount >= 0)
			{
				CString InsertString(lpLine,nCount);  // get this line indent
				strIndent = InsertString;  // return the line indent
				if (bAutoIndent)
				{
					if (NeedIncIndent(lpStr))
					{
						strIndent += "\t";
						nCount ++;
					}
					/******************************************/
					else
					{
						int nChar;

						nChar = GetLastValidChar(lpLine);
						if (nChar == ';')
						{
							CString strWord ;
							// Is complete line,maybe will be indent to prev line
							while(1)
							{
								if (iCurLine == 0)
									break;
								iCurLine --;
								LineInfo = m_aDataLine.GetAt(iCurLine);
								lpLine = LineInfo.m_strLine->LockBuffer();
								nChar = GetLastValidChar(lpLine);
								if (nChar == ';')
									break;
								GetFirstValidWord(lpLine,strWord,TRUE);
								LineInfo.m_strLine->UnlockBuffer();

								if (strWord.Compare("if") == 0 ||
									strWord.Compare("else") == 0 ||
									strWord.Compare("while") == 0 ||
									strWord.Compare("for") == 0)
								{
									nCount = GetLineHeadSpaceLen(LineInfo);
									CString InsertString(lpLine,nCount);  // get this line indent
									strIndent = InsertString;  // return the line indent
									break;
								}
								if (strWord.Compare("{") == 0 ||
									strWord.Compare("case") == 0 ||
									strWord.Compare("default") == 0)
								{
									break;
								}
							}
						}
					}
				}
				strNew.UnlockBuffer();
				return nCount; // return the line indent length
			}
			strNew.UnlockBuffer();
			if (iCurLine == 0)
				break;
			iCurLine --;
		}
		strIndent = "";  // the line indent is empty
		return 0;
}



BOOL CSpecialEdit::NeedIncIndent(LPTSTR lpStr)
{
	CString strWord;
	BOOL bIndent = FALSE;
	int nChar;

	while(1)
	{
		if (*lpStr == 0)
		{
			return FALSE;
		}
		if (*lpStr != ' ' && *lpStr != '\t')
			break;  // the next char is first valid word
		lpStr ++ ;
	}
/*
	while(*lpStr)
	{
		strWord = GetWord(lpStr);
		if (IsIndentWord(strWord))
			bIndent = TRUE;
		if (bIndent == FALSE && strWord.Compare("}") == 0)
		{
			lpStr += strWord.GetLength();
		}
		else
		{
			break;
		}
	}
	*/
	GetFirstValidWord(lpStr,strWord,TRUE);
	if (IsIndentWord(strWord))
		bIndent = TRUE;
	if (bIndent)
	{
		nChar = GetLastValidChar(lpStr);
		if (nChar == ';')
			bIndent = FALSE;
	}
	return bIndent;
}
static LPCTSTR Sys_IndentWord[]={
	_T("if"),
	_T("else"),
	_T("for"),
	_T("while"),
	_T("case"),
	_T("default"),
	_T("{"),
	NULL
};

BOOL CSpecialEdit::IsIndentWord(CString strWord)
{
	// the 'if' , 'for', 'while' ,'{' , ... need inc indent
	int i=0;

	while (TRUE)
	{
		if (Sys_IndentWord[i]==NULL)
			break;
		if (strWord.Compare(Sys_IndentWord[i]) ==0)	
			return TRUE;
		i++;
	}

	return FALSE;
}

int CSpecialEdit::GetLastValidChar(LPTSTR lpStr)
{
	int nLastValidChar = 0;
	BOOL bMaskFalg = FALSE;
	
	while(1)
	{
		if (*lpStr == 0)
		{  
			return nLastValidChar; 
		}
		
		if (*lpStr == '/' && *(lpStr+1) == '/')
		{
			return nLastValidChar;
		}
		if (*lpStr == '/' && *(lpStr+1) == '*')
		{
			bMaskFalg = TRUE;
			lpStr += 2;
			while(*lpStr)
			{/*  */
				if (*lpStr == '*' && *(lpStr+1) == '/')
				{
					lpStr += 2;
					bMaskFalg = FALSE;
					break;
				}
				lpStr ++;
			}
			if (bMaskFalg == TRUE)
			{
				return 0; 
			}
			continue;
		}
		if (*lpStr != ' ' && *lpStr != '\t')
		{
			nLastValidChar = *lpStr;
		}
		lpStr ++ ;
	}
	return nLastValidChar;
}

void CSpecialEdit::AdjustIndent(int nChar)
{
	LineInfo LineInfo;
	CString strNewIndent;
	int nCount, nCountSpace;
	CString strWord;
	LPTSTR lpLine;
	CPoint ptStart , ptEnd ;
	LPUNDOLINK newUndoLink = (LPUNDOLINK)malloc(sizeof(UNDOLINK));

	InitUndoLinkStruct( newUndoLink );

	if (m_ptCursor.y == 0)
	{
		goto EXIT;
	}
	LineInfo = m_aDataLine.GetAt(m_ptCursor.y);

	lpLine = LineInfo.m_strLine->LockBuffer(); 
	GetFirstValidWord(lpLine,strWord);
	LineInfo.m_strLine->UnlockBuffer();
	
//		return;
	if (nChar != '\r')
	{
		if (strWord.GetLength() == 0)
			goto EXIT;
		if (IsFirstValidChar(LineInfo.m_strLine,m_ptCursor.x-1) == FALSE)
		{
			if (nChar == ':' && (strWord.Compare("case") || strWord.Compare("default")))
			{
				nCountSpace = GetLineHeadSpaceLen(LineInfo);

				ptStart = ptEnd = m_ptCursor;
				ptStart.x = 0;
				ptEnd.x = nCountSpace;

				newUndoLink->iProcessStyle |= DELETEPROCESS;
				newUndoLink->startDelete = ptStart;
				newUndoLink->endDelete = ptEnd;
				GetStringText(*LineInfo.m_strLine,ptStart.x,ptEnd.x,*newUndoLink->strDelete);

				LineInfo.m_strLine->Delete(0,nCountSpace);

				nCount = GetMatchCaseIndent(m_ptCursor,strNewIndent);
				LineInfo.m_strLine->Insert(0,strNewIndent);
				m_ptCursor.x += nCount - nCountSpace;
				CursorPos2CaretPos();
				InvalidateLines(m_ptCursor.y,m_ptCursor.y);
				EnsureVisible();
				bModified = TRUE;

				if (newUndoLink)
				{
					ptStart = ptEnd = m_ptCursor;
					ptStart.x = 0;
					ptEnd.x = nCount;
					newUndoLink->startInsert = ptStart;
					newUndoLink->endInsert = ptEnd;
					newUndoLink->iProcessStyle |= INSERTPROCESS;
					newUndoLink->strInsert->Insert(0,strNewIndent);
	//				newUndoLink->endCursor = m_ptCursor;
				}

			}
			if (nChar != strWord.GetAt(0))
				goto EXIT;
	//			return ;
		}
	}

	nCountSpace = GetLineHeadSpaceLen(LineInfo);

	ptStart = ptEnd = m_ptCursor;
	ptStart.x = 0;
	ptEnd.x = nCountSpace;

	newUndoLink->iProcessStyle |= DELETEPROCESS;
	newUndoLink->startDelete = ptStart;
	newUndoLink->endDelete = ptEnd;
	GetStringText(*LineInfo.m_strLine,ptStart.x,ptEnd.x,*newUndoLink->strDelete);
//	GetText(ptStart, ptEnd, *lpUndoLink->strDelete);

	LineInfo.m_strLine->Delete(0,nCountSpace);

	if (nChar == '{')
	{
//		LineInfo.m_strLine->Delete(0,m_ptCursor.x - 1);
		nCount = GetLineIndent((m_ptCursor.y - 1), strNewIndent,FALSE);
		LineInfo.m_strLine->Insert(0,strNewIndent);
//		m_ptCursor.x = nCount + 1;
		m_ptCursor.x += nCount - nCountSpace;
		CursorPos2CaretPos();
		InvalidateLines(m_ptCursor.y,m_ptCursor.y);
		EnsureVisible();
		bModified = TRUE;
	}
	else  if (nChar == '}')
	{
//		nCount = GetLineIndent((m_ptCursor.y - 1), strNewIndent,FALSE);
		m_ptCursor.x -=nCountSpace;
		nCount = GetMatchBraceIndent(m_ptCursor,strNewIndent);
//		LineInfo.m_strLine->Delete(0,m_ptCursor.x - 1);
		LineInfo.m_strLine->Insert(0,strNewIndent);
		m_ptCursor.x += nCount;// - nCountSpace;
		CursorPos2CaretPos();
		InvalidateLines(m_ptCursor.y,m_ptCursor.y);
		EnsureVisible();
		bModified = TRUE;
	}
	else  // nChar == '\r'
	{
//		nCount = GetLineIndent((m_ptCursor.y - 1), strNewIndent,FALSE);
		m_ptCursor.x -=nCountSpace;
		if (m_ptCursor.x < 0)
		{
			// 如果光标位置的字符在空格位置，则将光标移动到第一个可见字符
			m_ptCursor.x = 0 ;
		}
//		nCount = GetLineIndent(m_ptCursor,strNewIndent);
		nCount = GetLineIndent((m_ptCursor.y - 1), strNewIndent,TRUE);
//		LineInfo.m_strLine->Delete(0,m_ptCursor.x - 1);
		LineInfo.m_strLine->Insert(0,strNewIndent);
		m_ptCursor.x += nCount;// - nCountSpace;
		CursorPos2CaretPos();
		InvalidateLines(m_ptCursor.y,m_ptCursor.y);
		EnsureVisible();
		bModified = TRUE;
	}

	if (newUndoLink)
	{
		ptStart = ptEnd = m_ptCursor;
		ptStart.x = 0;
		ptEnd.x = nCount;
		newUndoLink->startInsert = ptStart;
		newUndoLink->endInsert = ptEnd;
		newUndoLink->iProcessStyle |= INSERTPROCESS;
		newUndoLink->strInsert->Insert(0,strNewIndent);
//		newUndoLink->endCursor = m_ptCursor;
	}

EXIT:
	if (newUndoLink->iProcessStyle == NOPROCESS)
	{
//		delete newUndoLink;
		ReleaseUndoLink(newUndoLink);
	}
	else
	{
		newUndoLink->endCursor = m_ptCursor;
		InsertNewUndoLink(newUndoLink);
	}
}

BOOL CSpecialEdit::IsFirstValidChar(CString *line, int iIndex)
{
	LPTSTR lpStr;
	int iCount = 0;

		lpStr = line->LockBuffer();

		while(1)
		{
			if (*lpStr == 0)
			{
				return FALSE;
			}
			if (*lpStr != ' ' && *lpStr != '\t')
				break;  // the next char is first valid word
			lpStr ++ ;
			iCount ++;
		}
		line->UnlockBuffer();
		if (iCount == iIndex)
			return TRUE;
		else
			return FALSE;
}

int CSpecialEdit::GetMatchBraceIndent(CPoint ptCursor, CString &strIndent)
{
	CPoint ptMatchBrace;
	int nCount;

	ptMatchBrace = GetMatchBracePos(ptCursor);

	nCount = GetLineIndent((ptMatchBrace.y), strIndent,FALSE);
	
	return nCount;
}

#define DOWNWARD_DIRECTION  1
#define UPWARD_DIRECTION  2

CPoint CSpecialEdit::GetMatchBracePos(CPoint ptCursor)
{
	CPoint ptCurPos(0,0);
	LineInfo LineInfo;
	LPTSTR lpLine;
	int Direction;
	CString strNew;
//	BOOL bMask;

	LineInfo = m_aDataLine.GetAt(ptCursor.y);
	strNew = *LineInfo.m_strLine;
//	bMask =  GetLineMultiMaskFlag(LineInfo);	
//	ClearInvalidContent(bMask,strNew);
	ClearInvalidContent(LineInfo,strNew);
//	lpLine = LineInfo.m_strLine->LockBuffer();
	lpLine = strNew.LockBuffer();
	ptCurPos = ptCursor;

	if (lpLine[ptCursor.x] == '{')
	{
		Direction = DOWNWARD_DIRECTION;
		ptCurPos.x = ptCursor.x + 1;
	}
	else if (lpLine[ptCursor.x] == '}')
	{
		Direction = UPWARD_DIRECTION;
		ptCurPos.x = ptCursor.x ;
	}
	else if (ptCursor.x > 0 && lpLine[ptCursor.x-1] == '{')
	{
		Direction = DOWNWARD_DIRECTION;
		ptCurPos.x = ptCursor.x ;
	}
	else if (ptCursor.x > 0 && lpLine[ptCursor.x-1] == '}')
	{
		Direction = UPWARD_DIRECTION;
		ptCurPos.x = ptCursor.x -1;
	}
	else
	{  // Have no brace to match
		return ptCursor;
	}
	
//	LineInfo.m_strLine->UnlockBuffer();
	strNew.UnlockBuffer();
	if (Direction == DOWNWARD_DIRECTION)
	{
		int iMatch = 0;
		while(1)
		{
			int nLinelen;

			strNew = *LineInfo.m_strLine;
//			bMask =  GetLineMultiMaskFlag(LineInfo);	
//			ClearInvalidContent(bMask,strNew);
			ClearInvalidContent(LineInfo,strNew);
//			lpLine = LineInfo.m_strLine->LockBuffer();
			lpLine = strNew.LockBuffer();
			nLinelen = strNew.GetLength();
//			ptCurPos.x ++;  // Jump current brace '{'
			while(ptCurPos.x < nLinelen)
			{
				
				if (lpLine[ptCurPos.x] == '}')
				{
					if (iMatch == 0)
					{
						return ptCurPos;
					}
					iMatch --;
				}
				if (lpLine[ptCurPos.x] == '{')
				{
					iMatch ++;
				}
				ptCurPos.x ++;
			}
//			LineInfo.m_strLine->UnlockBuffer();
			strNew.UnlockBuffer();
			ptCurPos.y ++;
			if (ptCurPos.y >=m_aDataLine.GetSize())
			{  // Have not find match brace
				return ptCursor;
			}
			LineInfo = m_aDataLine.GetAt(ptCurPos.y);
//			ptCurPos.x = LineInfo.m_strLine->GetLength();
			ptCurPos.x = 0;
		}
	}
	else  // Direction == UPWARD_DIRECTION
	{
		int iMatch = 0;
		while(1)
		{
			strNew = *LineInfo.m_strLine;
//			bMask =  GetLineMultiMaskFlag(LineInfo);	
//			ClearInvalidContent(bMask,strNew);
			ClearInvalidContent(LineInfo,strNew);
			//lpLine = LineInfo.m_strLine->LockBuffer();
			lpLine = strNew.LockBuffer();
			while(ptCurPos.x)
			{
				ptCurPos.x --;
				if (lpLine[ptCurPos.x] == '{')
				{
					if (iMatch == 0)
					{
						return ptCurPos;
					}
					iMatch --;
				}
				if (lpLine[ptCurPos.x] == '}')
				{
					iMatch ++;
				}
				//ptCurPos.x --;
			}
//			LineInfo.m_strLine->UnlockBuffer();
			strNew.UnlockBuffer();

			if (ptCurPos.y == 0)
			{  // Have not find match brace
				return ptCursor;
			}
			ptCurPos.y --;
			LineInfo = m_aDataLine.GetAt(ptCurPos.y);
			ptCurPos.x = LineInfo.m_strLine->GetLength();
		}
	}
	return ptCurPos;
}

void CSpecialEdit::ClearInvalidContent(LineInfo LineInfo, CString &strNew)
{
	LPTSTR lpLine;
	BOOL bClear;
	CString strWord;
	int WordLen;
//	BOOL bMultiMask = FALSE;
//	BOOL bSingleMask = FALSE;
//	BOOL bDQuotation = FALSE;
//	BOOL bSingleQuotation = FALSE;

//		m_bSingleMask = FALSE;
//		m_bDQuotation = FALSE;
//		m_bSingleQuotation = FALSE;
//		m_bMultiMask = bMask;
		m_bSingleMask = GetLineSingleMaskFlag(LineInfo);
		m_bMultiMask =  GetLineMultiMaskFlag(LineInfo);
		m_bDQuotation = GetLineDoubleQuotationFlag(LineInfo);
		m_bSingleQuotation = GetLineSingleQuotationFlag(LineInfo);
		lpLine = strNew.LockBuffer();
		while(1)
		{
			strWord=GetWord(lpLine);
			WordLen = strWord.GetLength();
			if (WordLen == 0)
				break;
			if (*lpLine == '/')
			{
				if (m_bMultiMask == FALSE)  //  '/*  */' Mask
				{
					if (IsMultiMskStart(lpLine))  // x != 0 is because the end must not the first char
					{
						m_bMultiMask = TRUE;
						*lpLine++ = ' ';
						*lpLine++ = ' ';
//						lpLine += 2 ;
						continue;
					}
				}
				if (m_bSingleMask == FALSE && m_bMultiMask == FALSE)  //  '//' Mask
				{
					if (IsSingleMaskStart(lpLine))  // x != 0 is because the end must not the first char
					{
						m_bSingleMask = TRUE;
						*lpLine++ = ' ';
						*lpLine++ = ' ';
//						lpLine += 2 ;
						continue;
					}
				}
			}
			else if (*lpLine == '*')
			{
				if (m_bMultiMask == TRUE)  //  '/*  */' Mask
				{
					if (IsMultiMskEnd(lpLine))  
					{
						m_bMultiMask = FALSE;
						*lpLine++ = ' ';
						*lpLine++ = ' ';
//						lpLine  +=2;  // '*/*' ,then the second * is not Start Mask
						continue;
					}
				}
			}
			else if (m_bMultiMask == FALSE && IsDoubleQuotation(lpLine) ==TRUE )
			{
				if (m_bDQuotation == TRUE)
				{
					//pdc->SetTextColor(color_Quotation);
					m_bDQuotation = FALSE;
				}
				else
					m_bDQuotation = TRUE;
				*lpLine++ = ' ';
//				lpLine ++ ;
				continue;
			}
			else if (m_bMultiMask == FALSE && IsSingleQuotation(lpLine) == TRUE)
			{
				if (m_bSingleQuotation == TRUE)
				{
					//pdc->SetTextColor(color_Quotation);
					m_bSingleQuotation = FALSE;
				}
				else
					m_bSingleQuotation = TRUE;
				*lpLine++ = ' ';
//				lpLine ++ ;
				continue;
			}
			bClear = FALSE;
			if (m_bMultiMask == TRUE)  //  '/*  */' Mask
			{
				//pdc->SetTextColor(color_Mask);
				bClear = TRUE;
			}
			else if (m_bSingleMask == TRUE)  //  '//' Mask
			{
				//pdc->SetTextColor(color_Mask);
				bClear = TRUE;
			}
			else if (m_bDQuotation == TRUE)  //  "   "  
			{
				//pdc->SetTextColor(color_Quotation);
				bClear = TRUE;
			}
			else if (m_bSingleQuotation == TRUE)  // '   '
			{
				//pdc->SetTextColor(color_Quotation);
				bClear = TRUE;
			}
			if (bClear == TRUE)
			{
				for (int i = 0 ; i < WordLen ; i++)
				{
					lpLine[i] = ' ';
				}
			}
			lpLine += WordLen;
		}
		strNew.UnlockBuffer();
}


void CSpecialEdit::AutoIndent()
{
	LineInfo LineInfo;
//	int nChar;
	int iIndex ,iMaxIndex;
	int nCount;
	CString strNewIndent;
	CString strWord;
	LPTSTR lpLine;
	BOOL bMask;

	iMaxIndex = m_aDataLine.GetSize();
	for (iIndex = 0; iIndex < iMaxIndex ; iIndex ++)
	{
		LineInfo = m_aDataLine.GetAt(iIndex);
		bMask =  GetLineMultiMaskFlag(LineInfo);	
		if (bMask == TRUE)
			continue;
		nCount = GetLineHeadSpaceLen(LineInfo);
		LineInfo.m_strLine->Delete(0,nCount);
		if (LineInfo.m_strLine->IsEmpty() == FALSE)
		{
//			nChar = LineInfo.m_strLine->GetAt(0);
			lpLine = LineInfo.m_strLine->LockBuffer();
			GetFirstValidWord(lpLine,strWord);
			LineInfo.m_strLine->UnlockBuffer();
//			if (nChar == '{')
			if (strWord.Compare("{") == 0)
			{
				GetLineIndent((iIndex - 1), strNewIndent,FALSE);
				LineInfo.m_strLine->Insert(0,strNewIndent);
			}
//			else if (nChar == '}')
			else if (strWord.Compare("}") == 0)
			{
				CPoint ptCursor(0,iIndex);
				GetMatchBraceIndent(ptCursor,strNewIndent);
				LineInfo.m_strLine->Insert(0,strNewIndent);
			}
			else if (strWord.Compare("case") == 0)
			{
				CPoint ptCursor(0,iIndex);
				GetMatchCaseIndent(ptCursor,strNewIndent);
				LineInfo.m_strLine->Insert(0,strNewIndent);
			}
			else if (strWord.Compare("default") == 0)
			{
				CPoint ptCursor(0,iIndex);
				GetMatchCaseIndent(ptCursor,strNewIndent);
				LineInfo.m_strLine->Insert(0,strNewIndent);
			}
			else
			{
				GetLineIndent((iIndex - 1), strNewIndent,TRUE);
				LineInfo.m_strLine->Insert(0,strNewIndent);
			}
		}
	}
}

int CSpecialEdit::GetMatchCaseIndent(CPoint ptCursor, CString &strIndent)
{
	CPoint ptMatchCase;
	int nCount;
	LineInfo LineInfo;
	LPTSTR lpLine;
	CString strWord;

	ptMatchCase = GetMatchCasePos(ptCursor);

	LineInfo = m_aDataLine.GetAt(ptMatchCase.y);

	lpLine = LineInfo.m_strLine->LockBuffer();

	GetFirstValidWord(lpLine,strWord);
	LineInfo.m_strLine->UnlockBuffer();

	if (strWord.Compare("{") == 0)
	{
		nCount = GetLineIndent((ptMatchCase.y), strIndent,TRUE);
	}
	else
	{
		nCount = GetLineIndent((ptMatchCase.y), strIndent,FALSE);
	}
	
	return nCount;
}

CPoint CSpecialEdit::GetMatchCasePos(CPoint ptCursor)
{
	CPoint ptCurPos(0,0);
	LineInfo LineInfo;
	LPTSTR lpLine;
//	int Direction;
	int nBraceNum=0;

	if (ptCursor.y == 0)
		return 0;
	ptCursor.y --;
	LineInfo = m_aDataLine.GetAt(ptCursor.y);
	lpLine = LineInfo.m_strLine->LockBuffer();
	ptCurPos = ptCursor;

	int iMatch = 0;
	while(1)
	{
		CString strNew;
//		BOOL bMask;
		CString strWord;

		strNew = *LineInfo.m_strLine;
//		bMask =  GetLineMultiMaskFlag(LineInfo);	
//		ClearInvalidContent(bMask,strNew);
		ClearInvalidContent(LineInfo,strNew);
//			lpLine = LineInfo.m_strLine->LockBuffer();
		lpLine = strNew.LockBuffer();
		GetFirstValidWord(lpLine, strWord);
		if (nBraceNum == 0 && strWord.Compare("case") == 0)
		{
			//return the line 
			return ptCurPos;
		}
		else 
		{
			LPTSTR lpCur;
			lpCur = lpLine + strNew.GetLength();
			while(1)
			{
				if (*lpCur == '{')
				{
					if (nBraceNum == 0)
						return ptCurPos;
					nBraceNum --;
					//return the line 
				}
				else if (*lpCur == '}')
				{
					nBraceNum ++;
					//ptCurPos = GetMatchBracePos(ptCurPos);
				}
				if (lpCur == lpLine)
					break;
				lpCur --;
			}
		}
		strNew.UnlockBuffer();
		if (ptCurPos.y <= 0)
		{  // Have not find match brace
			return ptCursor;
		}
		ptCurPos.y --;
		LineInfo = m_aDataLine.GetAt(ptCurPos.y);
		ptCurPos.x = LineInfo.m_strLine->GetLength();
	}
	return ptCurPos;
}

void CSpecialEdit::GetFirstValidWord(LPTSTR lpLine, CString &strWord,BOOL bIgnoreBrace)
{
	while(1)
	{
		if (*lpLine == 0)
			break;
		if (*lpLine != ' ' && *lpLine != '\t')
			break;  // the next char is first valid word
		lpLine ++ ;
	}
	while(1)
	{
		strWord=GetWord(lpLine);
		if (bIgnoreBrace == TRUE)
		{
			if (strWord.Compare("}") == 0)
			{
				lpLine +=strWord.GetLength();
				continue;
			}
		}
		break;
	}
}

