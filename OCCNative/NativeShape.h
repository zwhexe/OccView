#pragma once
#ifndef OCC_DLL_CLASS_EXPORTS
#define OCC_DLL_CLASS __declspec(dllexport)
#else
#define OCC_DLL_CLASS __declspec(dllimport)
#endif
#include <gp_Ax2.hxx>
#include <TopoDS_Shape.hxx>
#include <BRepPrimAPI_MakeCone.hxx>

class OCC_DLL_CLASS NativeShape
{
public:
	NativeShape();
	void makeCone();
	TopoDS_Shape getShape();

private:
	TopoDS_Shape topoShape;
};

