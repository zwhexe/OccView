#pragma once
#include "OccPCH.h"
#include "OccCLI.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <json.h>

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;

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
	void TestTemp(HighLowTemp temp);
	void TestTempByt(char RecvBuf[1024]);
	void TestTempPtr(IntPtr ptr);
	IntPtr TestTempRet();

private:
	OCCT::OccCLI<Json::Value> mjson;
};
