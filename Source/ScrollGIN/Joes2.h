/* Joes2.h - Header for Joes Copter 2 AI.
	Copyright (c) 2004 Blaine Myers */

#ifndef __JOES2_H__
#define __JOES2_H__

#include "objman.h"


class CJoes2ObjMan: public SgObjectManager
{
protected:

public:
	CJoes2ObjMan();
	CJoes2ObjMan(int dwMax);

	virtual ~CJoes2ObjMan();
	virtual int Initialize();
	virtual void CreateObject(const OBJECTTYPE nType, int x, int y, int nXSpeed, int nYSpeed);
	virtual void DetectCollisions();
};



#endif // __JOES2_h__