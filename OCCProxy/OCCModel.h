#pragma once
#include "OccPCH.h"
#include "OccCLI.h"
#include "BRep.hpp"
#include "Shape.hpp"

public ref class OCCModel
{
public:
	OCCModel();
	TopoDS_Shape GetTopoShape();

	//make shape by OCC native
	void MakeBox();
	void MakeSphere();
	//make shape by OccCLI proxy
	void MakeCylinder();

private:
	OCCT::OccCLI<::TopoDS_Shape> occShp;
};

