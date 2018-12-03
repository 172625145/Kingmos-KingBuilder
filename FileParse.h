// FileParse.h: interface for the CFileParse class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEPARSE_H__83C254AA_7D2F_4090_AF74_ACD7B35EAECA__INCLUDED_)
#define AFX_FILEPARSE_H__83C254AA_7D2F_4090_AF74_ACD7B35EAECA__INCLUDED_

#include "srcData.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFileParse  
{
public:
	void SetFileName(CString strFileName);
	CFileParse();
	virtual ~CFileParse();
	BOOL Prase(HANDLE hFile,CsrcData *psrcData);

private:
	LPTSTR FilterSpace(LPTSTR lpLine);
	BOOL GetEnvironmentVariable(CString strLine,CString &strEnvironmentVariable,CString &strValue);
	HANDLE m_hFile;
	TCHAR *m_pBuffer;
	TCHAR *m_pCurPos;
	int m_nLinePos;
	CsrcData *m_psrcData;

	CString m_strKey;
	CString m_strValue;
	BOOL m_bNeedValue;

	CString m_strFileName;  

	int m_iEnterIfeqNum;

	BOOL ReadLine(CString& strLine);
	BOOL GetWord(CString& strLine,CString& strWord);
	BOOL PraseLine(CString& strLine);
	BOOL IsNeedKey(CString strKey);

};

#endif // !defined(AFX_FILEPARSE_H__83C254AA_7D2F_4090_AF74_ACD7B35EAECA__INCLUDED_)
