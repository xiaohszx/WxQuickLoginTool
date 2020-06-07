#pragma once
#include "WxConfigHelper.h"
#include "WxStruct.h"
class WxLoginUserConfig
{
public:
	WxLoginUserConfig(void);
	~WxLoginUserConfig(void);

	/*
	 *	打开
	 */
	bool open(std::wstring path);

	/*
	 *	获取info
	 */
	bool get_info(WxUserInfo& info);


private:
	WxConfigHelper config_helper;
};

