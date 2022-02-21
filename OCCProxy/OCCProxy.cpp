#include <d3d9.h>
#include <vcclr.h>
#include <windows.h>
#include "OccPCH.h"
#include "OCCView.h"
#include "OCCModel.h"


public ref class OCCProxy
{
public:
    OCCProxy()
    {
        occModel = gcnew OCCModel();
        occView = gcnew OCCView();
    }

    void Display()
    {
        if (!occModel->GetShape().IsNull())
        {
            occView->DisplayShape(occModel->GetShape());
        }
    }

public:
    OCCView^ occView;
    OCCModel^ occModel;
};