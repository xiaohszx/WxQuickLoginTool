#include "stdafx.h"
#include "WxAllUserConfig.h"
#include "WxConfigManager.h"
#include "FileHelp.h"
#include "StringHelper.h"

WxAllUserConfig::WxAllUserConfig(void)
{
}


WxAllUserConfig::~WxAllUserConfig(void)
{
}

bool WxAllUserConfig::Open()
{
	std::list<std::wstring> all_user_data;
	GetAllUserLoginFromConfig(all_user_data);

	m_all_login_user.clear();
	ParseAllUserLoginUSerFromUserList(all_user_data, m_all_login_user);
	if(m_all_login_user.size() > 0){
		return true;
	}else{
		return false;
	}
}

const std::list<WxUserInfo>& WxAllUserConfig::GetAllLoginUserInfo()
{
	return  m_all_login_user;
}

bool WxAllUserConfig::ModifyLoginWx(std::wstring wx_id)
{
	std::wstring path = WxConfigManager::GetWeChatFileDic();
	WxConfigHelper helper;
	if(helper.open_config(path + L"\\All Users\\config\\config.data")){
		std::wstring replace_path = path + L"\\" + wx_id + L"\\config\\AccInfo.dat";
		std::string utf8_string = wstringToUtf8(replace_path);
		helper.modify_key_string(0x32, utf8_string.c_str());
		helper.flush();
		return true;
	}

	return false;
}

void WxAllUserConfig::GetAllUserLoginFromConfig(std::list<std::wstring>& all_user_data)
{
	std::wstring path = WxConfigManager::GetWeChatFileDic();
	std::list<std::wstring> login_user_dic;
	get_current_all_dic(path, login_user_dic);
	for(std::list<std::wstring>::const_iterator it = login_user_dic.begin(); it != login_user_dic.end(); it++){
		std::wstring item_dic = it->c_str();
		if(item_dic != L"All Users" && item_dic != L"Applet"){
			std::wstring full_path = path + L"\\" + item_dic + L"\\config\\AccInfo.dat";
			OutputDebugString(L"ÅäÖÃÎÄ¼þ");
			OutputDebugString(full_path.c_str());
			if(PathFileExists(full_path.c_str())){
				all_user_data.push_back(full_path);
			}
		}
	}
}

void WxAllUserConfig::ParseAllUserLoginUSerFromUserList(const std::list<std::wstring>& all_user_data, std::list<WxUserInfo>& all_login_user)
{
	for (std::list<std::wstring>::const_iterator it=all_user_data.begin(); it != all_user_data.end(); it++)
	{
		WxLoginUserConfig config;
		if(config.open(*it)){
			WxUserInfo info;
			if(config.get_info(info)){
				all_login_user.push_back(info);
			}
		}
	}
}
