// RecentStringList.h: interface for the CRecentStringList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RECENTSTRINGLIST_H__BC325BD2_ED5E_4AC9_A1FB_F9465EA472B4__INCLUDED_)
#define AFX_RECENTSTRINGLIST_H__BC325BD2_ED5E_4AC9_A1FB_F9465EA472B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CRecentStringList  
{
public:
	BOOL GetAt(CString &strString,int nIndex);
	int GetSize() const;
	CString& operator[](int nIndex);

	void Remove(int nIndex);
	void Add(CString strString);
	void ReadList();
	void WriteList();
	
	CRecentStringList(UINT nStart, LPCTSTR lpszSection,
		LPCTSTR lpszEntryFormat, int nSize,
		int nMaxDispLen = AFX_ABBREV_FILENAME_LEN);
	virtual ~CRecentStringList();

	int m_nSize;                // contents of the MRU list
	CString* m_arrNames;
	CString m_strSectionName;   // for saving
	CString m_strEntryFormat;
	UINT m_nStart;              // for displaying
	int m_nMaxDisplayLength;
	CString m_strOriginal;      // original menu item contents

};
AFX_INLINE int CRecentStringList::GetSize() const
	{ return m_nSize; }
AFX_INLINE CString& CRecentStringList::operator[](int nIndex)
	{ ASSERT(nIndex < m_nSize); return m_arrNames[nIndex]; }

#endif // !defined(AFX_RECENTSTRINGLIST_H__BC325BD2_ED5E_4AC9_A1FB_F9465EA472B4__INCLUDED_)

