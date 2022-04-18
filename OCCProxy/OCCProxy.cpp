#include "OccProxy.h"

OccProxy::OccProxy()
{
    occModel = new OccModel();
    occContext = new OccContext();
}

/// <summary>
/// view manipulate
/// </summary>
/// <returns></returns>
bool OccProxy::InitViewer()
{
    return occContext->InitViewer();
}

void OccProxy::ResizeBridgeFBO(int theWinSizeX, int theWinSizeY)
{
    occContext->SetPos(theWinSizeX, theWinSizeY);
}

System::IntPtr OccProxy::GetD3dSurface()
{
    return System::IntPtr(occContext->GetD3dView()->D3dColorSurface());
}

void OccProxy::RedrawView()
{
    occContext->RedrawView();
}

void OccProxy::ZoomAllView()
{
    occContext->ZoomAllView();
}

void OccProxy::SetDisplayMode(int theMode)
{
    occContext->SetDisplayMode(theMode);
}

float OccProxy::Scale()
{
    return occContext->Scale();
}

void OccProxy::Zoom(int theX1, int theY1, int theX2, int theY2)
{
    occContext->Zoom(theX1, theY1, theX2, theY2);
}

void OccProxy::Rotation(int x, int y)
{
    occContext->Rotation(x, y);
}

void OccProxy::Pan(int x, int y)
{
    occContext->Pan(x, y);
}

void OccProxy::MoveTo(int x, int y)
{
    occContext->MoveTo(x, y);
}

void OccProxy::StartRotation(int x, int y)
{
    occContext->StartRotation(x, y);
}

bool OccProxy::TranslateModel(System::String^ aFilename, int format, bool update)
{
    std::string filename = msclr::interop::marshal_as<std::string>(aFilename);
    bool res;
    switch (format)
    {
    case 0:
        res = occContext->ImportBrep(filename); break;
    case 1:
        res = occContext->ImportStep(filename); break;
    case 2:
        res = occContext->ImportIges(filename); break;
    }

    if (update)
        occContext->UpdateCurrentViewer();
    return res;
}

/// <summary>
/// make shape
/// </summary>
void OccProxy::makeCone()
{
    occModel->makeCone();
}

void OccProxy::makeTorus()
{
    occModel->makeTorus();
}

void OccProxy::makeWedge()
{
    occModel->makeWedge();
}

/// <summary>
/// load jsoncpp
/// </summary>
void OccProxy::makeTest()
{
    occModel->makeTest();
}

bool OccProxy::loadJson(System::String^ aFilename)
{
    std::string filename = msclr::interop::marshal_as<std::string>(aFilename);
    return occModel->loadJson(filename);
}
