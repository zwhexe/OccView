#include "OccPCH.h"
#include "OCCModel.h"
#include <iostream>
#include <chrono>

OCCModel::OCCModel()
{
	
}

TopoDS_Shape OCCModel::GetTopoShape()
{
	return occShp();
}

void OCCModel::MakeBox()
{
	gp_Pln pln(gp_Pnt(0.0, 0.0, 30.0), gp_Dir(0, 0, 1));
	TopoDS_Shape aTopoFace = BRepBuilderAPI_MakeFace(pln, 0, 30, 0, 40).Shape();
	gp_Trsf trsf;
	trsf.SetRotation(gp_Ax1(gp_Pnt(0.0, 0.0, 30.0), gp::DX()), -M_PI_4);
	BRepBuilderAPI_Transform brepTrsf(aTopoFace, trsf);
	aTopoFace = brepTrsf.Shape();
	TopoDS_Shape aTopoBox = BRepPrimAPI_MakePrism(aTopoFace, gp_Vec(0.0, -20.0, -20.0));
	occShp() = aTopoBox;
}

void OCCModel::MakeSphere()
{
	gp_Pnt pnt(0, 0, 0);
	TopoDS_Shape aShpere = ::BRepPrimAPI_MakeSphere(pnt, 30).Shape();
	occShp() = aShpere;
}

void OCCModel::MakeCylinder()
{
	double R = 10;
	double H = 30;
	
	occShp() = OCCT::BRepPrimAPI_MakeCylinder(R, H).Shape();
}
