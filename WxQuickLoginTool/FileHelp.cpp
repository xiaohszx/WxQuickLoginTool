#include "stdafx.h"
#include "FileHelp.h"
#include <strsafe.h>

void get_current_all_dic(std::wstring path, std::list<std::wstring>& all_dic)
{
	WIN32_FIND_DATA FindFileData;
    HANDLE hListFile;
    WCHAR szFilePath[MAX_PATH];

	StringCbCopy(szFilePath, sizeof(szFilePath), path.c_str());
	StringCbCat(szFilePath, sizeof(szFilePath), L"\\*");

    hListFile = FindFirstFile(szFilePath, &FindFileData);
    if(hListFile == INVALID_HANDLE_VALUE)
    {
        return;
    }
    else
    {
        do
        {
            if(lstrcmp(FindFileData.cFileName, TEXT(".")) == 0 ||
            lstrcmp(FindFileData.cFileName, TEXT("..")) == 0)
            {
                continue;
            }

            // �ж��ļ����ԣ��Ƿ�Ϊ�����ļ������ļ���
            if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_ENCRYPTED)
            {
				continue;
            }
            // �ж��ļ����ԣ��Ƿ�Ϊ�����ļ����ļ���
            if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
            {
               continue;
            }

            // �ж��ļ����ԣ��Ƿ�ΪĿ¼
            if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                all_dic.push_back(FindFileData.cFileName);
            }
        }
        while(FindNextFile(hListFile, &FindFileData));
    }
}
