// Grep.cpp: implementation of the CGrep class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ambo.h"
#include "Grep.h"
#include "MainFrame.h"
#include <ctype.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGrep::CGrep()
{
	m_bLookInSubFolder = TRUE;
	m_bCaseMatch = FALSE;
	m_bMatchWholeWord = FALSE;

	m_strSearchFolder = _T("");
	m_strFilter = _T("");
	m_strSearch = _T("");

	m_lpFileData = NULL;
	m_dwFileSize = 0;
	m_lpCurPos = NULL;

	m_nCount = 0;
	m_bComplete = TRUE;
}

CGrep::~CGrep()
{

}

void CGrep::Go()
{
//	CString strTitle;

	ActiveMessage();
	ClearMessage();
	m_nCount = 0;
	m_bComplete = FALSE;
	m_bBreak = FALSE;

/*	strTitle.Format("Find string \'%s\' in \'%s\': \r\n",(LPCTSTR)m_strSearch,(LPCTSTR)m_strSearchFolder);
	OutputString(strTitle);
	GrepDir(m_strSearchFolder);	

	if (m_nCount == 0)
	{
		strTitle.Format("Can't Found string \'%s\'\r\n",(LPCTSTR)m_strSearch,(LPCTSTR)m_strSearchFolder);
		OutputString(strTitle);
	}
	else
	{
		strTitle.Format("Found %d occurrence(s)\r\n",m_nCount);
		OutputString(strTitle);
	}
*/
	CreateThread(NULL,0,GrepThreadProc,this,0,0);
}

void CGrep::SetSearchString(CString strSearch)
{
	m_strSearch = strSearch;

}

void CGrep::SetFilter(CString strFilter)
{
	m_strFilter = strFilter;

}

void CGrep::SetSearchFolder(CString strSearchFolder)
{
	m_strSearchFolder = strSearchFolder;
}

void CGrep::SetCaseMatch(BOOL bCasematch)
{
	m_bCaseMatch = bCasematch;
}

void CGrep::SetLookInSubFolder(BOOL bLookInFolder)
{
	m_bLookInSubFolder = bLookInFolder;
}

void CGrep::GrepFile(CString strFileName)
{
	CString line;
	CString newLine;
	CString strSearch;
	CString strTitle;
	int nLine = 1;
	int nPos = 0;
	
	if (LoadFile(strFileName)== FALSE) // 装载文件
	{
		// 装载文件失败
		strTitle.Format("Can't Open File \'%s\'\r\n",(LPCTSTR)strFileName);
		OutputString(strTitle);
		return ;
	}

	strSearch = m_strSearch;
	if (m_bCaseMatch == FALSE)
		strSearch.MakeUpper();

	while(1)
	{
		if (m_bBreak == TRUE)
			break;
		if (ReadLine(line) == FALSE) // 读一行文本
			break;
		if (m_bCaseMatch == FALSE)
		{
			newLine = line;
			newLine.MakeUpper();
			nPos = newLine.Find(strSearch);
		}
		else
		{
			newLine = line;
			nPos = newLine.Find(strSearch);
		}
		if (nPos!= -1)
		{
			BOOL bNeed = TRUE;
			if (m_bMatchWholeWord == TRUE)
			{  // 需要整个字符匹配
				while(1)
				{
					if (IsWholeWord(newLine,strSearch,nPos) == FALSE)
					{  //查找到的位置不是一个独立的单词，继续查找下一个
						nPos = newLine.Find(strSearch,nPos+1);
						if (nPos == -1)
						{  // 没有找到
							bNeed = FALSE;
							break;
						}
					}
					else
					{
						// 已经找到
						break;
					}
				}
			}
			if (bNeed)
			{
				// 在这一行中有要找的字符串
				strTitle.Format("%s(%d): %s\r\n",(LPCTSTR)strFileName,nLine,(LPCTSTR)line);
				OutputString(strTitle);
				m_nCount ++;
			}
		}
		nLine ++;
	}
	if (m_lpFileData)
		delete m_lpFileData;

	m_lpFileData = NULL;
}

void CGrep::GrepDir(CString strDir)
{

  WIN32_FIND_DATA FindFileData;
  HANDLE hFind;
  CString strSearch;
  CString strFileName;

//  printf ("Target file is %s.\n", argv[1]);
//  strSearch = strDir + "\\";
 // strSearch += "*.*";
  MergeFullFileName(strSearch,strDir,"*.*");

  hFind = FindFirstFile(strSearch, &FindFileData);

  if (hFind == INVALID_HANDLE_VALUE) {
	  return ;
  } 
  else 
  {
		while(TRUE)
		{  // 文件存在
			if (m_bBreak == TRUE)
				break;
			if (!(strcmp(".",FindFileData.cFileName) == 0 || strcmp("..",FindFileData.cFileName) == 0 ))
			{
//				strFileName = strDir + "\\";
//				strFileName += FindFileData.cFileName;
			    MergeFullFileName(strFileName,strDir,FindFileData.cFileName);
				if( FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY )
				{
					// 查找到的是一个目录
					if (m_bLookInSubFolder)
						GrepDir(strFileName);
				}
				else
				{
					// 查找到一个文件

//					if (MatchFileName(strFileName,m_strFilter) == TRUE)
					if (MatchFileName(FindFileData.cFileName,m_strFilter) == TRUE)
					{
						GrepFile(strFileName);				
					}
				}
			}
			if( FindNextFile(hFind,&FindFileData)==0 )  //查找下一个文件
			{ // 已经没有文件了
				break;
			}
		}
	    FindClose(hFind);
  }
	
}

void CGrep::OutputString(CString strMessage)
{
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;

	pMainFrame->m_outBar.OutputFindmessage(strMessage);
}

void CGrep::ClearMessage()
{
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;

	pMainFrame->m_outBar.ClearFindInFilesMessage();
}

void CGrep::ActiveMessage()
{
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;

	pMainFrame->m_outBar.ActiveFindInFilesMessage();
}

void CGrep::MergeFullFileName(CString &strFullName, CString strDir, CString strFilename)
{
	strFullName = strDir + "\\";
	strFullName += strFilename;
}

BOOL CGrep::MatchFileName(CString strFileName ,CString strFilter)
{
	CString strMask;
	int iStart = 0,iNext;
	int nCount;
	while(iStart!= -1)
	{
		iNext = strFilter.Find(";",iStart);
		if (iNext == -1)
		{
			strMask = strFilter.Mid(iStart);
			iStart = iNext;
		}
		else
		{
			nCount = iNext - iStart;
			strMask = strFilter.Mid(iStart,nCount);
			iStart = iNext +1;
		}
//		if (FileNameCompare(strFileName, strMask) == TRUE)
//			return TRUE;
		if (FileNameCompare((LPCTSTR)strMask, strMask.GetLength(), (LPCTSTR)strFileName, strFileName.GetLength()) == TRUE)
			return TRUE;
	}
	return FALSE;
}
BOOL CGrep::FileNameCompare(LPCTSTR lpcszMask, int iMaskLen, LPCTSTR lpcszSrc, int iSrcLen)
{

	TCHAR cMask;

	if( iMaskLen == 0 )
		iMaskLen = strlen( lpcszMask );
	if( iSrcLen == 0 )
		iSrcLen = strlen( lpcszSrc );

    while( iMaskLen && iSrcLen )
	{
        if( (cMask = *lpcszMask) == '*' )
		{
            lpcszMask++;
			iMaskLen--;
			if( iMaskLen && *lpcszMask == '.' )
			{ //filename is match , skip to ext name
	            lpcszMask++;
			    iMaskLen--;

				//  Will Jump the Source ext name, so must find the last '.'
				LPCTSTR lpcszSaveSrc = NULL;
				int iSaveSrcLen = 0;
				while( iSrcLen )
				{
					if( *lpcszSrc == '.' )
					{
						lpcszSrc++;
						iSrcLen--;
						lpcszSaveSrc = lpcszSrc;
						iSaveSrcLen = iSrcLen;
//						break;
					}
					lpcszSrc++;
					iSrcLen--;
				}
				if (lpcszSaveSrc)
				{
					lpcszSrc = lpcszSaveSrc;
					iSrcLen = iSaveSrcLen ;
				}
				continue;
			}
            if( iMaskLen ) 
			{
                while( iSrcLen )
				{
					if( FileNameCompare( lpcszMask, iMaskLen, lpcszSrc++, iSrcLen-- ) )
                        return TRUE;
                }
                return FALSE;
            }
            return TRUE;
        }
		else if( cMask == '?' || cMask == *lpcszSrc )
		{
			;  // continue next char
		}
		else if( cMask >= 'A' && cMask <= 'Z' )
		{
			if( ((int)cMask - (int)*lpcszSrc) != ('A' - 'a') )
				return FALSE;
		}
		else if( cMask >= 'a' && cMask <= 'z' )
		{
			if( ((int)cMask - (int)*lpcszSrc) != ('a' - 'A') )
				return FALSE;
		}
		else
			return FALSE;
		
        iMaskLen--;
        lpcszMask++;
        iSrcLen--;
        lpcszSrc++;
    }

	// Add By Jami chen in 2003.12.04
	// "ABC." = "ABC"

	if( iMaskLen && *lpcszMask == '.' )
	{ //filename is match , skip to ext name
	    lpcszMask++;
		iMaskLen--;
	}
	if( !iSrcLen && *lpcszSrc == '.' )
	{ //filename is match , skip to ext name
	    lpcszSrc++;
		iSrcLen--;
	}
    if( !iMaskLen && !iSrcLen )
        return TRUE;

    if( !iMaskLen )
        return FALSE;
    else
	{
		while( iMaskLen-- ) 
		{
            if( *lpcszMask++ != '*' )
                return FALSE;
		}
    }
    return TRUE;
}


BOOL CGrep::LoadFile(CString strFileName)
{
	BOOL bRet;
	DWORD dwRead;
	bRet = m_GrepFile.Open(strFileName,CFile::modeRead);

	if (bRet )
	{
		if (m_lpFileData)
		{
			delete m_lpFileData;
			m_lpFileData = NULL;
			m_dwFileSize = 0;
			m_lpCurPos = NULL;
		}
		m_dwFileSize = m_GrepFile.GetLength();
		if (m_dwFileSize)
		{
			m_lpFileData = new char[m_dwFileSize];
			if (m_lpFileData)
			{
				dwRead = m_GrepFile.Read(m_lpFileData,m_dwFileSize);
				if (dwRead != m_dwFileSize)
				{
					delete m_lpFileData;
					m_lpFileData = NULL;
					m_dwFileSize = 0;
				}
			}
		}
		m_GrepFile.Close();
	}
	else
	{
		// 不能打开文件
		return FALSE;
	}
	m_lpCurPos = m_lpFileData;
	if (m_lpFileData)
		return TRUE;
	return FALSE;
}

BOOL CGrep::ReadLine(CString &line)
{
	line = "";
	if (m_lpCurPos == NULL)
		return FALSE;
	if ((DWORD)(m_lpCurPos - m_lpFileData) >= m_dwFileSize)
		return FALSE;
	while(1)
	{
		if ( *m_lpCurPos==0x0d)
		{
			m_lpCurPos++; // to the next line start
			if (*m_lpCurPos==0x0a)
			{
				m_lpCurPos++; // to the next line start
			}
			return TRUE;
		}
		if (*m_lpCurPos==0x0a)
		{
			m_lpCurPos++; // to the next line start
			return TRUE;
		}

		line += *m_lpCurPos;
		m_lpCurPos ++;
		if ((DWORD)(m_lpCurPos - m_lpFileData) >= m_dwFileSize)
			return TRUE;
	}
	return FALSE;
}

ULONG CALLBACK CGrep::GrepThreadProc( LPVOID lpParameter )
{
	CGrep *pGrep = (CGrep *)lpParameter;
	CString strTitle;

	strTitle.Format("Find string \'%s\' in \'%s\': \r\n",(LPCTSTR)pGrep->m_strSearch,(LPCTSTR)pGrep->m_strSearchFolder);
	pGrep->OutputString(strTitle);
	pGrep->GrepDir(pGrep->m_strSearchFolder);	

	if (pGrep->m_bBreak == TRUE)
	{
		strTitle.Format("Cancel Find string \'%s\'\r\n",(LPCTSTR)pGrep->m_strSearch);
		pGrep->OutputString(strTitle);
	}
	else
	{
		if (pGrep->m_nCount == 0)
		{
			strTitle.Format("Can't Found string \'%s\'\r\n",(LPCTSTR)pGrep->m_strSearch);
			pGrep->OutputString(strTitle);
		}
		else
		{
			strTitle.Format("Found %d occurrence(s)\r\n",pGrep->m_nCount);
			pGrep->OutputString(strTitle);
		}
	}
	pGrep->m_bBreak = FALSE;
	pGrep->m_bComplete = TRUE;
	return 0;
}

BOOL CGrep::IsComplete()
{
	return m_bComplete;
}

void CGrep::Break()
{
	m_bBreak = TRUE;
}

void CGrep::SetMatchWholeWordOnly(BOOL bMatchWholeWordOnly)
{
	m_bMatchWholeWord = bMatchWholeWordOnly;

}

BOOL CGrep::IsWholeWord(CString line, CString find, int iPos)
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
