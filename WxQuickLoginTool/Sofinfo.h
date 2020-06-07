// ------------------------------------------------------------------------------------------------------------------------
// FlieNmae: 
//   SofInfo.h
// Remark:
//   ͨ����ȡע����ñ����Ѱ�װ�����Ϣ��
// ------------------------------------------------------------------------------------------------------------------------
#pragma once
#include <vector>
#include <string>

struct SoftInfo
{
	// �����
	std::wstring m_strSoftName;
	// ����汾��
	std::wstring m_strSoftVersion;
	// �����װĿ¼
	std::wstring m_strInstallLocation;
	// �����������
	std::wstring m_strPublisher;
	// ��������������·��
	std::wstring m_strMainProPath;
	// ж��exe��������·��
	std::wstring m_strUninstallPth;
};

class CSoftInfo
{
private:
	// �����Ѱ�װ���������װ��Ϣ
	std::vector<SoftInfo> m_SoftInfoArr;
	// ����ϵͳ������Ϣ
	std::vector<SoftInfo> m_SystemPatchesArr;

public:
	CSoftInfo();
	~CSoftInfo(){}

	// ��ȡһ���������������װ��Ϣ��Vector
	std::vector<SoftInfo> GetSoftInfo (void) const;
	// ��ȡ�����Ѱ�װ���������
	void GetSoftName (std::vector<std::wstring>& lpszSoftName);
	// ��ȡ�����Ѱ�װ��������汾��
	void GetSoftVersion (std::vector<std::wstring>& lpszSoftVersion);
	// ��ȡ�����Ѱ�װ���������װĿ¼
	void GetInstallLocation (std::vector<std::wstring>& lpszInstallLocation);
	// ��ȡ�����Ѱ�װ���������������
	void GetPublisher (std::vector<std::wstring>& lpszPublisher);
	// ��ȡ�����Ѱ�װ�����������������·��
	void GetMainProPath (std::vector<std::wstring>& lpszMainProPath);
	// ��ȡ�����Ѱ�װ�������ж�س�������·��
	void GetUninstallPth (std::vector<std::wstring>& lpszSoftName);

	// ��ȡһ������ϵͳ������Ϣ��Vector
	std::vector<SoftInfo> GetSystemPatchesInfo (void) const;
	// ��ȡ�����Ѱ�װϵͳ������
	void GetSystemPatchesName (std::vector<std::wstring>& lpszSoftName);
};