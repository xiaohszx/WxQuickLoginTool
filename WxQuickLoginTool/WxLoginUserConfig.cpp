#include "stdafx.h"
#include "WxLoginUserConfig.h"
#include "StringHelper.h"

const int wx_id_key = 0x4;
const int wx_numbner_Key = 0x40;
const int wx_nick_name_key = 0xA;
const int wx_big_head_key = 0x66;
const int wx_small_head_key = 0x68;
const int wx_phone_number_key = 0x10;
const int wx_sign_key = 0x34;

WxLoginUserConfig::WxLoginUserConfig(void)
{
}


WxLoginUserConfig::~WxLoginUserConfig(void)
{
}

bool WxLoginUserConfig::open(std::wstring path)
{
	return config_helper.open_config(path);
}

bool WxLoginUserConfig::get_info(WxUserInfo& info)
{
	if(!config_helper.is_ok()){
		return false;
	}

	if(false){
		std::string wx_big_head = config_helper.get_key_string(wx_big_head_key);
		return true;
	}

	std::wstring wx_id = utf8ToWstring(config_helper.get_key_string(wx_id_key));
	std::wstring wx_number = utf8ToWstring(config_helper.get_key_string(wx_numbner_Key));
	std::wstring wx_nick_name = utf8ToWstring(config_helper.get_key_string(wx_nick_name_key));
	std::wstring wx_big_head = utf8ToWstring(config_helper.get_key_string(wx_big_head_key));
	std::wstring wx_small_head = utf8ToWstring(config_helper.get_key_string(wx_small_head_key));
	std::wstring wx_phone_number = utf8ToWstring(config_helper.get_key_string(wx_phone_number_key));
	std::wstring wx_sign = utf8ToWstring(config_helper.get_key_string(wx_sign_key));

	info.big_pic = wx_big_head;
	info.phone_number = wx_phone_number;
	info.signature = wx_sign;
	info.small_pic = wx_small_head;
	info.wx_id = wx_id;
	info.wx_number = wx_number.length() > 0 ? wx_number : wx_id;
	info.nick_name = wx_nick_name;
	
	return true;
}
