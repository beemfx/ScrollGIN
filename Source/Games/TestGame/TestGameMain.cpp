// (c) 2025 Beem Media. All rights reserved.

#include <windows.h>
#include <memory>

#include "SgEngine.h"
#include "TestAI.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	std::unique_ptr<CTestObjman> ObjMan = std::make_unique<CTestObjman>(256);
	SgEngine_Run(L"ScrollGIN Test Game", reinterpret_cast<std::uintptr_t>(hInstance), ObjMan.get());
	ObjMan.reset();
	return 0;
}
