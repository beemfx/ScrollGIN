/*
	ScrollGIN, a 32bit scrolling based engine.

	main.cpp - entry point and functions for ScrollGIN technologies

	Copyright (c) 2002, 2003 Blaine Myers
*/
/*
	All files specified in the ScrollGIN project/workspace are:
		Copyright (c) 2002, 2003 Blaine Myers
		unless otherwise specified.
*/

#include <windows.h>
#include <memory>

#include "SgEngine.h"
#include "Joes2.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{

	std::unique_ptr<CJoes2ObjMan> ObjMan = std::make_unique<CJoes2ObjMan>(256);
	SgEngine_Run(L"Joe's Copter II: The Revenge", reinterpret_cast<std::uintptr_t>(hInstance), ObjMan.get(), "joeslevel1.map");
	ObjMan.reset();
	return 0;
}
