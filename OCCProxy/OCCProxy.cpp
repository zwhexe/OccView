#pragma once
#define OCC_DLL_CLASS_EXPORTS
#include <vcclr.h>
#include <windows.h>
#include "OCCView.h"
#include "OccPCH.h"
//!If not include OccShape.hpp, this class cannot be identified in C#???
#include "../OCCNative/NativeShape.h"

//use OCCProxy to process operation
public ref class OCCProxy
{
public:
    OCCProxy()
    {
        occView = gcnew OCCView();
        natShape = new NativeShape();
    }

    void SetView(OCCView^ view)
    {
        occView = view;
    }

    void Display()
    {
        if (!occModel->GetTopoShape().IsNull())
        {
            occView->DisplayShape(occModel->GetTopoShape());
        }
    }

    //NativeShape make shape to OCCT_Shape
    void MakeOctShape()
    {
        natShape->makeCone();
        TopoDS_Shape shp = natShape->getShape();
        octShape = gcnew OCCT::OCCT_Shape(shp);
    }

    //Get OCCT_Shape which wrappered TopoDS_Shape
    OCCT::OCCT_Shape^ GetOctShape()
    {
        return octShape;
    }

public:
    //OCC proxy class
    OCCView^ occView;
    OCCModel^ occModel;
    //OCC dll class
    NativeShape* natShape;

private:
    OCCT::OCCT_Shape^ octShape;
};