#include "StdAfx.h"
#include "vir_toolbox.hpp"

namespace fs = boost::filesystem;
using std::runtime_error;

ExistingPath::ExistingPath(const std::string& path) {
	_path = fs::path(path);
	if (!fs::exists(_path)) {
		throw runtime_error("file " + path + " doesn't exist.");
	}
	_stringRep = _path.string();
}

ExistingPath::operator const boost::filesystem::path() const {
	return _path;
}

ExistingPath::operator const std::string() const {
	return _stringRep;
}

const char* ExistingPath::c_str() const {
	return _stringRep.c_str();
}
