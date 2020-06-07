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
	 *	打开配置文件
	 */
	bool open_config(std::wstring path);

	/*
	 *	获取字符串配置,vx 字符串是utf-8编码的,通过切换编码就能猜测出来
	 */
	std::string get_key_string(int key);

	/*
	 *	修改
	 */
	bool modify_key_string(int key, std::string new_value);

	/*
	 *	刷新数据
	 */
	void flush();

	/*
	 *	判断是否解析成功
	 */
	bool is_ok();

public:
	/*
	 *	返回key的位置，如果不存在直接返回-1
	 */
	int get_key_pos(unsigned int key);

private:

	/*
	 *	解析key_value
	 */
	void parse();

	bool create_string_mem_key_value_info(unsigned int key, const std::string& s, WxKeyValueInfo& info);

	/*
	 *	获取size的大小
	 */
	unsigned short get_size(int pos);

	/*
	 *	根据给的pos获取Pos的实际数据存放起始位置
	 */
	unsigned int get_value_pos(int pos);

	/*
	 *	获取下一个起点
	 */
	unsigned int get_next_pos(int pos);

	/*
	 *	获取字符串的长度
	 */
	 int get_string_len(int pos);

	/*
	 *	计算一个字符串的的结构大小
	 */
	 int calc_string_mem_size(int str_len);

private:
	/*
	 *	字符串操作左移
	 */
	 void left_move_data(int pos, int number);

	/*
	 *	右偏移
	 */
	void right_move_data(int pos, int number);

private:
	char* raw_buffer;
	unsigned int raw_buffer_size;
	std::map<int, std::wstring> catch_string_map;
	std::wstring m_path;
	std::list<WxKeyValueInfo> m_key_value_info;
};

