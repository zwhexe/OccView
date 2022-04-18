#pragma once
#ifndef OCC_DLL_CLASS_EXPORTS
#define OCC_DLL_CLASS __declspec(dllexport)
#else
#define OCC_DLL_CLASS __declspec(dllimport)
#endif
#include "OccPCH.h"
#include "OccContext.h"
#include <json.h>
#include <regex>

static std::vector<std::string> stringSplit(const std::string& str, char delim)
{
	std::size_t previous = 0;
	std::size_t current = str.find(delim);
	std::vector<std::string> elems;
	while (current != std::string::npos)
	{
		if (current > previous)
			elems.push_back(str.substr(previous, current - previous));
		previous = current + 1;
		current = str.find(delim, previous);
	}
	if (previous != str.size())
	{
		elems.push_back(str.substr(previous));
	}
	return elems;
}

class OCC_DLL_CLASS OccModel
{
public:
	OccModel();
	void makeCone();
	void makeTorus();
	void makeWedge();
	void displayShape();
	TopoDS_Shape getShape();

	bool loadJson(std::string& filename);
	void makeTest();
private:
	TopoDS_Shape m_shape;
	Json::Value m_json;
};
