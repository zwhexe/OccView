#pragma once
#ifndef OCC_DLL_CLASS_EXPORTS
#define OCC_DLL_CLASS __declspec(dllexport)
#else
#define OCC_DLL_CLASS __declspec(dllimport)
#endif
#include "OccPCH.h"
#include <D3DHost_View.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <D3DHost_GraphicDriver.hxx>

class OCC_DLL_CLASS OccContext
{
public:
	OccContext();
	bool InitViewer();
	void SetPos(int winSizeX, int winSizeY);
	bool InitViewer(void* ptr);
	Handle(D3DHost_View) GetD3dView();

	bool Dump(const std::string& theFileName);
	void RedrawView();
	void UpdateView();
	void SetDegenerateModeOn();
	void SetDegenerateModeOff();
	void WindowFitAll(int theXmin, int theYmin, int theXmax, int theYmax);
	void Place(int theX, int theY, float theZoomFactor);
	void Zoom(int theX1, int theY1, int theX2, int theY2);
	void Pan(int theX, int theY);
	void Rotation(int theX, int theY);
	void StartRotation(int theX, int theY);
	void Display(Handle(AIS_InteractiveObject)& theIObj, Standard_Boolean theToUpdateViewer);
	void Select();
	void Select(int X1, int Y1, int X2, int Y2);
	void MoveTo(int X, int Y);
	void BackgroundColor(int& theR, int& theG, int& theB);
	void UpdateCurrentViewer();
	float Scale();
	void ZoomAllView();
	void Reset();
	void SetDisplayMode(int Mode);
	bool SetAisContext(Handle(AIS_InteractiveContext) aisContext);
	Handle(AIS_InteractiveContext) GetContext();

	bool ImportBrep(const std::string& theFileName);
	bool ImportStep(const std::string& theFileName);
	bool ImportIges(const std::string& theFileName);

public:
	Handle(V3d_View) mView;
	Handle(V3d_Viewer) mViewer;
	Handle(OpenGl_GraphicDriver) mOpenglDriver;
	Handle(D3DHost_GraphicDriver) mGraphicDriver;
};

extern Handle(AIS_InteractiveContext) gAisContext;

