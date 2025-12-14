// (c) 2025 Beem Media. All rights reserved.

#include <windows.h>
#include <memory>

#include "SgEngine.h"
#include "Joes2.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	std::unique_ptr<CJoes2ObjMan> ObjMan = std::make_unique<CJoes2ObjMan>(256);
	SgEngine_Run(L"Joe's Copter II: The Revenge", reinterpret_cast<std::uintptr_t>(hInstance), ObjMan.get());
	ObjMan.reset();
	return 0;
}
