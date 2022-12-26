#pragma once
#include <iostream>
#include <fstream>
#include "json/json.h"
#include <string>
namespace bm98::core
{
class FileManager
{

public:

	static std::string get_save_name();
	static void set_save_name(std::string s_n);

	static void save_to_file_styled(Json::Value obj, std::string file_path);
	static Json::Value load_from_file(std::string file_path);

	static Json::Value serialize_json();
	static void unserialize_json(Json::Value obj);

private:
	//std::string file_path;
	// start fresh game from start on launch
	//static bool restart_on_launch;
	static std::string save_name;

};
}