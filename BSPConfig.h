// BSPConfig.h: interface for the CBSPConfig class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BSPCONFIG_H__B4EF5212_3377_4600_9FC7_D5A15B688FD1__INCLUDED_)
#define AFX_BSPCONFIG_H__B4EF5212_3377_4600_9FC7_D5A15B688FD1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBSPConfig  
{
public:
	void LoadBSP(CString strFileName);
	void SaveBSP(CString strFilename);
	void SetCPUType(CString strCPUType);
	void SetPlatformName(CString strPlatformName);
	void SetDescription(CString strDescription);
	void SetVersion(CString strVersion);
	void SetVendor(CString strVendor);
	void SetBSPName(CString strBSPName);

	void GetCPUType(CString &strCPUType);
	void GetPlatformName(CString &strPlatformName);
	void GetDescription(CString &strDescription);
	void GetVersion(CString &strVersion);
	void GetVendor(CString &strVendor);
	void GetBSPName(CString &strBSPName);
	CBSPConfig();
	virtual ~CBSPConfig();

private:
	CString m_strCPUType;
	CString m_strPlatformName;
	CString m_strDescription;
	CString m_strVersion;
	CString m_strVendor;
	CString m_strBSPName;
};

#endif // !defined(AFX_BSPCONFIG_H__B4EF5212_3377_4600_9FC7_D5A15B688FD1__INCLUDED_)
