#pragma once
#include "OccPCH.h"
#include "OccCLI.h"

namespace OCCT
{
	public ref class OCCT_MakeCylinder : public OCCT::OccCLI<::BRepPrimAPI_MakeCylinder>
	{
	public:
		OCCT_MakeCylinder(double R, double H)
			: OCCT::OccCLI<::BRepPrimAPI_MakeCylinder>(OccCLI::InitMode::Uninitialized)
		{
			myPtr = new ::BRepPrimAPI_MakeCylinder(R, H);
		}

		OCCT_MakeCylinder(double R, double H, double Angle)
			: OCCT::OccCLI<::BRepPrimAPI_MakeCylinder>(OccCLI::InitMode::Uninitialized)
		{
			myPtr = new ::BRepPrimAPI_MakeCylinder(R, H, Angle);
		}

		::TopoDS_Shape Shape()
		{
			return myPtr->Shape();
		}
	};
}
