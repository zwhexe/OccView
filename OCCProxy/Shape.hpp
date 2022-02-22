#pragma once
#include "OccPCH.h"
#include "OccCLI.h"

namespace OCCT
{
	public ref class TopoDS_Shape : public OCCT::OccCLI<::TopoDS_Shape>
	{
	public:
		TopoDS_Shape() 
			: OCCT::OccCLI<::TopoDS_Shape>()
		{

		}
		
		TopoDS_Shape(::TopoDS_Shape* ptr) 
			: OCCT::OccCLI<::TopoDS_Shape>(*ptr)
		{

		}

		TopoDS_Shape(::TopoDS_Shape& rp)
			: OCCT::OccCLI<::TopoDS_Shape>(rp)
		{

		}

		TopoDS_Shape(OCCT::TopoDS_Shape^ tp) 
			: OCCT::OccCLI<::TopoDS_Shape>(OccCLI::InitMode::Uninitialized)
		{
			myPtr = new ::TopoDS_Shape(*(::TopoDS_Shape*)tp->myPtr);
		}

	};
}
