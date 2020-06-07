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
	 *	��ȡ΢�ŵ�����·��
	 */
	std::wstring GetWxConfigPath();

	/*
	 *	��ȡ��ǰ����
	 */
	bool Open();

	const std::list<WxUserInfo>& GetAllLoginUserInfo();

	/*
	 *	�޸ĵ�¼΢��,�����ǰ��¼�Ͳ���Ҫ�޸�
	 */
	bool ModifyLoginWx(std::wstring wx_id);

private:
	void GetAllUserLoginFromConfig(std::list<std::wstring>& all_user_data);

	/*
	 *	��LIST��������
	 */
	void ParseAllUserLoginUSerFromUserList(const std::list<std::wstring>& all_user_data, std::list<WxUserInfo>& all_login_user);

private:
	std::list<WxUserInfo> m_all_login_user;
};

