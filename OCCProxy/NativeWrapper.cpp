#include "NativeWrapper.h"

void OCCT::NativeWrapper::makeCone()
{
	myPtr->makeCone();
}

void OCCT::NativeWrapper::makeTorus()
{
	myPtr->makeTorus();
}

void OCCT::NativeWrapper::makeWedge()
{
	myPtr->makeWedge();
}

OCCT::OCCT_Shape^ OCCT::NativeWrapper::getShape()
{
	::TopoDS_Shape topoShp = myPtr->getShape();
	myShp = gcnew OCCT::OCCT_Shape(topoShp);
	return myShp;
}