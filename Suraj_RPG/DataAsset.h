#pragma once
#include "IData.h"
namespace bm98
{
class DataAsset : IData
{
public:
	DataAsset();
	virtual ~DataAsset();

	const std::string get_name() const;

	// Inherited via IData
	virtual Json::Value serialize_json() override;

	virtual void unserialize_json(Json::Value obj) override;


protected:
	std::string name;

};
}