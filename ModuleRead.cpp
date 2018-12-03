// ModuleRead.cpp: implementation of the CModuleRead class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ambo.h"
#include "ModuleRead.h"
#include "Infomation.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CModuleRead::CModuleRead()
{
	m_root = NULL;
	m_lpFileData = NULL;
	m_lpCurPos = NULL;
	m_dwFileSize = 0;
	m_root = new FEATURESTRUCT;
	if (m_root)
	{  // ����һ����
		m_root->strFeatureName = "Root";
		m_root->next = NULL;
		m_root->child = NULL;
	}
}

CModuleRead::~CModuleRead()
{//�ͷ������Ѿ�������ڴ�
	CloseModule();
	if (m_root)
		delete m_root;
	m_root = NULL;
}

void CModuleRead::LoadFile(CString strFileName)
{
	BOOL bRet;
	DWORD dwRead;
	bRet = m_ModuleFile.Open(strFileName,CFile::modeRead);

	if (bRet )
	{
		m_dwFileSize = m_ModuleFile.GetLength();
		if (m_dwFileSize)
		{
			m_lpFileData = new char[m_dwFileSize];
			if (m_lpFileData)
			{
				dwRead = m_ModuleFile.Read(m_lpFileData,m_dwFileSize);
				if (dwRead != m_dwFileSize)
				{
					delete m_lpFileData;
					m_lpFileData = NULL;
					m_dwFileSize = 0;
				}
			}
		}
		m_ModuleFile.Close();
	}
	m_lpCurPos = m_lpFileData;
}

BOOL CModuleRead::ReadLine(CString &line)
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

void CModuleRead::LoadModule(CString strFileName)
{
	CString line;
	LPFEATURESTRUCT lpCurFeature;
	LPDEFMODULE lpModuleDefine;
	LoadFile(strFileName); // װ���ļ�

	lpCurFeature = m_root;

	while(1)
	{
		if (ReadLine(line) == FALSE) // ��һ���ı�
			break;
		line.TrimLeft("\t "); // ���˵���ǰ��Ŀո��TAB
		if (line.IsEmpty())
			continue;  // ��һ������

		if (line.GetAt(0) == '#')
		{
			// �����ı���ע�ͣ�������
			continue;
		}
		if (line.GetAt(0) == '@')
		{
			// ���ﶨ�����һ������
			lpCurFeature = InsertFeature(line);
			continue;
		}
		//���ﶨ�����һ��ģ��
		if (lpCurFeature == NULL)
			break; //����ģ��ʧ��
		lpModuleDefine = GetModuleDefine(line);

		// Ĭ�ϸ�ģ���Ѿ�����
		lpModuleDefine->lpModuleVar->iValue = 1;
		if (lpModuleDefine)
		{
			lpCurFeature->moduleArray.Add(lpModuleDefine);
		}
	}
	if (m_lpFileData)
		delete m_lpFileData;

	m_lpFileData = NULL;
}

LPFEATURESTRUCT CModuleRead::InsertFeature(CString line)
{
	LPFEATURESTRUCT lpCurFeature,lpNew;
	CString strFeatureName;
	int nStart = 1,nCount,nEnd;

	lpCurFeature = m_root;

	while(1)
	{
		if (nStart == -1)
			break;
		nEnd = line.Find('\\',nStart);
		if (nEnd == -1)
		{
			nCount = line.GetLength() - nStart;
		}
		else
		{
			nCount = nEnd - nStart;
			nEnd ++ ; // ����'\\'
		}
		strFeatureName = line.Mid(nStart,nCount);
		nStart = nEnd;
		if (m_root == NULL)
		{
			return NULL;
		}
		else
		{
			lpNew = FeatureExist(lpCurFeature,strFeatureName);
			if (lpNew == NULL)
			{
				lpNew = new FEATURESTRUCT;
				if (lpNew)
				{
					lpNew->strFeatureName = strFeatureName;
					lpNew->next = NULL;
					lpNew->child = NULL;
					if (lpCurFeature ->child == NULL)
						lpCurFeature ->child = lpNew;
					else
					{
						lpCurFeature = lpCurFeature ->child;
						while(lpCurFeature->next)
						{
							lpCurFeature = lpCurFeature->next;
						}
						lpCurFeature->next = lpNew;
					}
				}
			}
		}
		lpCurFeature = lpNew; // ָ��ָ����Feature
	}
	return lpCurFeature;
}

LPFEATURESTRUCT CModuleRead::FeatureExist(LPFEATURESTRUCT lpFeature, CString strFeatureName)
{
	LPFEATURESTRUCT lpSearch;
	if (lpFeature == NULL)
		return NULL;
	if (lpFeature->child == NULL)
		return NULL;
	lpSearch = lpFeature->child;

	while(1)
	{
		if (lpSearch == NULL)
			return NULL;
		if (strFeatureName.CompareNoCase(lpSearch->strFeatureName) == 0)
			return lpSearch;
		lpSearch = lpSearch->next;
	}
}

LPDEFMODULE CModuleRead::GetModuleDefine(CString line)
{
	LPDEFMODULE lpModuleDefine;
	CString strWord;
	int nStart = 0;
	LPMODULEVAR lpModuleVar;

	lpModuleDefine = new DEFMODULE;
	if (lpModuleDefine == NULL)
		return NULL;
	lpModuleDefine->lpModuleVar = NULL;

	nStart = GetWord(line,nStart,strWord);
	if (nStart == -1)
		goto RET_ERROR;
	lpModuleDefine->strModuleName = strWord;

	nStart = GetWord(line,nStart,strWord);
	if (nStart == -1)
		goto RET_ERROR;
	lpModuleVar = new MODULEVAR;
	if (lpModuleVar == NULL)
		goto RET_ERROR;
	lpModuleVar->VarName = strWord;
	lpModuleVar->iValue = 0;

	lpModuleDefine->lpModuleVar = lpModuleVar;

	while(1)
	{
		nStart = GetWord(line,nStart,strWord);
		if (nStart == -1)
			break;
		lpModuleVar = new MODULEVAR;
		if (lpModuleVar == NULL)
			goto RET_ERROR;
		lpModuleVar->VarName = strWord;
		lpModuleVar->iValue = 0;
		lpModuleDefine->supportArray.Add(lpModuleVar);
	}

	return lpModuleDefine;

RET_ERROR:
	AfxMessageBox(line);
	DeleteModuleDefine(lpModuleDefine);
	return NULL;
}

int CModuleRead::GetWord(CString line, int nStart, CString &strWord)
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



void CModuleRead::DeleteModuleDefine(LPDEFMODULE lpModuleDefine)
{
	int iSize;
	LPMODULEVAR lpModuleVar;
	iSize = lpModuleDefine->supportArray.GetSize();
	while(iSize)
	{
		lpModuleVar = (LPMODULEVAR)lpModuleDefine->supportArray.GetAt(0);
		if (lpModuleVar)
			delete lpModuleVar;
		lpModuleDefine->supportArray.RemoveAt(0);
		iSize--;
	}
	if (lpModuleDefine->lpModuleVar)
		delete lpModuleDefine->lpModuleVar;
	if (lpModuleDefine)
		delete lpModuleDefine;

}


void CModuleRead::CloseModule()
{
	LPFEATURESTRUCT lpCurFeature;


	if (m_lpFileData)
		delete m_lpFileData;

	m_lpFileData = NULL;

	LPDEFMODULE lpModuleDefine;
	int iSize;
	iSize = m_root->moduleArray.GetSize();
	while(iSize)
	{
		lpModuleDefine = (LPDEFMODULE)m_root->moduleArray.GetAt(0);
		if (lpModuleDefine)
		{
			DeleteModuleDefine(lpModuleDefine);
		}
		m_root->moduleArray.RemoveAt(0);
		iSize--;
	}
	lpCurFeature = m_root->child;
	DeleteFeature(lpCurFeature);

	// ɾ��m_aModuleSetArray
	LPMODULEVAR lpCurModule;
	iSize = m_aModuleSetArray.GetSize();
	while(iSize)
	{
		lpCurModule = (LPMODULEVAR)m_aModuleSetArray.GetAt(0);
		if (lpCurModule)
		{
			delete lpCurModule;
		}
		m_aModuleSetArray.RemoveAt(0);
		iSize--;
	}
}

void CModuleRead::DeleteFeature(LPFEATURESTRUCT lpCurFeature)
{
	if (lpCurFeature == NULL)
		return ;
	if (lpCurFeature->next)
		DeleteFeature(lpCurFeature->next);  // ɾ�������ֵ�
	if (lpCurFeature->child)
		DeleteFeature(lpCurFeature->child); // ɾ�����Ķ���

	// ɾ������ģ��
	LPDEFMODULE lpModuleDefine;
	int iSize;
	iSize = lpCurFeature->moduleArray.GetSize();
	while(iSize)
	{
		lpModuleDefine = (LPDEFMODULE)lpCurFeature->moduleArray.GetAt(0);
		if (lpModuleDefine)
		{
			DeleteModuleDefine(lpModuleDefine);
		}
		lpCurFeature->moduleArray.RemoveAt(0);
		iSize--;
	}
	delete lpCurFeature;

}

void CModuleRead::SaveCurModuleState(CString strFileName)
{
	LPFEATURESTRUCT lpCurFeature;

	LPDEFMODULE lpModuleDefine;
	int iSize;
	int nIndex;
	iSize = m_root->moduleArray.GetSize();
	nIndex = 0;
	while(nIndex < iSize)
	{
		lpModuleDefine = (LPDEFMODULE)m_root->moduleArray.GetAt(nIndex);
		if (lpModuleDefine)
		{
			InsertModuleDefine(lpModuleDefine);
		}
		nIndex ++;
	}
	lpCurFeature = m_root->child;
//	DeleteFeature(lpCurFeature);
	InsertFeatureToMoudleSet(lpCurFeature);
	SaveModuleSet(strFileName);
}

LPMODULEVAR CModuleRead::InsertModuleVar(LPMODULEVAR lpModuleVar)
{
	LPMODULEVAR lpCurModule;
	int iSize;
	int nIndex;

	if (lpModuleVar == NULL)
		return NULL;
	iSize = m_aModuleSetArray.GetSize();
	nIndex = 0;
	while(nIndex < iSize)
	{
		lpCurModule = (LPMODULEVAR)m_aModuleSetArray.GetAt(nIndex);
		if (lpCurModule)
		{
			if (lpCurModule->VarName.CompareNoCase(lpModuleVar->VarName) == 0)
				return lpCurModule;
		}
		nIndex ++;
	}
	lpCurModule = new MODULEVAR;
	if (lpCurModule)
	{
		lpCurModule->VarName = lpModuleVar->VarName;
		lpCurModule->iValue = 0;
		m_aModuleSetArray.Add(lpCurModule);
	}
	return lpCurModule;
}

void CModuleRead::InsertModuleDefine(LPDEFMODULE lpModuleDefine)
{
	LPMODULEVAR lpCurModule;
	int iValue;

		if (lpModuleDefine== NULL)
			return ;

		if (lpModuleDefine->lpModuleVar == NULL)
			return ;

		iValue = lpModuleDefine->lpModuleVar->iValue;
		lpCurModule = InsertModuleVar(lpModuleDefine->lpModuleVar);
		if (lpCurModule)
		{
			SetModuleVar(lpCurModule,iValue);
		}

		int iSize;
		int nIndex;
		LPMODULEVAR lpSupportModule;

		iSize = lpModuleDefine->supportArray.GetSize();
		nIndex = 0;
		while(nIndex < iSize)
		{
			lpSupportModule = (LPMODULEVAR)lpModuleDefine->supportArray.GetAt(nIndex);
			lpCurModule = InsertModuleVar(lpSupportModule);
			if (lpCurModule && iValue)
			{
				SetModuleVar(lpCurModule,1);
			}
			nIndex ++;
		}

}

void CModuleRead::SetModuleVar(LPMODULEVAR lpModuleVar, int iValue)
{
	lpModuleVar->iValue = iValue;
}

void CModuleRead::InsertFeatureToMoudleSet(LPFEATURESTRUCT lpCurFeature)
{
	if (lpCurFeature == NULL)
		return ;

	if (lpCurFeature->child)
		InsertFeatureToMoudleSet(lpCurFeature->child); // ������Ķ���

	// �������ģ��
	LPDEFMODULE lpModuleDefine;
	int iSize;
	int nIndex = 0;
	iSize = lpCurFeature->moduleArray.GetSize();
	while(nIndex < iSize)
	{
		lpModuleDefine = (LPDEFMODULE)lpCurFeature->moduleArray.GetAt(nIndex);
		if (lpModuleDefine)
		{
			InsertModuleDefine(lpModuleDefine);
		}
		nIndex ++;
	}
	if (lpCurFeature->next)
		InsertFeatureToMoudleSet(lpCurFeature->next);  // ��������ֵ�
}

void CModuleRead::SaveModuleSet(CString strFileName)
{
	LPMODULEVAR lpCurModule;
	CString strDirectory;
	int iSize;
	int nIndex;
	CFile cfModuleSet;
	BOOL bRet;

	strDirectory = strFileName.Left(strFileName.ReverseFind('\\'));
	if (IsExistDirectory(strDirectory) == FALSE)
		CreateDirectory(strDirectory,NULL);
	bRet = cfModuleSet.Open(strFileName,CFile::modeWrite | CFile::modeCreate);
	if (bRet == FALSE)
		return;

	iSize = m_aModuleSetArray.GetSize();
	nIndex = 0;
	while(nIndex < iSize)
	{
		lpCurModule = (LPMODULEVAR)m_aModuleSetArray.GetAt(nIndex);
		if (lpCurModule)
		{
			//cfModuleSet.Write("EXPORT ",7);
			cfModuleSet.Write(OUTPUTTEXT,strlen(OUTPUTTEXT));
			cfModuleSet.Write(" ",1);
			cfModuleSet.Write(lpCurModule->VarName,lpCurModule->VarName.GetLength());
			cfModuleSet.Write("=",1);
			if (lpCurModule->iValue)
				cfModuleSet.Write("1 \r\n",4);
			else
				cfModuleSet.Write("0 \r\n",4);
		}
		nIndex ++;
	}
	cfModuleSet.Close();
}

void CModuleRead::LoadCurModuleState(CString strFileName)
{
	CString line;
	CString strWord;
	int nStart = 0;
	LPMODULEVAR lpModuleVar;
	LoadFile(strFileName); // װ���ļ�

	while(1)
	{
		if (ReadLine(line) == FALSE) // ��һ���ı�
			break;
		line.TrimLeft("\t "); // ���˵���ǰ��Ŀո��TAB
		if (line.IsEmpty())
			continue;  // ��һ������

		if (line.GetAt(0) == '#')
		{
			// �����ı���ע�ͣ�������
			continue;
		}
		nStart = 0;

		nStart = GetWord(line,nStart,strWord);  // ������õ��ؼ��֡�SET��
		if (nStart == -1)
			goto SET_ERROR;
		if (strWord.CompareNoCase(OUTPUTTEXT) != 0)
		{
			goto SET_ERROR;
		}

		nStart = GetWord(line,nStart,strWord);  // ������õ�������
		if (nStart == -1)
			goto SET_ERROR;

		lpModuleVar = new MODULEVAR;
		if (lpModuleVar == NULL)
			goto SET_ERROR;
		lpModuleVar->VarName = strWord;

		nStart = GetWord(line,nStart,strWord);  // ������õ��ؼ��֡�=��
		if (nStart == -1)
			goto SET_ERROR;
		if (strWord.CompareNoCase("=") != 0)
		{
			goto SET_ERROR;
		}

		nStart = GetWord(line,nStart,strWord);  // ������õ�����ֵ
		if (nStart == -1)
			lpModuleVar->iValue = 0;

		if (strWord.CompareNoCase("1") == 0)
		{
			lpModuleVar->iValue = 1;
		}
		else if (strWord.CompareNoCase("0") == 0)
		{
			lpModuleVar->iValue = 0;
		}
		else
			goto SET_ERROR;
		m_aModuleSetArray.Add(lpModuleVar);
	}
	goto SET_SUCCESS;

SET_ERROR:
	AfxMessageBox(line);

SET_SUCCESS:
	if (m_lpFileData)
		delete m_lpFileData;

	m_lpFileData = NULL;
}

int CModuleRead::GetModuleValue(CString strModuleName)
{
	LPMODULEVAR lpCurModule;
	int iSize;
	int nIndex;

	if (strModuleName.IsEmpty())
		return 0;
	iSize = m_aModuleSetArray.GetSize();
	nIndex = 0;
	while(nIndex < iSize)
	{
		lpCurModule = (LPMODULEVAR)m_aModuleSetArray.GetAt(nIndex);
		if (lpCurModule)
		{
			if (lpCurModule->VarName.CompareNoCase(strModuleName) == 0)
				return lpCurModule->iValue;
		}
		nIndex ++;
	}
	return 0;
}

BOOL CModuleRead::IsExistDirectory(CString strDirectory)
{
	WIN32_FIND_DATA  FindFileData;
    HANDLE						hFind;

		hFind=FindFirstFile(strDirectory, &FindFileData  );  // ���Ҹ�Ŀ¼�Ƿ����
		if( hFind == INVALID_HANDLE_VALUE) 
		{// �ļ�������
			return FALSE;
		}
		while(TRUE)
		{  // �ļ�����
			if( FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY )
			{
				// ���ҵ�����һ��Ŀ¼
				return TRUE;
			}
			if( FindNextFile(hFind,&FindFileData)==0 )  //������һ���ļ�
			{ // �Ѿ�û���ļ���
				break;
			}
		}
		FindClose(hFind);
		return FALSE;
}
