// FileParse.cpp: implementation of the CFileParse class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ambo.h"
#include "FileParse.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileParse::CFileParse()
{
	m_hFile = INVALID_HANDLE_VALUE;
	m_pBuffer = NULL;
	m_pCurPos = NULL;

	m_strKey = _T("");
	m_strValue = _T("");
	m_bNeedValue = FALSE;

	m_iEnterIfeqNum = 0;

	m_strFileName = _T("");
}

CFileParse::~CFileParse()
{
	if (m_pBuffer)
		delete m_pBuffer;
}

BOOL CFileParse::Prase(HANDLE hFile,CsrcData *psrcData)
{
	DWORD  dwFileSize;
	DWORD dwRead;

	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;
	m_hFile = hFile;
	m_psrcData = psrcData;

	dwFileSize = GetFileSize (hFile, NULL) ; 
	if (m_pBuffer)
		delete m_pBuffer;

	m_pBuffer = new TCHAR[(dwFileSize + 1 ) * sizeof(TCHAR)];
	if (m_pBuffer == NULL )
		return FALSE;

	ReadFile(hFile,m_pBuffer,dwFileSize,&dwRead,NULL);
	if (dwRead != dwFileSize)
		return FALSE;

	m_pBuffer[dwFileSize] = 0;
	m_pCurPos = m_pBuffer;

	while(1)
	{
		CString strLine;
		if (ReadLine(strLine) == FALSE)
			break;
		if (PraseLine(strLine) == FALSE)
		{
			CString strMsg;
			strMsg.Format("File: %s \r\n\r\nLine: \"%s\" have Some Error!!!",m_strFileName,strLine);
			((CAmboApp *)AfxGetApp())->OpenDocumentFile(m_strFileName);
			AfxMessageBox(strMsg,MB_OK);
			return FALSE;
		}
	}

	hFile = INVALID_HANDLE_VALUE;
	return TRUE;
}

BOOL CFileParse::ReadLine(CString& strLine)
{
	LPTSTR pNewPos;
	UINT nLineLength = 0;
	
		pNewPos = m_pCurPos;
		while(1)
		{
			if (*pNewPos == 0)
			{
				if (nLineLength == 0)
					return FALSE;
				break; // 我们还要处理最后一行
			}
			if (*pNewPos == '\r' && *(pNewPos+1) == '\n')
			{  // 在这里回车符是0x0d,0x0a两个字符
				pNewPos +=2;
				break;
			}
			if (*pNewPos == '\r' || *pNewPos == '\n')
			{  // 在这里回车符是0x0d或者0x0a一个字符
				pNewPos +=1;
				break;
			}
			nLineLength ++;
			pNewPos ++;
		}
		CString strNewLine( m_pCurPos, nLineLength);
		strLine = strNewLine;
		m_pCurPos = pNewPos;
		return TRUE;
}

BOOL CFileParse::GetWord(CString& strLine,CString& strWord)
{
	UINT nWordLength = 0;
	LPTSTR lpLine;

		lpLine = strLine.LockBuffer(); // 字符串锁定
		lpLine += m_nLinePos;

		// 在这里要将当前位置后的空格过滤掉
		while (*lpLine)
		{
			if ( *lpLine == ' ' || *lpLine == '\t')
			{
				m_nLinePos ++;
				lpLine ++;
				continue;
			}
			break;
		}
		// 在这里得到该单词的长度
		while (1)
		{
			if ( *lpLine == 0x00)
			{ // 行结束，一个单词同时也结束
				break;
			}
			if ( *lpLine == '#')
			{ // 后面的字符都是注释，行结束，一个单词同时也结束
				if (nWordLength == 0) // 如果行连接符是第一个字符，则将行连接符作为一个单词
					nWordLength ++;
				break;
			}
			if ( *lpLine == ' ' || *lpLine == '\t')
			{ // 碰到空格，表示一个单词已经结束
				break;
			}
			if ( *lpLine == '\\')
			{ // 碰到行连接符，也表示一个单词已经结束
//				break;
				//如果连接符后面没有有效符号，则这是一个连接符，否则是目录分隔符
				if ( *(lpLine+1) == 0x00)
				{ // 行结束，一个单词同时也结束
					if (nWordLength == 0) // 如果行连接符是第一个字符，则将行连接符作为一个单词
						nWordLength ++;
					break;
				}
				if ( *(lpLine+1) == ' ' || *(lpLine+1) == '\t')
				{ // 碰到空格，表示一个单词已经结束
					if (nWordLength == 0) // 如果行连接符是第一个字符，则将行连接符作为一个单词
						nWordLength ++;
					//错误，连接符后面不可以有任何字符。
					break;
				}
			}

			if ( *lpLine == '=')
			{ // 碰到等于号，也表示一个单词已经结束
				if (nWordLength == 0) // 如果等于号是第一个字符，则将等于号作为一个单词
					nWordLength ++;
				break;
			}

			lpLine ++;
			nWordLength ++;
		}

		if (nWordLength == 0)
			return FALSE;
		strLine.UnlockBuffer(); // 解除字符串锁定
		strWord = strLine.Mid (m_nLinePos, nWordLength);
		m_nLinePos += nWordLength;

		return TRUE;
}
/*
BOOL CFileParse::PraseLine(CString& strLine)
{
	CString strKey(""),strValue("");
	CString strEqual,strSingleValue;
	int iEnterIf = 0;
	int iNeedLink = 1;

	m_nLinePos = 0;
	
	// 得到关键字
	if (GetWord(strLine,strKey) == FALSE)
	{
		return FALSE;
	}
	// 判断关键字是否需要
	if (IsNeedKey(strKey) == FALSE)
		return FALSE;

	if (strKey == "ifeq")
	{
		// 这里是条件语句
		CString strEnvironmentVariable;
		CString strValue,strSysValue;
		
		if (GetEnvironmentVariable(strLine,strEnvironmentVariable,strValue) == FALSE)
		{
			CString strMsg;
			strMsg.Format("Line:%s have Some Error!!!",strLine);
			AfxMessageBox(strMsg,MB_OK);
			return FALSE;
		}
		((CAmboApp *)AfxGetApp())->GetSystemEnvironmentValue(strEnvironmentVariable,strSysValue);
		if (strSysValue == strValue)
		{
			//Will Need Link The Next Content
			return FALSE;
		}
		// 这里的条件不满足，则必须查找到else or endif
		while(1)
		{
			if (ReadLine(strLine) == FALSE)
				return FALSE;
			m_nLinePos = 0;

			if (GetWord(strLine,strKey) == FALSE)
			{
				continue;
			}
			if (strSingleValue == "else")
			{
				// In Here ,the condition is met
				return FALSE;
			}
			if (strSingleValue == "endif")
			{
				// In Here ,the condition is met
				return FALSE;
			}
		}
	}

	if (GetWord(strLine,strEqual) == FALSE)
	{
		return FALSE;
	}

	// 判断是不是“=”
	if (strEqual != "=")
		return FALSE;

	// 得到键值
	while(1)
	{
		if (GetWord(strLine,strSingleValue) == FALSE)
		{
			break;
		}
		if (strSingleValue == "ifeq")
		{
			// 这里是条件语句
// *
			if (GetWord(strLine,strSingleValue) == FALSE)  // 这里得到条件 "IF" or "ENDIF"
			{
				AfxMessageBox(strLine);
				return FALSE;
			}
			if (strSingleValue.CompareNoCase("IF") == 0)
			{
				iEnterIf = 1;
				if (GetWord(strLine,strSingleValue) == FALSE)  // 这里得到条件变量
				{
					AfxMessageBox(strLine);
					return FALSE;
				}
				iNeedLink = ((CAmboApp *)AfxGetApp())->GetModuleValue(strSingleValue);
			}
			else if (strSingleValue.CompareNoCase("ENDIF") == 0)
			{
				iEnterIf = 0;
				iNeedLink = 1;
			}
			else
			{
				AfxMessageBox(strLine);
				return FALSE;
			}
			ReadLine(strLine);
			m_nLinePos = 0;
			continue;
* //
			// 这里是条件语句
			CString strEnvironmentVariable;
			CString strValue,strSysValue;
			
			if (GetEnvironmentVariable(strLine,strEnvironmentVariable,strValue) == FALSE)
			{
				CString strMsg;
				strMsg.Format("Line:%s have Some Error!!!",strLine);
				AfxMessageBox(strMsg,MB_OK);
				return FALSE;
			}
			((CAmboApp *)AfxGetApp())->GetSystemEnvironmentValue(strEnvironmentVariable,strSysValue);
			if (strSysValue == strValue)
			{
				//Will Need Link The Next Content
				if (ReadLine(strLine) == FALSE)
					return FALSE;
				m_nLinePos = 0;
				continue;
			}
			// 这里的条件不满足，则必须查找到else or endif
			while(1)
			{
				if (GetWord(strLine,strSingleValue) == FALSE)
				{
					if (ReadLine(strLine) == FALSE)
						return FALSE;
					m_nLinePos = 0;
					continue;
				}
				if (strSingleValue == "else")
				{
					// In Here ,the condition is met
					break;
				}
				if (strSingleValue == "endif")
				{
					// In Here ,the condition is met
					break;
				}
			}
			if (ReadLine(strLine) == FALSE)
				return FALSE;
			m_nLinePos = 0;
			continue;
		}
		// 判断是不是行连接符
		if (strSingleValue == "\\")
		{
			ReadLine(strLine);
			m_nLinePos = 0;
			continue;
		}
		if  (strValue.IsEmpty() == FALSE)
			strValue +=" ";  // 已经有内容，新内容要用空格分开
		strValue +=strSingleValue;
	}
	
	m_psrcData->Set(strKey,strValue);
	return TRUE;
}
*/
/*
BOOL CFileParse::IsNeedKey(CString strKey)
{
	strKey.MakeUpper();

	if (strKey == "SOURCES")
		return TRUE;

	if (strKey == "DIRS")
		return TRUE;

	if (strKey == "TARGETNAME")
		return TRUE;

	if (strKey == "TARGETTYPE")
		return TRUE;

	if (strKey == "TARGETLIBS")
		return TRUE;

	if (strKey == "SOURCELIBS")
		return TRUE;

	if (strKey == "INCLUDES")
		return TRUE;

	return FALSE;
}*/

BOOL CFileParse::IsNeedKey(CString strKey)
{
	strKey.MakeUpper();

	if (strKey == "MODULENAME")
		return TRUE;

	if (strKey == "MODULETYPE")
		return TRUE;

	if (strKey == "MODULE_OPT")
		return TRUE;

	if (strKey == "LOCAL_FILES")
		return TRUE;

	if (strKey == "LISTS")
		return TRUE;

	if (strKey == "ROOTS")
		return TRUE;



	if (strKey == "LOCAL_DYNAMICLIBS")
		return TRUE;

	if (strKey == "LOCAL_STATICLIBS")
		return TRUE;

	if (strKey == "DYNAMICLIBS")
		return TRUE;

	if (strKey == "STATICLIBS")
		return TRUE;

	if (strKey == "MODULE_ADEFINES")
		return TRUE;

	if (strKey == "MODULE_CDEFINES")
		return TRUE;

	if (strKey == "MODULE_INCLUDES")
		return TRUE;

	if (strKey == "EXEENTRY")
		return TRUE;

	if (strKey == "DLLENTRY")
		return TRUE;

	if (strKey == "KERNELMODULE")
		return TRUE;

	if (strKey == "MAPFILE")
		return TRUE;

	if (strKey == "LINKSCRIPT")
		return TRUE;

	if (strKey == "DEFEXPORT")
		return TRUE;

	if (strKey == "OUTPUTPATH")
		return TRUE;

	if (strKey == "MEMCONFIG")
		return TRUE;
	
	if (strKey == "CODEADDR")
		return TRUE;
	
	if (strKey == "DATADDR")
		return TRUE;
	
	return FALSE;
}

BOOL CFileParse::GetEnvironmentVariable(CString strLine, CString &strEnvironmentVariable,CString &strValue)
{
	UINT nWordLength = 0;
	LPTSTR lpLine;

		lpLine = strLine.LockBuffer(); // 字符串锁定
		lpLine += m_nLinePos;

		// 在这里要将当前位置后的空格过滤掉
		lpLine = FilterSpace(lpLine);
		// 在这里分析ifeq or ifeq 的结构
		//ifeq ( $( abcd ) , 4 )
		// '('
		if (*lpLine != '(')
			goto ERROR_GETENV;
		lpLine ++;
		m_nLinePos ++;
		lpLine = FilterSpace(lpLine);

		// '$('
		if (!(*lpLine == '$' && *(lpLine+1) == '('))
			goto ERROR_GETENV;
		lpLine +=2;
		m_nLinePos += 2;
		lpLine = FilterSpace(lpLine);

		// Get Environment Variable
		while (1)
		{
			if ( *lpLine == 0x00)
			{ // 行结束，一个单词同时也结束
				goto ERROR_GETENV;
			}
			if ( *lpLine == ' ' || *lpLine == '\t' || *lpLine == ',' || *lpLine == ')')
			{ // 碰到空格，表示一个单词已经结束
				break;
			}
			if ( *lpLine == '\\')
			{ // 碰到行连接符，也表示一个单词已经结束
				goto ERROR_GETENV;
			}

			if ( *lpLine == '=')
			{ // 碰到等于号，也表示一个单词已经结束
				goto ERROR_GETENV;
			}
			strEnvironmentVariable += *lpLine;
			lpLine ++;
			nWordLength ++;
		}

		m_nLinePos += nWordLength;

		lpLine = FilterSpace(lpLine);

		// ')'
		if (*lpLine != ')')
			goto ERROR_GETENV;
		lpLine +=1;
		m_nLinePos += 1;
		lpLine = FilterSpace(lpLine);
		// ','
		if (*lpLine != ',')
			goto ERROR_GETENV;
		lpLine ++;
		m_nLinePos ++;
		lpLine = FilterSpace(lpLine);

		// Get Value
		while (1)
		{
			if ( *lpLine == 0x00)
			{ // 行结束，一个单词同时也结束
				goto ERROR_GETENV;
			}
			if ( *lpLine == ' ' || *lpLine == '\t' || *lpLine == ')')
			{ // 碰到空格，表示一个单词已经结束
				break;
			}
			if ( *lpLine == '\\')
			{ // 碰到行连接符，也表示一个单词已经结束
				goto ERROR_GETENV;
			}

			if ( *lpLine == '=')
			{ // 碰到等于号，也表示一个单词已经结束
				goto ERROR_GETENV;
			}
			strValue += *lpLine;
			lpLine ++;
			nWordLength ++;
		}

		m_nLinePos += nWordLength;

		lpLine = FilterSpace(lpLine);
		// ')'
		if (*lpLine != ')')
			goto ERROR_GETENV;

		strLine.UnlockBuffer(); // 解除字符串锁定
		return TRUE;

ERROR_GETENV:
		strLine.UnlockBuffer(); // 解除字符串锁定
		return FALSE;
}

LPTSTR CFileParse::FilterSpace(LPTSTR lpLine)
{
	while (*lpLine)
	{
		if ( *lpLine == ' ' || *lpLine == '\t')
		{
			m_nLinePos ++;
			lpLine ++;
			continue;
		}
		break;
	}
	return lpLine;
}

BOOL CFileParse::PraseLine(CString& strLine)
{
	CString strWord(""),strValue("");
	CString strEqual,strSingleValue;
	int iEnterIf = 0;
	int iNeedLink = 1;

	m_nLinePos = 0;
	
	// 得到关键字
/*	if (strLine.GetAt(0) == '#')
	{
		// 是一个注释行
		return TRUE;
	}
	*/
	if (GetWord(strLine,strWord) == FALSE)
	{
		
		if (m_bNeedValue == TRUE)
		{
			//应该有数据，可这里没有，错误。--> 可以支持
			if (m_strKey.IsEmpty() == FALSE)
				m_psrcData->Set(m_strKey,m_strValue);
			m_bNeedValue = FALSE; // 得到数据完成
			m_strKey = "";
			m_strValue = "";
			return TRUE;
		}
		

		// 这是一个空行
		return TRUE;
	}
	if (strWord == "#")
	{
		// 是一个注释行
		return TRUE;
	}
	
	// 判断是否是关键字
	if (IsNeedKey(strWord) == TRUE)
	{
		// 将上一个关键字的内容写入到变量
		
		if (m_bNeedValue == TRUE)
		{
			//这里应该是得到值，不应该是关键字，错误。-->可以支持
			if (m_strKey.IsEmpty() == FALSE)
				m_psrcData->Set(m_strKey,m_strValue);
			m_bNeedValue = FALSE; // 得到数据完成
			m_strKey = "";
			m_strValue = "";
//			return TRUE;
		}
		
		m_strKey = strWord;

		if (GetWord(strLine,strEqual) == FALSE)
		{
			// 语法错误
			return FALSE;
		}

		// 判断是不是“=”
		if (strEqual == "=")
		{
			m_strValue = "";
		}
		else if (strEqual == "+")
		{
			if (GetWord(strLine,strEqual) == FALSE)
			{
				return FALSE;
			}
			if (strEqual != "=")
			{
				return FALSE;
			}
			m_psrcData->Get(m_strKey,m_strValue);
		}
		else
		{
			return FALSE;  // 语法错误
		}

		m_bNeedValue = TRUE; // 开始得到数据
		// 得到键值
		while(1)
		{
			if (GetWord(strLine,strSingleValue) == FALSE)
			{
				if (m_strKey.IsEmpty() == FALSE)
					m_psrcData->Set(m_strKey,m_strValue);
				m_bNeedValue = FALSE; // 得到数据完成
				m_strKey = "";
				m_strValue = "";
				return TRUE;
			}
			// 判断是不是行连接符
			if (strSingleValue == "\\")
			{
				// 还有数据
				if (strLine.GetLength() != m_nLinePos)
					return FALSE;
				return TRUE;
			}
			if  (m_strValue.IsEmpty() == FALSE)
				m_strValue += " ";  // 已经有内容，新内容要用空格分开
			m_strValue+=strSingleValue;
		}
	}


	if ((strWord == "ifeq") || (strWord == "ifneq") )
	{
		// 这里是条件语句
		CString strEnvironmentVariable;
		CString strValue,strSysValue;
		
		if (GetEnvironmentVariable(strLine,strEnvironmentVariable,strValue) == FALSE)
		{  // 语法有错
			return FALSE;
		}
		((CAmboApp *)AfxGetApp())->GetSystemEnvironmentValue(strEnvironmentVariable,strSysValue);
		if (strWord == "ifeq")
		{  // ifeq
			if (strSysValue == strValue)
			{
				m_iEnterIfeqNum ++;//进入一层if *** else  endif 嵌套

				//Will Need Link The Next Content
				// 以后的数据有效
				return TRUE;
			}
		}
		else  // ifneq
		{
			if (strSysValue != strValue)
			{
				m_iEnterIfeqNum ++;//进入一层if *** else  endif 嵌套

				//Will Need Link The Next Content
				// 以后的数据有效
				return TRUE;
			}
		}
		// 以后的数据无效
		// 这里的条件不满足，则必须查找到else or endif
		int iEnterIfeqNum = 0;
		while(1)
		{
			if (ReadLine(strLine) == FALSE)
			{  //没有找到else or endif，语法错误
				return FALSE;
			}
			m_nLinePos = 0;

			if (GetWord(strLine,strWord) == FALSE)
			{
				continue;
			}
			if (strWord == "ifeq")
			{
				// ifeq 嵌套
				iEnterIfeqNum ++;
				continue;
			}
			if (strWord == "else")
			{
				// In Here ,the condition is met
				// 以后的数据有效
				if (iEnterIfeqNum == 0)//已经退出所有的if嵌套
				{
					m_iEnterIfeqNum ++;  //进入一层if else  ***  endif 嵌套
					return TRUE;
				}
				continue;
			}
			if (strWord == "endif")
			{
				// In Here ,the condition is met
				// 以后的数据有效
				if (iEnterIfeqNum == 0)
					return TRUE;  //已经退出所有的if嵌套
				iEnterIfeqNum --;
				continue;
			}
		}
	}
	if (strWord == "else")
	{
		//由于if 条件成立，所以这里的数据无效
		// 这里的条件不满足，则必须查找到endif
		int iEnterIfeqNum = 0;
		while(1)
		{
			if (ReadLine(strLine) == FALSE)
			{  //没有找到endif，语法错误
				return FALSE;
			}
			m_nLinePos = 0;

			if (GetWord(strLine,strWord) == FALSE)
			{
				continue;
			}
			if (strWord == "ifeq")
			{
				// ifeq 嵌套
				iEnterIfeqNum ++;
				continue;
			}
			if (strWord == "endif")
			{
				// In Here ,the condition is met
				// 以后的数据有效
				if (iEnterIfeqNum == 0)
					return TRUE;  //已经退出所有的if嵌套
				iEnterIfeqNum --;
				continue;
			}
		}
	}
	if (strWord == "endif")
	{
		m_iEnterIfeqNum --;  //进入一层if else  ***  endif 嵌套
		return TRUE;
	}

	// 这是一个值
	if (m_bNeedValue == FALSE)
	{
		// 不需要值，有错。
		return FALSE;
	}
	// 得到键值
	while(1)
	{
		if  (m_strValue.IsEmpty() == FALSE)
			m_strValue += " ";  // 已经有内容，新内容要用空格分开
		m_strValue+=strWord;

		//读取下一个值
		if (GetWord(strLine,strWord) == FALSE)
		{ //  已经没有内容了
			if (m_strKey.IsEmpty() == FALSE)
				m_psrcData->Set(m_strKey,m_strValue);
			m_bNeedValue = FALSE; // 得到数据完成
			m_strKey = "";
			m_strValue = "";
			return TRUE;
		}
		// 判断是不是行连接符
		if (strWord == "\\")
		{
			// 还有数据
			if (strLine.GetLength() != m_nLinePos)
				return FALSE;
			return TRUE;
		}
		if  (m_strValue.IsEmpty() == FALSE)
			m_strValue += " ";  // 已经有内容，新内容要用空格分开
		m_strValue+=strSingleValue;
	}
	return TRUE;
}


void CFileParse::SetFileName(CString strFileName)
{
	m_strFileName = strFileName;  //当前正在解释的文件名
}
