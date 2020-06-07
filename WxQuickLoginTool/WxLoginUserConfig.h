#pragma once
#include "WxConfigHelper.h"
#include "WxStruct.h"
class WxLoginUserConfig
{
public:
	WxLoginUserConfig(void);
	~WxLoginUserConfig(void);

	/*
	 *	��
	 */
	bool open(std::wstring path);

	/*
	 *	��ȡinfo
	 */
	bool get_info(WxUserInfo& info);


private:
	WxConfigHelper config_helper;
};

