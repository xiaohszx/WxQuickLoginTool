// ------------------------------------------------------------------------------------------------------------------------
// FlieNmae: 
//   SofInfo.h
// Remark:
//   通过读取注册表获得本机已安装软件信息。
// ------------------------------------------------------------------------------------------------------------------------
#pragma once
#include <vector>
#include <string>

struct SoftInfo
{
	// 软件名
	std::wstring m_strSoftName;
	// 软件版本号
	std::wstring m_strSoftVersion;
	// 软件安装目录
	std::wstring m_strInstallLocation;
	// 软件发布厂商
	std::wstring m_strPublisher;
	// 主程序所在完整路径
	std::wstring m_strMainProPath;
	// 卸载exe所在完整路径
	std::wstring m_strUninstallPth;
};

class CSoftInfo
{
private:
	// 保存已安装常用软件安装信息
	std::vector<SoftInfo> m_SoftInfoArr;
	// 保存系统补丁信息
	std::vector<SoftInfo> m_SystemPatchesArr;

public:
	CSoftInfo();
	~CSoftInfo(){}

	// 获取一个包含常用软件安装信息的Vector
	std::vector<SoftInfo> GetSoftInfo (void) const;
	// 获取所有已安装常用软件名
	void GetSoftName (std::vector<std::wstring>& lpszSoftName);
	// 获取所有已安装常用软件版本号
	void GetSoftVersion (std::vector<std::wstring>& lpszSoftVersion);
	// 获取所有已安装常用软件安装目录
	void GetInstallLocation (std::vector<std::wstring>& lpszInstallLocation);
	// 获取所有已安装常用软件发布厂商
	void GetPublisher (std::vector<std::wstring>& lpszPublisher);
	// 获取所有已安装常用软件主程序所在路径
	void GetMainProPath (std::vector<std::wstring>& lpszMainProPath);
	// 获取所有已安装常用软件卸载程序所在路径
	void GetUninstallPth (std::vector<std::wstring>& lpszSoftName);

	// 获取一个包含系统补丁信息的Vector
	std::vector<SoftInfo> GetSystemPatchesInfo (void) const;
	// 获取所有已安装系统补丁名
	void GetSystemPatchesName (std::vector<std::wstring>& lpszSoftName);
};