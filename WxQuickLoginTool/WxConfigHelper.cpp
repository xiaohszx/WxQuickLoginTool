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

	DWORD filesize = GetFileSize(pfile, NULL);//��ȡ�ļ���С

	//һ��ħ��ֵ���ļ�̫С�϶��Ǵ����
	if(filesize <= 10){
		return false;
	}

	if(raw_buffer != NULL){
		return false;
	}

	raw_buffer = new char[filesize + 1];//�����ڴ棬���һλΪ�ַ����Ľ�������
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
	parse();
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
				raw_value &= 127;	//ȥ������λ
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
					raw_value &= 127;	//ȥ������λ
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

//�޸�ԭ���ַ�����key������
//1�����ҵ�key����ȡPOS
//2���ж������ǲ����ַ���
//3����ȡԭ����size��Ҫ�ж��Ƿ���ڵ���128
//4����ȡԭ���ĳ��ȣ�Ҫ�ж��Ƿ��Լ����128
//5���µĳ��ȣ��µ�size���µ��޸ĵ���ʼλ��
//6�������ַ���
//7������һ�����µ�buffer�ĳ���
//8��д�����
bool WxConfigHelper::modify_key_string(int key, std::string new_value)
{
	if(new_value.length() > 0 && m_key_value_info.size() > 0){
		bool is_find = false;
		for (auto& item : m_key_value_info)
		{
			if(item.key == key){
				is_find = true;
				WxKeyValueInfo info;
				bool b_ret = create_string_mem_key_value_info(key, new_value, info);
				if(b_ret){
					item.buffer = info.buffer;
					item.total_size = info.total_size;
					return true;
				}
			}
		}

		if(is_find == false){
			WxKeyValueInfo info;
			bool b_ret = create_string_mem_key_value_info(key, new_value, info);
			if(b_ret){
				m_key_value_info.push_back(info);
				return true;
			}
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

	

	for (auto item : m_key_value_info)
	{
		if(item.total_size > 0){
			DWORD dwBytesWritten = 0;
			BOOL bErrorFlag = WriteFile( 
				pfile,           // open file handle
				item.buffer,      // start of data to write
				item.total_size,  // number of bytes to write
				&dwBytesWritten, // number of bytes that were written
				NULL);    
		}
	}
	SetEndOfFile(pfile);
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
				OutputDebugStringA("����������");
			}*/

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			sprintf_s(buffer, "pos:%d", pos);
			OutputDebugStringA(buffer);

			unsigned short value_size = raw_buffer[pos + 1];
			unsigned int remain_size = 0;
			unsigned int current_key_pos = 0;
			unsigned int value_pos = 0;

			//����128��2���ֽڴ��size
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

void WxConfigHelper::parse()
{
	if(raw_buffer != NULL && raw_buffer_size > 0){
		unsigned int pos = 0;

		bool is_error = false;
		while (true)
		{
			if(pos >= raw_buffer_size -1){
				break;
			}

			unsigned short value_size = raw_buffer[pos + 1];
			unsigned int remain_size = 0;
			unsigned int current_key_pos = 0;
			unsigned int value_pos = 0;
			int mem_size_size = 0x1;	//Ĭ����1

			//����128��2���ֽڴ��size
			value_size = get_size(pos);
			if(value_size >= 128){
				remain_size = raw_buffer_size - pos - 3;
				current_key_pos = pos + 4;
				value_pos = pos + 3;
				mem_size_size = 0x2;
			}else{
				remain_size = raw_buffer_size - pos - 2;
				current_key_pos = pos + 3;
				value_pos = pos + 2;
			}

			if(value_size <3 || remain_size < value_size){
				is_error = true;
				break;
			}

			unsigned int mem_key = raw_buffer[current_key_pos];
			
			//key ������list
			//����mem�ڴ�
			if(value_size > 0 && (pos + value_size) > raw_buffer_size){
				is_error = true;
				break;
			}

			unsigned int mem_buffer_size = value_size + 0x1 + mem_size_size;
			char* mem_buffer = new char[mem_buffer_size];
			memcpy_s(mem_buffer, mem_buffer_size, &raw_buffer[pos], mem_buffer_size);

			WxKeyValueInfo info;
			info.key = mem_key;
			info.buffer = mem_buffer;
			info.total_size = mem_buffer_size;
			m_key_value_info.push_back(info);
			pos = value_pos + value_size;
		}

		if(is_error){
			for(auto item : m_key_value_info){
				if(item.buffer != nullptr){
					delete item.buffer;
				}
			}
			m_key_value_info.clear();
		}
	}
}

bool WxConfigHelper::create_string_mem_key_value_info(unsigned int key, const std::string& s, WxKeyValueInfo& info)
{

	if(s.empty())
	{
		return false;
	}

	int str_len_size = 1;
	int mem_size_size = 1;	//Ĭ��һ���ֽڴ�Ŵ�С
	if(s.length() > 0){
		if(s.length() >= 128){
			str_len_size = 2;
		}
	}

	int value_size = 0x3 + str_len_size;
	if(value_size + s.length() >= 128){
		mem_size_size = 2;
	}


	int total_size = 0x1 + mem_size_size + value_size + s.length();
	char* buffer = new char[total_size];


	int pos = 0;
	buffer[pos] = 0x1a;
	short len = value_size + s.length();	//ȥ�����ͺʹ�ų���
	pos++;

	if(mem_size_size == 2){
		buffer[pos] = len | (~127);
		buffer[pos+1] = (char)(len >> 7);
		pos++;
	}else{
		buffer[pos] = (char)len;
	}
	pos++;
	buffer[pos] = 0x8;	//δ֪��־
	pos++;
	buffer[pos] = key;
	pos++;
	buffer[pos] = 0x12;	//δ֪��־
	pos++;

	if(s.length() >= 128){
		len = s.length();
		buffer[pos] = len | (~127);
		buffer[pos+1] = (char)(len >> 7);
		pos++;
	}else{
		buffer[pos] = (char)s.length();
	}
	pos++;

	len = total_size - pos;
	//�����ַ�������������
	memcpy_s(&buffer[pos], len, s.data(), s.length());

	info.key = key;
	info.total_size = total_size;
	info.buffer = buffer;
	return true;
}

/*
 *	��Ҫ����128,��Ϊ����õ�utf8���룬1~2���ֽڴ�����ݳ��ȣ��ܽ����
 */
unsigned short WxConfigHelper::get_size(int pos)
{
	if(pos >= 0){
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
	return 0;
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

unsigned int WxConfigHelper::get_next_pos(int pos)
{
	unsigned short value_size =  get_size(pos);
	if(value_size != 0){
		unsigned int value_pos = get_value_pos(pos);
		if(value_pos != 0){
			return value_pos + value_size;
		}
	}
	return 0;
}

int WxConfigHelper::get_string_len(int pos)
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

//1���ȼ����ַ��������Ƿ����128
//2���ټ���size�Ƿ����128
 int WxConfigHelper::calc_string_mem_size(int str_len)
{
	unsigned int len = 0;
	int str_len_size = 1;
	int mem_len_size = 1;
	if(str_len >= 128){
		str_len_size = 2;
		mem_len_size = 2;
	}else{
		if(str_len + 4 >= 128){
			mem_len_size = 2;
		}
	}

	return (1+mem_len_size+3+str_len_size);	//value_type + mem_size +  3��(2����־ һ��key) + �ַ�������size
}

 //���ƻ���Ҫ����λ��
void WxConfigHelper::left_move_data(int pos, int number)
{
	int new_start_pos = pos - number;
	unsigned int remain_size = raw_buffer_size - pos;
	char* old_mem = new char[remain_size];	//����ʣ�µ�
	memcpy(old_mem, &raw_buffer[pos], remain_size);
	memcpy(&raw_buffer[new_start_pos], old_mem, remain_size);	//��������ǰƫ��
	delete old_mem;
	old_mem = NULL;
	raw_buffer_size = raw_buffer_size - pos;
}

void WxConfigHelper::right_move_data(int pos, int number)
{
	unsigned int new_start_pos = pos + number;
	unsigned int add_size = number;
	unsigned int old_size = raw_buffer_size - pos;
	char* new_buffer = new char[raw_buffer_size+ add_size];
	memcpy(new_buffer, raw_buffer, raw_buffer_size); //����ԭ��
	memcpy(&new_buffer[new_start_pos], &raw_buffer[pos], old_size);	//��ƫ��
	delete raw_buffer;
	raw_buffer = new_buffer;
	raw_buffer_size = raw_buffer_size+ add_size;
}
