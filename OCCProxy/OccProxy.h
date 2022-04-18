#pragma once
#define OCC_DLL_CLASS_EXPORTS
#include <vcclr.h>
#include <windows.h>
#include <msclr/marshal_cppstd.h>
#include "OccPCH.h"
//!If not include OccShape.hpp, this class cannot be identified in C#???
#include "../OCCNative/OccModel.h"

using namespace System;
using namespace System::Runtime::InteropServices;

//use OCCProxy to process operation
public ref class OccProxy
{
public:
    OccProxy();

    /// <summary>
    /// view manipulate
    /// </summary>
    /// <returns></returns>
    bool InitViewer();

    void ResizeBridgeFBO(int theWinSizeX, int theWinSizeY);
    
    System::IntPtr GetD3dSurface();

    void RedrawView();

    void ZoomAllView();

    void SetDisplayMode(int theMode);

    float Scale();

    void Zoom(int theX1, int theY1, int theX2, int theY2);

    void Rotation(int x, int y);

    void Pan(int x, int y);
   
    void MoveTo(int x, int y);

    void StartRotation(int x, int y);

    bool TranslateModel(System::String^ aFilename, int format, bool update);

    /// <summary>
    /// make shape
    /// </summary>
    void makeCone();

    void makeTorus();

    void makeWedge();

    /// <summary>
    /// load json
    /// </summary>
    void makeTest();
    bool loadJson(System::String^ aFilename);

public:
    OccModel* occModel;
    OccContext* occContext;
};
