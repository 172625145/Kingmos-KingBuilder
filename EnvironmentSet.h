// EnvironmentSet.h: interface for the CEnvironmentSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENVIRONMENTSET_H__689FAF57_8D8D_4CF9_AA1A_90B525DB2341__INCLUDED_)
#define AFX_ENVIRONMENTSET_H__689FAF57_8D8D_4CF9_AA1A_90B525DB2341__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct {
	CString strVarName;
	CString strValue;
}ENVIRONMENTVAR,*LPENVIRONMENTVAR;

class CEnvironmentSet  
{
public:
	void Clear();
	BOOL GetEnvironmentValue(CString strEnvironment,CString &strValue);
	int GetWord(CString line, int nStart, CString &strWord);
	int LoadEnvironment(CString strFileName);
	int LoadFile(CString strFileName);
	CEnvironmentSet();
	virtual ~CEnvironmentSet();

private:
	BOOL ReadLine(CString &line);
	CFile m_ModuleFile;
	LPTSTR m_lpFileData;
	LPTSTR m_lpCurPos;
	DWORD m_dwFileSize;
	CPtrArray m_aEnvironmentArray;
};

#endif // !defined(AFX_ENVIRONMENTSET_H__689FAF57_8D8D_4CF9_AA1A_90B525DB2341__INCLUDED_)
