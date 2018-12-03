// RecentStringList.cpp: implementation of the CRecentStringList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ambo.h"
#include "RecentStringList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRecentStringList::CRecentStringList(UINT nStart, LPCTSTR lpszSection,
	LPCTSTR lpszEntryFormat, int nSize, int nMaxDispLen)
{
	ASSERT(nSize != 0);
	m_arrNames = new CString[nSize];
	m_nSize = nSize;

	m_nStart = nStart;
	m_strSectionName = lpszSection;
	m_strEntryFormat = lpszEntryFormat;
	m_nMaxDisplayLength = nMaxDispLen;
}

CRecentStringList::~CRecentStringList()
{
	delete[] m_arrNames;
}

void CRecentStringList::WriteList()
{
	ASSERT(m_arrNames != NULL);
	ASSERT(!m_strSectionName.IsEmpty());
	ASSERT(!m_strEntryFormat.IsEmpty());
	LPTSTR pszEntry = new TCHAR[m_strEntryFormat.GetLength()+5];
	CWinApp* pApp = AfxGetApp();
	pApp->WriteProfileString(m_strSectionName, NULL, NULL);
	for (int iMRU = 0; iMRU < m_nSize; iMRU++)
	{
		wsprintf(pszEntry, m_strEntryFormat, iMRU + 1);
		if (!m_arrNames[iMRU].IsEmpty())
		{
			pApp->WriteProfileString((LPCTSTR)m_strSectionName, pszEntry,
				(LPCTSTR)m_arrNames[iMRU]);
		}
	}
	delete[] pszEntry;
}

void CRecentStringList::ReadList()
{
	ASSERT(m_arrNames != NULL);
	ASSERT(!m_strSectionName.IsEmpty());
	ASSERT(!m_strEntryFormat.IsEmpty());
	LPTSTR pszEntry = new TCHAR[m_strEntryFormat.GetLength()+5];
	CWinApp* pApp = AfxGetApp();
	for (int iMRU = 0; iMRU < m_nSize; iMRU++)
	{
		wsprintf(pszEntry, m_strEntryFormat, iMRU + 1);
		m_arrNames[iMRU] = pApp->GetProfileString(
			m_strSectionName, pszEntry, &afxChNil);
	}
	delete[] pszEntry;
}

void CRecentStringList::Add(CString strString)
{
	ASSERT(m_arrNames != NULL);
	ASSERT(strString.IsEmpty() != TRUE);

	// update the MRU list, if an existing MRU string matches file name
	for (int iMRU = 0; iMRU < m_nSize-1; iMRU++)
	{
		if (m_arrNames[iMRU].Compare(strString) == 0)
			break;      // iMRU will point to matching entry
	}
	// move MRU strings before this one down
	for (; iMRU > 0; iMRU--)
	{
		ASSERT(iMRU > 0);
		ASSERT(iMRU < m_nSize);
		m_arrNames[iMRU] = m_arrNames[iMRU-1];
	}
	// place this one at the beginning
	m_arrNames[0] = strString;
}

void CRecentStringList::Remove(int nIndex)
{
	ASSERT(nIndex >= 0);
	ASSERT(nIndex < m_nSize);

	m_arrNames[nIndex].Empty();
	for (int iMRU = nIndex; iMRU < m_nSize-1; iMRU++)
		m_arrNames[iMRU] = m_arrNames[iMRU+1];

	ASSERT(iMRU < m_nSize);
	m_arrNames[iMRU].Empty();
}

BOOL CRecentStringList::GetAt(CString &strString, int nIndex)
{
	ASSERT(m_arrNames != NULL);
	ASSERT(nIndex < m_nSize);
	if (m_arrNames[nIndex].IsEmpty())
		return FALSE;

	strString = m_arrNames[nIndex];
	return TRUE;
}
