#pragma once
#define OCC_DLL_CLASS_EXPORTS
#include <vcclr.h>
#include <windows.h>
#include "OccCLI.h"
#include "Shape.hpp"
#include "../OCCNative/NativeShape.h"

namespace OCCT
{
	public ref class NativeWrapper : public OCCT::OccCLI<NativeShape>
	{
	public:
		NativeWrapper() : OCCT::OccCLI<NativeShape>()
		{

		}

		void makeCone();
		void makeTorus();
		void makeWedge();
		OCCT::OCCT_Shape^ getShape();

	private:
		OCCT::OCCT_Shape^ myShp;
	};
}
