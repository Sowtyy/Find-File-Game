// Find File
// Version 1.0.0
// Created by Sowtyy on 27.01.2021 @ 5:00 pm

#include <iostream>
#include <windows.h>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <lmcons.h>
#include <errno.h>
#include <shlobj.h>

#pragma warning(disable : 4996)

int _ReadFile(const wchar_t* Chr3Path)
{
	std::string FileConts1, FileConts2;

	//std::ifstream RFile("C:\\Users\\grinn\\Documents\\Swty\\File.swty");
	std::ifstream RFile(Chr3Path);

	if (RFile.is_open())
	{
		std::cout << "RFile " << RFile.is_open() << std::endl; //File opened successfully
		RFile.close();
		return 0;
	}
	else
	{
		std::cout << "RFile " << RFile.is_open() << std::endl; //Failed at opening file
		return -1;
	}
}

void _CreateFile(const wchar_t* Chr2Path, const wchar_t* Chr3Path)
{
	//int wmkdirCheck = _wmkdir(L"C:\\Users\\grinn\\Documents\\Swty");
	int wmkdirCheck = _wmkdir(Chr2Path);

	if (wmkdirCheck != 0)
		std::cout << "wmkdirCheck " << wmkdirCheck << std::endl; //Directory creation failed
	else
		std::cout << "wmkdirCheck " << wmkdirCheck << std::endl; //Directory created successfully

	//std::ofstream WFile("C:\\Users\\grinn\\Documents\\Swty\\File.swty");
	std::ofstream WFile(Chr3Path);

	if (WFile.is_open())
	{
		std::cout << "WFile " << WFile.is_open() << std::endl; //File opened successfully
		WFile << "7377747931313139";
		WFile.close();
	}
	else
	{
		std::cout << "WFile " << WFile.is_open() << std::endl; //Failed at opening file
		std::cout << GetLastError() << std::endl;
		std::cerr << strerror(errno);
	}
}

int main()
{
	SetConsoleTitle(L"Find File   by Sowtyy");

	PWSTR path = 0;
	HRESULT FoldLoc = SHGetKnownFolderPath(FOLDERID_LocalAppData, NULL, NULL, &path);

	std::wstring FiStr = path;
	std::wstring FiStr2 = FiStr + L"\\Microsoft\\Swty";
	std::wstring FiStr3 = FiStr2 + L"\\File.swty";

	const wchar_t* Fi2Chr = FiStr2.c_str();
	const wchar_t* Fi3Chr = FiStr3.c_str();

	CoTaskMemFree(path);

	if (FoldLoc == S_OK)
	{
		std::cout << "FoldLoc " << FoldLoc << std::endl; //Found location

		int	RFCheck = _ReadFile(Fi3Chr);

		if (RFCheck != 0)
		{
			_CreateFile(Fi2Chr, Fi3Chr);
		}
	}
	else
	{
		std::cout << "FoldLoc " << FoldLoc << std::endl; //Didn't find location
	}

	std::cout << "\n\nYou can close this window by pressing [ESC]\n"
		"Or you can delete created file by pressing [DEL]\n\n";

	bool FirstTime = true;

	while (true)
	{
		bool IsWinFocused = (GetConsoleWindow() == GetForegroundWindow());

		if (IsWinFocused != 0)
		{
			if (GetAsyncKeyState(VK_ESCAPE))
				break;
			else if (GetAsyncKeyState(VK_DELETE))
			{
				if (FirstTime == true)
				{
					_wremove(Fi3Chr);
					int RemDirCheck = RemoveDirectory(Fi2Chr);

					if (RemDirCheck != 0)
						std::cout << "File deleted successfully.\n" << std::endl; //Deleted dir successfully
					else
						std::cout << "Failed at deleting file.\n" << std::endl; //Failed at deleting dir

					FirstTime = false;
				}
			}
		}

		Sleep(400);
	}

	return 0;
}