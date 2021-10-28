#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <map>

class paths
{
	public:
	paths( const std::string );
	void NextConfigFile();
	const std::map<std::string, std::string>& Config_Paths();

	private:
	std::ifstream file_stream;
	std::map<std::string, std::string> config_paths;
	const std::string ReadFromTo( std::string, std::string );
};
