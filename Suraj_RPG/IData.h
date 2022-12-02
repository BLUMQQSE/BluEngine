#pragma once
#include <string>
#include "json/value.h"
namespace bm98
{
class IData
{
public:
	virtual Json::Value serialize_json() = 0;
	virtual void unserialize_json(Json::Value obj) = 0;


};
}