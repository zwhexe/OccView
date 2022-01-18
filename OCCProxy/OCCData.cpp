#include "OCCData.h"
#include <chrono>



OCCData::OCCData()
{

}

void OCCData::MarshalString(String^ s, std::string& os) {
	using namespace Runtime::InteropServices;
	const char* chars =
		(const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
	os = chars;
	Marshal::FreeHGlobal(IntPtr((void*)chars));
}

bool OCCData::LoadJson(System::String^ theFileName)
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

void OCCData::AnalyzeJson()
{
	if (mJson().empty())
		return;
	for (auto j : mJson())
	{
		std::cout << j << std::endl;
	}
}

void OCCData::TestTemp(HighLowTemp temp)
{
	std::cout << temp.high << " " << temp.low << std::endl;
}

void OCCData::TestTempByt(char RecvBuf[1024])
{
	HighLowTemp temp;
	temp = *(HighLowTemp*)&RecvBuf;
	std::cout << temp.high << " " << temp.low << std::endl;
}

void OCCData::TestTempPtr(IntPtr ptr)
{
	HighLowTemp* tp = static_cast<HighLowTemp*>(ptr.ToPointer());
	std::cout << tp->high << " " << tp->low << std::endl;
}

