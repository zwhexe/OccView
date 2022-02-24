#include "OCCView.h"
#include <d3d9.h>

OCCView::OCCView()
{
    InitViewer();
}

bool OCCView::InitViewer()
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
    mView()->MustBeResized();
    mView()->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_GOLD, 0.08, V3d_ZBUFFER);

    mAisContext() = new AIS_InteractiveContext(mViewer());
    mAisContext()->SetDisplayMode(AIS_Shaded, Standard_False);
    mAisContext()->Activate(AIS_Shape::SelectionMode(TopAbs_SHAPE), Standard_True);
    mAisContext()->SetPixelTolerance(1);
    mAisContext()->UpdateCurrentViewer();
    mView()->MustBeResized();
    return true;
}

System::IntPtr OCCView:: ResizeBridgeFBO(int theWinSizeX, int theWinSizeY)
{
    Handle(WNT_Window) aWNTWindow = Handle(WNT_Window)::DownCast(mView()->Window());
    aWNTWindow->SetPos(0, 0, theWinSizeX, theWinSizeY);
    mView()->MustBeResized();
    mView()->Invalidate();
    return System::IntPtr(Handle(D3DHost_View)::DownCast(mView()->View())->D3dColorSurface());
}

bool OCCView::InitViewer(System::IntPtr theWnd)
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

bool OCCView::Dump(const TCollection_AsciiString& theFileName)
{
    if (mView().IsNull())
    {
        return false;
    }
    mView()->Redraw();
    return mView()->Dump(theFileName.ToCString()) != Standard_False;
}

void OCCView::RedrawView()
{
    if (!mView().IsNull())
    {
        mView()->Redraw();
    }
}

void OCCView::UpdateView()
{
    if (!mView().IsNull())
    {
        mView()->MustBeResized();
    }
}

void OCCView::SetDegenerateModeOn()
{
    if (!mView().IsNull())
    {
        mView()->SetComputedMode(Standard_False);
        mView()->Redraw();
    }
}

void OCCView::SetDegenerateModeOff()
{
    if (!mView().IsNull())
    {
        mView()->SetComputedMode(Standard_True);
        mView()->Redraw();
    }
}

void OCCView::WindowFitAll(int theXmin, int theYmin, int theXmax, int theYmax)
{
    if (!mView().IsNull())
    {
        mView()->WindowFitAll(theXmin, theYmin, theXmax, theYmax);
    }
}

void OCCView::Place(int theX, int theY, float theZoomFactor)
{
    Standard_Real aZoomFactor = theZoomFactor;
    if (!mView().IsNull())
    {
        mView()->Place(theX, theY, aZoomFactor);
    }
}

void OCCView::Zoom(int theX1, int theY1, int theX2, int theY2)
{
    if (!mView().IsNull())
    {
        mView()->Zoom(theX1, theY1, theX2, theY2);
    }
}

void OCCView::Pan(int theX, int theY)
{
    if (!mView().IsNull())
    {
        mView()->Pan(theX, theY);
    }
}

void OCCView::Rotation(int theX, int theY)
{
    if (!mView().IsNull())
    {
        mView()->Rotation(theX, theY);
    }
}

void OCCView::StartRotation(int theX, int theY)
{
    if (!mView().IsNull())
    {
        mView()->StartRotation(theX, theY);
    }
}

void OCCView::Select(int theX1, int theY1, int theX2, int theY2)
{
    if (!mAisContext().IsNull())
    {
        mAisContext()->Select(theX1, theY1, theX2, theY2, mView(), Standard_True);
    }
}

void OCCView::Select()
{
    if (!mAisContext().IsNull())
    {
        mAisContext()->Select(Standard_True);
    }
}

void OCCView::MoveTo(int theX, int theY)
{
    if (!mAisContext().IsNull() && !mView().IsNull())
    {
        mAisContext()->MoveTo(theX, theY, mView(), Standard_True);
    }
}

void OCCView::BackgroundColor(int& theR, int& theG, int& theB)
{
    if (!mView().IsNull())
    {
        Quantity_Color aColor = mView()->BackgroundColor();
        theR = (int)aColor.Red() * 255;
        theG = (int)aColor.Green() * 255;
        theB = (int)aColor.Blue() * 255;
    }
}

void OCCView::UpdateCurrentViewer()
{
    if (!mAisContext().IsNull())
    {
        mAisContext()->UpdateCurrentViewer();
    }
}

float OCCView::Scale()
{
    return mView().IsNull() ? -1.0f : float(mView()->Scale());
}

void OCCView::ZoomAllView()
{
    if (!mView().IsNull())
    {
        mView()->FitAll();
        mView()->ZFitAll();
    }
}

void OCCView::Reset()
{
    if (mView().IsNull())
    {
        mView()->Reset();
    }
}

void OCCView::SetDisplayMode(int theMode)
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

float OCCView::GetOCCVersion()
{
    return (float)OCC_VERSION;
}

void OCCView::DisplayShape(TopoDS_Shape& theShp)
{
    Handle(AIS_Shape) aisShp = new AIS_Shape(theShp);
    GetContext()->Display(aisShp, Standard_True);
}

void OCCView::DisplayShape(OCCT::OCCT_Shape^ octShp)
{
    Handle(AIS_Shape) aisShp = new AIS_Shape(octShp());
    GetContext()->Display(aisShp, Standard_True);
}

bool OCCView::SetAisContext(OCCView^ theViewer)
{
    this->mAisContext() = theViewer->GetContext();
    if (mAisContext().IsNull())
    {
        return false;
    }
    return true;
}

Handle(AIS_InteractiveContext) OCCView::GetContext()
{
    return mAisContext();
}

bool OCCView::ImportBrep(System::String^ theFileName)
{
    return ImportBrep(toAsciiString(theFileName));
}

bool OCCView::ImportBrep(const TCollection_AsciiString& theFileName)
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


bool OCCView::ImportStep(const TCollection_AsciiString& theFileName)
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

bool OCCView::ImportIges(const TCollection_AsciiString& theFileName)
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

bool OCCView::TranslateModel(System::String^ theFileName, int theFormat, bool theIsImport)
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

void OCCView::InitOCCView()
{
    mView().Nullify();
    mViewer().Nullify();
    mAisContext().Nullify();
    mOpenglDriver().Nullify();
    mGraphicDriver().Nullify();
}
