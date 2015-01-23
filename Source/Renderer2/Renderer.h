////////////////////////////////////////////////////////////////////////////
//
// Renderer - A Very simple Renderer for 2D games.
//
// (c) 2015 Blaine Myers
//
////////////////////////////////////////////////////////////////////////////
#pragma once

struct sgRendererInitParms
{
	int   Width;
	int   Height;
	bool  Windowed;
	void* Wnd;
};

void Renderer_Init( const sgRendererInitParms* InitParms );
void Renderer_Deinit();

void Renderer_UpdateBounds();
void Renderer_OnActivateApp();

void Renderer_BeginFrame();
void Renderer_EndFrame();

class SgRendererImage* Renderer_CreateSprite( const struct sgRendererImageCreateParms* CreateParms );
void                   Renderer_DestroySprite( class SgRendererImage* Sprite );