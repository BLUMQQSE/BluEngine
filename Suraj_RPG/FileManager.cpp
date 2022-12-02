#include "pch.h"
#include "FileManager.h"
namespace bm98::core
{
bool FileManager::restart_on_launch = true;

void FileManager::save_to_file_styled(Json::Value obj, std::string file_path)
{
	std::ofstream ofs(file_path);
	Json::StyledWriter writer;
	ofs << obj;
	ofs.close();
}


Json::Value FileManager::load_from_file(std::string file_path, bool ignore_reset_on_launch)
{
	std::ifstream ifs(file_path);
	Json::Reader reader;
	Json::Value obj;
	reader.parse(ifs, obj);
	ifs.close();

	if ((obj.empty() || restart_on_launch) && !ignore_reset_on_launch)
	{
		FileManager::save_to_file_styled(serialize_json(), file_path);
		std::ifstream ifs(file_path);
		Json::Reader reader;
		Json::Value obj;
		reader.parse(ifs, obj);
		ifs.close();

		return obj;
	}
	return obj;
}

Json::Value FileManager::serialize_json()
{
	return Json::Value();
}

void FileManager::unserialize_json(Json::Value obj)
{

}
}