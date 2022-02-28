#include "OccModel.h"

OccModel::OccModel()
{
    m_shape.Nullify();
    m_view = new OccView();
}

void OccModel::makeCone()
{
	gp_Ax2 ax2 = gp::XOY();
    m_shape = BRepPrimAPI_MakeCone(ax2, 5, 10, 30);
}

void OccModel::makeTorus()
{
	gp_Ax2 anAxis;
	anAxis.SetLocation(gp_Pnt(0.0, 40.0, 0.0));
    m_shape = BRepPrimAPI_MakeTorus(anAxis, 3.0, 1.0).Shape();
}

void OccModel::makeWedge()
{
    Standard_Real dx = 12.0;
    Standard_Real dy = 8.0;
    Standard_Real dz = 1.5;
    Standard_Real xmin = 3.0;
    Standard_Real zmin = 0.0;
    Standard_Real xmax = 9.0;
    Standard_Real zmax = 1.5;
    Standard_Real ltx = 4.0;

    /* First Wedge method: anAx2, dx, dy, dz, ltx */
    gp_Ax2 anAx2;
    anAx2.SetLocation(gp_Pnt(12.0, 12.0, 0.0));
    TopoDS_Shape aTopoWedge1 = BRepPrimAPI_MakeWedge(anAx2, dx, dy, dz, ltx).Shape();

    /* Second Wedge method: anAx2, dx, dy, dz, xmin, xmax, zmix, zmax */
    anAx2.SetLocation(gp_Pnt(0.0, 0.0, 0.0));
    TopoDS_Shape aTopoWedge2 = BRepPrimAPI_MakeWedge(anAx2, dx, dy, dz, xmin, zmin, xmax, zmax);

    /* Third Wedges : transform wedge2 to fuze*/
    gp_Trsf aTrsf1, aTrsf2; // rotation and translation method
    gp_Ax1 anAx1(gp_Pnt(0.0, 0.0, dz), gp_Dir(1.0, 0.0, 0.0));
    aTrsf1.SetRotation(anAx1, M_PI_2);
    aTrsf2.SetTranslation(gp_Vec(0.0, 5.5, 0.0));

    /* overloaded '*' that transformation matrix to multiply */
    BRepBuilderAPI_Transform aTransform1(aTopoWedge2, aTrsf2 * aTrsf1);
    TopoDS_Shape sheet1 = aTransform1.Shape();

    gp_Trsf aTrsf3; // rotation sheet1 to get sheet2
    gp_Ax1 anAx3(gp_Pnt(6.0, 6.0, 0.0), gp_Dir(0.0, 0.0, 1.0));
    aTrsf3.SetRotation(anAx3, M_PI_2);
    BRepBuilderAPI_Transform aTransform2(sheet1, aTrsf3);
    TopoDS_Shape sheet2 = aTransform2.Shape();

    TopoDS_Shape sheet = BRepAlgoAPI_Fuse(sheet1, sheet2);
    m_shape = sheet;
}

TopoDS_Shape OccModel::getShape()
{
	return m_shape;
}

