#pragma once
#include "json.hpp"
#include "OccPCH.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;
using json = nlohmann::json;


static void MarshalString(String^ s, std::string& os) {
	using namespace Runtime::InteropServices;
	const char* chars = (const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
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
	HighLowTemp TestTempRet();

private:
	NCollection_Haft<TopoDS_Shape> topoShp;
	NCollection_Haft<json> mJson;
};

