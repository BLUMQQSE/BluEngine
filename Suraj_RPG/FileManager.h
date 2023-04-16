#pragma once
namespace bm98::core
{
class FileManager
{

public:
	static FileManager* Instance()
	{
		static FileManager instance;
		return &instance;
	}

	std::string get_save_name();
	void set_save_name(std::string s_n);

	void save_to_file_styled(Json::Value obj, std::string file_path);
	Json::Value load_from_file(std::string file_path);

private:
	FileManager() {}
	~FileManager() {  }
	FileManager(const FileManager& rhs)
	{

	}
	FileManager& operator=(const FileManager& rhs) {}

	//std::string file_path;
	// start fresh game from start on launch
	//static bool restart_on_launch;
	std::string save_name;

};
}