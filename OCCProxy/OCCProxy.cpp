#pragma once
#define OCC_DLL_CLASS_EXPORTS
#include <d3d9.h>
#include <vcclr.h>
#include <windows.h>
#include "OccPCH.h"
#include "OCCView.h"
#include "OCCModel.h"
#include "../OCCNative/NativeShape.h"

//use OCCProxy to process operation
public ref class OCCProxy
{
public:
    OCCProxy()
    {
        occModel = gcnew OCCModel();
        occView = gcnew OCCView();
        natShape = new NativeShape();
    }

    void Display()
    {
        if (!occModel->GetTopoShape().IsNull())
        {
            occView->DisplayShape(occModel->GetTopoShape());
        }
    }

    void MakeOctShape()
    {
        natShape->makeCone();
        TopoDS_Shape shp = natShape->getShape();
        octShape = gcnew OCCT::TopoDS_Shape(shp);
    }

public:
    //OCC proxy class
    OCCView^ occView;
    OCCModel^ occModel;
    //OCC dll class
    NativeShape* natShape;
    OCCT::TopoDS_Shape^ octShape;
};