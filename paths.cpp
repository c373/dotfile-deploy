#include "paths.h"

paths::paths( const std::string path ) : file_stream( path )
{

	while ( file_stream.good() )
		NextConfigFile();
}

void paths::NextConfigFile()
{
	std::string config = ReadFromTo("[", "]" );
	std::string path = ReadFromTo( "path=", "\n" );

	if( config != "" )
		config_paths.emplace( std::make_pair( config, path ));
}

const std::string paths::ReadFromTo( std::string begin, std::string end )
{
	if ( begin == "" || end == "" )
		return "";
	
	int match = 0;
	std::string delim = begin;
	bool start = false;
	char tmp;
	std::string output;

	//output the characters between the 'begin' and 'end' delims
	while ( file_stream.good() )
	{
		if ( match == delim.length() )
		{
			if ( !start )
			{
				start = true;
				match = 0;
				delim = end;
			}
			else
			{
				for ( int i = match; i > 0; i-- )
					output.pop_back();
				return output;
			}

		}

		tmp = file_stream.get();

		if ( tmp == delim[match] )
			match++;
		else
			match = 0;

		if( start )
			output += tmp;
	}

	return output;
}

const std::map<std::string, std::string>& paths::Config_Paths()
{
	return config_paths;
}
