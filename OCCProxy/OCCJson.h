#pragma once
#include <NCollection_Haft.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "json.hpp"

using namespace System;
using json = nlohmann::json;

public ref class OCCJson
{
public:
	OCCJson();
	bool LoadJson(System::String^ theFileName);
	void AnalyzeJson();
	void TestArray(const int N, const int n[], int& Z);

private:
	void MarshalString(String^ s, std::string& os);

private:
	NCollection_Haft<json> mJson;
};

