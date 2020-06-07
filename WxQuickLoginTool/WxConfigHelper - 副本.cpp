#include "stdafx.h"
#include "WxConfigHelper.h"

const int size_offset = 0x1;
const int key_offset = 0x2;


WxConfigHelper::WxConfigHelper(void)
{
	raw_buffer = NULL;
	raw_buffer_size = 0;
	m_path = L"";
}


WxConfigHelper::~WxConfigHelper(void)
{
	if(raw_buffer !=NULL){
		delete raw_buffer;
		raw_buffer = NULL;
	}
}

bool WxConfigHelper::open_config(std::wstring path)
{
	HANDLE pfile = CreateFile(path.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(pfile == NULL){
		return false;
	}

	DWORD filesize = GetFileSize(pfile, NULL);//获取文件大小

	//一个魔术值，文件太小肯定是错误的
	if(filesize <= 10){
		return false;
	}

	if(raw_buffer != NULL){
		//assert(raw_buffer != NULL);
		return false;
	}

	raw_buffer = new char[filesize + 1];//申请内存，最后一位为字符串的结束符。
	DWORD readsize;
	raw_buffer_size = filesize;
	if (ReadFile(pfile, raw_buffer, filesize, &readsize, NULL) == 0)
	{
		CloseHandle(pfile);
		delete raw_buffer;
		raw_buffer = NULL;
		return false;
	}
	
	m_path = path;
	CloseHandle(pfile);
	return true;
}

std::string WxConfigHelper::get_key_string(int key)
{
	int pos = get_key_pos(key);
	if(pos == -1){
		return "";
	}

	int value_type = raw_buffer[pos];
	if(value_type == 0x1A){
		unsigned short string_buffer_size = raw_buffer[pos + 0x1];
		if(string_buffer_size > 0x4){
			unsigned int string_len_pos = 0;
			unsigned int string_value_pos = 0;
			unsigned int remain_size = 0;
			if(string_buffer_size >= 128){
				unsigned char raw_value = (unsigned char)string_buffer_size;
				raw_value &= 127;	//去掉符号位
				unsigned short second_value = raw_buffer[pos + 2];
				second_value = second_value << 7;
				second_value += raw_value;
				string_buffer_size = second_value;
				string_len_pos = pos + 0x6;
				remain_size = raw_buffer_size - pos - 3;
			}else
			{
				string_len_pos = pos + 0x5;
				remain_size = raw_buffer_size - pos - 2;
			}

			
			unsigned int string_flag_pos = string_len_pos - 1;
			int string_flag_value = raw_buffer[string_flag_pos];
			if(string_flag_value == 0x12){
				unsigned int string_len = raw_buffer[string_len_pos];
				if(string_len >= 128){
					unsigned char raw_value = (unsigned char)string_len;
					raw_value &= 127;	//去掉符号位
					unsigned short second_value = raw_buffer[string_len_pos + 1];
					second_value = second_value << 7;
					second_value += raw_value;
					string_len = second_value;
					string_value_pos = string_len_pos + 2;
				}else{
					string_value_pos = string_len_pos + 1;
				}

				if(string_len <= remain_size){
					std::string real_string = "";
					char* mem_string = new char[string_len + 1];
					memset(mem_string, 0, string_len + 1);
					for(unsigned int i = 0; i < string_len; i++){
						mem_string[i] = raw_buffer[string_value_pos + i];
					}
					real_string = mem_string;
					delete mem_string;
					return real_string;
				}

			}
		}


	}
	return "";
}

//修改原来字符串的key的内容
//1：先找到key，获取POS
//2：判断类型是不是字符串
//3：获取原来的size，要判断是否大于等于128
//4：获取原来的长度，要判断是否大约等于128
//5：新的长度，新的size，新的修改的起始位置
//6：更新字符串
//7：更新一下最新的buffer的长度
//8：写入磁盘
bool WxConfigHelper::modify_key_string(int key, std::string new_value)
{
	int pos = get_key_pos(key);
	if(pos != -1){

		int value_type = raw_buffer[pos];
		if(value_type == 0x1A){
			//长度没有大于等于128的情况下
			int string_len_offset = 0x5;
			int string_value_offset = 0x6;
			unsigned int str_len = 0;
			int new_len = new_value.length();
			unsigned int string_remain_size = 0x4;	//类型和sieze 和 除掉字符串长度默认长度（默认小于128的）

			unsigned short string_buffer_size = get_size(pos);
			
			if(string_buffer_size >= 128){
				string_len_offset = pos + 0x6;
			}

			str_len = raw_buffer[pos + string_len_offset];
			if(str_len >= 128){
				string_value_offset = string_len_offset + 2;
				string_remain_size = 0x5;
			}else{
				string_value_offset = string_len_offset + 1;
			}
		
			if(new_value.length() < str_len){
				int old_start_pos = pos + 0x1 + string_buffer_size + 0x1;	//下一个位置
				int remain_size = raw_buffer_size - old_start_pos;	//等于0，说明后面没有别的KEY的值，所以不用偏移
				//说明后面没有收据，不用考虑位移
				if(remain_size > 0){
					int new_start_pos = old_start_pos - (str_len - new_value.length());
					char* old_mem = new char[remain_size];	//备份剩下的
					memcpy(old_mem, &raw_buffer[old_start_pos], remain_size);
					memcpy(&raw_buffer[new_start_pos], old_mem, remain_size);	//老数据向前偏移
					delete old_mem;
					old_mem = NULL;
					raw_buffer_size = raw_buffer_size - (str_len - new_value.length());
				}
			}

			if(new_value.length() > str_len){
				int old_start_pos = pos + 0x1 + string_buffer_size + 0x1;
				int remain_size = raw_buffer_size - old_start_pos;
				int add_size = new_value.length() - str_len;
				int new_start_pos = old_start_pos + add_size;
				char* new_buffer = new char[raw_buffer_size+ add_size];
				memcpy(new_buffer, raw_buffer, raw_buffer_size); //备份原来
				delete raw_buffer;
				raw_buffer = new_buffer;
				raw_buffer_size = raw_buffer_size+ add_size;
			}

			for(unsigned int i = 0; i < new_value.length(); i++){
				raw_buffer[pos + string_value_offset + i] = new_value.at(i);
			}

			raw_buffer[pos + size_offset] = string_remain_size + new_len;//size
			raw_buffer[pos + string_len_offset] = new_len;//len

			return true;
		}
	}
	return false;
}

void WxConfigHelper::flush()
{
	HANDLE pfile = CreateFile(m_path.c_str(), GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(pfile == NULL){
		return;
	}

	DWORD dwBytesWritten = 0;

	BOOL bErrorFlag = WriteFile( 
		pfile,           // open file handle
		raw_buffer,      // start of data to write
		raw_buffer_size,  // number of bytes to write
		&dwBytesWritten, // number of bytes that were written
		NULL);      

	CloseHandle(pfile);
}

bool WxConfigHelper::is_ok()
{
	if(raw_buffer == NULL || raw_buffer_size == 0){
		return false;
	}

	return true;
}

int WxConfigHelper::get_key_pos(unsigned int key)
{
	if(raw_buffer != NULL){
		unsigned int pos = 0;
		while (true)
		{
			if(pos >= raw_buffer_size -1){
				break;
			}

			/*if(pos == 217){
				OutputDebugStringA("就是这里了");
			}*/

			/*char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			sprintf_s(buffer, "pos:%d", pos);
			OutputDebugStringA(buffer);*/

			unsigned short value_size = raw_buffer[pos + 1];
			unsigned int remain_size = 0;
			unsigned int current_key_pos = 0;
			unsigned int value_pos = 0;

			//大于128用2个字节存放size
			value_size = get_size(pos);
			if(value_size >= 128){
				remain_size = raw_buffer_size - pos - 3;
				current_key_pos = pos + 4;
				value_pos = pos + 3;
			}else{
				remain_size = raw_buffer_size - pos - 2;
				current_key_pos = pos + 3;
				value_pos = pos + 2;
			}

			if(value_size <3 || remain_size < value_size){
				return -1;
			}

			unsigned int mem_key = raw_buffer[current_key_pos];
			if(mem_key == key){
				return pos;
			}

			pos = value_pos + value_size;
		}
	}
	return -1;
}

/*
 *	主要大于128,因为这个用的utf8编码，1~2个字节存放数据长度，总结规律
 */
unsigned short WxConfigHelper::get_size(int pos)
{
	unsigned short mem_size = 0;
	if(pos > 0){
		unsigned short value_size = raw_buffer[pos + 1];
		if(value_size >= 128){
			unsigned char raw_value = (unsigned char)value_size;
			raw_value &= 127;
			unsigned short second_value = raw_buffer[pos + 2];
			second_value = second_value << 7;
			second_value += raw_value;
			value_size = second_value;
		}

		return value_size;
	}
}

unsigned int WxConfigHelper::get_value_pos(int pos)
{
	unsigned short value_size = get_size(pos);
	if(value_size != 0){
		unsigned int value_pos = 0;
		if(value_size >= 128){
			value_pos = pos + 3;
		}else{
			value_pos = pos + 2;
		}
		return value_pos;
	}else{
		return 0;
	}
}

unsigned int WxConfigHelper::get_string_len(int pos)
{
	unsigned short mem_size = get_size(pos);
	if((mem_size > 0) && (raw_buffer[pos] = 0x1A)){
		unsigned int string_len_pos = get_value_pos(pos) + 0x3;
		if(string_len_pos <= raw_buffer_size - 1){
			return get_size(string_len_pos - 1);
		}
	}

	return 0;
}
