#include "NativeShape.h"

NativeShape::NativeShape()
{

}

void NativeShape::makeCone()
{
	gp_Ax2 ax2 = gp::XOY();
	topoShape = BRepPrimAPI_MakeCone(ax2, 5, 10, 30);
}

TopoDS_Shape NativeShape::getShape()
{
	return topoShape;
}
