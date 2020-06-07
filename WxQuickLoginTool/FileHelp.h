#pragma once
#include <string>
#include <list>

/*
 *	获取指定文件夹中，第一层所有文件夹
 */
void get_current_all_dic(std::wstring path, std::list<std::wstring>& all_dic);