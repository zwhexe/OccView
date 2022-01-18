#pragma once
#include <NCollection_Haft.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "json.hpp"

using namespace System;
using json = nlohmann::json;


public value struct HighLowTemp
{
	int high;
	int low;
};

public ref class OCCData
{
public:
	OCCData();
	bool LoadJson(System::String^ theFileName);
	void AnalyzeJson();
	void TestTemp(HighLowTemp temp);
	void TestTempByt(char RecvBuf[1024]);
	void TestTempPtr(IntPtr ptr);

private:
	void MarshalString(String^ s, std::string& os);

private:
	NCollection_Haft<json> mJson;

};

