// FlieNmae: Softinfo.cpp

#include "stdafx.h"
#include "Sofinfo.h"

CSoftInfo::CSoftInfo()
{
	struct SoftInfo softinfo;
	HKEY RootKey;            // 主键
	std::wstring lpSubKey;        // 子键名称
	HKEY hkResult;            // 将要打开键的句柄 
	HKEY hkRKey;
	LONG lReturn;            // 记录读取注册表是否成功
	std::wstring strBuffer;
	std::wstring strMidReg;

	DWORD index = 0;
	TCHAR szKeyName[255] = {0};        // 注册表项名称
	TCHAR szBuffer[255] = {0};
	DWORD dwKeyLen = 255;
	DWORD dwNameLen = 255;
	DWORD dwType = REG_BINARY|REG_DWORD|REG_EXPAND_SZ|REG_MULTI_SZ|REG_NONE|REG_SZ;
	RootKey = HKEY_LOCAL_MACHINE;
	lpSubKey = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall");

	lReturn = RegOpenKeyEx(RootKey, lpSubKey.c_str(), 0, KEY_ALL_ACCESS, &hkResult);

	if (lReturn == ERROR_SUCCESS)
	{

		while (ERROR_NO_MORE_ITEMS !=RegEnumKeyEx(hkResult, index, szKeyName, &dwKeyLen, 0, NULL, NULL, NULL))
		{
			index++;
			strBuffer = szKeyName;
			if (!strBuffer.empty())
			{
				strMidReg = (std::wstring)lpSubKey +_T("\\") + strBuffer;

				if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, strMidReg.c_str(), 0, KEY_ALL_ACCESS, &hkRKey)==ERROR_SUCCESS) 
				{ 
					RegQueryValueEx(hkRKey, _T("DisplayName"), 0, &dwType, (LPBYTE)szBuffer,&dwNameLen); 
					softinfo.m_strSoftName = szBuffer;
					dwNameLen = 255;
					memset(szBuffer, 0, 255);

					RegQueryValueEx(hkRKey, _T("DisplayVersion"), 0, &dwType, (LPBYTE)szBuffer,&dwNameLen); 
					softinfo.m_strSoftVersion = szBuffer;
					dwNameLen = 255;
					memset(szBuffer, 0, 255);

					RegQueryValueEx(hkRKey, _T("InstallLocation"), 0, &dwType, (LPBYTE)szBuffer,&dwNameLen); 
					softinfo.m_strInstallLocation = szBuffer;
					dwNameLen = 255;
					memset(szBuffer, 0, 255);

					RegQueryValueEx(hkRKey, _T("Publisher"), 0, &dwType, (LPBYTE)szBuffer,&dwNameLen); 
					softinfo.m_strPublisher = szBuffer;
					dwNameLen = 255;

					RegQueryValueEx(hkRKey, _T("InstallLocation"), 0, &dwType, (LPBYTE)szBuffer,&dwNameLen); 
					softinfo.m_strMainProPath = szBuffer;
					dwNameLen = 255;
					memset(szBuffer, 0, 255);

					RegQueryValueEx(hkRKey, _T("UninstallString"), 0, &dwType, (LPBYTE)szBuffer,&dwNameLen); 
					softinfo.m_strUninstallPth = szBuffer;
					dwNameLen = 255;
					memset(szBuffer, 0, 255);

					if(!softinfo.m_strSoftName.empty())
					{
						if(strBuffer.at(0) == 'K' && strBuffer.at(1) == 'B')
						{
							m_SystemPatchesArr.push_back(softinfo);
						}
						else
						{
							m_SoftInfoArr.push_back(softinfo);
						}
					}
				}

				dwKeyLen = 255;
				memset(szKeyName,0, 255);
			}
		}
		RegCloseKey(hkResult);
	}
	else
	{
		::MessageBox(NULL, _T("打开注册表失败!"), NULL, MB_ICONWARNING);
	}
}

std::vector<SoftInfo> CSoftInfo::GetSoftInfo (void) const
{
	return m_SoftInfoArr;
}

void CSoftInfo::GetSoftName (std::vector<std::wstring>& lpszSoftName)
{
	std::vector<SoftInfo>::iterator iter;
	for (iter = m_SoftInfoArr.begin(); iter != m_SoftInfoArr.end(); iter++)
	{
		lpszSoftName.push_back(iter->m_strSoftName);
	}
}

void CSoftInfo::GetSoftVersion (std::vector<std::wstring>& lpszSoftVersion)
{
	std::vector<SoftInfo>::iterator iter;
	for (iter = m_SoftInfoArr.begin(); iter != m_SoftInfoArr.end(); iter++)
	{
		if (!(iter->m_strSoftVersion).empty())
		{
			lpszSoftVersion.push_back(iter->m_strSoftVersion);
		}
	}
}

void CSoftInfo::GetInstallLocation (std::vector<std::wstring>& lpszInstallLocation)
{
	std::vector<SoftInfo>::iterator iter;
	for (iter = m_SoftInfoArr.begin(); iter != m_SoftInfoArr.end(); iter++)
	{
		if (!(iter->m_strInstallLocation).empty())
		{
			lpszInstallLocation.push_back(iter->m_strInstallLocation);
		}
	}
}

void CSoftInfo::GetPublisher (std::vector<std::wstring>& lpszPublisher)
{
	std::vector<SoftInfo>::iterator iter;

	bool bSign;
	for (iter = m_SoftInfoArr.begin(); iter != m_SoftInfoArr.end(); iter++)
	{
		bSign = true;

		// 初步去重复厂商
		std::vector<std::wstring>::iterator itr;
		for (itr = lpszPublisher.begin(); itr != lpszPublisher.end(); itr++)
		{
			if (iter->m_strPublisher == (std::wstring)*itr)
			{
				bSign = false;
			}
		}

		if (bSign)
		{
			lpszPublisher.push_back(iter->m_strPublisher);
		}
	}
}

void CSoftInfo::GetMainProPath (std::vector<std::wstring>& lpszMainProPath)
{
	std::vector<SoftInfo>::iterator iter;
	for (iter = m_SoftInfoArr.begin(); iter != m_SoftInfoArr.end(); iter++)
	{
		if (!(iter->m_strMainProPath).empty())
		{
			lpszMainProPath.push_back(iter->m_strMainProPath);
		}
	}
}

void CSoftInfo::GetUninstallPth (std::vector<std::wstring>& lpszSoftName)
{
	std::vector<SoftInfo>::iterator iter;
	for (iter = m_SoftInfoArr.begin(); iter != m_SoftInfoArr.end(); iter++)
	{
		if (!(iter->m_strUninstallPth).empty())
		{
			lpszSoftName.push_back(iter->m_strUninstallPth);
		}
	}
}

std::vector<SoftInfo> CSoftInfo::GetSystemPatchesInfo (void) const
{
	return m_SystemPatchesArr;
}

void CSoftInfo::GetSystemPatchesName (std::vector<std::wstring>& lpszSoftName)
{
	std::vector<SoftInfo>::iterator iter;
	for (iter = m_SystemPatchesArr.begin(); iter != m_SystemPatchesArr.end(); iter++)
	{
		lpszSoftName.push_back(iter->m_strSoftName);
	}
}