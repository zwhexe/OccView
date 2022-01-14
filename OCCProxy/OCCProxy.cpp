#include <d3d9.h>
#include <windows.h>

// include required OCCT headers
#include <Standard_Version.hxx>
#include <Message_ProgressIndicator.hxx>
#include <Message_ProgressScope.hxx>
//for OCC graphic
#include <WNT_Window.hxx>
#include <WNT_WClass.hxx>
#include <Graphic3d_CView.hxx>
#include <Graphic3d_Camera.hxx>
#include <Graphic3d_TextureParams.hxx>
#include <D3DHost_GraphicDriver.hxx>
#include <D3DHost_View.hxx>
//for object display
#include <V3d_Viewer.hxx>
#include <V3d_View.hxx>
#include <AIS_InteractiveContext.hxx>
#include <AIS_Shape.hxx>
//topology
#include <TopoDS_Shape.hxx>
#include <TopoDS_Compound.hxx>
//brep tools
#include <BRep_Builder.hxx>
#include <BRepTools.hxx>
// iges I/E
#include <IGESControl_Reader.hxx>
#include <IGESControl_Controller.hxx>
#include <IGESControl_Writer.hxx>
#include <IFSelect_ReturnStatus.hxx>
#include <Interface_Static.hxx>
//step I/E
#include <STEPControl_Reader.hxx>
#include <STEPControl_Writer.hxx>
//for stl export
#include <StlAPI_Writer.hxx>
//for vrml export
#include <VrmlAPI_Writer.hxx>
//wrapper of pure C++ classes to ref classes
#include <NCollection_Haft.h>

#include <vcclr.h>

// list of required OCCT libraries
// seems like this part cannot be missed even I have add to Config
#pragma comment(lib, "TKernel.lib")
#pragma comment(lib, "TKMath.lib")
#pragma comment(lib, "TKBRep.lib")
#pragma comment(lib, "TKXSBase.lib")
#pragma comment(lib, "TKService.lib")
#pragma comment(lib, "TKV3d.lib")
#pragma comment(lib, "TKOpenGl.lib")
#pragma comment(lib, "TKD3dHost.lib")
#pragma comment(lib, "TKIGES.lib")
#pragma comment(lib, "TKSTEP.lib")
#pragma comment(lib, "TKStl.lib")
#pragma comment(lib, "TKVrml.lib")
#pragma comment(lib, "TKLCAF.lib")
#pragma comment(lib, "D3D9.lib")

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

public ref class OCCProxy
{
public:
    OCCProxy() {}

    //This render by Direct3D
    bool InitViewer()
    {
        mGraphicDriver() = new D3DHost_GraphicDriver();
        mGraphicDriver()->ChangeOptions().buffersNoSwap = true;
        //mGraphicDriver()->ChangeOptions().contextDebug = true;
        mViewer() = new V3d_Viewer(mGraphicDriver());
        mViewer()->SetDefaultLights();
        mViewer()->SetLightOn();
        mView() = mViewer()->CreateView();

        static Handle(WNT_WClass) aWClass = new WNT_WClass("OCC_Viewer", NULL, CS_OWNDC);
        Handle(WNT_Window) aWNTWindow = new WNT_Window("OCC_Viewer", aWClass, WS_POPUP, 64, 64, 64, 64);
        aWNTWindow->SetVirtual(Standard_True);
        mView()->SetWindow(aWNTWindow);

        mAisContext() = new AIS_InteractiveContext(mViewer());
        mAisContext()->UpdateCurrentViewer();
        mView()->MustBeResized();
        return true;
    }

    //Resize custom FBO(Frame Buffer Object) for Direct3D output
    System::IntPtr ResizeBridgeFBO(int theWinSizeX, int theWinSizeY)
    {
        Handle(WNT_Window) aWNTWindow = Handle(WNT_Window)::DownCast(mView()->Window());
        aWNTWindow->SetPos(0, 0, theWinSizeX, theWinSizeY);
        mView()->MustBeResized();
        mView()->Invalidate();
        return System::IntPtr(Handle(D3DHost_View)::DownCast(mView()->View())->D3dColorSurface());
    }

    //This render by WinForm
    bool InitViewer(System::IntPtr theWnd)
    {
        try
        {
            Handle(Aspect_DisplayConnection) aDisplayConnection;
            mOpenglDriver() = new OpenGl_GraphicDriver(aDisplayConnection);
        }
        catch (Standard_Failure)
        {
            return false;
        }

        mViewer() = new V3d_Viewer(mOpenglDriver());
        mViewer()->SetDefaultLights();
        mViewer()->SetLightOn();
        mView() = mViewer()->CreateView();
        Handle(WNT_Window) aWNTWindow = new WNT_Window(reinterpret_cast<HWND> (theWnd.ToPointer()));
        mView()->SetWindow(aWNTWindow);
        if (!aWNTWindow->IsMapped())
        {
            aWNTWindow->Map();
        }
        mAisContext() = new AIS_InteractiveContext(mViewer());
        mAisContext()->UpdateCurrentViewer();
        mView()->Redraw();
        mView()->MustBeResized();
        return true;
    }

    bool Dump(const TCollection_AsciiString& theFileName)
    {
        if (mView().IsNull())
        {
            return false;
        }
        mView()->Redraw();
        return mView()->Dump(theFileName.ToCString()) != Standard_False;
    }

    void RedrawView()
    {
        if (!mView().IsNull())
        {
            mView()->Redraw();
        }
    }

    void UpdateView()
    {
        if (!mView().IsNull())
        {
            mView()->MustBeResized();
        }
    }

    void SetDegenerateModeOn()
    {
        if (!mView().IsNull())
        {
            mView()->SetComputedMode(Standard_False);
            mView()->Redraw();
        }
    }

    void SetDegenerateModeOff()
    {
        if (!mView().IsNull())
        {
            mView()->SetComputedMode(Standard_True);
            mView()->Redraw();
        }
    }

    void WindowFitAll(int theXmin, int theYmin, int theXmax, int theYmax)
    {
        if (!mView().IsNull())
        {
            mView()->WindowFitAll(theXmin, theYmin, theXmax, theYmax);
        }
    }

    void Place(int theX, int theY, float theZoomFactor)
    {
        Standard_Real aZoomFactor = theZoomFactor;
        if (!mView().IsNull())
        {
            mView()->Place(theX, theY, aZoomFactor);
        }
    }

    void Zoom(int theX1, int theY1, int theX2, int theY2)
    {
        if (!mView().IsNull())
        {
            mView()->Zoom(theX1, theY1, theX2, theY2);
        }
    }

    void Pan(int theX, int theY)
    {
        if (!mView().IsNull())
        {
            mView()->Pan(theX, theY);
        }
    }

    void Rotation(int theX, int theY)
    {
        if (!mView().IsNull())
        {
            mView()->Rotation(theX, theY);
        }
    }

    void StartRotation(int theX, int theY)
    {
        if (!mView().IsNull())
        {
            mView()->StartRotation(theX, theY);
        }
    }

    void Select(int theX1, int theY1, int theX2, int theY2)
    {
        if (!mAisContext().IsNull())
        {
            mAisContext()->Select(theX1, theY1, theX2, theY2, mView(), Standard_True);
        }
    }

    void Select()
    {
        if (!mAisContext().IsNull())
        {
            mAisContext()->Select(Standard_True);
        }
    }

    void MoveTo(int theX, int theY)
    {
        if (!mAisContext().IsNull() && !mView().IsNull())
        {
            mAisContext()->MoveTo(theX, theY, mView(), Standard_True);
        }
    }

    void BackgroundColor(int& theR, int& theG, int& theB)
    {
        if (!mView().IsNull())
        {
            Quantity_Color aColor = mView()->BackgroundColor();
            theR = (int)aColor.Red() * 255;
            theG = (int)aColor.Green() * 255;
            theB = (int)aColor.Blue() * 255;
        }
    }

    void UpdateCurrentViewer()
    {
        if (!mAisContext().IsNull())
        {
            mAisContext()->UpdateCurrentViewer();
        }
    }

    float Scale()
    {
        return mView().IsNull() ? -1.0f : float(mView()->Scale());
    }

    void ZoomAllView()
    {
        if (!mView().IsNull())
        {
            mView()->FitAll();
            mView()->ZFitAll();
        }
    }

    void Reset()
    {
        if (mView().IsNull())
        {
            mView()->Reset();
        }
    }

    void SetDisplayMode(int theMode)
    {
        if (mAisContext().IsNull())
        {
            return;
        }

        AIS_DisplayMode aCurrentMode = theMode == 0 ? AIS_WireFrame : AIS_Shaded;
        if (mAisContext()->NbSelected() == 0)
        {
            mAisContext()->SetDisplayMode(aCurrentMode, Standard_False);
        }
        else
        {
            for (mAisContext()->InitSelected(); mAisContext()->MoreSelected(); mAisContext()->NextSelected())
            {
                mAisContext()->SetDisplayMode(mAisContext()->SelectedInteractive(), theMode, Standard_False);
            }
        }
        mAisContext()->UpdateCurrentViewer();
    }

    float GetOCCVersion()
    {
        return (float)OCC_VERSION;
    }

    bool SetAisContext(OCCProxy^ theViewer)
    {
        this->mAisContext() = theViewer->GetContext();
        if (mAisContext().IsNull())
        {
            return false;
        }
        return true;
    }

    Handle(AIS_InteractiveContext) GetContext()
    {
        return mAisContext();
    }

public:
    bool ImportBrep(System::String^ theFileName)
    {
        return ImportBrep(toAsciiString(theFileName));
    }

    bool ImportBrep(const TCollection_AsciiString& theFileName)
    {
        TopoDS_Shape aShape;
        BRep_Builder aBuilder;
        if (!BRepTools::Read(aShape, theFileName.ToCString(), aBuilder))
        {
            return false;
        }

        Handle(AIS_Shape) aPrs = new AIS_Shape(aShape);
        mAisContext()->SetMaterial(aPrs, Graphic3d_NameOfMaterial_Gold, Standard_False);
        mAisContext()->SetDisplayMode(aPrs, AIS_Shaded, Standard_False);
        mAisContext()->Display(aPrs, Standard_True);
        return true;
    }


    bool ImportStep(const TCollection_AsciiString& theFileName)
    {
        STEPControl_Reader aReader;
        if (aReader.ReadFile(theFileName.ToCString()) != IFSelect_RetDone)
        {
            return false;
        }

        bool isFailsonly = false;
        aReader.PrintCheckLoad(isFailsonly, IFSelect_ItemsByEntity);

        int aNbRoot = aReader.NbRootsForTransfer();
        aReader.PrintCheckTransfer(isFailsonly, IFSelect_ItemsByEntity);
        for (Standard_Integer aRootIter = 1; aRootIter <= aNbRoot; ++aRootIter)
        {
            aReader.TransferRoot(aRootIter);
            int aNbShap = aReader.NbShapes();
            if (aNbShap > 0)
            {
                for (int aShapeIter = 1; aShapeIter <= aNbShap; ++aShapeIter)
                {
                    mAisContext()->Display(new AIS_Shape(aReader.Shape(aShapeIter)), Standard_False);
                }
                mAisContext()->UpdateCurrentViewer();
            }
        }
        return true;
    }

    bool ImportIges(const TCollection_AsciiString& theFileName)
    {
        IGESControl_Reader aReader;
        if (aReader.ReadFile(theFileName.ToCString()) != IFSelect_RetDone)
        {
            return false;
        }

        aReader.TransferRoots();
        TopoDS_Shape aShape = aReader.OneShape();
        mAisContext()->Display(new AIS_Shape(aShape), Standard_False);
        mAisContext()->UpdateCurrentViewer();
        return true;
    }

    bool TranslateModel(System::String^ theFileName, int theFormat, bool theIsImport)
    {
        bool  isResult = false;
        const TCollection_AsciiString aFilename = toAsciiString(theFileName);
        if (theIsImport)
        {
            switch (theFormat)
            {
            case 0: isResult = ImportBrep(aFilename); break;
            case 1: isResult = ImportStep(aFilename); break;
            case 2: isResult = ImportIges(aFilename); break;
            }
        }

        return isResult;
    }

    void InitOCCProxy()
    {
        mViewer().Nullify();
        mView().Nullify();
        mAisContext().Nullify();
        mOpenglDriver().Nullify();
        mGraphicDriver().Nullify();
    }

private:
    NCollection_Haft<Handle(V3d_Viewer)> mViewer;
    NCollection_Haft<Handle(V3d_View)> mView;
    NCollection_Haft<Handle(AIS_InteractiveContext)> mAisContext;
    NCollection_Haft<Handle(OpenGl_GraphicDriver)> mOpenglDriver;
    NCollection_Haft<Handle(D3DHost_GraphicDriver)> mGraphicDriver;
};