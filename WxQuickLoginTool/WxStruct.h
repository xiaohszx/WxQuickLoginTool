#pragma once
#include <string>

struct WxUserInfo{
	std::wstring wx_id;
	std::wstring nick_name;
	std::wstring wx_number;
	std::wstring big_pic;
	std::wstring small_pic;
	std::wstring phone_number;
	std::wstring signature;
	long last_login_time;

	WxUserInfo(){
		wx_id = L"";
		nick_name = L"";
		wx_number = L"";
		big_pic = L"";
		small_pic = L"";
		phone_number = L"";
		signature = L"";
		last_login_time = 0;
	}
};

struct WxKeyValueInfo{
	int key;
	char* buffer;
	int total_size;

	WxKeyValueInfo(){
		key = 0;
		total_size = 0;
		buffer = NULL;
	}
};