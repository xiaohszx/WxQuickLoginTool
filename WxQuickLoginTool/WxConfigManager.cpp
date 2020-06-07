#include "stdafx.h"
#include "WxConfigManager.h"
#include "StringHelper.h"

static std::wstring wx_wechat_save_path = L"";

WxConfigManager::WxConfigManager(void)
{

}


WxConfigManager::~WxConfigManager(void)
{
}

std::wstring WxConfigManager::GetWeChatFileDic()
{
	if(!wx_wechat_save_path.empty()){
		return wx_wechat_save_path;
	}

	std::wstring path = L"%USERPROFILE%\\AppData\\Roaming\\Tencent\\WeChat\\All Users\\config\\3ebffe94.ini";
	std::wstring all_user_config = L"";
	std::wstring wechat_file_path = L"";
	wchar_t max_path[256];
	memset(max_path, 0 , sizeof(max_path));
	DWORD path_ret = GetEnvironmentVariable(L"USERPROFILE", max_path ,sizeof(max_path));
	if(path_ret > 0){
		path = std::wstring(max_path) + L"\\AppData\\Roaming\\Tencent\\WeChat\\All Users\\config\\3ebffe94.ini";
	}

	char* raw_buffer = NULL;
	unsigned int raw_buffer_size = 0;

	HANDLE pfile = CreateFile(path.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(pfile == INVALID_HANDLE_VALUE){
		return L"";
	}

	DWORD filesize = GetFileSize(pfile, NULL);//获取文件大小

	//一个魔术值，文件太小肯定是错误的
	if(filesize < 2){
		return L"";
	}

	if(raw_buffer != NULL){
		//assert(raw_buffer != NULL);
		return L"";
	}

	raw_buffer = new char[filesize + 1];//申请内存，最后一位为字符串的结束符。
	memset(raw_buffer, 0, filesize + 1);

	DWORD readsize;
	raw_buffer_size = filesize;
	if (ReadFile(pfile, raw_buffer, filesize, &readsize, NULL) == 0)
	{
		CloseHandle(pfile);
		delete raw_buffer;
		raw_buffer = NULL;
		return L"";
	}

	CloseHandle(pfile);
	if(raw_buffer != NULL){
		wechat_file_path = utf8ToWstring(raw_buffer);
		delete raw_buffer;
		raw_buffer = NULL;
	}
	//wechat_file_path = wechat_file_path.replace(L"MyDocument:", std::wstring(max_path));
	if(wechat_file_path == L"MyDocument:"){
		OutputDebugString(L"在我的文档目录");
		SHGetSpecialFolderPath(NULL, max_path, CSIDL_MYDOCUMENTS, FALSE);
		wechat_file_path = std::wstring(max_path);
	}
	wx_wechat_save_path = wechat_file_path + L"\\WeChat Files";
	OutputDebugString(L"配置文件的目录:");
	OutputDebugString(wx_wechat_save_path.c_str());
	return wx_wechat_save_path;
}

bool WxConfigManager::StartWeChat()
{
	std::wstring path = GetWechatExePath();
	if(!path.empty()){
		HINSTANCE h = ShellExecute(NULL, L"open", path.c_str(), NULL, NULL, SW_SHOWNORMAL);
		if((int)h > 32){
			return true;
		}
	}
	return false;
}

std::wstring WxConfigManager::GetWechatExePath()
{
	SoftInfo info;
	bool ret = GetWxSoftInfo(&info);
	if(ret){
		std::wstring wx_path = info.m_strMainProPath + L"\\WeChat.exe"; 
		return wx_path;
	}else{
		return L"";
	}
}

bool WxConfigManager::GetWxSoftInfo(SoftInfo* info)
{
	if(info == nullptr){
		return false;
	}

	std::vector<SoftInfo> softinfo_v = m_soft_infos.GetSoftInfo();
	for(unsigned int i = 0; i < softinfo_v.size(); i++){
		std::wstring wx_path_end = L"";
		if(softinfo_v[i].m_strMainProPath.length() > 5){
			int begin =  softinfo_v[i].m_strMainProPath.length() - 1 - 5;
			wx_path_end = softinfo_v[i].m_strMainProPath.substr(begin);
		}

		if(wx_path_end == L"WeChat"){
			std::wstring s = softinfo_v[i].m_strMainProPath + L"\\WeChat.exe";
			if(PathFileExists(s.c_str())){
				*info = softinfo_v[i];
				return true;
			}
		}
	}

	return false;
}
