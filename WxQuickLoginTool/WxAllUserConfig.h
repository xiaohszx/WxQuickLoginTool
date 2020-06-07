#pragma once
#include <string>
#include "WxStruct.h"
#include <list>
#include "WxLoginUserConfig.h"

class WxAllUserConfig
{
public:
	WxAllUserConfig(void);
	~WxAllUserConfig(void);

	/*
	 *	获取微信的配置路径
	 */
	std::wstring GetWxConfigPath();

	/*
	 *	读取当前配置
	 */
	bool Open();

	const std::list<WxUserInfo>& GetAllLoginUserInfo();

	/*
	 *	修改登录微信,如果当前登录就不需要修改
	 */
	bool ModifyLoginWx(std::wstring wx_id);

private:
	void GetAllUserLoginFromConfig(std::list<std::wstring>& all_user_data);

	/*
	 *	从LIST解析所有
	 */
	void ParseAllUserLoginUSerFromUserList(const std::list<std::wstring>& all_user_data, std::list<WxUserInfo>& all_login_user);

private:
	std::list<WxUserInfo> m_all_login_user;
};

