#include "StdAfx.h"
#include "property.h"

namespace po = boost::program_options;
namespace fs = boost::filesystem;
using std::cout;
using std::endl;
using std::runtime_error;

void InitProgramOptions(int argc, char** argv) {
	// Declare the supported options.
	po::options_description desc("Allowed options");
	desc.add_options()
		("help", "produce help message")
		("window_size", po::value<int>(), "set window size")
		("config_file", po::value<string>(), "fully-qualified path to config file");

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);    
	int windowSize = vm["window_size"].as<int>();

	if (vm.count("help")) {
		cout << desc << endl;
		exit(EXIT_SUCCESS);
	}

	if (vm.count("config_file")) {
		fs::path pathToConfig(vm["config_file"].as<string>());
		if (!fs::exists(pathToConfig)) {
			throw runtime_error("file not found " + pathToConfig.string());
		}
		po::store(po::parse_config_file<char>(pathToConfig.string().c_str(), desc), vm);
		po::notify(vm);    
	}
	windowSize = vm["window_size"].as<int>();
}