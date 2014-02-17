#pragma once
#include "boost/filesystem.hpp"
#include <memory>
#include <vector>
#include <cstring>
#include <map>
#include <utility>
#include <mutex>

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

template <class T, class V> static void InsertHelper(std::map<T,V>& theMap, 
	const T& key, const V& value) 
{
	theMap.insert(std::pair<T,V>(key, value));
}


template<class T> class LazyInitOnce {
private:
	std::once_flag _initFlag;
	T _data;
	std::function<T()> _initFunc;
private:
	void init() {
		_data = _initFunc();
	}
public:
	void Set(const std::function<T()>& lambda) {
		_initFunc = lambda ;
	}
	operator T&() {
		std::call_once(_initFlag, &LazyInitOnce::init, this);
		return _data;
	}

};