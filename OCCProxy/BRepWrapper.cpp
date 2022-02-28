#include "BRepWrapper.h"

void OCCT::BRepWrapper::makeCone()
{
	myPtr->makeCone();
}

void OCCT::BRepWrapper::makeTorus()
{
	myPtr->makeTorus();
}

void OCCT::BRepWrapper::makeWedge()
{
	myPtr->makeWedge();
}

OCCT::OCCT_Shape^ OCCT::BRepWrapper::getShape()
{
	::TopoDS_Shape topoShp = myPtr->getShape();
	myShp = gcnew OCCT::OCCT_Shape(topoShp);
	return myShp;
}