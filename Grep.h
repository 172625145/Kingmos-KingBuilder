// Grep.h: interface for the CGrep class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GREP_H__62F20906_8C9F_40F0_9AA0_198C16DEEA64__INCLUDED_)
#define AFX_GREP_H__62F20906_8C9F_40F0_9AA0_198C16DEEA64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGrep  
{
public:
	void SetMatchWholeWordOnly(BOOL bMatchWholeWordOnly);
	void Break();
	BOOL m_bBreak;
	BOOL m_bComplete;
	BOOL IsComplete();
	int m_nCount;
	BOOL IsWholeWord(CString line, CString find, int iPos);
	BOOL ReadLine(CString &line);
	BOOL LoadFile(CString strFileName);
	BOOL FileNameCompare(LPCTSTR lpcszMask, int iMaskLen, LPCTSTR lpcszSrc, int iSrcLen);
	BOOL MatchFileName(CString strFileName, CString strFilter);
	void MergeFullFileName(CString &strFullName,CString strDir,CString strFilename);
	void ActiveMessage();
	void ClearMessage();
	void OutputString(CString strMessage);
	void GrepDir(CString strDir);
	void GrepFile(CString strFileName);
	void SetLookInSubFolder(BOOL bLookInFolder);
	void SetCaseMatch(BOOL bCasematch);
	void SetSearchFolder(CString strSearchFolder);
	void SetFilter(CString strFilter);
	void SetSearchString(CString strSearch);
	void Go();
	CGrep();
	virtual ~CGrep();
	static ULONG CALLBACK GrepThreadProc( LPVOID lpParameter );

private:
	BOOL	m_bLookInSubFolder;
	BOOL	m_bCaseMatch;
	BOOL	m_bMatchWholeWord;

	CString m_strSearchFolder;
	CString m_strFilter;
	CString m_strSearch;


	CFile m_GrepFile;
	LPTSTR m_lpFileData;
	LPTSTR m_lpCurPos;
	DWORD m_dwFileSize;

};

#endif // !defined(AFX_GREP_H__62F20906_8C9F_40F0_9AA0_198C16DEEA64__INCLUDED_)
