// srcData.h: interface for the CsrcData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SRCDATA_H__91BC867B_F1A4_43B0_A99F_63919C75E8BE__INCLUDED_)
#define AFX_SRCDATA_H__91BC867B_F1A4_43B0_A99F_63919C75E8BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CsrcData  
{
public:
	CMapStringToString & StringMap () { return m_mssExp; };
	BOOL Assign (CsrcData &src);
	void RemoveAll()
	{ m_mssExp.RemoveAll (); };
	BOOL Load (HANDLE hFile,CString strFileName);
	BOOL Load(LPCTSTR lpstrSrc);
	BOOL Get (LPCTSTR lpstrKey, CString &csVal);
	BOOL Set (LPCSTR lpstrKey, LPCSTR lpstrVal);
	CsrcData();
	virtual ~CsrcData();

protected:
	CMapStringToString m_mssExp;
};

#endif // !defined(AFX_SRCDATA_H__91BC867B_F1A4_43B0_A99F_63919C75E8BE__INCLUDED_)
