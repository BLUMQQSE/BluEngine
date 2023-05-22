#include "../pch.h"
#include "DataAsset.h"
#include "../core/FileManager.h"
namespace bm98
{

Json::Value DataAsset::serialize_json()
{
    return Json::Value();
}

void DataAsset::unserialize_json(Json::Value obj)
{
    name = obj["name"].asString();
}

}