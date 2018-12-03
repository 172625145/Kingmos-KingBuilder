// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1998 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#include "myFilelist.h"

#ifdef AFX_CORE1_SEG
#pragma code_seg(AFX_CORE1_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyRecentFileList

CMyRecentFileList::CMyRecentFileList(UINT nStart, LPCTSTR lpszSection,
						LPCTSTR lpszEntryFormat, int nSize, int nMaxDispLen):CRecentFileList(nStart, lpszSection,lpszEntryFormat, nSize, nMaxDispLen)
{
/*	ASSERT(nSize != 0);
	m_arrNames = new CString[nSize];
	m_nSize = nSize;

	m_nStart = nStart;
	m_strSectionName = lpszSection;
	m_strEntryFormat = lpszEntryFormat;
	m_nMaxDisplayLength = nMaxDispLen;
	*/
}

CMyRecentFileList::~CMyRecentFileList()
{
//	delete[] m_arrNames;
}

// Operations
void CMyRecentFileList::Add(LPCTSTR lpszPathName)
{
	CRecentFileList::Add(lpszPathName);
}

void CMyRecentFileList::Remove(int nIndex)
{
	CRecentFileList::Remove(nIndex);
}

void CMyRecentFileList::UpdateMenu(CCmdUI* pCmdUI)
{
	ASSERT(m_arrNames != NULL);

	UINT nIndex = 0;

	if (pCmdUI->m_pMenu == NULL)
		return ;
	CMenu* pMenu = pCmdUI->m_pMenu->GetSubMenu(pCmdUI->m_nIndex);

	if (m_strOriginal.IsEmpty() && pCmdUI->m_pMenu != NULL)
		pCmdUI->m_pMenu->GetMenuString(pCmdUI->m_nID, m_strOriginal, MF_BYCOMMAND);

	if (m_arrNames[0].IsEmpty())
	{
		// no MRU files
		if (!m_strOriginal.IsEmpty())
			pCmdUI->SetText(m_strOriginal);
		pCmdUI->Enable(FALSE);
		return;
	}

	if (pMenu == NULL)
		return;

	for (int iMRU = 0; iMRU < m_nSize; iMRU++)
	{
		pMenu->DeleteMenu(0, MF_BYPOSITION);
	}

	nIndex = 0;

	TCHAR szCurDir[_MAX_PATH];
	GetCurrentDirectory(_MAX_PATH, szCurDir);
	int nCurDir = lstrlen(szCurDir);
	ASSERT(nCurDir >= 0);
	szCurDir[nCurDir] = '\\';
	szCurDir[++nCurDir] = '\0';

	CString strName;
	CString strTemp;
	for (iMRU = 0; iMRU < m_nSize; iMRU++)
	{
		if (!GetDisplayName(strName, iMRU, szCurDir, nCurDir))
			break;

		// double up any '&' characters so they are not underlined
		LPCTSTR lpszSrc = strName;
		LPTSTR lpszDest = strTemp.GetBuffer(strName.GetLength()*2);
		while (*lpszSrc != 0)
		{
			if (*lpszSrc == '&')
				*lpszDest++ = '&';
			if (_istlead(*lpszSrc))
				*lpszDest++ = *lpszSrc++;
			*lpszDest++ = *lpszSrc++;
		}
		*lpszDest = 0;
		strTemp.ReleaseBuffer();

		// insert mnemonic + the file name
		TCHAR buf[10];
//		wsprintf(buf, _T("&%d "), (iMRU+1+m_nStart) % 10);
		wsprintf(buf, _T("&%d "), (iMRU+1+m_nStart) % 100);
		pMenu->InsertMenu(nIndex++,
			MF_STRING | MF_BYPOSITION, pCmdUI->m_nID++,
			CString(buf) + strTemp);
	}

	// update end menu count
//	pCmdUI->m_nIndex--; // point to last menu added
	pCmdUI->m_nIndexMax = pCmdUI->m_pMenu->GetMenuItemCount();

	pCmdUI->m_bEnableChanged = TRUE;    // all the added items are enabled
}

void CMyRecentFileList::WriteList()
{
	CRecentFileList::WriteList();
}

void CMyRecentFileList::ReadList()
{
	CRecentFileList::ReadList();
}

/////////////////////////////////////////////////////////////////////////////
