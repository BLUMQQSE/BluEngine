#pragma once
#include "IData.h"
namespace bm98
{
class DataAsset : IData
{
public:
	DataAsset() {}
	virtual ~DataAsset() {}

	std::string& get_name() { return file_name; }
	std::string& get_file_name() { return file_name; }
	void set_file_name(std::string file_name) { this->file_name = file_name; }

	// Inherited via IData
	virtual Json::Value serialize_json() override;

	virtual void unserialize_json(Json::Value obj) override;


protected:
	std::string name;
	std::string file_name;

};
}