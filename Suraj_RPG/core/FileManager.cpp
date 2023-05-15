#include "../pch.h"
#include "FileManager.h"
namespace bm98::core
{

std::string FileManager::get_save_name()
{
	return save_name;
}

void FileManager::set_save_name(std::string s_n)
{
	save_name = s_n;
}

void FileManager::save_to_file_styled(Json::Value obj, std::string file_path)
{
	std::ofstream ofs(file_path);
	Json::StyledWriter writer;
	ofs << obj;
	ofs.close();
}


Json::Value FileManager::load_from_file(std::string file_path)
{
	std::ifstream ifs(file_path);
	Json::Reader reader;
	Json::Value obj;
	reader.parse(ifs, obj);
	ifs.close();


	return obj;
}

}