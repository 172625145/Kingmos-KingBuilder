// BSPConfig.cpp: implementation of the CBSPConfig class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ambo.h"
#include "BSPConfig.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBSPConfig::CBSPConfig()
{
	m_strCPUType = _T("");
	m_strPlatformName = _T("");
	m_strDescription = _T("");
	m_strVersion = _T("");
	m_strVendor = _T("");
	m_strBSPName = _T("");
}

CBSPConfig::~CBSPConfig()
{

}

void CBSPConfig::SetBSPName(CString strBSPName)
{
	m_strBSPName = strBSPName;
}

void CBSPConfig::SetVendor(CString strVendor)
{
	m_strVendor = strVendor;
}

void CBSPConfig::SetVersion(CString strVersion)
{
	m_strVersion = strVersion;
}

void CBSPConfig::SetDescription(CString strDescription)
{
	m_strDescription = strDescription;
}

void CBSPConfig::SetPlatformName(CString strPlatformName)
{
	m_strPlatformName = strPlatformName;
}

void CBSPConfig::SetCPUType(CString strCPUType)
{
	m_strCPUType = strCPUType;
}


void CBSPConfig::GetBSPName(CString &strBSPName)
{
	strBSPName = m_strBSPName;
}

void CBSPConfig::GetVendor(CString &strVendor)
{
	strVendor = m_strVendor;
}

void CBSPConfig::GetVersion(CString &strVersion)
{
	strVersion = m_strVersion;
}

void CBSPConfig::GetDescription(CString &strDescription)
{
	strDescription = m_strDescription;
}

void CBSPConfig::GetPlatformName(CString &strPlatformName)
{
	strPlatformName = m_strPlatformName;
}

void CBSPConfig::GetCPUType(CString &strCPUType)
{
	strCPUType = m_strCPUType;
}


void CBSPConfig::SaveBSP(CString strFilename)
{
		CString strbspFile;

		if (strFilename.IsEmpty())
		{
			CString strBuilderRoot;
			CString strBSPConfigRoot;

			((CAmboApp *)AfxGetApp())->GetKingbuilderRoot(strBuilderRoot);
			if (strBuilderRoot.IsEmpty() != 0)
				return;

			strBSPConfigRoot = strBuilderRoot ;
			strBSPConfigRoot += "\\bspconfig";
			CreateDirectory(strBSPConfigRoot,NULL);

			strbspFile = strBSPConfigRoot + "\\";
			strbspFile += m_strBSPName;
			strbspFile += ".bcf";
		}
		else
		{
			strbspFile = strFilename;
		}

		CFile bspfile(strbspFile, CFile::modeWrite | CFile::modeCreate );
		bspfile.Write(m_strBSPName,m_strBSPName.GetLength());
		bspfile.Write("\r\n",2);
		bspfile.Write(m_strVendor,m_strVendor.GetLength());
		bspfile.Write("\r\n",2);
		bspfile.Write(m_strVersion,m_strVersion.GetLength());
		bspfile.Write("\r\n",2);
		bspfile.Write(m_strDescription,m_strDescription.GetLength());
		bspfile.Write("\r\n",2);
		bspfile.Write(m_strPlatformName,m_strPlatformName.GetLength());
		bspfile.Write("\r\n",2);
		bspfile.Write(m_strCPUType,m_strCPUType.GetLength());
		bspfile.Write("\r\n",2);
		bspfile.Close();

}

void CBSPConfig::LoadBSP(CString strFileName)
{
	CString strBSPData;

	CFile bspfile(strFileName, CFile::modeRead);
	DWORD dwFileLen = bspfile.GetLength();
	bspfile.Read(strBSPData.GetBuffer(dwFileLen+1),dwFileLen);
	strBSPData.ReleaseBuffer();
	bspfile.Close();

	int iStart,ilen,iNext;
	CString strItemName;
	DWORD dwIndex = 0;

	iStart = 0;
	while(iStart != -1)
	{
		iNext = strBSPData.Find("\r\n",iStart);
		if (iNext == -1)
		{
			ilen = (strBSPData.GetLength() -1 ) - iStart;  // (strCPUDatal.GetLength() -1 )  The last Char position is (length -1)
			if (ilen == 0)
				break;
		}
		else
		{
			ilen = iNext - iStart;
		}
		if (ilen == 0)
		{
			dwIndex ++;
			iStart += strlen("\r\n");
			continue;
		}
		switch(dwIndex)
		{
			case 0:
				m_strBSPName = strBSPData.Mid(iStart,ilen);
				break;
			case 1:
				m_strVendor = strBSPData.Mid(iStart,ilen);
				break;
			case 2:
				m_strVersion = strBSPData.Mid(iStart,ilen);
				break;
			case 3:
				m_strDescription = strBSPData.Mid(iStart,ilen);
				break;
			case 4:
				m_strPlatformName = strBSPData.Mid(iStart,ilen);
				break;
			case 5:
				m_strCPUType = strBSPData.Mid(iStart,ilen);
				break;
		}
		iStart = iNext ;
	}	
}


