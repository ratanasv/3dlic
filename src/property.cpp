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
		propertyToString[WINDOW_SIZE] = "window_size";
	}
	return propertyToString[prop];
}


void InitProgramOptions(int argc, char** argv) {
	// Declare the supported options.
	po::options_description desc("Allowed options");
	desc.add_options()
		("help", "produce help message")
		(toString(WINDOW_SIZE).c_str(), po::value<int>(), "set window size")
		("config_file", po::value<string>(), "fully-qualified path to config file");

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

	if (variable_map.count("config_file")) {
		fs::path pathToConfig(variable_map["config_file"].as<string>());
		try {
			po::store(po::parse_config_file<char>(
				pathToConfig.string().c_str(), desc), variable_map);
		} catch (exception& e) {
			throw logic_error(e.what());
		}
		po::notify(variable_map);    
	}
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