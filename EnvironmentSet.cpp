// EnvironmentSet.cpp: implementation of the CEnvironmentSet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ambo.h"
#include "EnvironmentSet.h"
#include "Infomation.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEnvironmentSet::CEnvironmentSet()
{
	m_lpFileData = NULL;
	m_lpCurPos = NULL;
	m_dwFileSize = 0;
}

CEnvironmentSet::~CEnvironmentSet()
{

	if (m_lpFileData)
		delete m_lpFileData;

	m_lpFileData = NULL;

	// 删除m_aEnvironmentArray
	LPENVIRONMENTVAR lpEnvironment;
	int iSize;
	iSize = m_aEnvironmentArray.GetSize();
	while(iSize)
	{
		lpEnvironment = (LPENVIRONMENTVAR)m_aEnvironmentArray.GetAt(0);
		if (lpEnvironment)
		{
			delete lpEnvironment;
		}
		m_aEnvironmentArray.RemoveAt(0);
		iSize--;
	}
}

int CEnvironmentSet::LoadFile(CString strFileName)
{
	BOOL bRet;
	DWORD dwRead;
	bRet = m_ModuleFile.Open(strFileName,CFile::modeRead);

	if (bRet )
	{
		bRet = FALSE;
		m_dwFileSize = m_ModuleFile.GetLength();
		if (m_dwFileSize)
		{
			m_lpFileData = new char[m_dwFileSize];
			if (m_lpFileData)
			{
				bRet = TRUE;
				dwRead = m_ModuleFile.Read(m_lpFileData,m_dwFileSize);
				if (dwRead != m_dwFileSize)
				{
					delete m_lpFileData;
					m_lpFileData = NULL;
					m_dwFileSize = 0;
					bRet = FALSE;
				}
			}
		}
		m_ModuleFile.Close();
	}
	m_lpCurPos = m_lpFileData;
	return bRet;
}

int CEnvironmentSet::LoadEnvironment(CString strFileName)
{
	BOOL bRet;
	CString line;
	CString strWord;
	int nStart = 0;
	LPENVIRONMENTVAR lpEnvironment;
	bRet = LoadFile(strFileName); // 装载文件
	if (bRet == FALSE)
		return bRet;

	while(1)
	{
		if (ReadLine(line) == FALSE) // 读一行文本
			break;
		line.TrimLeft("\t "); // 过滤掉最前面的空格和TAB
		if (line.IsEmpty())
			continue;  // 是一个空行

		if (line.GetAt(0) == '#')
		{
			// 这行文本是注释，跳过。
			continue;
		}
		nStart = 0;

		nStart = GetWord(line,nStart,strWord);  // 在这里得到关键字“SET”
		if (nStart == -1)
			goto SET_ERROR;
		if (strWord.CompareNoCase(OUTPUTTEXT) != 0)
		{
			goto SET_ERROR;
		}

		nStart = GetWord(line,nStart,strWord);  // 在这里得到变量名
		if (nStart == -1)
			goto SET_ERROR;

		lpEnvironment = new ENVIRONMENTVAR;
		if (lpEnvironment == NULL)
			goto SET_ERROR;
		lpEnvironment->strVarName = strWord;

		nStart = GetWord(line,nStart,strWord);  // 在这里得到关键字“=”
		if (nStart == -1)
			goto SET_ERROR;
		if (strWord.CompareNoCase("=") != 0)
		{
			goto SET_ERROR;
		}

		nStart = GetWord(line,nStart,strWord);  // 在这里得到变量值
		if (nStart == -1)
			lpEnvironment->strValue = "";

		lpEnvironment->strValue = strWord;
		m_aEnvironmentArray.Add(lpEnvironment);
	}
	goto SET_SUCCESS;

SET_ERROR:
	AfxMessageBox(line);
	bRet = FALSE;
SET_SUCCESS:
	if (m_lpFileData)
		delete m_lpFileData;

	m_lpFileData = NULL;

	return bRet;
}

int CEnvironmentSet::GetWord(CString line, int nStart, CString &strWord)
{
	LPTSTR lpCurPos;
	strWord = "";

	if (line.GetLength() <= nStart)
		return -1;
	lpCurPos = line.LockBuffer();
	lpCurPos += nStart;

	while(*lpCurPos == ' ' || *lpCurPos == '\t')
	{
		lpCurPos ++;
		nStart++;
	}
	if (*lpCurPos == '=')
	{
		strWord += *lpCurPos;
		lpCurPos++;
		nStart++;
		return nStart;
	}
	if (*lpCurPos == 0)
		return -1;
	if (*lpCurPos == '"')
	{
		lpCurPos ++;
		nStart++;
		while(*lpCurPos)
		{
			strWord += *lpCurPos;
			lpCurPos++;
			nStart++;
			if ( *lpCurPos == '"')
				break;
		}
		if ( *lpCurPos != '"')
			return -1;
		lpCurPos ++;
		nStart++;
	}
	else
	{
		while(*lpCurPos)
		{
			strWord += *lpCurPos;
			lpCurPos++;
			nStart++;
			if ( *lpCurPos == ' ' || *lpCurPos == '\t' || *lpCurPos == '=')
				break;
		}
	}
	line.UnlockBuffer();
	return nStart;
}

BOOL CEnvironmentSet::GetEnvironmentValue(CString strEnvironment, CString &strValue)
{
	LPENVIRONMENTVAR lpEnvironment;
	int iSize;
	int nIndex;

	if (strEnvironment.IsEmpty())
		return FALSE;
	iSize = m_aEnvironmentArray.GetSize();
	nIndex = 0;
	while(nIndex < iSize)
	{
		lpEnvironment = (LPENVIRONMENTVAR)m_aEnvironmentArray.GetAt(nIndex);
		if (lpEnvironment)
		{
			if (lpEnvironment->strVarName.CompareNoCase(strEnvironment) == 0)
			{
				strValue = lpEnvironment->strValue;
				return TRUE;
			}
		}
		nIndex ++;
	}
	return FALSE;
}

BOOL CEnvironmentSet::ReadLine(CString &line)
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

		line += *m_lpCurPos;
		m_lpCurPos ++;
		if ((DWORD)(m_lpCurPos - m_lpFileData) >= m_dwFileSize)
			return TRUE;
	}
}



void CEnvironmentSet::Clear()
{
	LPENVIRONMENTVAR lpEnvironment;
	int iSize;
	iSize = m_aEnvironmentArray.GetSize();
	while(iSize)
	{
		lpEnvironment = (LPENVIRONMENTVAR)m_aEnvironmentArray.GetAt(0);
		if (lpEnvironment)
		{
			delete lpEnvironment;
		}
		m_aEnvironmentArray.RemoveAt(0);
		iSize--;
	}
}
