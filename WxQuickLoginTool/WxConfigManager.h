#pragma once
#include <string>
#include "Sofinfo.h"

class WxConfigManager
{
public:
	WxConfigManager(void);
	~WxConfigManager(void);

public: 
	static WxConfigManager& GetInstance(){
		static WxConfigManager s;
		return s;
	}

public:
	static std::wstring GetWeChatFileDic();

	bool StartWeChat();

private:
	std::wstring GetWechatExePath();

	//��ȡ΢�ŵ�
	bool GetWxSoftInfo(SoftInfo* info);

private:
	CSoftInfo m_soft_infos;
};

