#include "JsonData.h"
#include "OccPCH.h"
#include <chrono>

JsonData::JsonData()
{

}

bool JsonData::LoadJson(System::String^ theFileName)
{
	std::string filename;
	MarshalString(theFileName, filename);
	std::ifstream jfile(filename);
	Json::Reader reader;

	auto t1 = std::chrono::steady_clock::now();
	reader.parse(jfile, mjson());
	auto t2 = std::chrono::steady_clock::now();
	double dur = std::chrono::duration<double, std::milli>(t2 - t1).count();
	std::cout << "Json::Value size " << mjson().size() << " cost " << dur << " ms" << std::endl;

	return true;
}

void JsonData::TestTemp(HighLowTemp temp)
{
	std::cout << temp.high << " " << temp.low << std::endl;
}

void JsonData::TestTempPtr(IntPtr ptr)
{
	HighLowTemp* tp = static_cast<HighLowTemp*>(ptr.ToPointer());
	std::cout << tp->high << " " << tp->low << std::endl;
	Marshal::FreeHGlobal(ptr);
}

void JsonData::TestTempByt(char RecvBuf[1024])
{
	HighLowTemp temp;
	temp = *(HighLowTemp*)&RecvBuf;
	std::cout << temp.high << " " << temp.low << std::endl;
}

IntPtr JsonData::TestTempRet()
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
