#pragma once
#include "OccPCH.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "json.hpp"

using namespace System;
using json = nlohmann::json;


static void MarshalString(String^ s, std::string& os) {
	using namespace Runtime::InteropServices;
	const char* chars =
		(const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
	os = chars;
	Marshal::FreeHGlobal(IntPtr((void*)chars));
}

public value struct HighLowTemp
{
	int high;
	int low;
};

public ref class OCCModel
{
public:
	OCCModel();
	TopoDS_Shape GetShape();

	void MakeBox();

	bool LoadJson(System::String^ theFileName);
	void AnalyzeJson();
	void TestTemp(HighLowTemp temp);
	void TestTempByt(char RecvBuf[1024]);
	void TestTempPtr(IntPtr ptr);

private:
	NCollection_Haft<TopoDS_Shape> topoShp;
	NCollection_Haft<json> mJson;
};

