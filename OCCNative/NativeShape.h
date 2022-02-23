#pragma once
#ifndef OCC_DLL_CLASS_EXPORTS
#define OCC_DLL_CLASS __declspec(dllexport)
#else
#define OCC_DLL_CLASS __declspec(dllimport)
#endif
#include "NativePCH.h"

class OCC_DLL_CLASS NativeShape
{
public:
	NativeShape();
	void makeCone();
	void makeTorus();
	void makeWedge();

	TopoDS_Shape getShape();

private:
	TopoDS_Shape topoShape;
};

