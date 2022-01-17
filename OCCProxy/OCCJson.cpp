#include "OCCJson.h"

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
	std::string filename;
	MarshalString(theFileName, filename);
	std::ifstream jfile(filename);
	jfile >> mJson();
	AnalyzeJson();

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


