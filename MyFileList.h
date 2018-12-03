#ifndef __MYFILELIST_H__
#define __MYFILELIST_H__

#ifndef __AFXADV_H__
	#include <afxadv.h>
#endif

class CMyRecentFileList:public CRecentFileList
{
// Constructors
public:
	CMyRecentFileList(UINT nStart, LPCTSTR lpszSection,
		LPCTSTR lpszEntryFormat, int nSize,
		int nMaxDispLen = AFX_ABBREV_FILENAME_LEN);

// Operations
	virtual void Remove(int nIndex);
	virtual void Add(LPCTSTR lpszPathName);
	virtual void UpdateMenu(CCmdUI* pCmdUI);
	virtual void ReadList();    // reads from registry or ini file
	virtual void WriteList();   // writes to registry or ini file

// Implementation
	virtual ~CMyRecentFileList();

};

#endif // __MYFILELIST_H__
