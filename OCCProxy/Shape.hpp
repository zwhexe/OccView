#pragma once
#include "OccPCH.h"
#include "OccCLI.h"

namespace OCCT
{
	public ref class OCCT_Shape : public OCCT::OccCLI<::TopoDS_Shape>
	{
	public:
		OCCT_Shape()
			: OCCT::OccCLI<::TopoDS_Shape>()
		{

		}
		
		OCCT_Shape(::TopoDS_Shape* ptr)
			: OCCT::OccCLI<::TopoDS_Shape>(*ptr)
		{

		}

		OCCT_Shape(::TopoDS_Shape& rp)
			: OCCT::OccCLI<::TopoDS_Shape>(rp)
		{

		}

		OCCT_Shape(OCCT::OCCT_Shape^ tp)
			: OCCT::OccCLI<::TopoDS_Shape>(OccCLI::InitMode::Uninitialized)
		{
			myPtr = new ::TopoDS_Shape(*(::TopoDS_Shape*)tp->myPtr);
		}
	};
}
