// (c) 2025 Beem Media. All rights reserved.

#include "SgFuncs.h"

#if defined(_WIN32)
#include <Windows.h>
#endif

std::wstring SgFunc_ToWString(const std::string& In)
{
#if defined(_WIN32)
	const int NeededSize = MultiByteToWideChar(CP_ACP, 0, In.c_str(), -1, NULL, 0);

	if (NeededSize == 0)
	{
		return L"";
	}

	std::vector<wchar_t> Temp;
	Temp.resize(NeededSize);
	MultiByteToWideChar(CP_ACP, 0, In.c_str(), -1, Temp.data(), Temp.size());

	return Temp.data();
#endif
}

std::string SgFunc_ToMBString(const std::wstring& In)
{
#if defined(_WIN32)
	const int NeededSize = WideCharToMultiByte(CP_ACP, 0, In.c_str(), -1,NULL, 0, NULL, NULL);

	if (NeededSize == 0)
	{
		return "";
	}

	std::vector<char> Temp;
	Temp.resize(NeededSize);
	WideCharToMultiByte(CP_ACP, 0, In.c_str(), -1, Temp.data(), Temp.size(), NULL, NULL);

	return Temp.data();
#endif
}
