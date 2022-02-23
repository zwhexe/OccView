#pragma once
#include <d3d9.h>
#include <vcclr.h>
#include <windows.h>
#include "OccPCH.h"
#include "Shape.hpp"

static TCollection_AsciiString toAsciiString(String^ theString)
{
    if (theString == nullptr)
    {
        return TCollection_AsciiString();
    }

    pin_ptr<const wchar_t> aPinChars = PtrToStringChars(theString);
    const wchar_t* aWCharPtr = aPinChars;
    if (aWCharPtr == NULL
        || *aWCharPtr == L'\0')
    {
        return TCollection_AsciiString();
    }
    return TCollection_AsciiString(aWCharPtr);
}

public ref class OCCView
{
public:
    OCCView();
	bool InitViewer();
	System::IntPtr ResizeBridgeFBO(int theWinSizeX, int theWinSizeY);
	bool InitViewer(System::IntPtr theWnd);
	bool Dump(const TCollection_AsciiString& theFileName);
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
    void Select(int theX1, int theY1, int theX2, int theY2);
    void Select();
    void MoveTo(int theX, int theY);
    void BackgroundColor(int& theR, int& theG, int& theB);
    void UpdateCurrentViewer();
    float Scale();
    void ZoomAllView();
    void Reset();
    void SetDisplayMode(int theMode);
    float GetOCCVersion();
    void DisplayShape(TopoDS_Shape& theShp);
    void DisplayShape(OCCT::OCCT_Shape^ octShp);
    bool SetAisContext(OCCView^ theViewer);
    Handle(AIS_InteractiveContext) GetContext();
    bool ImportBrep(System::String^ theFileName);
    bool ImportBrep(const TCollection_AsciiString& theFileName);
    bool ImportStep(const TCollection_AsciiString& theFileName);
    bool ImportIges(const TCollection_AsciiString& theFileName);
    bool TranslateModel(System::String^ theFileName, int theFormat, bool theIsImport);
    void InitOCCView();

private:
    NCollection_Haft<Handle(V3d_View)> mView;
    NCollection_Haft<Handle(V3d_Viewer)> mViewer;
    NCollection_Haft<Handle(AIS_InteractiveContext)> mAisContext;
    NCollection_Haft<Handle(OpenGl_GraphicDriver)> mOpenglDriver;
    NCollection_Haft<Handle(D3DHost_GraphicDriver)> mGraphicDriver;
};

