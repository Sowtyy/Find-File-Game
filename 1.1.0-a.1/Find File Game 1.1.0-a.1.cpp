// Find File
// Version 1.1.0-a.1
// Created by Sowtyy on 29.01.2021 @ 07:30 am

#include <iostream>
#include <windows.h>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <lmcons.h>
#include <errno.h>
#include <shlobj.h>

#pragma warning(disable : 4996)


int TextCount = 0;

void CursorPos(int posX = 0, int posY = 0)
{
	COORD Coords{};
	Coords.X = posX, Coords.Y = posY;

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Coords);
}

void TextOP(std::string Text, bool ifNewLine = 0)
{
	if (TextCount <= 7)
		CursorPos(0, TextCount);
	else
	{
		std::string Spaces(120, ' ');

		for (int i = 0; i < TextCount + 2; i++)
		{
			CursorPos(0, i);
			std::cout << Spaces;
		}

		TextCount = 0;
		CursorPos(0, TextCount);
	}

	if (ifNewLine == 0)
		std::cout << Text;
	else
		std::cout << Text << std::endl;

	TextCount++;
}

char* XOR(char Text[])
{
	char XORkey[] = "7377747931313139";
	int keyLen = strlen(XORkey);
	int TextLen = strlen(Text);

	for (int i = 0, j = 0; i < TextLen; i++, j++)
	{
		Text[i] ^= XORkey[j];

		if (j == keyLen)
			j = 0;
	}

	return Text;
}

int _ReadFile(const wchar_t* Chr3Path, bool ifRead = false)
{
	std::string FileConts1, FileConts2;

	//std::ifstream RFile("C:\\Users\\grinn\\Documents\\Swty\\File.swty");
	std::ifstream RFile(Chr3Path);

	if (RFile.is_open())
	{
		TextOP("File was found.\n"); //File opened successfully

		if (ifRead == true)
		{
			std::string FileText;
			std::string FileText2;

			while (std::getline(RFile, FileText))
			{
				FileText2 += FileText += '\n';
			}

			FileText2.pop_back();

			TextOP(FileText2, true);

			char* chFileText = new char[FileText2.length() + 1];

			strcpy_s(chFileText, FileText.length() + 1, FileText2.c_str());

			TextOP(XOR(chFileText), true);
		}

		RFile.close();
		return 0;
	}
	else
	{
		TextOP("File was not found.\n"); //Failed at opening file
		return -1;
	}
}

void _CreateFile(const wchar_t* Chr2Path, const wchar_t* Chr3Path)
{
	//int wmkdirCheck = _wmkdir(L"C:\\Users\\grinn\\Documents\\Swty");
	int wmkdirCheck = _wmkdir(Chr2Path);

	/*if (wmkdirCheck != 0)
		std::cout << "wmkdirCheck: " << wmkdirCheck << std::endl; //Directory creation failed
	else
		std::cout << "wmkdirCheck: " << wmkdirCheck << std::endl; //Directory created successfully*/

	//std::ofstream WFile("C:\\Users\\grinn\\Documents\\Swty\\File.swty");
	std::ofstream WFile(Chr3Path);

	if (WFile.is_open())
	{
		TextOP("File created succsessfully.\n"); //File opened successfully
		//WFile << "7377747931313139";
		char chrText[] = "__swty";
		WFile << XOR(chrText);
		WFile.close();
	}
	else
	{
		TextOP("Failed to create file.\n"); //Failed at opening file
		std::cout << GetLastError() << std::endl;
		std::cerr << strerror(errno);
	}
}

int main()
{
	SetConsoleTitle(L"Find File 1.1.0-a.1  by Sowtyy");

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
		TextOP("Folder location found.\n"); //Found dir location

		_ReadFile(Fi3Chr);
	}
	else
	{
		TextOP("Folder location was not found.\n"); //Didn't find dir location
	}

	CursorPos(0, 10);

	std::cout <<
		"(HOLD) [R]   - Create file\n"
		"(HOLD) [F]   - Delete created file\n"
		"(HOLD) [C]   - Enter answer {PROTOTYPE}\n"
		"(HOLD) [ESC] - Close this window\n";

	CursorPos(0, TextCount);

	while (true)
	{
		bool IsWinFocused = (GetConsoleWindow() == GetForegroundWindow());
		bool BlockIfs = false;

		if (IsWinFocused != 0)
		{
			if (GetAsyncKeyState('R') & 0x8000)
			{
				int	RFCheck = _ReadFile(Fi3Chr);

				if (RFCheck != 0)
					_CreateFile(Fi2Chr, Fi3Chr);
			}
			else if (GetAsyncKeyState('F') & 0x8000)
			{
				_wremove(Fi3Chr);
				int RemDirCheck = RemoveDirectory(Fi2Chr);

				if (RemDirCheck != 0)
					TextOP("File deleted successfully.\n"); //Deleted dir successfully
				else
					TextOP("Failed to delete file.\n"); //Failed at deleting dir
			}
			else if (GetAsyncKeyState('C') & 0x8000)
			{
				char Answer[51]{};

				std::cin.clear();

				std::cin.getline(Answer, 50);

				char* ch = new char[51];
				ch = XOR(Answer);

				_ReadFile(Fi3Chr, true);
			}
			else if (GetAsyncKeyState('O') & 0x8000)
			{
				_ReadFile(Fi3Chr, true);
			}
			else if (GetAsyncKeyState(VK_LCONTROL) & 0x8000)
			{
				if (GetAsyncKeyState(VK_HOME) & 0x8000)
					ShellExecute(NULL, L"Open", Fi2Chr, NULL, NULL, SW_SHOWDEFAULT);
			}
			else if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
				break;
		}

		Sleep(200);
	}

	return 0;
}