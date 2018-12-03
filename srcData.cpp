// srcData.cpp: implementation of the CsrcData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "amboapp.h"
#include "srcData.h"
#include "FileParse.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CsrcData::CsrcData()
{

}

CsrcData::~CsrcData()
{

}

BOOL CsrcData::Set(LPCSTR lpstrKey, LPCSTR lpstrVal)
{
	CString csKeyVal;
	CString csKey(lpstrKey);
	CString csVal(lpstrVal);

	csKey.MakeLower();

	BOOL bRet = m_mssExp.Lookup (csKey, csKeyVal);

	m_mssExp.SetAt (csKey, csVal);

	return bRet;

}

BOOL CsrcData::Get(LPCTSTR lpstrKey, CString &csVal)
{
//	POSITION rNextPosition;

	CString csKey(lpstrKey);
	csKey.MakeLower();
	BOOL bRet = m_mssExp.Lookup (csKey, csVal);
	return bRet;
}

BOOL CsrcData::Load(LPCTSTR lpstrSrc)
{
	ASSERT (lpstrSrc != NULL);

//	FILE *pFile = fopen (lpstrSrc, TEXT("r"));
	HANDLE hFile = CreateFile(
					lpstrSrc,                         // file name
					GENERIC_READ,                      // access mode
					FILE_SHARE_READ,                          // share mode
					NULL, // SD
					OPEN_EXISTING,                // how to create
					FILE_ATTRIBUTE_ARCHIVE,                 // file attributes
					NULL// handle to template file
					);	
	if (hFile == INVALID_HANDLE_VALUE)
	{
		//CString csMsg;
		//csMsg.Format (TEXT("Open Sources File : %s Failure!"), lpstrSrc);

		//AfxMessageBox (csMsg);
		return FALSE;
	}
	
	BOOL bRet = Load(hFile,lpstrSrc);     // load me
	if (! bRet)
	{
//		CString csMsg;
//		csMsg.Format (TEXT("Parse Sources File : %s Error!"), lpstrSrc);

//		AfxMessageBox (csMsg);
	}

	//fclose (pFile);
	CloseHandle(hFile);
	
	return bRet;
}

BOOL CsrcData::Load(HANDLE hFile,CString strFileName)
{
/*	ASSERT (pFile != NULL);

extern FILE *amboin;
extern int amboparse(void *);

	amboin = pFile;
	if (amboparse (& StringMap ()) == -1)
		return FALSE;
*/
	CFileParse fileParse;
	fileParse.SetFileName(strFileName);
	BOOL bRet = fileParse.Prase(hFile,this);
	return bRet; 
}

BOOL CsrcData::Assign(CsrcData &src)
{
	POSITION pos;
	CString csKey, csVal;
	
	for( pos = src.m_mssExp.GetStartPosition (); pos != NULL; )
	{
		src.m_mssExp.GetNextAssoc (pos, csKey, csVal);

		m_mssExp[(LPCTSTR)csKey] = csVal;
	}
	
	return TRUE;
}

