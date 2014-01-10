#pragma once
#include "boost/filesystem.hpp"
#include <memory>
#include <vector>
#include <cstring>

template <class T> std::shared_ptr<std::vector<T>> initSmartArray(const int size = 0) {
	std::shared_ptr<vector<T>> returned(new std::vector<T>(size));
	return returned;
}

template <class T> std::shared_ptr<T> initCStyleArray(T* dumb) {
	return std::shared_ptr<T>(dumb, [](T* ptr) {
		delete[] ptr;
	});
}

class ExistingPath {
private:
	boost::filesystem::path _path;
	std::string _stringRep;
public:
	ExistingPath(const std::string& path);
	operator const boost::filesystem::path() const;
	operator const std::string() const;
	const char* c_str() const;
};