#pragma once
#include <vector>
#include "json.hpp"
using json = nlohmann::json;

ref class OCCJson
{
public:
	OCCJson();
	OCCJson(std::string filename);

};

