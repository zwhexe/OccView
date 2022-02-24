#pragma once
#include "OccPCH.h"
#include "OccCLI.h"
#include "json.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;
using json = nlohmann::json;


static void MarshalString(String^ s, std::string& os) {
	//convert String^ to std::string
	const char* chars = (const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
	os = chars;
	Marshal::FreeHGlobal(IntPtr((void*)chars));
}

public value struct HighLowTemp
{
	int high;
	int low;
};

public ref class JsonData
{
public:
	JsonData();

	bool LoadJson(System::String^ theFileName);
	void AnalyzeJson();
	void TestTemp(HighLowTemp temp);
	void TestTempByt(char RecvBuf[1024]);
	void TestTempPtr(IntPtr ptr);
	IntPtr TestTempRet();

private:
	OCCT::OccCLI<json> mJson;
};