#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <experimental/filesystem>
#include <string>
#include <iostream>
#include <cstdarg>
#include "paths.h"

namespace fs = std::experimental::filesystem;;

int main( int argc, char* argv[] )
{
	bool overwrite = false;
	bool verbose = false;
	bool pull = false;

	for ( int i = 0; i < argc; i++ )
	{
		if ( argv[i][0] == '-' )
		{
			for ( int j = 1; j < sizeof( argv[i] ) / sizeof( argv[i][0] ); j++ )
			{
				if ( argv[i][j] == 'f' )
					overwrite = true;
				if( argv[i][j] == 'v' )
					verbose = true;
				if( argv[i][j] == 'p' )
					pull = true;
			}
		}
	}

	if ( !fs::is_directory( "config" ) )
	{
		std::cout << "Please ensure the 'config' folder exists." << std::endl;
		return 1;
	}

	if ( !fs::exists( "config/paths.ini" ) )
	{
		std::cout << "Please ensure 'config/paths.ini' exists." << std::endl;
		return 2;
	}

	pull ? std::cout << "Pulling config files... " : std::cout << "Deploying config files... ";

	if ( overwrite )
		std::cout << "overwriting existing files.";
	else
		std::cout << "updating if they already exist.";

	std::cout << std::endl;

	struct passwd* pwd = getpwuid( getuid() );

	const fs::path home = fs::path( pwd->pw_dir );

	paths filePath_map = paths( "config/paths.ini" );
	
	for ( auto p : filePath_map.Config_Paths() )
	{
		auto source = "config/" + p.first;

		// create the destination path, prepend a '/' if not the first character
		auto dest_folder = ( p.second.front() == '/' ) ? home.string() + p.second : home.string() + '/' + p.second;
		
		// append a '/' if not the last character
		if ( dest_folder.back() != '/' )
			dest_folder.push_back( '/' );

		auto destination = dest_folder + p.first;

		if ( pull )
			std::swap( source, destination );

		// skip if source file does not exist
		if( !fs::exists( source ) )
		{
			if ( verbose )
				std::cout << source << " does not exist... skipping." << std::endl;
			continue;
		}

		// attempt to create the destination directory if it doesn't already exist
		if ( !fs::is_directory( dest_folder ) && !pull )
		{
			try
			{
				if ( verbose )
					std::cout << "Creating " << dest_folder << "...";
				
				std::error_code ec;

				if ( fs::create_directories( dest_folder, ec ) )
				{
					if ( verbose )
						std::cout << "done." << std::endl;
				}
				else
				{
					std::cout << ec.message() << std::endl;
				}
			}
			catch ( const std::exception& e )
			{
				std::cout << e.what() << std::endl;
			}
		}

		try
		{
			if ( verbose )
				std::cout << "Copying " << source << " -> " << destination << "...";
			
			std::error_code ec;

			if ( !overwrite && fs::exists( destination ) )
			{
				if ( fs::last_write_time( source ) <= fs::last_write_time( destination ) )
				{
					if ( verbose )
						std::cout << "already up to date." << std::endl;
					continue;
				}
			}

			if ( fs::copy_file( source, destination, overwrite ? fs::copy_options::overwrite_existing : fs::copy_options::update_existing, ec ) )
			{	if( verbose )
					std::cout << "done." << std::endl;
			}
			else
			{
				std::cout << ec.message() << std::endl;
			}

		}
		catch ( const std::exception& e )
		{
			std::cout << e.what() << std::endl;
		}
	}

	std::cout << "Done." << std::endl;

	return 0;
}
