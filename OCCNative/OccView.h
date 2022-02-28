#pragma once
#ifdef _WIN32
#include <WNT_Window.hxx>
#else
#undef None
#include <Xw_Window.hxx>
#endif
#include "OccPCH.h"
#include <windows.h>
#include <Aspect_Handle.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <D3DHost_View.hxx>
#include <D3DHost_GraphicDriver.hxx>

class OccView
{
public:
	OccView();
	bool InitViewer();
	bool InitViewer(Handle(WNT_Window) aWNTWindow);
	Handle(V3d_View) ResizeBridgeFBO(int theWinSizeX, int theWinSizeY);
private:
	Handle(V3d_View) mView;
	Handle(V3d_Viewer) mViewer;
	Handle(AIS_InteractiveContext) mAisContext;
	Handle(OpenGl_GraphicDriver) mOpenglDriver;
	Handle(D3DHost_GraphicDriver) mGraphicDriver;
};

