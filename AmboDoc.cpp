// AmboDoc.cpp : implementation of the CAmboDoc class
//

#include "stdafx.h"
#include "Ambo.h"
#include "MainFrame.h"

#include "AmboDoc.h"
#include "SpecialView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

AFX_STATIC UINT AfxGetFileName(LPCTSTR lpszPathName, LPTSTR lpszTitle, UINT nMax)
{
	ASSERT(lpszTitle == NULL ||
		AfxIsValidAddress(lpszTitle, _MAX_FNAME));
	ASSERT(AfxIsValidString(lpszPathName));

	// always capture the complete file name including extension (if present)
	LPTSTR lpszTemp = (LPTSTR)lpszPathName;
	for (LPCTSTR lpsz = lpszPathName; *lpsz != '\0'; lpsz = _tcsinc(lpsz))
	{
		// remember last directory/drive separator
		if (*lpsz == '\\' || *lpsz == '/' || *lpsz == ':')
			lpszTemp = (LPTSTR)_tcsinc(lpsz);
	}

	// lpszTitle can be NULL which just returns the number of bytes
	if (lpszTitle == NULL)
		return lstrlen(lpszTemp)+1;

	// otherwise copy it into the buffer provided
	lstrcpyn(lpszTitle, lpszTemp, nMax);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// lpszCanon = C:\MYAPP\DEBUGS\C\TESWIN.C
//
// cchMax   b   Result
// ------   -   ---------
//  1- 7    F   <empty>
//  1- 7    T   TESWIN.C
//  8-14    x   TESWIN.C
// 15-16    x   C:\...\TESWIN.C
// 17-23    x   C:\...\C\TESWIN.C
// 24-25    x   C:\...\DEBUGS\C\TESWIN.C
// 26+      x   C:\MYAPP\DEBUGS\C\TESWIN.C

static void AFXAPI _AfxAbbreviateName(LPTSTR lpszCanon, int cchMax, BOOL bAtLeastName)
{
	int cchFullPath, cchFileName, cchVolName;
	const TCHAR* lpszCur;
	const TCHAR* lpszBase;
	const TCHAR* lpszFileName;

	lpszBase = lpszCanon;
	cchFullPath = lstrlen(lpszCanon);

	cchFileName = AfxGetFileName(lpszCanon, NULL, 0) - 1;
	lpszFileName = lpszBase + (cchFullPath-cchFileName);
//	AfxGetFileTitle(lpszCanon, lpszFileName, _MAX_FNAME);

	// If cchMax is more than enough to hold the full path name, we're done.
	// This is probably a pretty common case, so we'll put it first.
	if (cchMax >= cchFullPath)
		return;

	// If cchMax isn't enough to hold at least the basename, we're done
	if (cchMax < cchFileName)
	{
		lstrcpy(lpszCanon, (bAtLeastName) ? lpszFileName : &afxChNil);
		return;
	}

	// Calculate the length of the volume name.  Normally, this is two characters
	// (e.g., "C:", "D:", etc.), but for a UNC name, it could be more (e.g.,
	// "\\server\share").
	//
	// If cchMax isn't enough to hold at least <volume_name>\...\<base_name>, the
	// result is the base filename.

	lpszCur = lpszBase + 2;                 // Skip "C:" or leading "\\"

	if (lpszBase[0] == '\\' && lpszBase[1] == '\\') // UNC pathname
	{
		// First skip to the '\' between the server name and the share name,
		while (*lpszCur != '\\')
		{
			lpszCur = _tcsinc(lpszCur);
			ASSERT(*lpszCur != '\0');
		}
	}
	// if a UNC get the share name, if a drive get at least one directory
	ASSERT(*lpszCur == '\\');
	// make sure there is another directory, not just c:\filename.ext
	if (cchFullPath - cchFileName > 3)
	{
		lpszCur = _tcsinc(lpszCur);
		while (*lpszCur != '\\')
		{
			lpszCur = _tcsinc(lpszCur);
			ASSERT(*lpszCur != '\0');
		}
	}
	ASSERT(*lpszCur == '\\');

	cchVolName = lpszCur - lpszBase;
	if (cchMax < cchVolName + 5 + cchFileName)
	{
		lstrcpy(lpszCanon, lpszFileName);
		return;
	}

	// Now loop through the remaining directory components until something
	// of the form <volume_name>\...\<one_or_more_dirs>\<base_name> fits.
	//
	// Assert that the whole filename doesn't fit -- this should have been
	// handled earlier.

	ASSERT(cchVolName + (int)lstrlen(lpszCur) > cchMax);
	while (cchVolName + 4 + (int)lstrlen(lpszCur) > cchMax)
	{
		do
		{
			lpszCur = _tcsinc(lpszCur);
			ASSERT(*lpszCur != '\0');
		}
		while (*lpszCur != '\\');
	}

	// Form the resultant string and we're done.
	lpszCanon[cchVolName] = '\0';
	lstrcat(lpszCanon, _T("\\..."));
	lstrcat(lpszCanon, lpszCur);
}
/////////////////////////////////////////////////////////////////////////////
// CAmboDoc

IMPLEMENT_DYNCREATE(CAmboDoc, CDocument)

BEGIN_MESSAGE_MAP(CAmboDoc, CDocument)
	//{{AFX_MSG_MAP(CAmboDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAmboDoc construction/destruction

CAmboDoc::CAmboDoc()
{
	// TODO: add one-time construction code here

}

CAmboDoc::~CAmboDoc()
{
}

BOOL CAmboDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CAmboDoc serialization

void CAmboDoc::Serialize(CArchive& ar)
{
/*	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
	*/
//	((CSpecialView *)m_viewList.GetHead())->SerializeRaw(ar);
	((CSpecialView *)m_viewList.GetTail())->SerializeRaw(ar);

}

/////////////////////////////////////////////////////////////////////////////
// CAmboDoc diagnostics

#ifdef _DEBUG
void CAmboDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAmboDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAmboDoc commands

void CAmboDoc::OnCloseDocument() 
{
	CString strFileName;
	// TODO: Add your specialized code here and/or call the base class
	POSITION pos;
	CSpecialView *pView;

	pos = GetFirstViewPosition();
	if (pos == NULL)
		return;
	pView = (CSpecialView*)GetNextView(pos); // get first one
	if (pView == NULL)
		return;
	
	if (pView->h_Edit->GetModify())
	{
		TCHAR szFormat[128];
		TCHAR szShow[MAX_PATH+128];
		TCHAR szFile[MAX_PATH];

		LoadString( GetModuleHandle(NULL), IDS_SAVE_CHANGE, szFormat, sizeof( szFormat ) );
		szFile[0] = 0;
		pView->GetWindowText(szFile, sizeof(szFile) );
		szShow[0] = 0;
		sprintf( szShow, szFormat, szFile );
		UINT idResult = AfxMessageBox(szShow,MB_YESNOCANCEL);
		if ( idResult == IDYES)
			CDocument::OnFileSave();
		else if ( idResult == IDCANCEL)
			return ;
	}
	
	CDocument::OnCloseDocument();
}

BOOL CAmboDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: Add your specialized code here and/or call the base class
	BOOL bResult;
	
	bResult = CDocument::OnSaveDocument(lpszPathName);
	if (bResult)
	{
		((CSpecialView *)m_viewList.GetTail())->h_Edit->SetModify(FALSE);
		// 如果是LISTS 或者 MODULES 文件， 则重新刷新工程文件
		CMainFrame* pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
		pMainFrame->m_workBar.RefreshModules(lpszPathName);
		UpdateFileAttrib();
	}

	return bResult;

}

BOOL CAmboDoc::IsModified()
{ 
//	ASSERT(this != NULL); 
//	return ((CSpecialView *)m_viewList.GetTail())->h_Edit->GetModify();
	return CDocument::IsModified();
}
void CAmboDoc::SetModifiedFlag(BOOL bModified)
{ 
//	ASSERT(this != NULL); 
//	((CSpecialView *)m_viewList.GetTail())->h_Edit->SetModify(bModified);
	if (IsModified() == bModified)
		return ;
	CDocument::SetModifiedFlag(bModified);
	CString strTitle;
	strTitle = GetTitle();
	//GetDocString(strTitle, CDocTemplate::docName);
	SetTitle(strTitle);
}
void CAmboDoc::SetTitle(LPCTSTR lpszTitle)
{
	CString strNewTitle(lpszTitle);
	int iIndex;

	iIndex = strNewTitle.Find(" *");
	if (iIndex != -1)
		strNewTitle.Delete(iIndex,2);
	if (IsModified())
	{
//		CString strNewTitle(lpszTitle);

			strNewTitle += " *";
//			CDocument::SetTitle(strNewTitle);
//			return ;
	}
	CDocument::SetTitle(strNewTitle);
}

POSITION CAmboDoc::GetFirstViewPosition() const
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDocument::GetFirstViewPosition();
}

CView* CAmboDoc::GetNextView(POSITION& rPosition) const
{
	// TODO: Add your specialized code here and/or call the base class
	
//	return (CView*)m_viewList.GetPrev( rPosition );
	return CDocument::GetNextView(rPosition);
}

BOOL CAmboDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Add your specialized creation code here
	
	return TRUE;
}

void CAmboDoc::SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDocument::SetPathName(lpszPathName, bAddToMRU);
	CString strTitle;
//	int iMaxTitleLen;
	strTitle = m_strPathName;
	LPTSTR lpch = strTitle.GetBuffer(_MAX_PATH);
	_AfxAbbreviateName(lpch,64,TRUE);
	strTitle.ReleaseBuffer();
	SetTitle(strTitle);
	UpdateFileAttrib();
}


BOOL CAmboDoc::CanCloseFrame(CFrameWnd* pFrame) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return TRUE;
//	return CDocument::CanCloseFrame(pFrame);
}

void CAmboDoc::CheckFileChanged( void )
{
	if( IsFileChanged() )
	{
		CString strFile = GetPathName();
		CString strFormat = "源文件:" + strFile + "\r\n已经改变,是否更新?。";
		//strFormat += strFile;
		//strFormat += "\r\n";
		if( IDOK == MessageBox( AfxGetMainWnd()->GetSafeHwnd(), strFormat, "文件改变", MB_OKCANCEL ) )
		{
			SetModifiedFlag(FALSE);
			OnOpenDocument(strFile);
			UpdateAllViews(NULL);			
		}
		UpdateFileAttrib();
	}
}

void CAmboDoc::UpdateFileAttrib( void )
{
	CString strFile = GetPathName();
	WIN32_FIND_DATA wfd;

	HANDLE hFind = FindFirstFile( strFile, &wfd );
	if( hFind != INVALID_HANDLE_VALUE )
	{
		FindClose( hFind );
		m_ftLastWriteTime = wfd.ftLastWriteTime; 
		m_fileSizeLow = wfd.nFileSizeLow;
		m_fileSizeHigh = wfd.nFileSizeHigh;
	}
}

BOOL CAmboDoc::IsFileChanged( void )
{
	CString strFile = GetPathName();
	WIN32_FIND_DATA wfd;

	HANDLE hFind = FindFirstFile( strFile, &wfd );
	if( hFind != INVALID_HANDLE_VALUE )
	{
		FindClose( hFind );
		if( m_ftLastWriteTime.dwHighDateTime != wfd.ftLastWriteTime.dwHighDateTime ||
			m_ftLastWriteTime.dwLowDateTime != wfd.ftLastWriteTime.dwLowDateTime || 
			m_fileSizeLow != wfd.nFileSizeLow || 
			m_fileSizeHigh != wfd.nFileSizeHigh )
			return TRUE;
	}
	return FALSE;
}


