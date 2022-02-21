#include "OccPCH.h"
#include "OCCModel.h"
#include <chrono>

OCCModel::OCCModel()
{
	
}

TopoDS_Shape OCCModel::GetShape()
{
	return topoShp();
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
	topoShp() = aTopoBox;
}

bool OCCModel::LoadJson(System::String^ theFileName)
{
	std::string filename;
	MarshalString(theFileName, filename);	
	std::ifstream jfile(filename);	

	auto t1 = std::chrono::steady_clock::now();
	mJson() = json::parse(jfile);
	auto t2 = std::chrono::steady_clock::now();
	double dur = std::chrono::duration<double, std::milli>(t2 - t1).count();
	std::cout << "nlohmann::LoadJson() size " << mJson().size() << " cost " << dur << " ms" << std::endl;

	return true;
}

void OCCModel::AnalyzeJson()
{
	if (mJson().empty())
		return;
	for (auto j : mJson())
	{
		std::cout << j << std::endl;
	}
}

void OCCModel::TestTemp(HighLowTemp temp)
{
	std::cout << temp.high << " " << temp.low << std::endl;
}

void OCCModel::TestTempByt(char RecvBuf[1024])
{
	HighLowTemp temp;
	temp = *(HighLowTemp*)&RecvBuf;
	std::cout << temp.high << " " << temp.low << std::endl;
}

void OCCModel::TestTempPtr(IntPtr ptr)
{
	HighLowTemp* tp = static_cast<HighLowTemp*>(ptr.ToPointer());
	std::cout << tp->high << " " << tp->low << std::endl;
}

HighLowTemp OCCModel::TestTempRet()
{
	HighLowTemp* temp = new HighLowTemp;
	temp->high = 39;
	temp->low = 19;
	return *temp;
}

