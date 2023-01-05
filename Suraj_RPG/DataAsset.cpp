#include "pch.h"
#include "DataAsset.h"
#include "FileManager.h"
namespace bm98
{
DataAsset::DataAsset()
{
}

DataAsset::~DataAsset()
{
}

const std::string DataAsset::get_name() const
{
    return name;
}

Json::Value DataAsset::serialize_json()
{
    return Json::Value();
}

void DataAsset::unserialize_json(Json::Value obj)
{
    name = obj["name"].asString();
}
}