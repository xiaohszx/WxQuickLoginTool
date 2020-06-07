#pragma once
#include <string>
#include <map>
#include <list>
#include "WxStruct.h"

class WxConfigHelper
{
public:
	WxConfigHelper(void);
	~WxConfigHelper(void);

	/*
	 *	�������ļ�
	 */
	bool open_config(std::wstring path);

	/*
	 *	��ȡ�ַ�������,vx �ַ�����utf-8�����,ͨ���л�������ܲ²����
	 */
	std::string get_key_string(int key);

	/*
	 *	�޸�
	 */
	bool modify_key_string(int key, std::string new_value);

	/*
	 *	ˢ������
	 */
	void flush();

	/*
	 *	�ж��Ƿ�����ɹ�
	 */
	bool is_ok();

public:
	/*
	 *	����key��λ�ã����������ֱ�ӷ���-1
	 */
	int get_key_pos(unsigned int key);

private:

	/*
	 *	����key_value
	 */
	void parse();

	bool create_string_mem_key_value_info(unsigned int key, const std::string& s, WxKeyValueInfo& info);

	/*
	 *	��ȡsize�Ĵ�С
	 */
	unsigned short get_size(int pos);

	/*
	 *	���ݸ���pos��ȡPos��ʵ�����ݴ����ʼλ��
	 */
	unsigned int get_value_pos(int pos);

	/*
	 *	��ȡ��һ�����
	 */
	unsigned int get_next_pos(int pos);

	/*
	 *	��ȡ�ַ����ĳ���
	 */
	 int get_string_len(int pos);

	/*
	 *	����һ���ַ����ĵĽṹ��С
	 */
	 int calc_string_mem_size(int str_len);

private:
	/*
	 *	�ַ�����������
	 */
	 void left_move_data(int pos, int number);

	/*
	 *	��ƫ��
	 */
	void right_move_data(int pos, int number);

private:
	char* raw_buffer;
	unsigned int raw_buffer_size;
	std::map<int, std::wstring> catch_string_map;
	std::wstring m_path;
	std::list<WxKeyValueInfo> m_key_value_info;
};

