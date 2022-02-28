#include "OccView.h"
#include <Aspect_Handle.hxx>
#include <Aspect_DisplayConnection.hxx>

OccView::OccView()
{
    InitViewer();
}

bool OccView::InitViewer()
{
    mGraphicDriver = new D3DHost_GraphicDriver();
    mGraphicDriver->ChangeOptions().buffersNoSwap = true;
    //mGraphicDriver()->ChangeOptions().contextDebug = true;
    mViewer = new V3d_Viewer(mGraphicDriver);
    mViewer->SetDefaultLights();
    mViewer->SetLightOn();
    mView = mViewer->CreateView();

    static Handle(WNT_WClass) aWClass = new WNT_WClass("OCC_Viewer", NULL, CS_OWNDC);
    Handle(WNT_Window) aWNTWindow = new WNT_Window("OCC_Viewer", aWClass, WS_POPUP, 64, 64, 64, 64);
    aWNTWindow->SetVirtual(Standard_True);
    mView->SetWindow(aWNTWindow);
    mView->MustBeResized();
    mView->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_GOLD, 0.08, V3d_ZBUFFER);

    mAisContext = new AIS_InteractiveContext(mViewer);
    mAisContext->SetDisplayMode(AIS_Shaded, Standard_False);
    mAisContext->Activate(AIS_Shape::SelectionMode(TopAbs_SHAPE), Standard_True);
    mAisContext->SetPixelTolerance(1);
    mAisContext->UpdateCurrentViewer();
    mView->MustBeResized();
    return true;
}

bool OccView::InitViewer(Handle(WNT_Window) aWNTWindow)
{
    try
    {
        Handle(Aspect_DisplayConnection) aDisplayConnection;
        mOpenglDriver = new OpenGl_GraphicDriver(aDisplayConnection);
    }
    catch (Standard_Failure)
    {
        return false;
    }

    mViewer = new V3d_Viewer(mOpenglDriver);
    mViewer->SetDefaultLights();
    mViewer->SetLightOn();
    mView = mViewer->CreateView();
    mView->SetWindow(aWNTWindow);
    if (!aWNTWindow->IsMapped())
    {
        aWNTWindow->Map();
    }
    mAisContext = new AIS_InteractiveContext(mViewer);
    mAisContext->UpdateCurrentViewer();
    mView->Redraw();
    mView->MustBeResized();
    return true;
}

Handle(V3d_View) OccView::ResizeBridgeFBO(int theWinSizeX, int theWinSizeY)
{
    Handle(WNT_Window) aWNTWindow = Handle(WNT_Window)::DownCast(mView->Window());
    aWNTWindow->SetPos(0, 0, theWinSizeX, theWinSizeY);
    mView->MustBeResized();
    mView->Invalidate();
    return mView;
}


