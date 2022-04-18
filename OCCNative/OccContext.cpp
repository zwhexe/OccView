#include "OccContext.h"

OccContext::OccContext()
{

}

bool OccContext::InitViewer()
{
	mGraphicDriver = new D3DHost_GraphicDriver();
	mGraphicDriver->ChangeOptions().buffersNoSwap = true;
	mGraphicDriver->ChangeOptions().contextDebug = false;

	mViewer = new V3d_Viewer(mGraphicDriver);
	mViewer->SetDefaultLights();
	mViewer->SetLightOn();
	mView = mViewer->CreateView();

	static Handle(WNT_WClass) aWClass = new WNT_WClass("OCC_WNT", NULL, CS_OWNDC);
	Handle(WNT_Window) aWNTWindow = new WNT_Window("OCC_WNT", aWClass, WS_POPUP, 64, 64, 64, 64);
	aWNTWindow->SetVirtual(Standard_True);
	mView->SetWindow(aWNTWindow);
	mView->MustBeResized();
	mView->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_GOLD, 0.08, V3d_ZBUFFER);

	gAisContext = new AIS_InteractiveContext(mViewer);
	gAisContext->SetDisplayMode(AIS_Shaded, Standard_False);
	gAisContext->Activate(AIS_Shape::SelectionMode(TopAbs_SHAPE), Standard_True);
	gAisContext->SetPixelTolerance(1);
	gAisContext->UpdateCurrentViewer();
	mView->MustBeResized();
	return true;
}

void OccContext::SetPos(int winSizeX, int winSizeY)
{
	Handle(WNT_Window) aWNTWindow = Handle(WNT_Window)::DownCast(mView->Window());
	aWNTWindow->SetPos(0, 0, winSizeX, winSizeY);
	mView->MustBeResized();
	mView->Invalidate();
}

bool OccContext::InitViewer(void* ptr)
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
	Handle(WNT_Window) aWNTWindow = new WNT_Window(reinterpret_cast<HWND> (ptr));

	mView->SetWindow(aWNTWindow);
	if (!aWNTWindow->IsMapped())
	{
		aWNTWindow->Map();
	}
	gAisContext = new AIS_InteractiveContext(mViewer);
	gAisContext->UpdateCurrentViewer();
	mView->Redraw();
	mView->MustBeResized();
	return true;
}

Handle(D3DHost_View) OccContext::GetD3dView()
{
	return Handle(D3DHost_View)::DownCast(mView->View());
}

void OccContext::Display(Handle(AIS_InteractiveObject)& theIObj, Standard_Boolean theToUpdateViewer)
{
	gAisContext->Display(theIObj, theToUpdateViewer);
}

bool OccContext::Dump(const std::string& theFileName)
{
	if (mView.IsNull())
	{
		return false;
	}
	mView->Redraw();
	return mView->Dump(theFileName.c_str()) != Standard_False;
}

void OccContext::RedrawView()
{
	if (mView.IsNull())
		mView->Redraw();
}

void OccContext::UpdateView()
{
	if (mView.IsNull())
		mView->MustBeResized();
}

void OccContext::SetDegenerateModeOn()
{
	if (!mView.IsNull())
	{
		mView->SetComputedMode(Standard_False);
		mView->Redraw();
	}
}

void OccContext::SetDegenerateModeOff()
{
	if (!mView.IsNull())
	{
		mView->SetComputedMode(Standard_True);
		mView->Redraw();
	}
}

void OccContext::WindowFitAll(int theXmin, int theYmin, int theXmax, int theYmax)
{
	if (!mView.IsNull())
	{
		mView->WindowFitAll(theXmin, theYmin, theXmax, theYmax);
	}
}

void OccContext::Place(int theX, int theY, float theZoomFactor)
{
	Standard_Real aZoomFactor = theZoomFactor;
	if (!mView.IsNull())
	{
		mView->Place(theX, theY, aZoomFactor);
	}
}

void OccContext::Zoom(int theX1, int theY1, int theX2, int theY2)
{
	if (!mView.IsNull())
	{
		mView->Zoom(theX1, theY1, theX2, theY2);
	}
}

void OccContext::Pan(int theX, int theY)
{
	if (!mView.IsNull())
	{
		mView->Pan(theX, theY);
	}
}

void OccContext::Rotation(int theX, int theY)
{
	if (!mView.IsNull())
	{
		mView->Rotation(theX, theY);
	}
}

void OccContext::StartRotation(int theX, int theY)
{
	if (!mView.IsNull())
	{
		mView->StartRotation(theX, theY);
	}
}

void OccContext::Select()
{
	if (gAisContext.IsNull())
		gAisContext->Select(Standard_True);
}

void OccContext::Select(int X1, int Y1, int X2, int Y2)
{
	if (gAisContext.IsNull())
		gAisContext->Select(X1, Y1, X2, Y2, mView, Standard_True);
}

void OccContext::MoveTo(int X, int Y)
{
	if (!gAisContext.IsNull() && !mView.IsNull())
		gAisContext->MoveTo(X, Y, mView, Standard_True);
}

void OccContext::BackgroundColor(int& theR, int& theG, int& theB)
{
	if (!mView.IsNull())
	{
		Quantity_Color aColor = mView->BackgroundColor();
		theR = (int)aColor.Red() * 255;
		theG = (int)aColor.Green() * 255;
		theB = (int)aColor.Blue() * 255;
	}
}

void OccContext::UpdateCurrentViewer()
{
	if (gAisContext.IsNull())
		gAisContext->UpdateCurrentViewer();
}

float OccContext::Scale()
{
	return mView.IsNull() ? -1.0f : float(mView->Scale());
}

void OccContext::ZoomAllView()
{
	if (!mView.IsNull())
	{
		mView->FitAll();
		mView->ZFitAll();
	}
}

void OccContext::Reset()
{
	if (mView.IsNull())
	{
		mView->Reset();
	}
}

void OccContext::SetDisplayMode(int Mode)
{
	if (gAisContext.IsNull())
	{
		return;
	}

	AIS_DisplayMode aCurrentMode = Mode == 0 ? AIS_WireFrame : AIS_Shaded;
	if (gAisContext->NbSelected() == 0)
	{
		gAisContext->SetDisplayMode(aCurrentMode, Standard_False);
	}
	else
	{
		for (gAisContext->InitSelected(); gAisContext->MoreSelected(); gAisContext->NextSelected())
		{
			gAisContext->SetDisplayMode(gAisContext->SelectedInteractive(), Mode, Standard_False);
		}
	}
	gAisContext->UpdateCurrentViewer();
}

bool OccContext::SetAisContext(Handle(AIS_InteractiveContext) aisContext)
{
	gAisContext = aisContext;
	if (gAisContext.IsNull())
		return false;
	return true;
}

Handle(AIS_InteractiveContext) OccContext::GetContext()
{
	return gAisContext;
}

bool OccContext::ImportBrep(const std::string& theFileName)
{
	TopoDS_Shape aShape;
	BRep_Builder aBuilder;
	if (!BRepTools::Read(aShape, theFileName.c_str(), aBuilder))
	{
		return false;
	}

	Handle(AIS_Shape) aPrs = new AIS_Shape(aShape);
	gAisContext->SetMaterial(aPrs, Graphic3d_NameOfMaterial_Gold, Standard_False);
	gAisContext->SetDisplayMode(aPrs, AIS_Shaded, Standard_False);
	gAisContext->Display(aPrs, Standard_True);
	return true;
}

bool OccContext::ImportIges(const std::string& theFileName)
{
	IGESControl_Reader aReader;
	if (aReader.ReadFile(theFileName.c_str()) != IFSelect_RetDone)
	{
		return false;
	}

	aReader.TransferRoots();
	TopoDS_Shape aShape = aReader.OneShape();
	gAisContext->Display(new AIS_Shape(aShape), Standard_False);
	gAisContext->UpdateCurrentViewer();
	return true;
}

bool OccContext::ImportStep(const std::string& theFileName)
{
	STEPControl_Reader aReader;
	if (aReader.ReadFile(theFileName.c_str()) != IFSelect_RetDone)
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
				gAisContext->Display(new AIS_Shape(aReader.Shape(aShapeIter)), Standard_False);
			}
			gAisContext->UpdateCurrentViewer();
		}
	}
	return true;
}

Handle(AIS_InteractiveContext) gAisContext = NULL;
