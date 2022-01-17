#include "OCCJson.h"
#include <chrono>

OCCJson::OCCJson()
{

}

void OCCJson::MarshalString(String^ s, std::string& os) {
	using namespace Runtime::InteropServices;
	const char* chars =
		(const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
	os = chars;
	Marshal::FreeHGlobal(IntPtr((void*)chars));
}

bool OCCJson::LoadJson(System::String^ theFileName)
{
	auto t1 = std::chrono::steady_clock::now();

	std::string filename;
	MarshalString(theFileName, filename);
	std::ifstream jfile(filename);
	jfile >> mJson();

	auto t2 = std::chrono::steady_clock::now();
	double dur = std::chrono::duration<double, std::milli>(t2 - t1).count();
	std::cout << "OCCJson::OCCJson() cost " << dur << " ms" << std::endl;
	//AnalyzeJson();

	return true;
}

void OCCJson::AnalyzeJson()
{
	if (mJson().empty())
		return;
	for (auto j : mJson())
	{
		std::cout << j << std::endl;
	}
}


