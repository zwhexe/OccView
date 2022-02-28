#pragma once
#ifndef OCC_DLL_CLASS_EXPORTS
#define OCC_DLL_CLASS __declspec(dllexport)
#else
#define OCC_DLL_CLASS __declspec(dllimport)
#endif
#include "OccPCH.h"
#include "OccView.h"

class OCC_DLL_CLASS OccModel
{
public:
	OccModel();
	void makeCone();
	void makeTorus();
	void makeWedge();

	TopoDS_Shape getShape();

private:
	TopoDS_Shape m_shape;
	OccView* m_view;
};

