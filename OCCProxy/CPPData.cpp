#include "CPPData.h"
#include "OccPCH.h"
#include <chrono>

CPPData::CPPData()
{

}

bool CPPData::LoadJson(System::String^ theFileName)
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

void CPPData::AnalyzeJson()
{
	if (mJson().empty())
		return;
	for (auto j : mJson())
	{
		std::cout << j << std::endl;
	}
}

void CPPData::TestTemp(HighLowTemp temp)
{
	std::cout << temp.high << " " << temp.low << std::endl;
}

void CPPData::TestTempPtr(IntPtr ptr)
{
	HighLowTemp* tp = static_cast<HighLowTemp*>(ptr.ToPointer());
	std::cout << tp->high << " " << tp->low << std::endl;
	Marshal::FreeHGlobal(ptr);
}

void CPPData::TestTempByt(char RecvBuf[1024])
{
	HighLowTemp temp;
	temp = *(HighLowTemp*)&RecvBuf;
	std::cout << temp.high << " " << temp.low << std::endl;
}

IntPtr CPPData::TestTempRet()
{
	HighLowTemp^ temp = gcnew HighLowTemp;
	temp->high = 39;
	temp->low = 19;

	int size = Marshal::SizeOf(temp);
	IntPtr structPtr = Marshal::AllocHGlobal(size);
	Marshal::StructureToPtr(temp, structPtr, false);

	return structPtr;
	//Marshal::FreeHGlobal(structPtr);
}