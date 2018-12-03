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
				break; // ���ǻ�Ҫ�������һ��
			}
			if (*pNewPos == '\r' && *(pNewPos+1) == '\n')
			{  // ������س�����0x0d,0x0a�����ַ�
				pNewPos +=2;
				break;
			}
			if (*pNewPos == '\r' || *pNewPos == '\n')
			{  // ������س�����0x0d����0x0aһ���ַ�
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

		lpLine = strLine.LockBuffer(); // �ַ�������
		lpLine += m_nLinePos;

		// ������Ҫ����ǰλ�ú�Ŀո���˵�
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
		// ������õ��õ��ʵĳ���
		while (1)
		{
			if ( *lpLine == 0x00)
			{ // �н�����һ������ͬʱҲ����
				break;
			}
			if ( *lpLine == '#')
			{ // ������ַ�����ע�ͣ��н�����һ������ͬʱҲ����
				if (nWordLength == 0) // ��������ӷ��ǵ�һ���ַ����������ӷ���Ϊһ������
					nWordLength ++;
				break;
			}
			if ( *lpLine == ' ' || *lpLine == '\t')
			{ // �����ո񣬱�ʾһ�������Ѿ�����
				break;
			}
			if ( *lpLine == '\\')
			{ // ���������ӷ���Ҳ��ʾһ�������Ѿ�����
//				break;
				//������ӷ�����û����Ч���ţ�������һ�����ӷ���������Ŀ¼�ָ���
				if ( *(lpLine+1) == 0x00)
				{ // �н�����һ������ͬʱҲ����
					if (nWordLength == 0) // ��������ӷ��ǵ�һ���ַ����������ӷ���Ϊһ������
						nWordLength ++;
					break;
				}
				if ( *(lpLine+1) == ' ' || *(lpLine+1) == '\t')
				{ // �����ո񣬱�ʾһ�������Ѿ�����
					if (nWordLength == 0) // ��������ӷ��ǵ�һ���ַ����������ӷ���Ϊһ������
						nWordLength ++;
					//�������ӷ����治�������κ��ַ���
					break;
				}
			}

			if ( *lpLine == '=')
			{ // �������ںţ�Ҳ��ʾһ�������Ѿ�����
				if (nWordLength == 0) // ������ں��ǵ�һ���ַ����򽫵��ں���Ϊһ������
					nWordLength ++;
				break;
			}

			lpLine ++;
			nWordLength ++;
		}

		if (nWordLength == 0)
			return FALSE;
		strLine.UnlockBuffer(); // ����ַ�������
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
	
	// �õ��ؼ���
	if (GetWord(strLine,strKey) == FALSE)
	{
		return FALSE;
	}
	// �жϹؼ����Ƿ���Ҫ
	if (IsNeedKey(strKey) == FALSE)
		return FALSE;

	if (strKey == "ifeq")
	{
		// �������������
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
		// ��������������㣬�������ҵ�else or endif
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

	// �ж��ǲ��ǡ�=��
	if (strEqual != "=")
		return FALSE;

	// �õ���ֵ
	while(1)
	{
		if (GetWord(strLine,strSingleValue) == FALSE)
		{
			break;
		}
		if (strSingleValue == "ifeq")
		{
			// �������������
// *
			if (GetWord(strLine,strSingleValue) == FALSE)  // ����õ����� "IF" or "ENDIF"
			{
				AfxMessageBox(strLine);
				return FALSE;
			}
			if (strSingleValue.CompareNoCase("IF") == 0)
			{
				iEnterIf = 1;
				if (GetWord(strLine,strSingleValue) == FALSE)  // ����õ���������
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
			// �������������
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
			// ��������������㣬�������ҵ�else or endif
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
		// �ж��ǲ��������ӷ�
		if (strSingleValue == "\\")
		{
			ReadLine(strLine);
			m_nLinePos = 0;
			continue;
		}
		if  (strValue.IsEmpty() == FALSE)
			strValue +=" ";  // �Ѿ������ݣ�������Ҫ�ÿո�ֿ�
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

		lpLine = strLine.LockBuffer(); // �ַ�������
		lpLine += m_nLinePos;

		// ������Ҫ����ǰλ�ú�Ŀո���˵�
		lpLine = FilterSpace(lpLine);
		// ���������ifeq or ifeq �Ľṹ
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
			{ // �н�����һ������ͬʱҲ����
				goto ERROR_GETENV;
			}
			if ( *lpLine == ' ' || *lpLine == '\t' || *lpLine == ',' || *lpLine == ')')
			{ // �����ո񣬱�ʾһ�������Ѿ�����
				break;
			}
			if ( *lpLine == '\\')
			{ // ���������ӷ���Ҳ��ʾһ�������Ѿ�����
				goto ERROR_GETENV;
			}

			if ( *lpLine == '=')
			{ // �������ںţ�Ҳ��ʾһ�������Ѿ�����
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
			{ // �н�����һ������ͬʱҲ����
				goto ERROR_GETENV;
			}
			if ( *lpLine == ' ' || *lpLine == '\t' || *lpLine == ')')
			{ // �����ո񣬱�ʾһ�������Ѿ�����
				break;
			}
			if ( *lpLine == '\\')
			{ // ���������ӷ���Ҳ��ʾһ�������Ѿ�����
				goto ERROR_GETENV;
			}

			if ( *lpLine == '=')
			{ // �������ںţ�Ҳ��ʾһ�������Ѿ�����
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

		strLine.UnlockBuffer(); // ����ַ�������
		return TRUE;

ERROR_GETENV:
		strLine.UnlockBuffer(); // ����ַ�������
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
	
	// �õ��ؼ���
/*	if (strLine.GetAt(0) == '#')
	{
		// ��һ��ע����
		return TRUE;
	}
	*/
	if (GetWord(strLine,strWord) == FALSE)
	{
		
		if (m_bNeedValue == TRUE)
		{
			//Ӧ�������ݣ�������û�У�����--> ����֧��
			if (m_strKey.IsEmpty() == FALSE)
				m_psrcData->Set(m_strKey,m_strValue);
			m_bNeedValue = FALSE; // �õ��������
			m_strKey = "";
			m_strValue = "";
			return TRUE;
		}
		

		// ����һ������
		return TRUE;
	}
	if (strWord == "#")
	{
		// ��һ��ע����
		return TRUE;
	}
	
	// �ж��Ƿ��ǹؼ���
	if (IsNeedKey(strWord) == TRUE)
	{
		// ����һ���ؼ��ֵ�����д�뵽����
		
		if (m_bNeedValue == TRUE)
		{
			//����Ӧ���ǵõ�ֵ����Ӧ���ǹؼ��֣�����-->����֧��
			if (m_strKey.IsEmpty() == FALSE)
				m_psrcData->Set(m_strKey,m_strValue);
			m_bNeedValue = FALSE; // �õ��������
			m_strKey = "";
			m_strValue = "";
//			return TRUE;
		}
		
		m_strKey = strWord;

		if (GetWord(strLine,strEqual) == FALSE)
		{
			// �﷨����
			return FALSE;
		}

		// �ж��ǲ��ǡ�=��
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
			return FALSE;  // �﷨����
		}

		m_bNeedValue = TRUE; // ��ʼ�õ�����
		// �õ���ֵ
		while(1)
		{
			if (GetWord(strLine,strSingleValue) == FALSE)
			{
				if (m_strKey.IsEmpty() == FALSE)
					m_psrcData->Set(m_strKey,m_strValue);
				m_bNeedValue = FALSE; // �õ��������
				m_strKey = "";
				m_strValue = "";
				return TRUE;
			}
			// �ж��ǲ��������ӷ�
			if (strSingleValue == "\\")
			{
				// ��������
				if (strLine.GetLength() != m_nLinePos)
					return FALSE;
				return TRUE;
			}
			if  (m_strValue.IsEmpty() == FALSE)
				m_strValue += " ";  // �Ѿ������ݣ�������Ҫ�ÿո�ֿ�
			m_strValue+=strSingleValue;
		}
	}


	if ((strWord == "ifeq") || (strWord == "ifneq") )
	{
		// �������������
		CString strEnvironmentVariable;
		CString strValue,strSysValue;
		
		if (GetEnvironmentVariable(strLine,strEnvironmentVariable,strValue) == FALSE)
		{  // �﷨�д�
			return FALSE;
		}
		((CAmboApp *)AfxGetApp())->GetSystemEnvironmentValue(strEnvironmentVariable,strSysValue);
		if (strWord == "ifeq")
		{  // ifeq
			if (strSysValue == strValue)
			{
				m_iEnterIfeqNum ++;//����һ��if *** else  endif Ƕ��

				//Will Need Link The Next Content
				// �Ժ��������Ч
				return TRUE;
			}
		}
		else  // ifneq
		{
			if (strSysValue != strValue)
			{
				m_iEnterIfeqNum ++;//����һ��if *** else  endif Ƕ��

				//Will Need Link The Next Content
				// �Ժ��������Ч
				return TRUE;
			}
		}
		// �Ժ��������Ч
		// ��������������㣬�������ҵ�else or endif
		int iEnterIfeqNum = 0;
		while(1)
		{
			if (ReadLine(strLine) == FALSE)
			{  //û���ҵ�else or endif���﷨����
				return FALSE;
			}
			m_nLinePos = 0;

			if (GetWord(strLine,strWord) == FALSE)
			{
				continue;
			}
			if (strWord == "ifeq")
			{
				// ifeq Ƕ��
				iEnterIfeqNum ++;
				continue;
			}
			if (strWord == "else")
			{
				// In Here ,the condition is met
				// �Ժ��������Ч
				if (iEnterIfeqNum == 0)//�Ѿ��˳����е�ifǶ��
				{
					m_iEnterIfeqNum ++;  //����һ��if else  ***  endif Ƕ��
					return TRUE;
				}
				continue;
			}
			if (strWord == "endif")
			{
				// In Here ,the condition is met
				// �Ժ��������Ч
				if (iEnterIfeqNum == 0)
					return TRUE;  //�Ѿ��˳����е�ifǶ��
				iEnterIfeqNum --;
				continue;
			}
		}
	}
	if (strWord == "else")
	{
		//����if �������������������������Ч
		// ��������������㣬�������ҵ�endif
		int iEnterIfeqNum = 0;
		while(1)
		{
			if (ReadLine(strLine) == FALSE)
			{  //û���ҵ�endif���﷨����
				return FALSE;
			}
			m_nLinePos = 0;

			if (GetWord(strLine,strWord) == FALSE)
			{
				continue;
			}
			if (strWord == "ifeq")
			{
				// ifeq Ƕ��
				iEnterIfeqNum ++;
				continue;
			}
			if (strWord == "endif")
			{
				// In Here ,the condition is met
				// �Ժ��������Ч
				if (iEnterIfeqNum == 0)
					return TRUE;  //�Ѿ��˳����е�ifǶ��
				iEnterIfeqNum --;
				continue;
			}
		}
	}
	if (strWord == "endif")
	{
		m_iEnterIfeqNum --;  //����һ��if else  ***  endif Ƕ��
		return TRUE;
	}

	// ����һ��ֵ
	if (m_bNeedValue == FALSE)
	{
		// ����Ҫֵ���д�
		return FALSE;
	}
	// �õ���ֵ
	while(1)
	{
		if  (m_strValue.IsEmpty() == FALSE)
			m_strValue += " ";  // �Ѿ������ݣ�������Ҫ�ÿո�ֿ�
		m_strValue+=strWord;

		//��ȡ��һ��ֵ
		if (GetWord(strLine,strWord) == FALSE)
		{ //  �Ѿ�û��������
			if (m_strKey.IsEmpty() == FALSE)
				m_psrcData->Set(m_strKey,m_strValue);
			m_bNeedValue = FALSE; // �õ��������
			m_strKey = "";
			m_strValue = "";
			return TRUE;
		}
		// �ж��ǲ��������ӷ�
		if (strWord == "\\")
		{
			// ��������
			if (strLine.GetLength() != m_nLinePos)
				return FALSE;
			return TRUE;
		}
		if  (m_strValue.IsEmpty() == FALSE)
			m_strValue += " ";  // �Ѿ������ݣ�������Ҫ�ÿո�ֿ�
		m_strValue+=strSingleValue;
	}
	return TRUE;
}


void CFileParse::SetFileName(CString strFileName)
{
	m_strFileName = strFileName;  //��ǰ���ڽ��͵��ļ���
}
