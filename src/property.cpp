#include "StdAfx.h"
#include "property.h"
#include <map>

namespace po = boost::program_options;
namespace fs = boost::filesystem;
using std::cout;
using std::endl;
using std::runtime_error;
using std::logic_error;
using std::exception;
using std::map;

static po::variables_map variable_map;


static const string toString(Property prop) {
	static map<Property, string> propertyToString;
	if (propertyToString.size() == 0) {
		propertyToString[Property::WINDOW_SIZE] = "window_size";
		propertyToString[Property::WINDOW_TITLE] = "window_title";
		propertyToString[Property::PATH_DATA] = "path_data";
	}
	return propertyToString[prop];
}


void InitProgramOptions(int argc, char** argv) {
	// Declare the supported options.
	po::options_description desc("Allowed options");
	desc.add_options()
		("help", "produce help message")
		(toString(Property::WINDOW_SIZE).c_str(), po::value<int>(), "set window size")
		(toString(Property::WINDOW_TITLE).c_str(), po::value<string>(), "set window title")
		("config_file", po::value<string>(), "fully-qualified path to config file")
		(toString(Property::PATH_DATA).c_str(), po::value<string>(), "path to vector data");

	try {
		po::store(po::parse_command_line(argc, argv, desc), variable_map);
	} catch (exception& e) {
		throw logic_error(e.what());
	}
	po::notify(variable_map);    

	if (variable_map.count("help")) {
		cout << desc << endl;
		exit(EXIT_SUCCESS);
	}

	fs::path pathToConfig;
	if (variable_map.count("config_file")) {
		pathToConfig = fs::path(variable_map["config_file"].as<string>());  
	} else {
		pathToConfig = fs::path("E:\\Vault\\3dlic\\config.cfg");
	}
	try {
		po::store(po::parse_config_file<char>(
			pathToConfig.string().c_str(), desc), variable_map);
	} catch (exception& e) {
		throw logic_error(e.what());
	}
	po::notify(variable_map);  
}

int GetIntProperty(Property prop) {
	try {
		return variable_map[toString(prop)].as<int>();
	} catch(exception& e) {
		throw logic_error(e.what());
	}
}

string GetStringProperty(Property prop) {
	try {
		return variable_map[toString(prop)].as<string>();
	} catch(exception& e) {
		throw logic_error(e.what());
	}
}
float GetFloatProperty(Property prop) {
	try {
		return variable_map[toString(prop)].as<float>();
	} catch(exception& e) {
		throw logic_error(e.what());
	}
}